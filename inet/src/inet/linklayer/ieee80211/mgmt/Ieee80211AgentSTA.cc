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

#include "inet/linklayer/ieee80211/mgmt/Ieee80211AgentSTA.h"
#include "inet/linklayer/ieee80211/mgmt/Ieee80211Primitives_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/NotifierConsts.h"
#include "inet/common/INETUtils.h"

#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include "mcda/MCDM_Algorithms.h"

namespace inet {

namespace ieee80211 {

Define_Module(Ieee80211AgentSTA);

#define MK_STARTUP    1
#define START_SCAN    3

simsignal_t Ieee80211AgentSTA::sentRequestSignal = registerSignal(
        "sentRequest");
simsignal_t Ieee80211AgentSTA::acceptConfirmSignal = registerSignal(
        "acceptConfirm");
simsignal_t Ieee80211AgentSTA::dropConfirmSignal = registerSignal(
        "dropConfirm");

simsignal_t Ieee80211AgentSTA::rssSignal = registerSignal("RSS");
simsignal_t Ieee80211AgentSTA::availBitrateSignal = registerSignal(
        "availBitrate");
simsignal_t Ieee80211AgentSTA::costSignal = registerSignal("cost");
simsignal_t Ieee80211AgentSTA::apIDSignal = registerSignal("apID");
simsignal_t Ieee80211AgentSTA::numberOfHOSignal = registerSignal("numberOfHO");
simsignal_t Ieee80211AgentSTA::currentEchoAppsBitrateSignal = registerSignal(
        "currentEchoAppsBitrate");
simsignal_t Ieee80211AgentSTA::relOverfOfAvailBitrateSignal = registerSignal(
        "relOverfOfAvailBitrate");

simsignal_t Ieee80211AgentSTA::disassociateSTASignal = registerSignal(
        "disassociateSTA");

simsignal_t Ieee80211AgentSTA::packetLossSignal = registerSignal("packetLoss");

//simsignal_t Ieee80211AgentSTA::numberOfHOSignal = registerSignal("numberOfHO");

void Ieee80211AgentSTA::initialize(int stage) {
    cSimpleModule::initialize(stage);

    EV << " 111444 initialization of AgentSTA " << "\n";

    if (stage == INITSTAGE_LOCAL) {
        // read parameters
        activeScan = par("activeScan");
        probeDelay = par("probeDelay");
        minChannelTime = par("minChannelTime");
        maxChannelTime = par("maxChannelTime");
        authenticationTimeout = par("authenticationTimeout");
        associationTimeout = par("associationTimeout");
        cStringTokenizer tokenizer(par("channelsToScan"));

        //EV << " 111444 initialization of AgentSTA " << "\n";
        //EV << " 111444 appBitrateRescalingFactor: " << appBitrateRescalingFactor << endl;

        // eew
        regularScanTimeout = this->par("regularScanTimeout");
        decisionAlgorithm = this->par("decisionAlgorithm").stringValue();
        mcdmAlgConfigFile = this->par("mcdmAlgConfigFile").stringValue();
        trafficType = this->par("trafficType").stringValue();

        const char *token;
        while ((token = tokenizer.nextToken()) != nullptr)
            channelsToScan.push_back(atoi(token));

        cModule *host = getContainingNode(this);
        host->subscribe(NF_L2_BEACON_LOST, this);

        // JcM add: get the default ssid, if there is one.
        default_ssid = par("default_ssid").stringValue();

        // start up: send scan request
        simtime_t startingTime = par("startingTime").doubleValue();
        if (startingTime < SIMTIME_ZERO)
            startingTime = uniform(SIMTIME_ZERO, maxChannelTime);
        scheduleAt(simTime() + startingTime,
                new cMessage("startUp", MK_STARTUP));
        myIface = nullptr;

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

    // FIXME
    // make sure the mac addresses are configured for all STAs
    EV << " 232323 this->getFullName()" << this->getFullName() << "\n";
    EV << " 232323 this->getFullPath()" << this->getFullPath() << "\n";
    EV << " 232323 this->getParentModule()" << this->getParentModule() << "\n";
    staMacAddressStr = this->getParentModule()->getSubmodule("mac", 0)->par(
            "address").stringValue();
    EV << " 232323 staMacAddressStr" << staMacAddressStr << "\n";

    appBitrateRescalingFactor =
            this->getParentModule()->getParentModule()->getParentModule()->par(
                    "appBitrateRescalingFactor").doubleValue();
    EV << " 111444 appBitrateRescalingFactor: " << appBitrateRescalingFactor
              << endl;

}

void Ieee80211AgentSTA::getConfiguredUdpEchoApps() {
    // get the list of configured udpApps and put them into appList

    // get configured applications for this STA, put them into the appList
    // get required bitrate
    // FIXME the function will only work with the UdpEchoApp!!!
    // for other application types it will cause an error
    // get all APPs of the mobile node in the list
    double numUdpApps;
    int appId;
    double messageLength = 0;
    double sendInterval = 0;
    double startTime = 0;
    double stopTime = 0;
    double bitrateBuf = 0;
    std::string possibleBitratesKbpsStr = "";
    std::list<std::string> possibleBitratesKbpsStrList;
    std::string applicationName = "";
    std::string bitrateBufStr = "";

    int n = 0;

    this->getParentModule()->getParentModule()->par("numUdpApps");
    // get all configured APPs for this STA
    numUdpApps =
            this->getParentModule()->getParentModule()->par("numUdpApps").doubleValue();
    //EV << " ini file numUdpApps:" << numUdpApps << endl;

    for (appId = 0; appId < numUdpApps; ++appId) {
        possibleBitratesKbpsStrList.clear();
        /*if (this->getParentModule()->getParentModule()->getSubmodule("udpApp",
         appId)->par("messageLength")==nullptr) {
         continue;
         }
         if (this->getParentModule()->getParentModule()->getSubmodule("udpApp",
         appId)->par("sendInterval")==nullptr) {
         continue;
         }*/

        EV << " 21212121 --- --- --- \n";
        EV << " 21212121 this->getParentModule(): " << this->getParentModule()
                  << "\n";
        EV << " 21212121 getParentModule()->getParentModule(): "
                  << getParentModule()->getParentModule() << "\n";
        EV
                  << " 21212121 this->getParentModule()->getParentModule()->getSubmodule(udpApp, appId): "
                  << this->getParentModule()->getParentModule()->getSubmodule(
                          "udpApp", appId) << "\n";
        EV
                  << " 21212121 this->getParentModule()->getParentModule()->getSubmodule(udpApp, appId)->par(applicationName).stringValue(): "
                  << this->getParentModule()->getParentModule()->getSubmodule(
                          "udpApp", appId)->par("applicationName").stringValue()
                  << "\n";
        EV
                  << " 21212121 this->getParentModule()->getParentModule()->getSubmodule(udpApp, appId)->par(possibleBitratesKbps).doubleValue(): "
                  << this->getParentModule()->getParentModule()->getSubmodule(
                          "udpApp", appId)->par("possibleBitratesKbps").stringValue()
                  << "\n";
        EV
                  << " 21212121 this->getParentModule()->getParentModule()->getSubmodule(udpApp, appId)->par(sendInterval).doubleValue(): "
                  << this->getParentModule()->getParentModule()->getSubmodule(
                          "udpApp", appId)->par("sendInterval").doubleValue()
                  << "\n";
        EV
                  << " 21212121 this->getParentModule()->getParentModule()->getSubmodule(udpApp, appId)->par(startTime).doubleValue(): "
                  << this->getParentModule()->getParentModule()->getSubmodule(
                          "udpApp", appId)->par("startTime").doubleValue()
                  << "\n";

        applicationName =
                this->getParentModule()->getParentModule()->getSubmodule(
                        "udpApp", appId)->par("applicationName").stringValue();
        messageLength =
                this->getParentModule()->getParentModule()->getSubmodule(
                        "udpApp", appId)->par("messageLength").doubleValue();
        possibleBitratesKbpsStr =
                this->getParentModule()->getParentModule()->getSubmodule(
                        "udpApp", appId)->par("possibleBitratesKbps").stringValue();
        sendInterval = this->getParentModule()->getParentModule()->getSubmodule(
                "udpApp", appId)->par("sendInterval").doubleValue();
        startTime = this->getParentModule()->getParentModule()->getSubmodule(
                "udpApp", appId)->par("startTime").doubleValue();
        stopTime = this->getParentModule()->getParentModule()->getSubmodule(
                "udpApp", appId)->par("stopTime").doubleValue();

        boost::algorithm::trim(possibleBitratesKbpsStr);

        EV << "21212121 --- \n";
        EV << "21212121 applicationName: " << applicationName << "\n";
        EV << "21212121 possibleBitratesKbpsStr: -" << possibleBitratesKbpsStr
                  << "-\n";
        EV << "21212121 messageLength: " << messageLength << "\n";
        EV << "21212121 sendInterval: " << sendInterval << "\n";
        EV << "21212121 startTime: " << startTime << "\n";
        EV << "21212121 stopTime: " << stopTime << "\n";

        // ToDo
        // check if possibleBitratesKbpsStr have the format "\d+,\d+,\d+,\d+,\d+"

        // collect appInfo
        APPInfo appInfo;
        appInfo.appName = applicationName;
        appInfo.appType = "udpApp";
        appInfo.messageLength = messageLength;
        appInfo.sendInterval = sendInterval;
        appInfo.startTime = startTime;
        appInfo.stopTime = stopTime;

        EV << "21212121 !!! possibleBitratesKbpsStrList.size()="
                  << possibleBitratesKbpsStrList.size() << std::endl;

        if (possibleBitratesKbpsStr.empty()) {
            appList[appId] = appInfo;
            continue;
        }

        boost::split(possibleBitratesKbpsStrList, possibleBitratesKbpsStr,
                boost::is_any_of(","));

        EV << "21212121 !!! possibleBitratesKbpsStrList.size()" << std::endl;
        EV << "21212121 !!! possibleBitratesKbpsStrList.size()="
                  << possibleBitratesKbpsStrList.size() << std::endl;
        /*if (possibleBitratesKbpsStrList.size()==0){
         continue;
         }*/

        for (std::list<std::string>::iterator it =
                possibleBitratesKbpsStrList.begin();
                it != possibleBitratesKbpsStrList.end(); it++) {
            EV << " 21212121          possibleBitratesKbpsStrList: " << *it
                      << endl;
            bitrateBufStr = *it;
            boost::algorithm::trim(bitrateBufStr);
            EV << "21212121      bitrateBufStr" + bitrateBufStr << std::endl;

            if (bitrateBufStr.empty()) {
                EV_ERROR << "PossibleBitrate is incorrect!!!" + bitrateBufStr
                                << endl;
                EV_ERROR
                                << "Please correct this bitrate in yor .ini file and try again."
                                        + bitrateBufStr << endl;
                EV_ERROR << "We stop here" << endl;
                exit(0);
            }
            try {
                bitrateBuf = boost::lexical_cast<double>(bitrateBufStr);
                EV << "21212121      bitrateBuf: " << bitrateBuf << std::endl;
                appInfo.possibleBitratesKbpsList.push_front(bitrateBuf);
            } catch (boost::bad_lexical_cast const&) {
                EV_ERROR << "21212121 possibleBitrate is incorrect!!!" + *it
                                << endl;
                exit(0);
            }

        }
        appList[appId] = appInfo;
    } // for appId

    EV << " 909099 -----" << endl;
    EV << " 909099 app list" << endl;
    int x = 0;
    for (std::map<int, APPInfo>::iterator iter = appList.begin();
            iter != appList.end(); ++iter) {
        EV << " 909099 appId: " << (*iter).first << endl;
        EV << " 909099 appName: " << (*iter).second.appName << endl;
        EV << " 909099 appType: " << (*iter).second.appType << endl;
        EV << " 909099 messageLength: " << (*iter).second.messageLength << endl;
        EV << " 909099 possibleBitrates: " << endl;
        EV << " 909099 possibleBitratesKbpsList: "
                  << (*iter).second.possibleBitratesKbpsList.size() << endl;

        (*iter).second.possibleBitratesKbpsList.sort();
        for (std::list<double>::iterator it =
                (*iter).second.possibleBitratesKbpsList.begin();
                it != (*iter).second.possibleBitratesKbpsList.end(); it++) {
            EV << " 909099          possibleBitratesKbpsList: " << *it << endl;
        }
        EV << " 909099 sendInterval: " << (*iter).second.sendInterval << endl;
        EV << " 909099 startTime: " << (*iter).second.startTime << endl;
        EV << " 909099 stopTime: " << (*iter).second.stopTime << endl;
    }

}

void Ieee80211AgentSTA::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage())
        handleTimer(msg);
    else
        handleResponse(msg);
}

void Ieee80211AgentSTA::handleTimer(cMessage *msg) {

    cModule *host = getContainingNode(this);

    if (msg->getKind() == MK_STARTUP) {
        EV << "Starting up\n";
        sendScanRequest();
        scheduleAt(simTime() + regularScanTimeout,
                new cMessage("startScan", START_SCAN));
        delete msg;
    } else if (msg->getKind() == START_SCAN) {
        EV << simTime() << " Start scan " << host->getFullName()  << " \n";
        sendRegularScanRequest();
        scheduleAt(simTime() + regularScanTimeout,
                new cMessage("startScan", START_SCAN));
    } else {
        throw cRuntimeError("internal error: unrecognized timer '%s'",
                msg->getName());
    }
}

void Ieee80211AgentSTA::handleResponse(cMessage *msg) {
    cObject *ctrl = msg->removeControlInfo();
    delete msg;

    EV << "Processing confirmation from mgmt: " << ctrl->getClassName() << "\n";

    if (dynamic_cast<Ieee80211Prim_ScanConfirm *>(ctrl))
        processScanConfirm((Ieee80211Prim_ScanConfirm *) ctrl);
    else if (dynamic_cast<Ieee80211Prim_AuthenticateConfirm *>(ctrl))
        processAuthenticateConfirm((Ieee80211Prim_AuthenticateConfirm *) ctrl);
    else if (dynamic_cast<Ieee80211Prim_AssociateConfirm *>(ctrl))
        processAssociateConfirm((Ieee80211Prim_AssociateConfirm *) ctrl);
    else if (dynamic_cast<Ieee80211Prim_ReassociateConfirm *>(ctrl))
        processReassociateConfirm((Ieee80211Prim_ReassociateConfirm *) ctrl);
    else if (ctrl)
        throw cRuntimeError(
                "handleResponse(): unrecognized control info class `%s'",
                ctrl->getClassName());
    else
        throw cRuntimeError("handleResponse(): control info is nullptr");
    delete ctrl;
}

void Ieee80211AgentSTA::receiveSignal(cComponent *source, simsignal_t signalID,
        cObject *obj) {
    Enter_Method_Silent
    ();
    printNotificationBanner(signalID, obj);

    EV_TRACE << "Associated AP: " << this->default_ssid << endl;

    if (signalID == NF_L2_BEACON_LOST) {
        //XXX should check details if it's about this NIC
        EV << "beacon lost, starting scanning again\n";
        getContainingNode(this)->bubble("Beacon lost!");

        //sendDisassociateRequest();

        EV << " 1111111 \n";
        sendScanRequest();
        emit(NF_L2_DISASSOCIATED, myIface);
    } else if (signalID == START_SCAN) {
        EV << "Start scanning once more\n";
    }
}

void Ieee80211AgentSTA::sendRequest(Ieee80211PrimRequest *req) {
    cMessage *msg = new cMessage(req->getClassName());
    msg->setControlInfo(req);
    send(msg, "mgmtOut");
}

void Ieee80211AgentSTA::sendScanRequest() {
    EV << simTime() << " Sending ScanRequest primitive to mgmt\n";
    Ieee80211Prim_ScanRequest *req = new Ieee80211Prim_ScanRequest();

    req->setBSSType(BSSTYPE_INFRASTRUCTURE);
    req->setActiveScan(activeScan);
    req->setProbeDelay(probeDelay);
    req->setMinChannelTime(minChannelTime);
    req->setMaxChannelTime(maxChannelTime);
    req->setChannelListArraySize(channelsToScan.size());
    for (int i = 0; i < (int) channelsToScan.size(); i++)
        req->setChannelList(i, channelsToScan[i]);
    //XXX BSSID, SSID are left at default ("any")
    // EEW
    req->setRegularScan(false);
    emit(sentRequestSignal, PR_SCAN_REQUEST);
    sendRequest(req);
}

// EEW
void Ieee80211AgentSTA::sendRegularScanRequest() {
    EV << simTime()
              << " Sending ScanRequest primitive for REGULAR SCAN to mgmt\n";
    Ieee80211Prim_ScanRequest *req = new Ieee80211Prim_ScanRequest();
    req->setBSSType(BSSTYPE_INFRASTRUCTURE);
    req->setActiveScan(activeScan);
    req->setProbeDelay(probeDelay);
    req->setMinChannelTime(minChannelTime);
    req->setMaxChannelTime(maxChannelTime);
    req->setChannelListArraySize(channelsToScan.size());
    for (int i = 0; i < (int) channelsToScan.size(); i++)
        req->setChannelList(i, channelsToScan[i]);
    //XXX BSSID, SSID are left at default ("any")
    // EEW
    req->setRegularScan(true);
    emit(sentRequestSignal, PR_SCAN_REQUEST);
    sendRequest(req);
}

void Ieee80211AgentSTA::sendChangeChannelRequest(int _channelNumber) {
    EV << simTime()
              << " Sending ChangeChannelRequest primitive to mgmt. Channel number:"
              << _channelNumber << "\n";
    Ieee80211Prim_ChangeChannelRequest *req =
            new Ieee80211Prim_ChangeChannelRequest();
    req->setChannelNummer(_channelNumber);
    sendRequest(req);
}

void Ieee80211AgentSTA::sendAuthenticateRequest(const MACAddress& address) {
    EV << "Sending AuthenticateRequest primitive to mgmt\n";
    Ieee80211Prim_AuthenticateRequest *req =
            new Ieee80211Prim_AuthenticateRequest();
    req->setAddress(address);
    req->setTimeout(authenticationTimeout);
    emit(sentRequestSignal, PR_AUTHENTICATE_REQUEST);
    sendRequest(req);
}

void Ieee80211AgentSTA::sendDeauthenticateRequest(const MACAddress& address,
        int reasonCode) {
    EV << "Sending DeauthenticateRequest primitive to mgmt\n";
    Ieee80211Prim_DeauthenticateRequest *req =
            new Ieee80211Prim_DeauthenticateRequest();
    req->setAddress(address);
    req->setReasonCode(reasonCode);
    emit(sentRequestSignal, PR_DEAUTHENTICATE_REQUEST);
    sendRequest(req);
}

void Ieee80211AgentSTA::sendAssociateRequest(const MACAddress& address) {
    EV << "Sending AssociateRequest primitive to mgmt\n";
    Ieee80211Prim_AssociateRequest *req = new Ieee80211Prim_AssociateRequest();
    req->setAddress(address);
    req->setTimeout(associationTimeout);
    emit(sentRequestSignal, PR_ASSOCIATE_REQUEST);
    sendRequest(req);

}

void Ieee80211AgentSTA::sendReassociateRequest(const MACAddress& address) {
    EV << "Sending ReassociateRequest primitive to mgmt\n";
    Ieee80211Prim_ReassociateRequest *req =
            new Ieee80211Prim_ReassociateRequest();
    req->setAddress(address);
    req->setTimeout(associationTimeout);
    emit(sentRequestSignal, PR_REASSOCIATE_REQUEST);
    sendRequest(req);
}

void Ieee80211AgentSTA::sendDisassociateRequest(const MACAddress& address,
        int reasonCode) {
    EV << "Sending DisassociateRequest primitive to mgmt\n";
    Ieee80211Prim_DisassociateRequest *req =
            new Ieee80211Prim_DisassociateRequest();
    req->setAddress(address);
    req->setReasonCode(reasonCode);
    emit(sentRequestSignal, PR_DISASSOCIATE_REQUEST);
    sendRequest(req);
}

void Ieee80211AgentSTA::processScanConfirm(Ieee80211Prim_ScanConfirm *resp) {
    // choose best AP
    double relOverfOfAvailBitrate = 0;
    double requiredBitrate = 0;

    EV_TRACE << " 212121 choose best AP here" << endl;

    //double weights[6] { 0.1, 0.1, 0.3, 0.3, 0.2, 0.4 };
    // first two symbols of the AP's mac
    std::string apIDStr = "";

    int bssIndex = -1;

    //bssIndex = chooseBSS(resp);
    bssIndex = chooseBSS_new(resp);
    std::string ssidStrBuf = "";

    if (bssIndex == -1) {
        EV << " 212121 No (suitable) AP found, continue scanning\n";
        emit(dropConfirmSignal, PR_SCAN_CONFIRM);
        EV << " 222222 \n";
        // EEW FIXME, the scan will be triggered only by regular scan
        //sendScanRequest();
        return;
    }

    dumpAPList(resp);
    emit(acceptConfirmSignal, PR_SCAN_CONFIRM);

    Ieee80211Prim_BSSDescription& bssDesc = resp->getBssList(bssIndex);

    EV_TRACE << " 212121 --- list of APs " << endl;
    for (int i = 0; i < (int) resp->getBssListArraySize(); i++) {
        EV_TRACE << " 212121        AP BSSID: "
                        << resp->getBssList(i).getBSSID().str() << endl;
    }

    EV << " 212121 AP Current, mac address=" << prevAP << " \n";
    EV << " 212121 AP Choosen, mac address=" << bssDesc.getBSSID().str()
              << "\n";

    // get the AP id, fist two symbols of the MAC address
    apIDStr = std::string(bssDesc.getBSSID().str()).substr(0, 2);
    EV << " 212121 AP ID apIDStr" << apIDStr << "\n";

    if (prevAP == bssDesc.getBSSID()) {
        // if the we have the same AP
        EV << " 212121 we have the same AP" << " \n";
        //sendAuthenticateRequest(bssDesc.getBSSID());
        // go back the the channel of associated AP
        EV << " 212121 switch to channel: " << bssDesc.getChannelNumber()
                  << endl;
        sendChangeChannelRequest(bssDesc.getChannelNumber());
        // just put statistics in the log file
        EV << "Criteria parameters of current AP:" << simTime() << ";"
                  << bssDesc.getBSSID() << ";" << bssDesc.getRxPower_dBm()
                  << ";" << bssDesc.getDelay() << ";" << bssDesc.getJitter()
                  << ";" << bssDesc.getThroughputAP() << ";"
                  << bssDesc.getMaxBitrate() << ";" << bssDesc.getAvailBitrate()
                  << ";" << bssDesc.getCost() << ";" << numberOfHandovers << ";"
                  << endl;

        // get the AP avail bitrate and reconfigure the application bitrate respectively

        // change the application bitrate

    } else {
        numberOfHandovers++;
        emit(numberOfHOSignal, numberOfHandovers);
        EV << " 212121 we have a new AP" << " \n";

        // send signal to AP to deauthenticate the STA
        if (prevAP.getInt() != 0) {
            //emit(NF_L2_DISASSOCIATED, myIface);
            EV << " 212121 emit disassociateSTASignal" << "\n";
            std::string message = prevAP.str() + "," + staMacAddressStr;
            emit(disassociateSTASignal, message.c_str());
        }

        // authenticate
        EV << " 212121 sendAuthenticateRequest" << " \n";
        sendAuthenticateRequest(bssDesc.getBSSID());
        EV << " 212121 Criteria parameters of current AP:" << simTime() << ";"
                  << bssDesc.getBSSID() << ";" << bssDesc.getRxPower_dBm()
                  << ";" << bssDesc.getDelay() << ";" << bssDesc.getJitter()
                  << ";" << bssDesc.getThroughputAP() << ";"
                  << bssDesc.getMaxBitrate() << ";" << bssDesc.getAvailBitrate()
                  << ";" << bssDesc.getCost() << ";" << numberOfHandovers << ";"
                  << endl;

    }

    // emit signals for the statistics
    emit(rssSignal, bssDesc.getRxPower_dBm());
    emit(availBitrateSignal, bssDesc.getAvailBitrate());
    emit(costSignal, bssDesc.getCost());
    ssidStrBuf = bssDesc.getSSID();
    emit(apIDSignal, boost::lexical_cast<double>(apIDStr));
    emit(packetLossSignal, bssDesc.getPacketLoss());

    // calculate relative overfitting of the avail bitrate and save them into statistics
    //requiredBitrate = getRequiredBitrateForConfUdpEchoApps()/ appBitrateRescalingFactor;
    requiredBitrate = getMaxRequiredBitrateForConfUdpEchoApps();

    relOverfOfAvailBitrate = (bssDesc.getAvailBitrate() - requiredBitrate)
            / bssDesc.getMaxBitrate();
    if (relOverfOfAvailBitrate < 0) {
        relOverfOfAvailBitrate = 0;
    }

    EV << " 989898 --- ---" << endl;
    EV << " 989898 get RelOverfOfAvailBitrate" << endl;
    EV << " 989898 requiredBitrate:" << requiredBitrate << endl;
    EV << " 989898 bssDesc.getAvailBitrate():" << bssDesc.getAvailBitrate() << endl;
    EV << " 989898 bssDesc.getMaxBitrate():" << bssDesc.getMaxBitrate() << endl;
    EV << " 989898 RelOverfOfAvailBitrate:" << relOverfOfAvailBitrate << endl;

    emit(relOverfOfAvailBitrateSignal, relOverfOfAvailBitrate);

    // reconfigure the udp applications bitrate according to the avail bitrate
    //reconfAppsBitrates(&bssDesc);
    reconfAppsBitratesNew(&bssDesc);

}

void Ieee80211AgentSTA::reconfAppsBitratesNew(
        Ieee80211Prim_BSSDescription *bssDesc) {
    // get running apps
    cModule *host = getContainingNode(this);
    bool appActive = false;
    double availBitrate = 0;
    double newBitrate = 0;
    double bitrateResevation = 0.7;
    double messageLength = 0;
    double sendInterval = 0;

    EV << "919191 --- --- --- " << endl;
    EV << "919191 --- From reconfAppsBitratesNew: " << endl;
    EV << "919191 STA name: " << host->getFullName() << endl;

    for (std::map<int, APPInfo>::iterator iter = appList.begin();
            iter != appList.end(); ++iter) {
        newBitrate = 0;
        // check if app is active
        EV << " 919191 - appId: " << (*iter).first << endl;
        EV << " 919191 appName: " << (*iter).second.appName << endl;

        if (iter->second.startTime < 1
                && iter->second.stopTime > simTime().dbl()) {
            appActive = true;
        } else if (iter->second.startTime < simTime().dbl()
                && iter->second.stopTime < 1) {
            appActive = true;
        } else if (iter->second.startTime < 1 && iter->second.stopTime < 1) {
            appActive = true;
        } else if (iter->second.startTime < simTime().dbl()
                && iter->second.stopTime > simTime().dbl()) {
            // application is active
            appActive = true;
        } else {
            appActive = false;
        }

        if (!appActive) {
            EV << "919191 " << simTime() << "app is NOT active" << endl;

        } // if
        else {
            EV << "919191 " << simTime() << "app is active" << endl;

        }

        EV << "919191 iter->second.possibleBitratesKbpsList.size(): "
                  << iter->second.possibleBitratesKbpsList.size() << endl;

        if (appActive && iter->second.possibleBitratesKbpsList.size() > 0) {
            // check and eventually reconfigure app bitrate
            availBitrate = bssDesc->getAvailBitrate() * bitrateResevation;

            EV << "919191 " << "Check and eventually reconfigure app bitrate."
                      << endl;
            EV << "919191 " << "Avail bitrate=" << availBitrate << endl;
            messageLength = iter->second.messageLength;
            // walk through all possibleBitrates
            // sort in descending order
            iter->second.possibleBitratesKbpsList.sort(std::greater<double>());

            for (std::list<double>::iterator it =
                    iter->second.possibleBitratesKbpsList.begin();
                    it != iter->second.possibleBitratesKbpsList.end(); it++) {
                EV << " 919191          possibleBitratesKbpsList: " << *it
                          << endl;
                if (*it * 1000 < availBitrate) {
                    newBitrate = *it * 1000;
                    break;
                }
            }
        }

        EV << " 919191 newBitrate: " << newBitrate << endl;
        if (newBitrate != 0) {
            EV << " 919191 we are going to change the app bitrate " << endl;
            newBitrate = newBitrate * appBitrateRescalingFactor;
            EV << " 919191 rescalled newBitrate: " << newBitrate << endl;

            // change the message length
            if (boost::algorithm::contains((*iter).second.appName, "youtube")) {
                messageLength = 1200;
            } else if (boost::algorithm::contains((*iter).second.appName,
                    "skypeVoice")) {
                messageLength = 160;
            }

            // we just recalculate the sendInterval
            sendInterval = 1 / ((newBitrate / 8) / messageLength);

            EV << " 919191 messageLength=" << messageLength << endl;
            EV << " 919191 sendInterval=" << sendInterval << endl;

            cModule *host = getContainingNode(this);
            if (host->getSubmodule("udpApp", (*iter).first)) {
                host->getSubmodule("udpApp", (*iter).first)->par(
                        "messageLength").setLongValue(messageLength);
                host->getSubmodule("udpApp", (*iter).first)->par("sendInterval").setDoubleValue(
                        sendInterval);
            }
        }

        newBitrate = newBitrate * appBitrateRescalingFactor;

        //sendInterval = 1 / ((newBitrate / 8) / messageLength);

        /*EV_TRACE << " 919191 avail bitrate ----- :" << availBitrate << endl;
         EV_TRACE << " 919191 bitrate to be configured:" << newBitrate << endl;
         EV_TRACE << " 919191 messageLen to be set:" << messageLength << endl;
         EV_TRACE << " 919191 sendInterval to be set:" << sendInterval << endl;*/

        /*if (appIsActive(*iter) == true) {
         EV << "919191 app is active" << endl;
         }
         else {
         EV << "919191 app is NOT active" << endl;
         }*/
        //if ( appIsActive(*iter) == true && second.possibleBitratesKbpsList.size()>0) {
        // calculate new bitrate for these apps
        //setNewAppBitrate(Ieee80211Prim_BSSDescription *bssDesc, APPInfo);
        //}
        //
    } // for
}

void Ieee80211AgentSTA::dumpAPList(Ieee80211Prim_ScanConfirm *resp) {
    EV << "Received AP list:\n";
    for (int i = 0; i < (int) resp->getBssListArraySize(); i++) {
        Ieee80211Prim_BSSDescription& bssDesc = resp->getBssList(i);
        EV << "    " << i << ". " << " address=" << bssDesc.getBSSID()
                  << " channel=" << bssDesc.getChannelNumber() << " SSID="
                  << bssDesc.getSSID() << " beaconIntvl="
                  << bssDesc.getBeaconInterval() << " rxPower="
                  << bssDesc.getRxPower() << endl;
        // later: supportedRates
    }
}

// function for the new MCDM-Framework
int Ieee80211AgentSTA::chooseBSS_new(Ieee80211Prim_ScanConfirm *resp) {

    EV_TRACE << " chooseBSS --------- " << endl;



    double rxPower;
    double rxPower_dBm;
    double snir;
    double snir_dB;
    double availBitrate = 0;
    double APThroghputExeptAssociatedAP = 0;
    double throughputAPOfAssocSTAs = 0;

    double requiredBitrate = 0;

    double delayBuf = 0;
    double jitterBuf = 0;
    double packetLossBuf = 0;

    // FIXME
    /*if (delayBuf == 0)
            return 0;*/


    std::string allPathsCriteriaValues = "";
    std::string pathsCriteriaValues = "";
    std::string pathsCriteriaValues2 = "";

    std::string userName = "";
    std::string runningAppsStr = "";
    std::list<std::string> runningAppsStrList;
    std::string criteriaNamesStr = "";
    std::list<std::string> critValuesPerPathList;
    std::string critValuesPerPathStr = "";

    std::string rssStr = "";
    std::string delayStr = "";
    std::string jitterStr = "";
    std::string availBitrateStr = "";
    std::string costStr = "";
    std::string ltimeStr = "";
    std::stringstream strsBuff;

    std::vector<std::string> criteriaStr;

    if (resp->getBssListArraySize() == 0)
        return -1;

    cModule *host = getContainingNode(this);

    time_t ltime;
    time(&ltime);
    struct tm* timeinfo = gmtime(&ltime); /* Convert to UTC */
    ltime = mktime(timeinfo); /* Store as unix timestamp */

    strsBuff << ltime;
    ltimeStr = strsBuff.str();

    EV_TRACE << "from chooseBSS" << endl;
    EV_TRACE << " chooseBSS simTime(): " << simTime() << endl;
    EV_TRACE << " chooseBSS STA name: " << host->getFullName() << endl;

    // get configured udp apps
    //getConfiguredUdpEchoApps();

    userName = this->par("userName").stringValue();
    runningAppsStrList = getRunningAppsStr();

    if (runningAppsStrList.size() > 0) {
        runningAppsStr = boost::algorithm::join(runningAppsStrList, ",");
    } else {
        // FIXME
        runningAppsStr = "noRunningApps";
    }
    EV_TRACE << " chooseBSS runningAppsStrList.size(): "
                    << runningAppsStrList.size() << endl;
    EV_TRACE << " chooseBSS runningAppsStr: " << runningAppsStr << endl;

    // set criteria
    //criteriaNamesStr =
    //        "delay,jitter,packetLoss,availBitrate,cost,PoALoad,PoAMaxBR";
    criteriaNamesStr =
            "delay,jitter,packetLoss,availBitrate,cost,PoALoad,PoAMaxBR";
    EV_TRACE << " chooseBSS criteriaNamesStr: " << criteriaNamesStr << endl;

    // get strList of running applications
    // get strList of criteria
    requiredBitrate = getRequiredBitrateForConfUdpEchoApps()
            / appBitrateRescalingFactor;
    emit(currentEchoAppsBitrateSignal, requiredBitrate); // the current bitrate for the EchoApps
    EV_TRACE << " chooseBSS emit requiredBitrate = "<< requiredBitrate << endl;


//cModule *host = getContainingNode(this);
//EV_TRACE << "host->getFullName()" << host->getFullName() <<  endl;

//EV_TRACE << "Test=" << resp->getBssList(i).getRxPower() <<  endl;

// here, just choose the one with the greatest receive power
// TODO and which supports a good data rate we support
    int bestIndex = 0;

// check how many APs have feasible RSS
    int apWithNotFeasRSS = 0;

    EV_TRACE << " chooseBSS resp->getBssListArraySize() = "
                    << resp->getBssListArraySize() << endl;
    ;
    EV_TRACE << " chooseBSS --- Check how many APs have feasible RSS ---";

    for (int i = 0; i < (int) resp->getBssListArraySize(); i++) {
        EV_TRACE << " chooseBSS --- " << endl;
        EV_TRACE << " chooseBSS rxPower W: " << resp->getBssList(i).getRxPower()
                        << endl;
        EV_TRACE << " chooseBSS rxPower_dBm: "
                        << resp->getBssList(i).getRxPower_dBm() << endl;
        // FIXME set these values in the .ini files
        if (!(resp->getBssList(i).getRxPower_dBm() <= -30
                && resp->getBssList(i).getRxPower_dBm() >= -90)) {
                //&& resp->getBssList(i).getRxPower_dBm() >= -85)) {
            EV_TRACE << " chooseBSS !!! the RSS is too low" << endl;
            EV_TRACE << " chooseBSS !!! this AP will be ignored" << endl;
            apWithNotFeasRSS++;
            continue;
        }
    }
    if (apWithNotFeasRSS == resp->getBssListArraySize()) {
        EV_TRACE << " chooseBSS all APs have too week RSS";
        EV_TRACE << " chooseBSS scan has to be started once more";
        return -1;
    }

    decisionAttemptID++;

    for (int i = 0; i < (int) resp->getBssListArraySize(); i++) {
        EV_TRACE << " chooseBSS --- " << endl;
        availBitrate = 0;
        //EEW
        // here !!!
        // exactly here and not somewhere else
        // we need following information per AP/communication path:

        throughputAPOfAssocSTAs =
                resp->getBssList(i).getThroughputAPOfAssocSTAs()
                        / appBitrateRescalingFactor;

        if (resp->getBssList(i).getBSSID() == prevAP) {
            EV_TRACE << " chooseBSS this is associated AP " << endl;
            //APThroghputExeptAssociatedAP =
            //        resp->getBssList(i).getThroughputAPOfAssocSTAs()
            //                - requiredBitrate;
            APThroghputExeptAssociatedAP = throughputAPOfAssocSTAs
                    - requiredBitrate + 5000;
        } else {
            APThroghputExeptAssociatedAP = throughputAPOfAssocSTAs;
            //APThroghputExeptAssociatedAP =
            //        resp->getBssList(i).getThroughputAPOfAssocSTAs();
        }
        if (APThroghputExeptAssociatedAP <= 0) {
            APThroghputExeptAssociatedAP = 5000;
        }

        EV_TRACE << " chooseBSS access point MAC: "
                        << resp->getBssList(i).getBSSID() << endl;

        EV_TRACE << " chooseBSS access point SSID: "
                        << resp->getBssList(i).getSSID() << endl;

        // RSS
        //rxPower = resp->getBssList(i).getRxPower();
        //rxPower_dBm = 30 + 10 * log10(rxPower);
        EV_TRACE << " chooseBSS rxPower W: " << resp->getBssList(i).getRxPower()
                        << endl;
        EV_TRACE << " chooseBSS rxPower_dBm: "
                        << resp->getBssList(i).getRxPower_dBm() << endl;

        // SNIR
        snir = resp->getBssList(i).getSnir();
        snir_dB = 10 * log10(snir);
        EV_TRACE << " chooseBSS snir: " << snir << endl;
        EV_TRACE << " chooseBSS snir_dB: " << snir_dB << endl;

        // Delay
        EV_TRACE << " chooseBSS delay: " << resp->getBssList(i).getDelay()
                        << endl;
        // Jitter
        EV_TRACE << " chooseBSS jitter: " << resp->getBssList(i).getJitter()
                        << endl;
        // PacketLoss
        EV_TRACE << " chooseBSS packetLoss: "
                        << resp->getBssList(i).getPacketLoss() << endl;

        // throughput AP
        EV_TRACE << " chooseBSS throughput on AP (measured): "
                        << resp->getBssList(i).getThroughputAP() << endl;

        // throughput AP of associated STAs, this is not a measured value, but a value collected from the the .ini
        // this is a theoretical throughput of all associated STAs
        EV_TRACE
                        << " chooseBSS throughput on AP of associated STAs for configured upd echoApps: "
                        //<< resp->getBssList(i).getThroughputAPOfAssocSTAs()
                        << throughputAPOfAssocSTAs << endl;

        EV_TRACE
                        << " chooseBSS throughput on AP of associated STAs for configured upd echoApps except associated STA: "
                        << APThroghputExeptAssociatedAP << endl;

        // throughput STA
        EV_TRACE << " testiii throughput on STA (measured): "
                        << resp->getBssList(i).getThroughputSTA() << endl;

        // required bitrate for currents STA configured upd echo apps
        EV_TRACE
                        << " chooseBSS throughput on current STA for configured upd echoApps: "
                        << requiredBitrate << endl;

        // max bitrate
        EV_TRACE << " chooseBSS maximal bitrate AP: "
                        << resp->getBssList(i).getMaxBitrate() << endl;

        // FIXME set these values in the .ini files
        if (!(resp->getBssList(i).getRxPower_dBm() <= -30
                //&& resp->getBssList(i).getRxPower_dBm() >= -85)) {
                && resp->getBssList(i).getRxPower_dBm() >= -90)) {
            EV_TRACE << " chooseBSS !!! the RSS is to low" << endl;
            EV_TRACE << " chooseBSS !!! this AP will be ignored" << endl;
            //continue;
        }

        if (prevAP == resp->getBssList(i).getBSSID()) {
            // avail bitrate
            availBitrate = resp->getBssList(i).getMaxBitrate()
            //- resp->getBssList(i).getThroughputAPOfAssocSTAs()
                    - throughputAPOfAssocSTAs + requiredBitrate;
            //+ resp->getBssList(i).getThroughputSTA();
        } else {
            availBitrate = resp->getBssList(i).getMaxBitrate()
            //- resp->getBssList(i).getThroughputAPOfAssocSTAs();
                    - throughputAPOfAssocSTAs;
        }

        if (availBitrate > resp->getBssList(i).getMaxBitrate()
                || availBitrate < 0) {
            availBitrate = resp->getBssList(i).getMaxBitrate();
        }
        resp->getBssList(i).setAvailBitrate(availBitrate);
        EV_TRACE << " chooseBSS available bitrate: "
                        << resp->getBssList(i).getAvailBitrate() << endl;

        // Cost
        EV_TRACE << " chooseBSS cost: " << resp->getBssList(i).getCost()
                        << endl;

        // put all criteria values in this string in form:
        // RSS, delay, jitter, avail bitrate, cost
        // get unixtime

        //criteriaStr.push_back(ltimeStr);
        //criteriaStr.push_back(resp->getBssList(i).getBSSID().str());
        criteriaStr.push_back(
                boost::lexical_cast<std::string>(
                        resp->getBssList(i).getRxPower_dBm()));
//        criteriaStr.push_back(
//                boost::lexical_cast<std::string>(
//                        resp->getBssList(i).getDelay()));
//        criteriaStr.push_back(
//                boost::lexical_cast<std::string>(
//                        resp->getBssList(i).getJitter()));
        criteriaStr.push_back(
                boost::lexical_cast<std::string>(
                        resp->getBssList(i).getAvailBitrate()));
        criteriaStr.push_back(
                boost::lexical_cast<std::string>(
                        resp->getBssList(i).getCost()));

        //delayBuf = (std::round(resp->getBssList(i).getDelay()*1000000))/1000000;
        //EV_TRACE << " chooseBSS delayBuf: " << delayBuf                                << endl;

        critValuesPerPathList.push_back(ltimeStr);
        critValuesPerPathList.push_back(resp->getBssList(i).getBSSID().str());
        critValuesPerPathList.push_back(
                boost::lexical_cast<std::string>(
                        resp->getBssList(i).getDelay()));
        critValuesPerPathList.push_back(
                boost::lexical_cast<std::string>(
                        resp->getBssList(i).getJitter()));
        critValuesPerPathList.push_back(
                boost::lexical_cast<std::string>(
                        resp->getBssList(i).getPacketLoss()));
        critValuesPerPathList.push_back(
                boost::lexical_cast<std::string>(
                        resp->getBssList(i).getAvailBitrate()));
        critValuesPerPathList.push_back(
                boost::lexical_cast<std::string>(
                        resp->getBssList(i).getCost()));
        critValuesPerPathList.push_back(
                boost::lexical_cast<std::string>(APThroghputExeptAssociatedAP));
        critValuesPerPathList.push_back(
                boost::lexical_cast<std::string>(
                        resp->getBssList(i).getMaxBitrate()));

        critValuesPerPathStr = boost::algorithm::join(critValuesPerPathList,
                ",");
        EV_TRACE << " chooseBSS critValuesPerPathStr: " << critValuesPerPathStr
                        << endl;

        pathsCriteriaValues = "";
        //pathsCriteriaValues2 = "";
        pathsCriteriaValues = "";
        //pathsCriteriaValues2 = "";
        for (int a = 0; a < criteriaStr.size(); ++a) {
            //EV_TRACE << " chooseBSS criteriaStr[a]:" << criteriaStr[a] << endl;
            if (a == 0) {
                pathsCriteriaValues = pathsCriteriaValues + criteriaStr[a];
            } else {
                pathsCriteriaValues = pathsCriteriaValues + ","
                        + criteriaStr[a];
                //pathsCriteriaValues2 = pathsCriteriaValues2 + ","
                        //+ criteriaStr[a];
            }
        }

//        EV << "CriteriaValues for decision num:," << decisionAttemptID
//                  << pathsCriteriaValues2 << endl;
//
//        EV << "CriteriaValues for decision num:," << decisionAttemptID
//                  << pathsCriteriaValues2 << endl;

        criteriaStr.clear();
        critValuesPerPathList.clear();

        allPathsCriteriaValues = allPathsCriteriaValues + pathsCriteriaValues
                + ",";

        if (resp->getBssList(i).getRxPower()
                > resp->getBssList(bestIndex).getRxPower())
            bestIndex = i;
    } // for path

    EV_TRACE << " chooseBSS allPathsCriteriaValues:" << allPathsCriteriaValues
                    << endl;

// get best index here

    // getBestIndex
    // if the algorithms is NOT RSS, proceed with the MCDM algorithms

    if (resp->getBssListArraySize() == 1) {
        EV_TRACE << " chooseBSS we have only one AP in the list" << endl;
        EV_TRACE << " chooseBSS so return index 0 as the bestIndex" << endl;
        return 0;
    }

    if (decisionAlgorithm.compare("RSS")) {
        bestIndex = McdaAlg::selectAPWithMCDA(allPathsCriteriaValues,decisionAlgorithm,trafficType,3);
        //std::cout
        EV_TRACE<<"\n testiii"<<decisionAlgorithm<<"  "<<trafficType<<"  "
                <<allPathsCriteriaValues<<"\n"<<"bestIndex  "<<bestIndex<<"\n";
    }

    EV_TRACE << " chooseBSS decisionAlgorithm: " << decisionAlgorithm
                    << endl;
    EV_TRACE << " chooseBSS bestIndex: " << bestIndex << endl;
    return bestIndex;
}

std::list<std::string> Ieee80211AgentSTA::getRunningAppsStr() {
    std::list<std::string> runningAppsStrList;
    bool appActive = false;
    EV_TRACE << " chooseBSS ... from getRunningAppsStr() : " << endl;
    for (std::map<int, APPInfo>::iterator iter = appList.begin();
            iter != appList.end(); ++iter) {
        // check the if the app is active based on sim time and app active time
        appActive == false;
        if (iter->second.startTime < 1
                && iter->second.stopTime > simTime().dbl()) {
            appActive = true;
        } else if (iter->second.startTime < simTime().dbl()
                && iter->second.stopTime < 1) {
            appActive = true;
        } else if (iter->second.startTime < 1 && iter->second.stopTime < 1) {
            appActive = true;
        } else if (iter->second.startTime < simTime().dbl()
                && iter->second.stopTime > simTime().dbl()) {
            // application is active
            appActive = true;

        }
        if (appActive) {
            EV_TRACE << " chooseBSS ... activeApp name: "
                            << iter->second.appName << endl;
            runningAppsStrList.push_back(iter->second.appName);
        }

    } // for app

    return runningAppsStrList;
}



double Ieee80211AgentSTA::getRequiredBitrateForConfUdpEchoApps() {
    double configuredThroughputSTAs = 0;
    double configuredThroughputAPP = 0;

    APPList appListBuff;
    bool appActive = false;

    EV << " 909099 --- --- " << " \n";
    EV << " 909099 get the required bitrate for configured udp echo apps"
              << " \n";
    EV << " 909099  for the STA:" << staMacAddressStr << " \n";
    EV << " 909099  for the STA:" << this->getFullPath() << " \n";
// go through all apps and get configured throughput
    for (std::map<int, APPInfo>::iterator iter = appList.begin();
            iter != appList.end(); ++iter) {
        // check the if the app time
        appActive == false;

        if (iter->second.startTime < 1
                && iter->second.stopTime > simTime().dbl()) {
            appActive = true;
        } else if (iter->second.startTime < simTime().dbl()
                && iter->second.stopTime < 1) {
            appActive = true;
        } else if (iter->second.startTime < 1 && iter->second.stopTime < 1) {
            appActive = true;
        } else if (iter->second.startTime < simTime().dbl()
                && iter->second.stopTime > simTime().dbl()) {
            // application is active
            appActive = true;
        } else {
            appActive = false;
        }

        if (appActive == true) {
            configuredThroughputAPP = (iter->second.messageLength
                    / iter->second.sendInterval) * 8;
            configuredThroughputSTAs = configuredThroughputSTAs
                    + configuredThroughputAPP;
            EV << " 909091 configuredThroughputAPP of app "
                      << configuredThroughputAPP << " \n";
        }

    } // for app

    EV << " 909099 configuredThroughputSTAs " << configuredThroughputSTAs
              << " \n";

// get throughput generated by node with the given mac

// get configured throughput for each STA

// get all application

// get application time window

// get sendInterval and packetSize

// calculate bitrate

// get throughput of associated STA

    return configuredThroughputSTAs;
}

double Ieee80211AgentSTA::getMaxRequiredBitrateForConfUdpEchoApps() {
    // get running apps
    cModule *host = getContainingNode(this);
    bool appActive = false;
    double availBitrate = 0;
    double maxPossibleBitrateBuff = 0;
    double maxPossibleBitrateResult = 0;

    EV << "91919191 --- --- --- " << endl;
    EV << "91919191 --- From getMaxRequiredBitrateForConfUdpEchoApps: " << endl;
    EV << "91919191 STA name: " << host->getFullName() << endl;

    for (std::map<int, APPInfo>::iterator iter = appList.begin();
            iter != appList.end(); ++iter) {

        maxPossibleBitrateBuff = 0;

        // check if app is active
        EV << " 91919191 - appId: " << (*iter).first << endl;
        EV << " 91919191 appName: " << (*iter).second.appName << endl;

        if (iter->second.startTime < 1
                && iter->second.stopTime > simTime().dbl()) {
            appActive = true;
        } else if (iter->second.startTime < simTime().dbl()
                && iter->second.stopTime < 1) {
            appActive = true;
        } else if (iter->second.startTime < 1 && iter->second.stopTime < 1) {
            appActive = true;
        } else if (iter->second.startTime < simTime().dbl()
                && iter->second.stopTime > simTime().dbl()) {
            // application is active
            appActive = true;
        } else {
            appActive = false;
        }

        if (!appActive) {
            EV << "91919191 " << simTime() << "app is NOT active" << endl;

        } // if
        else {
            EV << "91919191 " << simTime() << "app is active" << endl;

        }

        EV << "91919191 iter->second.possibleBitratesKbpsList.size(): "
                  << iter->second.possibleBitratesKbpsList.size() << endl;

        if (appActive && iter->second.possibleBitratesKbpsList.size() > 0) {
            // check and eventually reconfigure app bitrate


            EV << "91919191 " << "Check and eventually reconfigure app bitrate."
                      << endl;
            EV << "91919191 " << "Avail bitrate=" << availBitrate << endl;

            // walk through all possibleBitrates
            // sort in descending order
            iter->second.possibleBitratesKbpsList.sort(std::greater<double>());

            for (std::list<double>::iterator it =
                    iter->second.possibleBitratesKbpsList.begin();
                    it != iter->second.possibleBitratesKbpsList.end(); it++) {
                maxPossibleBitrateBuff = *it*1000;
                break;
            }
            EV << " 91919191          maxPossibleBitrateBuff: " << maxPossibleBitrateBuff
                                      << endl;

        }
        maxPossibleBitrateResult = maxPossibleBitrateResult + maxPossibleBitrateBuff;
    }

    EV << " 91919191          maxPossibleBitrateResult: " << maxPossibleBitrateResult
                              << endl;

    return maxPossibleBitrateResult;
}



int Ieee80211AgentSTA::calculateOptimalPathMCDM(Ieee80211Prim_ScanConfirm *resp,
        std::string _allPathsCriteriaValues) {

}

int Ieee80211AgentSTA::calculateOptimalPathMCDM_new(
        Ieee80211Prim_ScanConfirm *resp,
        std::string _argumentsStringForMCDMJavaLib) {


}

int Ieee80211AgentSTA::chooseBSS_MCDA_2(NWinfo *nw1, NWinfo *nw2,
        double _rank[6])
//int Ieee80211AgentSTA::chooseBSS_MCDA_2(Ieee80211Prim_ScanConfirm *resp, NWinfo *nw1, NWinfo *nw2)
        {

}

//int Ieee80211AgentSTA::chooseBSS_MCDA(vector( std::map < std::string, std::string >)  _VectorCriteriaMap)
int Ieee80211AgentSTA::chooseBSS_MCDA(Ieee80211Prim_ScanConfirm *resp) {

}

void Ieee80211AgentSTA::processAuthenticateConfirm(
        Ieee80211Prim_AuthenticateConfirm *resp) {
    if (resp->getResultCode() != PRC_SUCCESS) {
        EV << "Authentication error\n";
        emit(dropConfirmSignal, PR_AUTHENTICATE_CONFIRM);

        // try scanning again, maybe we'll have better luck next time, possibly with a different AP
        EV << "Going back to scanning\n";
        EV << " 333333 \n";
        // EEW FIXME, the scan will be triggered only by regular scan
        //sendScanRequest();
    } else {
        EV << "Authentication successful, let's try to associate\n";
        emit(acceptConfirmSignal, PR_AUTHENTICATE_CONFIRM);
        sendAssociateRequest(resp->getAddress());
    }
}

void Ieee80211AgentSTA::processAssociateConfirm(
        Ieee80211Prim_AssociateConfirm *resp) {
    std::string bubbleText = "";

    if (resp->getResultCode() != PRC_SUCCESS) {
        EV << "Association error\n";
        emit(dropConfirmSignal, PR_ASSOCIATE_CONFIRM);

        // try scanning again, maybe we'll have better luck next time, possibly with a different AP
        EV << "Going back to scanning\n";
        EV << " 444444 \n";
        // EEW FIXME, the scan will be triggered only by regular scan
        // sendScanRequest();
    } else {
        EV << "Association successful\n";
        emit(acceptConfirmSignal, PR_ASSOCIATE_CONFIRM);
        // we are happy!

        getContainingNode(this)->bubble("Associated with AP");
        //bubbleText = "Associated with AP "+ resp->getAddress().str();
        //getContainingNode(this)->bubble(bubbleText.c_str());
        EV << " 21212121 AP Current, mac address=" << prevAP << " \n";
        EV << " 21212121 AP Choosen, resp->getAddress()=" << resp->getAddress()
                  << "\n";
        if (prevAP.isUnspecified() || prevAP != resp->getAddress()) {
            emit(NF_L2_ASSOCIATED_NEWAP, myIface); //XXX detail: InterfaceEntry?
            prevAP = resp->getAddress();

        } else {
            emit(NF_L2_ASSOCIATED_OLDAP, myIface);
        }

    }
}

void Ieee80211AgentSTA::processReassociateConfirm(
        Ieee80211Prim_ReassociateConfirm *resp) {
// treat the same way as AssociateConfirm
    if (resp->getResultCode() != PRC_SUCCESS) {
        EV << "Reassociation error\n";
        emit(dropConfirmSignal, PR_REASSOCIATE_CONFIRM);
        EV << "Going back to scanning\n";
        EV << " 555555 \n";
        // EEW FIXME, the scan will be triggered only by regular scan
        // sendScanRequest();
    } else {
        EV << "Reassociation successful\n";
        emit(NF_L2_ASSOCIATED_OLDAP, myIface); //XXX detail: InterfaceEntry?
        emit(acceptConfirmSignal, PR_REASSOCIATE_CONFIRM);
        // we are happy!
    }
}

} // namespace ieee80211

} // namespace inet

