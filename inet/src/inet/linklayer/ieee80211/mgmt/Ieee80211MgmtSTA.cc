//
// Copyright (C) 2006 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "inet/linklayer/ieee80211/mgmt/Ieee80211MgmtSTA.h"

#include "inet/physicallayer/contract/packetlevel/IRadioMedium.h"
#include "inet/networklayer/common/InterfaceEntry.h"
#include "inet/common/ModuleAccess.h"
#include "inet/linklayer/common/Ieee802Ctrl.h"
#include "inet/common/NotifierConsts.h"
#include "inet/physicallayer/contract/packetlevel/RadioControlInfo_m.h"
#include "inet/physicallayer/ieee80211/packetlevel/Ieee80211ControlInfo_m.h"
#include "inet/common/INETUtils.h"
#include <algorithm>
#include <vector>
#include <string>

namespace inet {

namespace ieee80211 {

using namespace physicallayer;

//TBD supportedRates!
//TBD use command msg kinds?
//TBD implement bitrate switching (Radio already supports it)
//TBD where to put LCC header (SNAP)..?
//TBD mac should be able to signal when msg got transmitted

Define_Module(Ieee80211MgmtSTA);

// message kind values for timers
#define MK_AUTH_TIMEOUT           1
#define MK_ASSOC_TIMEOUT          2
#define MK_SCAN_SENDPROBE         3
#define MK_SCAN_MINCHANNELTIME    4
#define MK_SCAN_MAXCHANNELTIME    5
#define MK_BEACON_TIMEOUT         6

//#define MAX_BEACONS_MISSED        3.5  // beacon lost timeout, in beacon intervals (doesn't need to be integer)
#define MAX_BEACONS_MISSED        7  // EEW adjausted to avoid the packet loss during scan

std::ostream& operator<<(std::ostream& os,
        const Ieee80211MgmtSTA::ScanningInfo& scanning) {
    os << "activeScan=" << scanning.activeScan << " probeDelay="
            << scanning.probeDelay << " curChan=";
    if (scanning.channelList.empty())
        os << "<none>";
    else
        os << scanning.channelList[scanning.currentChannelIndex];
    os << " minChanTime=" << scanning.minChannelTime << " maxChanTime="
            << scanning.maxChannelTime;
    os << " chanList={";
    for (int i = 0; i < (int) scanning.channelList.size(); i++)
        os << (i == 0 ? "" : " ") << scanning.channelList[i];
    os << "}";

    return os;
}

std::ostream& operator<<(std::ostream& os, const Ieee80211MgmtSTA::APInfo& ap) {
    os << "AP addr=" << ap.address << " chan=" << ap.channel << " ssid="
            << ap.ssid
            //TBD supportedRates
            << " beaconIntvl=" << ap.beaconInterval << " rxPower=" << ap.rxPower
            << " authSeqExpected=" << ap.authSeqExpected << " isAuthenticated="
            << ap.isAuthenticated;
    return os;
}

std::ostream& operator<<(std::ostream& os,
        const Ieee80211MgmtSTA::AssociatedAPInfo& assocAP) {
    os << "AP addr=" << assocAP.address << " chan=" << assocAP.channel
            << " ssid=" << assocAP.ssid << " beaconIntvl="
            << assocAP.beaconInterval << " receiveSeq="
            << assocAP.receiveSequence << " rxPower=" << assocAP.rxPower;
    return os;
}

void Ieee80211MgmtSTA::initialize(int stage) {
    Ieee80211MgmtBase::initialize(stage);

    // EEW
    EV << "From the function Ieee80211MgmtSTA::initialize\n";

    if (stage == INITSTAGE_LOCAL) {
        isScanning = false;
        isAssociated = false;
        assocTimeoutMsg = nullptr;
        myIface = nullptr;
        numChannels = par("numChannels");

        host = getContainingNode(this);
        host->subscribe(NF_LINK_FULL_PROMISCUOUS, this);

        WATCH(isScanning);
        WATCH(isAssociated);

        WATCH(scanning);
        WATCH(assocAP);
        WATCH_LIST(apList);
    } else if (stage == INITSTAGE_LINK_LAYER_2) {
        IInterfaceTable *ift = findModuleFromPar<IInterfaceTable>(
                par("interfaceTableModule"), this);
        if (ift) {
            myIface =
                    ift->getInterfaceByName(
                            utils::stripnonalnum(
                                    findModuleUnderContainingNode(this)->getFullName()).c_str());
        }
    }

}

void Ieee80211MgmtSTA::handleTimer(cMessage *msg) {
    if (msg->getKind() == MK_AUTH_TIMEOUT) {
        // authentication timed out
        APInfo *ap = (APInfo *) msg->getContextPointer();
        EV << "Authentication timed out, AP address = " << ap->address << "\n";

        // send back failure report to agent
        sendAuthenticationConfirm(ap, PRC_TIMEOUT);
    } else if (msg->getKind() == MK_ASSOC_TIMEOUT) {
        // association timed out
        APInfo *ap = (APInfo *) msg->getContextPointer();
        EV << "Association timed out, AP address = " << ap->address << "\n";

        // send back failure report to agent
        sendAssociationConfirm(ap, PRC_TIMEOUT);
    } else if (msg->getKind() == MK_SCAN_MAXCHANNELTIME) {
        // go to next channel during scanning
        bool done = scanNextChannel();
        if (done)
            sendScanConfirm(); // send back response to agents' "scan" command
        delete msg;
    } else if (msg->getKind() == MK_SCAN_SENDPROBE) {
        // Active Scan: send a probe request, then wait for minChannelTime (11.1.3.2.2)
        delete msg;
        sendProbeRequest();
        cMessage *timerMsg = new cMessage("minChannelTime",
        MK_SCAN_MINCHANNELTIME);
        scheduleAt(simTime() + scanning.minChannelTime, timerMsg); //XXX actually, we should start waiting after ProbeReq actually got transmitted
    } else if (msg->getKind() == MK_SCAN_MINCHANNELTIME) {
        // Active Scan: after minChannelTime, possibly listen for the remaining time until maxChannelTime
        delete msg;
        if (scanning.busyChannelDetected) {
            EV
                      << "Busy channel detected during minChannelTime, continuing listening until maxChannelTime elapses\n";
            cMessage *timerMsg = new cMessage("maxChannelTime",
            MK_SCAN_MAXCHANNELTIME);
            scheduleAt(
                    simTime() + scanning.maxChannelTime
                            - scanning.minChannelTime, timerMsg);
        } else {
            EV
                      << "Channel was empty during minChannelTime, going to next channel\n";
            bool done = scanNextChannel();
            if (done) {
                sendScanConfirm(); // send back response to agents' "scan" command
            }
        }
    } else if (msg->getKind() == MK_BEACON_TIMEOUT) {
        // missed a few consecutive beacons
        beaconLost();
    } else {
        throw cRuntimeError("internal error: unrecognized timer '%s'",
                msg->getName());
    }
}

void Ieee80211MgmtSTA::handleUpperMessage(cPacket *msg) {
    if (!isAssociated || assocAP.address.isUnspecified()) {
        EV << "STA is not associated with an access point, discarding packet"
                  << msg << "\n";
        delete msg;
        return;
    }

    Ieee80211DataFrame *frame = encapsulate(msg);
    sendOrEnqueue(frame);
}

void Ieee80211MgmtSTA::handleCommand(int msgkind, cObject *ctrl) {
    if (dynamic_cast<Ieee80211Prim_ScanRequest *>(ctrl))
        processScanCommand((Ieee80211Prim_ScanRequest *) ctrl);
    else if (dynamic_cast<Ieee80211Prim_AuthenticateRequest *>(ctrl))
        processAuthenticateCommand((Ieee80211Prim_AuthenticateRequest *) ctrl);
    else if (dynamic_cast<Ieee80211Prim_DeauthenticateRequest *>(ctrl))
        processDeauthenticateCommand(
                (Ieee80211Prim_DeauthenticateRequest *) ctrl);
    else if (dynamic_cast<Ieee80211Prim_AssociateRequest *>(ctrl))
        processAssociateCommand((Ieee80211Prim_AssociateRequest *) ctrl);
    else if (dynamic_cast<Ieee80211Prim_ReassociateRequest *>(ctrl))
        processReassociateCommand((Ieee80211Prim_ReassociateRequest *) ctrl);
    else if (dynamic_cast<Ieee80211Prim_DisassociateRequest *>(ctrl))
        processDisassociateCommand((Ieee80211Prim_DisassociateRequest *) ctrl);
    // EEW just a change channel function for the agent
    else if (dynamic_cast<Ieee80211Prim_ChangeChannelRequest *>(ctrl))
        changeChannel(
                (dynamic_cast<Ieee80211Prim_ChangeChannelRequest *>(ctrl))->getChannelNummer());
    else if (ctrl)
        throw cRuntimeError(
                "handleCommand(): unrecognized control info class `%s'",
                ctrl->getClassName());
    else
        throw cRuntimeError("handleCommand(): control info is nullptr");
    delete ctrl;
}

Ieee80211DataFrame *Ieee80211MgmtSTA::encapsulate(cPacket *msg) {
    Ieee80211DataFrameWithSNAP *frame = new Ieee80211DataFrameWithSNAP(
            msg->getName());

    // frame goes to the AP
    frame->setToDS(true);

    // receiver is the AP
    frame->setReceiverAddress(assocAP.address);

    // destination address is in address3
    Ieee802Ctrl *ctrl = check_and_cast<Ieee802Ctrl *>(msg->removeControlInfo());
    frame->setAddress3(ctrl->getDest());
    frame->setEtherType(ctrl->getEtherType());
    delete ctrl;

    frame->encapsulate(msg);
    return frame;
}

cPacket *Ieee80211MgmtSTA::decapsulate(Ieee80211DataFrame *frame) {
    cPacket *payload = frame->decapsulate();

    Ieee802Ctrl *ctrl = new Ieee802Ctrl();
    ctrl->setSrc(frame->getAddress3());
    ctrl->setDest(frame->getReceiverAddress());
    Ieee80211DataFrameWithSNAP *frameWithSNAP =
            dynamic_cast<Ieee80211DataFrameWithSNAP *>(frame);
    if (frameWithSNAP)
        ctrl->setEtherType(frameWithSNAP->getEtherType());
    payload->setControlInfo(ctrl);

    delete frame;
    return payload;
}

Ieee80211MgmtSTA::APInfo *Ieee80211MgmtSTA::lookupAP(
        const MACAddress& address) {
    for (auto & elem : apList)
        if (elem.address == address)
            return &(elem);

    return nullptr;
}

void Ieee80211MgmtSTA::clearAPList() {
    for (auto & elem : apList)
        if (elem.authTimeoutMsg)
            delete cancelEvent(elem.authTimeoutMsg);

    apList.clear();
}

void Ieee80211MgmtSTA::changeChannel(int channelNum) {
    EV << " changeChannel Tuning to channel #" << channelNum << "\n";

    Ieee80211ConfigureRadioCommand *configureCommand =
            new Ieee80211ConfigureRadioCommand();
    configureCommand->setChannelNumber(channelNum);
    cMessage *msg = new cMessage("changeChannel", RADIO_C_CONFIGURE);
    msg->setControlInfo(configureCommand);
    send(msg, "macOut");
}

void Ieee80211MgmtSTA::beaconLost() {
    EV << "Missed a few consecutive beacons -- AP is considered lost\n";
    emit(NF_L2_BEACON_LOST, myIface);
}

void Ieee80211MgmtSTA::sendManagementFrame(Ieee80211ManagementFrame *frame,
        const MACAddress& address) {
    // frame goes to the specified AP
    frame->setToDS(true);
    frame->setReceiverAddress(address);
    //XXX set sequenceNumber?

    sendOrEnqueue(frame);
}

void Ieee80211MgmtSTA::startAuthentication(APInfo *ap, simtime_t timeout) {

    if (ap->authTimeoutMsg)
        throw cRuntimeError(
                "startAuthentication: authentication currently in progress with AP address=",
                ap->address.str().c_str());
    if (ap->isAuthenticated)
        throw cRuntimeError(
                "startAuthentication: already authenticated with AP address=",
                ap->address.str().c_str());

    changeChannel(ap->channel);

    EV << "Sending initial Authentication frame with seqNum=1\n";

    // create and send first authentication frame
    Ieee80211AuthenticationFrame *frame = new Ieee80211AuthenticationFrame(
            "Auth");
    frame->getBody().setSequenceNumber(1);
    //XXX frame length could be increased to account for challenge text length etc.
    sendManagementFrame(frame, ap->address);

    ap->authSeqExpected = 2;

    // schedule timeout
    ASSERT(ap->authTimeoutMsg == nullptr);
    ap->authTimeoutMsg = new cMessage("authTimeout", MK_AUTH_TIMEOUT);
    ap->authTimeoutMsg->setContextPointer(ap);
    scheduleAt(simTime() + timeout, ap->authTimeoutMsg);
}

void Ieee80211MgmtSTA::startAssociation(APInfo *ap, simtime_t timeout) {

    if (isAssociated || assocTimeoutMsg) {
        //throw cRuntimeError(
       //        "startAssociation: already associated or association currently in progress");
        // EEW FIXME!!! FIXME
        // return;
        // EEW FIXME!!! FIXME
    }

    if (isAssociated) {
        // FIXME

        //in caso of beacon lost an deass req is to be sended
        disassociate();

        //throw cRuntimeError(
        //          "startAssociation: already associated");
    }

    if (assocTimeoutMsg) {
            throw cRuntimeError(
                    "startAssociation: association currently in progress");
    }

    if (!ap->isAuthenticated)
       throw cRuntimeError(
                "startAssociation: not yet authenticated with AP address=",
                ap->address.str().c_str());

    // switch to that channel
    changeChannel(ap->channel);

    // create and send association request
    Ieee80211AssociationRequestFrame *frame =
            new Ieee80211AssociationRequestFrame("Assoc");

    //XXX set the following too?
    // string SSID
    // Ieee80211SupportedRatesElement supportedRates;

    sendManagementFrame(frame, ap->address);

    // schedule timeout
    ASSERT(assocTimeoutMsg == nullptr);
    assocTimeoutMsg = new cMessage("assocTimeout", MK_ASSOC_TIMEOUT);
    assocTimeoutMsg->setContextPointer(ap);
    scheduleAt(simTime() + timeout, assocTimeoutMsg);
}

void Ieee80211MgmtSTA::receiveSignal(cComponent *source, simsignal_t signalID,
        long value) {
    //EEW
    EV_TRACE << "1234 From the function Ieee80211MgmtSTA::receiveSignal\n";

    Enter_Method_Silent
    ();
    // Note that we are only subscribed during scanning!
    if (signalID == IRadio::receptionStateChangedSignal) {
        IRadio::ReceptionState newReceptionState =
                (IRadio::ReceptionState) value;
        if (newReceptionState != IRadio::RECEPTION_STATE_UNDEFINED
                && newReceptionState != IRadio::RECEPTION_STATE_IDLE) {
            EV << "busy radio channel detected during scanning\n";
            scanning.busyChannelDetected = true;
        }
    }
}

void Ieee80211MgmtSTA::receiveSignal(cComponent *source, simsignal_t signalID,
        cObject *obj) {
    Enter_Method_Silent
    ();
    printNotificationBanner(signalID, obj);

    //EEW
    simtime_t macFrameDelay;
    EV << "1234 From the function Ieee80211MgmtSTA::receiveSignal\n";

    // Note that we are only subscribed during scanning!
    if (signalID == NF_LINK_FULL_PROMISCUOUS) {
        // EEW
        EV << "1234 From the function Ieee80211MgmtSTA::receiveSignal 2\n";
        Ieee80211DataOrMgmtFrame *frame =
                dynamic_cast<Ieee80211DataOrMgmtFrame *>(obj);
        if (!frame || frame->getControlInfo() == nullptr)
            return;
        if (frame->getType() != ST_BEACON)
            return;

        Ieee80211BeaconFrame *bframe = dynamic_cast<Ieee80211BeaconFrame *>(obj);
        if (!bframe) {
            return;
        }

        //Ieee80211ReceptionIndication *ctl = dynamic_cast<Ieee80211ReceptionIndication *>(frame->getControlInfo());
        ReceptionIndication *ctl =
                dynamic_cast<ReceptionIndication *>(frame->getControlInfo());
        if (ctl == nullptr)
            return;
        Ieee80211BeaconFrame *beacon = (check_and_cast<Ieee80211BeaconFrame *>(
                frame));
        APInfo *ap = lookupAP(beacon->getTransmitterAddress());
        EV_INFO << " handleBeacon mgmtsta ************ .\n";
        EV_INFO << " handleBeacon mgmtsta SNIR=" << ctl->getMinSNIR() << " .\n";
        EV_INFO << " handleBeacon mgmtsta SNIR="
                       << 10 * log10(ctl->getMinSNIR()) << "dB .\n";
        EV_INFO << " handleBeacon mgmtsta RSS=" << ctl->getMinRSSI() << " .\n";
        EV_INFO << " handleBeacon mgmtsta RSS="
                       << 30 + 10 * log10(ctl->getMinRSSI().get()) << "dBm .\n";
        EV_INFO << " handleBeacon mgmtsta ctl->getPacketErrorRate()="
                       << ctl->getPacketErrorRate() << " .\n";

        if (ap) {
            //ap->rxPower = ctl->getRecPow();
            //ap->rxPower = ctl->getMinSNIR();
            //ap->rxPower = ctl->getMinSNIR();
            //ap->macFrameDelay = macFrameDelay;
            ap->rxPower = ctl->getMinRSSI().get();
            ap->snir = ctl->getMinSNIR();
            ap->packetErrorRate = ctl->getPacketErrorRate();

        }
    }
}

void Ieee80211MgmtSTA::processScanCommand(Ieee80211Prim_ScanRequest *ctrl) {

    cModule *host = getContainingNode(this);

    EV << simTime() << " processScanCommand: " << host->getFullName()
              << " Received Scan Request from agent, clearing AP list and starting scanning...\n";

    //if (isScanning) {
    if (isScanning && ctrl->getRegularScan()) {
        // do not perform REGULAR SCAN if one scan is already running
        //throw cRuntimeError("processScanCommand: scanning already in progress");
        EV_TRACE << simTime() << " processScanCommand: " << host->getFullName()
                        << " REGULAR SCAN tried, but scanning already in progress"
                        << endl;
        return;
    } else if (isScanning) {
        EV_TRACE << simTime() << " processScanCommand: " << host->getFullName()
                        << " usual SCAN tried, but scanning already in progress"
                        << endl;
        return;
    }

    EV << simTime() << " processScanCommand: " << host->getFullName()
              << " Mobile node, isAssociated = " << isAssociated << "\n";
    if (isAssociated) {
        // EEW
        //disassociate();
    }
    // EEW
    else if (!isAssociated && ctrl->getRegularScan()) {
        // do not perform REGULAR SCAN if the STA is not associated
        //EV_TRACE << "processScanCommand:" << host->getFullName() << " the STA is not associated, do not perform REGULAR SCAN!!!" << endl;
        EV_TRACE << "processScanCommand: " << host->getFullName()
                        << " the STA is not associated, ANYWAY do perform REGULAR SCAN!!!"
                        << endl;

        if (assocTimeoutMsg) {
            EV << simTime() << " processScanCommand: " << host->getFullName()
                      << " Canceling ongoing association process\n";
            delete cancelEvent(assocTimeoutMsg);
            assocTimeoutMsg = nullptr;
        }

        //return;
    } else if (assocTimeoutMsg) {
        EV << simTime() << " processScanCommand: " << host->getFullName()
                  << " Canceling ongoing association process\n";
        delete cancelEvent(assocTimeoutMsg);
        assocTimeoutMsg = nullptr;
    }

    // clear existing AP list (and cancel any pending authentications) -- we want to start with a clean page
    EV << simTime() << " processScanCommand clearAPList: " << " \n";
    clearAPList();

    EV << simTime() << " processScanCommand: " << host->getFullName()
              << " we are going to start regular scan here\n";

    // fill in scanning state
    ASSERT(ctrl->getBSSType() == BSSTYPE_INFRASTRUCTURE);
    scanning.bssid =
            ctrl->getBSSID().isUnspecified() ?
                    MACAddress::BROADCAST_ADDRESS : ctrl->getBSSID();
    scanning.ssid = ctrl->getSSID();
    scanning.activeScan = ctrl->getActiveScan();
    scanning.probeDelay = ctrl->getProbeDelay();
    scanning.channelList.clear();
    scanning.minChannelTime = ctrl->getMinChannelTime();
    scanning.maxChannelTime = ctrl->getMaxChannelTime();
    ASSERT(scanning.minChannelTime <= scanning.maxChannelTime);

    // channel list to scan (default: all channels)
    for (int i = 0; i < (int) ctrl->getChannelListArraySize(); i++)
        scanning.channelList.push_back(ctrl->getChannelList(i));
    if (scanning.channelList.empty())
        for (int i = 0; i < numChannels; i++)
            scanning.channelList.push_back(i);

    // start scanning
    if (scanning.activeScan)
        host->subscribe(IRadio::receptionStateChangedSignal, this);
    scanning.currentChannelIndex = -1;    // so we'll start with index==0
    isScanning = true;
    scanNextChannel();
}

bool Ieee80211MgmtSTA::scanNextChannel() {

    cModule *host = getContainingNode(this);

    // if we're already at the last channel, we're through
    if (scanning.currentChannelIndex == (int) scanning.channelList.size() - 1) {
        EV << " scanNextChannel: " << host->getFullName()
                  << " Scan finished. It was last channel.\n";
        if (scanning.activeScan)
            host->unsubscribe(IRadio::receptionStateChangedSignal, this);
        isScanning = false;
        return true;    // we're done
    }

    // tune to next channel
    int newChannel = scanning.channelList[++scanning.currentChannelIndex];
    changeChannel(newChannel);
    scanning.busyChannelDetected = false;

    if (scanning.activeScan) {
        // Active Scan: first wait probeDelay, then send a probe. Listening
        // for minChannelTime or maxChannelTime takes place after that. (11.1.3.2)
        EV << simTime() << " scanNextChannel: " << host->getFullName()
                  << " MgmtSTA activeScan\n";
        scheduleAt(simTime() + scanning.probeDelay,
                new cMessage("sendProbe", MK_SCAN_SENDPROBE));
    } else {
        // Passive Scan: spend maxChannelTime on the channel (11.1.3.1)
        EV << "MgmtSTA passiveScan\n";
        cMessage *timerMsg = new cMessage("maxChannelTime",
        MK_SCAN_MAXCHANNELTIME);
        scheduleAt(simTime() + scanning.maxChannelTime, timerMsg);
    }

    return false;
}

void Ieee80211MgmtSTA::sendProbeRequest() {
    EV << "Sending Probe Request, BSSID=" << scanning.bssid << ", SSID=\""
              << scanning.ssid << "\"\n";
    Ieee80211ProbeRequestFrame *frame = new Ieee80211ProbeRequestFrame(
            "ProbeReq");
    frame->getBody().setSSID(scanning.ssid.c_str());
    sendManagementFrame(frame, scanning.bssid);
}

void Ieee80211MgmtSTA::sendScanConfirm() {
    EV << "Scanning complete, found " << apList.size()
              << " APs, sending confirmation to agent\n";

    // copy apList contents into a ScanConfirm primitive and send it back
    int n = apList.size();
    Ieee80211Prim_ScanConfirm *confirm = new Ieee80211Prim_ScanConfirm();
    confirm->setBssListArraySize(n);
    auto it = apList.begin();
    //XXX filter for req'd bssid and ssid
    for (int i = 0; i < n; i++, it++) {
        APInfo *ap = &(*it);
        Ieee80211Prim_BSSDescription& bss = confirm->getBssList(i);
        bss.setChannelNumber(ap->channel);
        bss.setBSSID(ap->address);
        bss.setSSID(ap->ssid.c_str());
        bss.setSupportedRates(ap->supportedRates);
        bss.setBeaconInterval(ap->beaconInterval);
        bss.setRxPower(ap->rxPower);
        bss.setRxPower_dBm(30 + 10 * log10(ap->rxPower));
        bss.setSnir(ap->snir);
        bss.setPacketLoss(ap->packetErrorRate);
        bss.setDelay(ap->supportedRates.rate[0]);
        bss.setJitter(ap->supportedRates.rate[1]);
        bss.setThroughputAP(ap->supportedRates.rate[3]);
        bss.setThroughputAPOfAssocSTAs(ap->supportedRates.rate[7]);
        bss.setThroughputSTA(ap->supportedRates.rate[6]);

        bss.setMaxBitrate(ap->supportedRates.rate[4]);
        bss.setCost(ap->supportedRates.rate[5]);
        //bss.setThroughputSTA(ap->supportedRates.rate[6]);

        //EV << " 070707 ap->rxPower " << ap->rxPower  << endl;
        EV << " 070707  ap->address "
                          << ap->address << endl;
        EV << " 070707 ap->supportedRates.rate[0] (delay)"
                  << ap->supportedRates.rate[0] << endl;
        EV << " 070707 ap->supportedRates.rate[1] (jitter)"
                  << ap->supportedRates.rate[1] << endl;
        EV << " 070707 ap->packetErrorRate " << ap->packetErrorRate << endl;

    }
    sendConfirm(confirm, PRC_SUCCESS);
}

void Ieee80211MgmtSTA::processAuthenticateCommand(
        Ieee80211Prim_AuthenticateRequest *ctrl) {

    //EEW
    /*f (isAssociated) {
        // EEW
        disassociate();
    }*/

    const MACAddress& address = ctrl->getAddress();
    APInfo *ap = lookupAP(address);

    // EEW
    // Iterate through apList
    for (auto & elem : apList){
        EV << " elem.address" << elem.address << endl;
    }

    if (!ap)
        throw cRuntimeError(
                "processAuthenticateCommand: AP not known: address = %s",
                address.str().c_str());
    startAuthentication(ap, ctrl->getTimeout());
}

void Ieee80211MgmtSTA::processDeauthenticateCommand(
        Ieee80211Prim_DeauthenticateRequest *ctrl) {
    const MACAddress& address = ctrl->getAddress();
    APInfo *ap = lookupAP(address);
    if (!ap)
        throw cRuntimeError(
                "processDeauthenticateCommand: AP not known: address = %s",
                address.str().c_str());

    if (isAssociated && assocAP.address == address)
        disassociate();

    if (ap->isAuthenticated)
        ap->isAuthenticated = false;

    // cancel possible pending authentication timer
    if (ap->authTimeoutMsg) {
        delete cancelEvent(ap->authTimeoutMsg);
        ap->authTimeoutMsg = nullptr;
    }

    // create and send deauthentication request
    EV << "Processing deauthentication request on the MGMT STA" << endl;
    EV << "Processing deauthentication address: " << address << endl;
    Ieee80211DeauthenticationFrame *frame = new Ieee80211DeauthenticationFrame(
            "Deauth");
    frame->getBody().setReasonCode(ctrl->getReasonCode());
    sendManagementFrame(frame, address);
}

void Ieee80211MgmtSTA::processAssociateCommand(
        Ieee80211Prim_AssociateRequest *ctrl) {
    const MACAddress& address = ctrl->getAddress();

    EV << " processAssociateCommand " << "\n";

    APInfo *ap = lookupAP(address);
    if (!ap)
        throw cRuntimeError(
                "processAssociateCommand: AP not known: address = %s",
                address.str().c_str());
    startAssociation(ap, ctrl->getTimeout());
}

void Ieee80211MgmtSTA::processReassociateCommand(
        Ieee80211Prim_ReassociateRequest *ctrl) {
    // treat the same way as association
    //XXX refine

    EV << " processReassociateCommand " << "\n";
    processAssociateCommand(ctrl);
}

void Ieee80211MgmtSTA::processDisassociateCommand(
        Ieee80211Prim_DisassociateRequest *ctrl) {
    const MACAddress& address = ctrl->getAddress();

    if (isAssociated && address == assocAP.address) {
        disassociate();
    } else if (assocTimeoutMsg) {
        // pending association
        delete cancelEvent(assocTimeoutMsg);
        assocTimeoutMsg = nullptr;
    }

    // create and send disassociation request
    Ieee80211DisassociationFrame *frame = new Ieee80211DisassociationFrame(
            "Disass");
    frame->getBody().setReasonCode(ctrl->getReasonCode());
    sendManagementFrame(frame, address);
}

void Ieee80211MgmtSTA::disassociate() {
    EV << "Disassociating from AP address=" << assocAP.address << "\n";
    ASSERT(isAssociated);
    isAssociated = false;
    delete cancelEvent(assocAP.beaconTimeoutMsg);
    assocAP.beaconTimeoutMsg = nullptr;
    assocAP = AssociatedAPInfo();    // clear it
}

void Ieee80211MgmtSTA::sendAuthenticationConfirm(APInfo *ap, int resultCode) {
    Ieee80211Prim_AuthenticateConfirm *confirm =
            new Ieee80211Prim_AuthenticateConfirm();
    confirm->setAddress(ap->address);
    sendConfirm(confirm, resultCode);
}

void Ieee80211MgmtSTA::sendAssociationConfirm(APInfo *ap, int resultCode) {
    Ieee80211Prim_AssociateConfirm *assocConfirm =
            new Ieee80211Prim_AssociateConfirm();
    assocConfirm->setAddress(ap->address);
    sendConfirm(assocConfirm, resultCode);
}

void Ieee80211MgmtSTA::sendConfirm(Ieee80211PrimConfirm *confirm,
        int resultCode) {
    confirm->setResultCode(resultCode);
    cMessage *msg = new cMessage(confirm->getClassName());
    msg->setControlInfo(confirm);
    send(msg, "agentOut");
}

int Ieee80211MgmtSTA::statusCodeToPrimResultCode(int statusCode) {
    return statusCode == SC_SUCCESSFUL ? PRC_SUCCESS : PRC_REFUSED;
}

void Ieee80211MgmtSTA::handleDataFrame(Ieee80211DataFrame *frame) {
    // Only send the Data frame up to the higher layer if the STA is associated with an AP,
    // else delete the frame
    if (isAssociated)
        sendUp(decapsulate(frame));
    else {
        EV << "Rejecting data frame as STA is not associated with an AP"
                  << endl;
        delete frame;
    }
}

void Ieee80211MgmtSTA::handleAuthenticationFrame(
        Ieee80211AuthenticationFrame *frame) {
    MACAddress address = frame->getTransmitterAddress();
    int frameAuthSeq = frame->getBody().getSequenceNumber();
    EV << "Received Authentication frame from address=" << address
              << ", seqNum=" << frameAuthSeq << "\n";

    APInfo *ap = lookupAP(address);
    if (!ap) {
        EV << "AP not known, discarding authentication frame\n";
        delete frame;
        return;
    }

    // what if already authenticated with AP
    if (ap->isAuthenticated) {
        EV << "AP already authenticated, ignoring frame\n";
        delete frame;
        return;
    }

    // is authentication is in progress with this AP?
    if (!ap->authTimeoutMsg) {
        EV << "No authentication in progress with AP, ignoring frame\n";
        delete frame;
        return;
    }

    // check authentication sequence number is OK
    if (frameAuthSeq != ap->authSeqExpected) {
        EV << "Wrong sequence number, " << ap->authSeqExpected << " expected\n";
        Ieee80211AuthenticationFrame *resp = new Ieee80211AuthenticationFrame(
                "Auth-ERROR");
        resp->getBody().setStatusCode(SC_AUTH_OUT_OF_SEQ);
        sendManagementFrame(resp, frame->getTransmitterAddress());
        delete frame;

        // cancel timeout, send error to agent
        delete cancelEvent(ap->authTimeoutMsg);
        ap->authTimeoutMsg = nullptr;
        sendAuthenticationConfirm(ap, PRC_REFUSED);    //XXX or what resultCode?
        return;
    }

    // check if more exchanges are needed for auth to be complete
    int statusCode = frame->getBody().getStatusCode();

    if (statusCode == SC_SUCCESSFUL && !frame->getBody().getIsLast()) {
        EV << "More steps required, sending another Authentication frame\n";

        // more steps required, send another Authentication frame
        Ieee80211AuthenticationFrame *resp = new Ieee80211AuthenticationFrame(
                "Auth");
        resp->getBody().setSequenceNumber(frameAuthSeq + 1);
        resp->getBody().setStatusCode(SC_SUCCESSFUL);
        // XXX frame length could be increased to account for challenge text length etc.
        sendManagementFrame(resp, address);
        ap->authSeqExpected += 2;
    } else {
        if (statusCode == SC_SUCCESSFUL)
            EV << "Authentication successful\n";
        else
            EV << "Authentication failed\n";

        // authentication completed
        ap->isAuthenticated = (statusCode == SC_SUCCESSFUL);
        delete cancelEvent(ap->authTimeoutMsg);
        ap->authTimeoutMsg = nullptr;
        sendAuthenticationConfirm(ap, statusCodeToPrimResultCode(statusCode));
    }

    delete frame;
}

void Ieee80211MgmtSTA::handleDeauthenticationFrame(
        Ieee80211DeauthenticationFrame *frame) {
    EV << "Received Deauthentication frame\n";
    const MACAddress& address = frame->getAddress3();    // source address
    APInfo *ap = lookupAP(address);
    if (!ap || !ap->isAuthenticated) {
        EV
                  << "Unknown AP, or not authenticated with that AP -- ignoring frame\n";
        delete frame;
        return;
    }
    if (ap->authTimeoutMsg) {
        delete cancelEvent(ap->authTimeoutMsg);
        ap->authTimeoutMsg = nullptr;
        EV << "Cancelling pending authentication\n";
        delete frame;
        return;
    }

    EV << "Setting isAuthenticated flag for that AP to false\n";
    ap->isAuthenticated = false;
    delete frame;
}

void Ieee80211MgmtSTA::handleAssociationRequestFrame(
        Ieee80211AssociationRequestFrame *frame) {
    dropManagementFrame(frame);
}

void Ieee80211MgmtSTA::handleAssociationResponseFrame(
        Ieee80211AssociationResponseFrame *frame) {
    EV << "Received Association Response frame\n";

    if (!assocTimeoutMsg) {
        EV << "No association in progress, ignoring frame\n";
        delete frame;
        return;
    }

    // extract frame contents
    MACAddress address = frame->getTransmitterAddress();
    int statusCode = frame->getBody().getStatusCode();
    //XXX short aid;
    //XXX Ieee80211SupportedRatesElement supportedRates;

    // look up AP data structure
    APInfo *ap = lookupAP(address);
    delete frame;

    for (auto & elem : apList){
           std::cout << "\n chooseBSS elem.address" << elem.address << endl;
    }
    if (!ap)
    {
        throw cRuntimeError(
                        "handleAssociationResponseFrame: AP not known: address=%s",
                        address.str().c_str());
    }
    if (isAssociated) {
        EV << "Breaking existing association with AP address="
                  << assocAP.address << "\n";
        isAssociated = false;
        delete cancelEvent(assocAP.beaconTimeoutMsg);
        assocAP.beaconTimeoutMsg = nullptr;
        assocAP = AssociatedAPInfo();
    }

    delete cancelEvent(assocTimeoutMsg);
    assocTimeoutMsg = nullptr;

    if (statusCode != SC_SUCCESSFUL) {
        EV << "Association failed with AP address=" << ap->address << "\n";
    } else {
        EV << "Association successful, AP address=" << ap->address << "\n";

        // change our state to "associated"
        isAssociated = true;
        (APInfo&) assocAP = (*ap);

        emit(NF_L2_ASSOCIATED, myIface);

        assocAP.beaconTimeoutMsg = new cMessage("beaconTimeout",
        MK_BEACON_TIMEOUT);
        scheduleAt(simTime() + MAX_BEACONS_MISSED * assocAP.beaconInterval,
                assocAP.beaconTimeoutMsg);
    }

    // report back to agent
    sendAssociationConfirm(ap, statusCodeToPrimResultCode(statusCode));
}

void Ieee80211MgmtSTA::handleReassociationRequestFrame(
        Ieee80211ReassociationRequestFrame *frame) {
    dropManagementFrame(frame);
}

void Ieee80211MgmtSTA::handleReassociationResponseFrame(
        Ieee80211ReassociationResponseFrame *frame) {
    EV << "Received Reassociation Response frame\n";
    //TBD handle with the same code as Association Response?
}

void Ieee80211MgmtSTA::handleDisassociationFrame(
        Ieee80211DisassociationFrame *frame) {
    EV << "Received Disassociation frame\n";
    const MACAddress& address = frame->getAddress3();    // source address

    if (assocTimeoutMsg) {
        // pending association
        delete cancelEvent(assocTimeoutMsg);
        assocTimeoutMsg = nullptr;
    }
    if (!isAssociated || address != assocAP.address) {
        EV << "Not associated with that AP -- ignoring frame\n";
        delete frame;
        return;
    }

    EV << "Setting isAssociated flag to false\n";
    isAssociated = false;
    delete cancelEvent(assocAP.beaconTimeoutMsg);
    assocAP.beaconTimeoutMsg = nullptr;
}

void Ieee80211MgmtSTA::handleBeaconFrame(Ieee80211BeaconFrame *frame) {

    cModule *host = getContainingNode(this);

    EV_INFO << simTime() << " 123123 " << host->getFullName()
                   << " Received Beacon frame\n";
    EV_INFO << simTime() << " 123123 " << host->getFullName()
                   << " frame->getTransmitterAddress(): "
                   << frame->getTransmitterAddress() << ".\n";

    // EEW
    EV_TRACE
                    << "12345 From Ieee80211MgmtSTA::handleBeaconFrame Received Beacon frame\n";
    // access here the QoS data from the bodyF
    //ReceptionIndication *ctl =
    //                dynamic_cast<ReceptionIndication *>(frame->getControlInfo());
    //        if (ctl == nullptr)
    //            return;

    EV_INFO << " handleBeacon mgmtsta  --- " << endl;
    EV_INFO << " handleBeacon mgmtsta simTime() " << simTime() << endl;
    EV_INFO << "handleBeacon mgmtsta NAME of mobile host: "
                   << host->getFullName() << endl;
    EV_INFO << "handleBeacon mgmtsta frame->getTransmitterAddress(): "
                   << frame->getTransmitterAddress() << ".\n";
    EV_INFO << "handleBeacon mgmtsta body.getSSID(): "
                   << frame->getBody().getSSID() << ".\n";
    EV_INFO << "handleBeacon mgmtsta body.getChannelNumber(): "
                   << frame->getBody().getChannelNumber() << ".\n";
    // QoS info from frame
    EV_INFO << " handleBeacon mgmtsta supported rates Delay: "
                   << frame->getBody().getSupportedRates().rate[0] << endl;
    EV_INFO << " handleBeacon mgmtsta supported rates Jitter: "
                   << frame->getBody().getSupportedRates().rate[1] << endl;
    EV_INFO << " handleBeacon mgmtsta supported rates PacketLoss: "
                   << frame->getBody().getSupportedRates().rate[2] << endl;
    EV_INFO << " handleBeacon mgmtsta supported rates throughput AP: "
                   << frame->getBody().getSupportedRates().rate[3] << endl;
    EV_INFO << " handleBeacon mgmtsta supported rates throughput STA: "
                   << frame->getBody().getSupportedRates().rate[6] << endl;
    EV_INFO << " handleBeacon mgmtsta supported rates max Cost: "
                   << frame->getBody().getSupportedRates().rate[4] << endl;

    // EEW a workaround, put the AP cost into supported rates
    //bss.setDelay(ap->supportedRates.rate[0]);
    //bss.setJitter(ap->supportedRates.rate[1]);
    //bss.setPacketLoss(ap->supportedRates.rate[2]);
    //bss.setThroughputAP(ap->supportedRates.rate[3]);
    //bss.setThroughputAPOfAssocSTAs(ap->supportedRates.rate[7]);
    //bss.setThroughputSTA(ap->supportedRates.rate[6]);
    //bss.setMaxBitrate(ap->supportedRates.rate[4]);
    //bss.setCost(ap->supportedRates.rate[5]);

    // QoS info from control Info
    ReceptionIndication *ctl =
            dynamic_cast<ReceptionIndication *>(frame->getControlInfo());
    if (ctl != nullptr) {
        //APInfo *ap = lookupAP(beacon->getTransmitterAddress());
        EV_INFO << " handleBeacon ctl->getMinSNIR()" << ctl->getMinSNIR()
                       << " .\n";
        EV_INFO << " handleBeacon ctl->getMinRSSI()" << ctl->getMinRSSI()
                       << " .\n";
        EV_INFO << " handleBeacon ctl->getPacketErrorRate()"
                       << ctl->getPacketErrorRate() << " .\n";
        EV_INFO << " handleBeacon ctl->getBitErrorCount()"
                       << ctl->getBitErrorCount() << " .\n";
    }

    storeAPInfo(frame->getTransmitterAddress(), frame->getBody());

    // if it is out associate AP, restart beacon timeout
    if (isAssociated && frame->getTransmitterAddress() == assocAP.address) {
        EV << "Beacon is from associated AP, restarting beacon timeout timer\n";
        ASSERT(assocAP.beaconTimeoutMsg != nullptr);
        cancelEvent(assocAP.beaconTimeoutMsg);
        scheduleAt(simTime() + MAX_BEACONS_MISSED * assocAP.beaconInterval,
                assocAP.beaconTimeoutMsg);

        //APInfo *ap = lookupAP(frame->getTransmitterAddress());
        //ASSERT(ap!=nullptr);
    }

    delete frame;
}

void Ieee80211MgmtSTA::handleProbeRequestFrame(
        Ieee80211ProbeRequestFrame *frame) {
    dropManagementFrame(frame);
}

void Ieee80211MgmtSTA::handleProbeResponseFrame(
        Ieee80211ProbeResponseFrame *frame) {
    EV << "Received Probe Response frame\n";
    //FIXME
    //storeAPInfo(frame->getTransmitterAddress(), frame->getBody());
    delete frame;
}

void Ieee80211MgmtSTA::storeAPInfo(const MACAddress& address,
        const Ieee80211BeaconFrameBody& body) {

    // EEW
    EV << "From Ieee80211MgmtSTA::storeAPInfo\n";

    APInfo *ap = lookupAP(address);
    if (ap) {
        EV << "AP address=" << address << ", SSID=" << body.getSSID()
                  << " already in our AP list, refreshing the info\n";
    } else {
        EV << "Inserting AP address=" << address << ", SSID=" << body.getSSID()
                  << " into our AP list\n";
        apList.push_back(APInfo());
        ap = &apList.back();
    }

    EV << "7171717      From Ieee80211MgmtSTA::storeAPInfo" << endl;
    EV << "7171717 address" << address << endl;
    EV << "7171717 body.getSSID()" << body.getSSID() << endl;
    EV << "7171717 body.getSupportedRates().rate[0]"
              << body.getSupportedRates().rate[0] << endl;
    EV << "7171717 body.getSupportedRates().rate[1]"
              << body.getSupportedRates().rate[1] << endl;

    ap->channel = body.getChannelNumber();
    ap->address = address;
    ap->ssid = body.getSSID();
    ap->supportedRates = body.getSupportedRates();
    ap->beaconInterval = body.getBeaconInterval();

    //XXX where to get this from?
    //ap->rxPower = ...
    // get the AP from Radio.cc
    // get AP

}

} // namespace ieee80211

} // namespace inet

