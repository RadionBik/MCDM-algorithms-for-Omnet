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

#include "inet/linklayer/ieee80211/mgmt/Ieee80211MgmtAP.h"

#include "inet/linklayer/ieee80211/mac/Ieee80211Frame_m.h"
#include "inet/linklayer/common/Ieee802Ctrl.h"

#ifdef WITH_ETHERNET
#include "inet/linklayer/ethernet/EtherFrame.h"
#endif // ifdef WITH_ETHERNET

#include "inet/physicallayer/ieee80211/packetlevel/Ieee80211Radio.h"
#include "inet/linklayer/ieee80211/mgmt/Ieee80211AgentSTA.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/NotifierConsts.h"
#include <boost/lexical_cast.hpp>

namespace inet {

namespace ieee80211 {

using namespace physicallayer;

simsignal_t Ieee80211MgmtAP::availBitrateSignal = registerSignal(
        "availBitrate");
simsignal_t Ieee80211MgmtAP::loadBalCoeffSignal = registerSignal(
        "loadBalCoeff");

simsignal_t Ieee80211MgmtAP::currentLoadSignal = registerSignal("currentLoad");

Define_Module(Ieee80211MgmtAP);
Register_Class(Ieee80211MgmtAP::NotificationInfoSta);

static std::ostream& operator<<(std::ostream& os,
        const Ieee80211MgmtAP::STAInfo& sta) {
    os << "state:" << sta.status;
    return os;
}

Ieee80211MgmtAP::~Ieee80211MgmtAP() {
    cancelAndDelete(beaconTimer);
}

void Ieee80211MgmtAP::initialize(int stage) {
    Ieee80211MgmtAPBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        // read params and init vars

        //int cost = par("cost");
        //EV << "cost:" << par("cost") << "\n";

        int cost = 0;

        EV << "111222 this->getFullName()" << this->getFullName() << "\n";
        EV << "111222 this->getParentModule()->getFullName()"
                  << this->getParentModule()->getFullName() << "\n";
        EV << "111222 this->getParentModule()->getParentModule()->getFullName()"
                  << this->getParentModule()->getParentModule()->getFullName()
                  << "\n";
        simulationName =
                this->getParentModule()->getParentModule()->getParentModule()->getFullName();
        EV << "111222 simulationName: " << simulationName << "\n";

        appBitrateRescalingFactor =
                this->getParentModule()->getParentModule()->getParentModule()->par(
                        "appBitrateRescalingFactor").doubleValue();
        EV << "111222 appBitrateRescalingFactor: " << appBitrateRescalingFactor
                  << endl;

        apName = this->getParentModule()->getParentModule()->getFullName();
        EV << "apName: " << apName << "\n";

        cost = par("cost");
        EV << "cost: " << cost << "\n";

        EV << "this->getFullPath(): " << this->getFullPath() << "\n";

        bitrate = this->getParentModule()->par("bitrate");
        EV << "bitrate: " << bitrate << "\n";

        EV << " 111333 getSimulation()->getFullName(); : "
                  << getSimulation()->getFullName() << "\n";
        EV << " 111333 getSimulation()->getFullName(); : "
                  << getSimulation()->getClassName() << "\n";
        EV << " 111333 getSimulation()->getFullName(); : "
                  << getSimulation()->getName() << "\n";

        //EV << " 111333 ev.getConfig()->getConfigValue(sim-time-limit)" << ev.getConfig()->getConfigValue("sim-time-limit")  << "\n";
        //EV << " 111333 ev.getConfig()->getConfigValue(test)" << ev.getConfig()->getConfigValue("test")  << "\n";

        ssid = par("ssid").stringValue();
        beaconInterval = par("beaconInterval");
        numAuthSteps = par("numAuthSteps");
        if (numAuthSteps != 2 && numAuthSteps != 4)
            throw cRuntimeError(
                    "parameter 'numAuthSteps' (number of frames exchanged during authentication) must be 2 or 4, not %d",
                    numAuthSteps);
        channelNumber = -1; // value will arrive from physical layer in receiveChangeNotification()
        WATCH(ssid);
        WATCH(channelNumber);
        WATCH(beaconInterval);
        WATCH(numAuthSteps);
        WATCH_MAP(staList);

        // EEW a workaround, put the AP cost into supported rates
        //bss.setDelay(ap->supportedRates.rate[0]);
        //bss.setJitter(ap->supportedRates.rate[1]);
        //bss.setPacketLoss(ap->supportedRates.rate[2]);
        //bss.setThroughputAP(ap->supportedRates.rate[3]);
        //bss.setThroughputAPOfAssocSTAs(ap->supportedRates.rate[7]);
        //bss.setThroughputSTA(ap->supportedRates.rate[6]);
        //bss.setMaxBitrate(ap->supportedRates.rate[4]);
        //bss.setCost(ap->supportedRates.rate[5]);

        supportedRates.rate[4] = bitrate;
        supportedRates.rate[5] = cost;

        //TBD fill in supportedRates

        // subscribe for notifications
        cModule *radioModule = getModuleFromPar<cModule>(par("radioModule"),
                this);
        radioModule->subscribe(Ieee80211Radio::radioChannelChangedSignal, this);

        /*cModule *mgmtModule = getModuleFromPar<cModule>(par("mgmt"),
         this);
         mgmtModule->subscribe(Ieee80211MgmtAP::currentLoadSignal, this);*/

        // EEW
        getSimulation()->getSystemModule()->subscribe(
                Ieee80211AgentSTA::disassociateSTASignal, this);
        //!!! we subscribe only for the AP1
        // we calculate the network loadBalancingIndex only on AP1 !!!
        if (apName.compare("AP1") == 0) {
            getSimulation()->getSystemModule()->subscribe(
                    Ieee80211MgmtAP::currentLoadSignal, this);
        }

        // start beacon timer (randomize startup time)
        beaconTimer = new cMessage("beaconTimer");
        availBitrateCollectionTimer = new cMessage(
                "availBitrateCollectionTimer");
        loadBalanceIndexCalcTimer = new cMessage("loadBalanceIndexCalcTimer");

    } else if (stage == INITSTAGE_LINK_LAYER) {
        if (isOperational) {
            scheduleAt(simTime() + uniform(0, beaconInterval), beaconTimer);
            // collect availBitrate each second
            scheduleAt(simTime() + 1, availBitrateCollectionTimer);
            scheduleAt(simTime() + 1, loadBalanceIndexCalcTimer);
        }
    }
}

void Ieee80211MgmtAP::handleTimer(cMessage *msg) {
    std::string currentLoadSignalMsg = ""; // apName;currentRelativeLoad
    double loadBalacingCoeff = 0;

    double avrLoad;
    double sumOfLoads = 0;
    double sumOfLoadsSquares = 0;
    double signallingBitrate = 10000; // kbps

    if (msg == beaconTimer) {
        sendBeacon();
        scheduleAt(simTime() + beaconInterval, beaconTimer);
    } else if (msg == availBitrateCollectionTimer) {
        getWirelessHostsAndTheirApps();
        // calculate the throughput generated by all associated STAs
        configuredThroughputAssocSTAs = signallingBitrate
                + getConfiguredThroughputOfAccociatedSTAs()
                        / appBitrateRescalingFactor;

        availBitrate = bitrate - configuredThroughputAssocSTAs;

        EV << " 5555555 --- --- ---" << endl;
        EV << " 5555555 --- maxBitrate:" << bitrate << endl;
        EV << " 5555555 --- configuredThroughputAssocSTAs:"
                  << configuredThroughputAssocSTAs << endl;
        EV << " 5555555 --- availBitrate:" << availBitrate << endl;

        if (availBitrate < 0) {
            availBitrate = 0;
        }
        emit(availBitrateSignal, availBitrate);

        currentRelativeLoad = configuredThroughputAssocSTAs / bitrate;
        if (currentRelativeLoad > 1) {
            currentRelativeLoad = 1;
        }
        currentLoadSignalMsg = apName + ";"
                + boost::lexical_cast<std::string>(currentRelativeLoad);
        emit(currentLoadSignal, currentLoadSignalMsg.c_str());
        scheduleAt(simTime() + 1, availBitrateCollectionTimer);
    } else if (msg == loadBalanceIndexCalcTimer) {
        if (apName.compare("AP1") == 0) {
            // calculate here the load balanceIndex
            // and write them to statistics
            // walk through the map of APs and current load
            EV << " 555555 --- calculate load balancing index:" << endl;
            EV << " 555555 --- simtime()" << simTime() << endl;
            EV << " 555555 --- all APs:" << endl;
            avrLoad = 0;
            loadBalacingCoeff = 0;
            currentLoadApBuff.clear();
            for (std::map<std::string, double>::iterator ii =
                    apCurrentLoadMap.begin(); ii != apCurrentLoadMap.end();
                    ++ii) {
                /* Output first (which is the string) and second (which is the char) */
                EV << " 555555 apName:" << ii->first
                          << " current relative Load: " << ii->second << endl;
                avrLoad += ii->second;
                currentLoadApBuff.push_back(ii->second);
            }
            avrLoad = avrLoad / apCurrentLoadMap.size();
            EV << " 555555 avrLoad:" << avrLoad << endl;
            //

            sumOfLoads = 0;
            sumOfLoadsSquares = 0;
            for (int y = 0; y < currentLoadApBuff.size(); y++) {
                EV << " 555555 currentLoadApBuff[] " << currentLoadApBuff[y]
                          << endl;
                //loadBalacingCoeff = loadBalacingCoeff
                //        + std::pow(currentLoadApBuff[y] - avrLoad, 2);

                // sumOfLoads sumOfLoadsSquares
                sumOfLoads += currentLoadApBuff[y];
                sumOfLoadsSquares += std::pow(currentLoadApBuff[y], 2);
                if (sumOfLoadsSquares == 0) {
                    sumOfLoads = 1;
                    sumOfLoadsSquares = 1;
                }
            }

            loadBalacingCoeff = std::pow(sumOfLoads, 2)
                    / (currentLoadApBuff.size() * sumOfLoadsSquares);

            if (loadBalacingCoeffPrev == 0) {
                loadBalacingCoeffPrev = loadBalacingCoeff;
            }

            EV << " 555555 sumOfLoads: " << sumOfLoads << endl;
            EV << " 555555 sumOfLoadsSquares: " << sumOfLoadsSquares << endl;
            EV << " 555555 loadBalacingCoeffPrev=" << loadBalacingCoeffPrev << endl;
            EV << " 555555 loadBalacingCoeff=" << loadBalacingCoeff << endl;

            if ((loadBalacingCoeff / loadBalacingCoeffPrev) > 1.5) {
                // may be we have a spike here
                EV
                          << " 555555     ooops...     it looks like we have a spike here"
                          << endl;
                EV << " 555555     ooops...     don't log this value" << endl;
            } else {
                emit(loadBalCoeffSignal, loadBalacingCoeff);
            }
            loadBalacingCoeffPrev = loadBalacingCoeff;

            scheduleAt(simTime() + 1, loadBalanceIndexCalcTimer);
        } // if apName
    } else {
        throw cRuntimeError("internal error: unrecognized timer '%s'",
                msg->getName());
    }

}

void Ieee80211MgmtAP::handleUpperMessage(cPacket *msg) {
    Ieee80211DataFrame *frame = encapsulate(msg);
    MACAddress macAddr = frame->getReceiverAddress();
    if (!macAddr.isMulticast()) {
        auto it = staList.find(macAddr);
        if (it == staList.end() || it->second.status != ASSOCIATED) {
            EV << "STA with MAC address " << macAddr
                      << " not associated with this AP, dropping frame\n";
            delete frame;    // XXX count drops?
            return;
        }
    }

    sendOrEnqueue(frame);
}

void Ieee80211MgmtAP::handleCommand(int msgkind, cObject *ctrl) {
    throw cRuntimeError("handleCommand(): no commands supported");
}

void Ieee80211MgmtAP::receiveSignal(cComponent *source, simsignal_t signalID,
        long value) {
    Enter_Method_Silent
    ();
    if (signalID == Ieee80211Radio::radioChannelChangedSignal) {
        EV << "updating channel number\n";
        channelNumber = value;
    }
    //if (signalID == Ieee80211AgentSTA::disassociateSTASignal) {
    //EV << "disassociateSTASignal received, value: " << value << "\n";

    //}
}

void Ieee80211MgmtAP::receiveSignal(cComponent *source, simsignal_t signalID,
        const char* value) {
    Enter_Method_Silent
    ();
    std::string valueStr = "";
    std::string apAddressStr = "";
    std::string apAddressDisassoc = "";
    std::string staAddressDisassoc = "";

    std::string apNameLoad = "";
    std::string currentLoadOnApStr = "";
    double currentLoadOnAp = 0;
    std::size_t pos;

    if (signalID == Ieee80211AgentSTA::disassociateSTASignal) {
        // check if the disassociation signal is for this AP
        // if yes, set the STA status to the NOT_ASSOCIATED

        EV << "disassociateSTASignal received, value: " << value << "\n";
        EV << "disassociateSTASignal received, this->getFullName(): "
                  << this->getFullName() << "\n";
        EV << "disassociateSTASignal received, this->getFullPath(): "
                  << this->getFullPath() << "\n";
        valueStr = value;
        apAddressDisassoc = valueStr.substr(0, valueStr.find(","));
        staAddressDisassoc = valueStr.substr(valueStr.find(",") + 1,
                valueStr.size());
        EV << "disassociateSTASignal received, apAddressDisassoc: "
                  << apAddressDisassoc << "\n";
        EV << "disassociateSTASignal received, staAddressDisassoc: ---"
                  << staAddressDisassoc << "---\n";

        if (!myAddress.str().compare(apAddressDisassoc)) {
            EV
                      << "disassociateSTASignal received, The disassociation signal is designed for that AP!"
                      << "\n";
            updateStaList(staAddressDisassoc);
        }

        // remove diassociatedSTA from staList
    } else if (signalID == Ieee80211MgmtAP::currentLoadSignal) {
        // calculate here the load balancing index

        // let only AP1 collect the current load
        if (apName.compare("AP1") == 0) {
            // Collect the curentLoad from each AP
            valueStr = value;
            EV << " 7777 --- --- " << "\n";
            EV << " 7777 Received current load from the following AP: "
                      << valueStr << "\n";

            apNameLoad = valueStr.substr(0, valueStr.find(";"));
            currentLoadOnApStr = valueStr.substr(valueStr.find(";") + 1,
                    valueStr.size());
            currentLoadOnAp = boost::lexical_cast<double>(currentLoadOnApStr);
            EV << " 7777 apNameLoad: " << apNameLoad << endl;
            EV << " 7777 currentLoadOnApStr: " << currentLoadOnApStr << endl;
            EV << " 7777 currentLoadOnAp: " << currentLoadOnAp << endl;
            //updateAPsCurrentLoadMap(apNameLoad currentLoadOnAp);
            // this is the relative load on AP, relative to the AP max load
            apCurrentLoadMap[apNameLoad] = currentLoadOnAp;

            EV << " 7777 --- calculate load balancing index:" << endl;
            EV << " 7777 --- all APs:" << endl;
            for (std::map<std::string, double>::iterator ii =
                    apCurrentLoadMap.begin(); ii != apCurrentLoadMap.end();
                    ++ii) {
                /* Output first (which is the string) and second (which is the char) */
                EV << " 7777 apName:" << ii->first << " currentLoad: "
                          << ii->second << endl;
            }

        }
    }
}

void Ieee80211MgmtAP::updateStaList(std::string _staAddressDisassoc) {

    for (std::map<MACAddress, STAInfo, MAC_compare>::iterator iter =
            staList.begin(); iter != staList.end(); ++iter) {
        EV << " disassociateSTASignal staList entry: " << (*iter).first << endl;
        EV << " disassociateSTASignal staList entry: " << (*iter).second.status
                  << endl;
    }

    MACAddress macAddr;
    macAddr.setAddress(_staAddressDisassoc.c_str());
    auto it = staList.find(macAddr);
    if (it == staList.end()) {

    } else {
        EV << " disassociateSTASignal STA with MAC address found!!!" << macAddr
                  << " \n";
        EV
                  << " disassociateSTASignal set the STA status to NOT_AUTHENTICATED\n";
        it->second.status = NOT_AUTHENTICATED;
    }

}

Ieee80211MgmtAP::STAInfo *Ieee80211MgmtAP::lookupSenderSTA(
        Ieee80211ManagementFrame *frame) {
    auto it = staList.find(frame->getTransmitterAddress());
    return it == staList.end() ? nullptr : &(it->second);
}

void Ieee80211MgmtAP::sendManagementFrame(Ieee80211ManagementFrame *frame,
        const MACAddress& destAddr) {
    frame->setFromDS(true);
    frame->setReceiverAddress(destAddr);
    frame->setAddress3(myAddress);
    sendOrEnqueue(frame);
}

double Ieee80211MgmtAP::getThroughputGeneratedByNode(
        const MACAddress& _macAddress) {

    double resultThroughput = 0;
// get the node configuration by mac
//EV << "this->getFullPath(): " << this->getModuleByPath("wirelessClientX") << "\n";
    cModule* mod = getSimulation()->getModuleByPath(
            "MyTests_Wireless_v11.wirelessClientX");
//INetworkNode *myHost = check_and_cast<INetworkNode *>(mod);
    if (mod->getSubmodule("udpApp", 0)) {
        // FIXME
        //if (mod->getSubmodule("udpApp", 0)->getSubmodule("UDPBasicApp",0)) {
        EV << "1 myHost->getSubmodule : "
                  << mod->getSubmodule("udpApp", 0)->getFullName() << "\n";
        EV << "2 myHost->getSubmodule : "
                  << mod->getSubmodule("udpApp", 0)->par("sendInterval").doubleValue()
                  << "\n";
        EV << "3 myHost->getSubmodule : "
                  << mod->getSubmodule("udpApp", 0)->par("messageLength").doubleValue()
                  << "\n";
    }

    return resultThroughput;
}

void Ieee80211MgmtAP::getWirelessHostsAndTheirApps() {
    // fill staConfiguredList here
    // get all mobile nodes inclusive their apps and put them into staConfiguredList
    int i = 0;
    int appId = 0;
    std::string moduleTypeName = "";
    std::string macAddressStr = "";
    std::string appType = "";
    double numUdpApps = 0;
    double messageLength = 0;
    double sendInterval = 0;
    double startTime = 0;
    double stopTime = 0;
    EV << "getSimulation()->getLastModuleId():"
              << getSimulation()->getLastModuleId() << endl;
    for (i = 0; i < getSimulation()->getLastModuleId(); ++i) {
        if (!getSimulation()->getModule(i)) {
            continue;
        }
        EV << " ini file module id:" << i << endl;
        EV << " ini file getSimulation()->getModule(i)->getFullName():"
                  << getSimulation()->getModule(i)->getFullName() << endl;
        EV << " ini file getSimulation()->getModule(i)->getFullName():"
                  << getSimulation()->getModule(i)->getComponentType()->getName()
                  << endl;

        // fill list with configured STA
        moduleTypeName =
                getSimulation()->getModule(i)->getComponentType()->getName();
        if (!moduleTypeName.compare("WirelessHost")) {

            //EV << " ini file we have a wireless host module configuration here"
            //          << endl;

            //MACAddress staAddress
            macAddressStr = getSimulation()->getModule(i)->getSubmodule("wlan",
                    0)->getSubmodule("mac", 0)->par("address").stringValue();
            //EV << " ini file macAddressStr:" << macAddressStr << endl;

            // create a map entry
            MACAddress staAddress;
            staAddress.setAddress(macAddressStr.c_str());
            //setAddress(macAddressStr);

            STAInfoDetailed *sta;
            sta = &staConfiguredList[staAddress]; // this implicitly creates a new entry
            sta->address = staAddress;

            APPList appListBuff;

            // get all configured APPs for this STA
            numUdpApps =
                    getSimulation()->getModule(i)->par("numUdpApps").doubleValue();
            //EV << " ini file numUdpApps:" << numUdpApps << endl;

            // get all APPs of the mobile node in the list
            for (appId = 0; appId < numUdpApps; ++appId) {
                messageLength = getSimulation()->getModule(i)->getSubmodule(
                        "udpApp", appId)->par("messageLength").doubleValue();
                sendInterval = getSimulation()->getModule(i)->getSubmodule(
                        "udpApp", appId)->par("sendInterval").doubleValue();
                startTime = getSimulation()->getModule(i)->getSubmodule(
                        "udpApp", appId)->par("startTime").doubleValue();
                stopTime = getSimulation()->getModule(i)->getSubmodule("udpApp",
                        appId)->par("stopTime").doubleValue();

                /*EV << " ini file appId:" << appId << endl;
                 EV << " ini file messageLength:" << messageLength << endl;
                 EV << " ini file sendInterval:" << sendInterval << endl;
                 EV << " ini file startTime:" << startTime << endl;
                 EV << " ini file stopTime:" << stopTime << endl;*/

                // collect appInfo
                APPInfo appInfo;
                appType = "udpApp";
                appInfo.appType = appType;
                appInfo.messageLength = messageLength;
                appInfo.sendInterval = sendInterval;
                appInfo.startTime = startTime;
                appInfo.stopTime = stopTime;
                appListBuff[appId] = appInfo;

            } // if appId
              // put the apListBuff into the staConfiguredList
            sta->appList = appListBuff;
        } // if WirellessHost
    }

    // just print out the content of staConfiguredList for check
    APPList appListBuff2;
    for (std::map<MACAddress, STAInfoDetailed, MAC_compare>::iterator iter =
            staConfiguredList.begin(); iter != staConfiguredList.end();
            ++iter) {
        EV << " 909090 staConfiguredList MACAddress: " << (*iter).first
                  << " value:" << (*iter).second.appList.size() << endl;

        //iterate appList
        appListBuff2 = (*iter).second.appList;
        for (std::map<int, APPInfo>::iterator iter2 = appListBuff2.begin();
                iter2 != appListBuff2.end(); ++iter2) {
            EV << " 909090 appId: " << (*iter2).first << endl;
            EV << " 909090 appType: " << (*iter2).second.appType << endl;
            EV << " 909090 messageLength: " << (*iter2).second.messageLength
                      << endl;
            EV << " 909090 sendInterval: " << (*iter2).second.sendInterval
                      << endl;
            EV << " 909090 startTime: " << (*iter2).second.startTime << endl;
            EV << " 909090 stopTime: " << (*iter2).second.stopTime << endl;
        }
    }
}

double Ieee80211MgmtAP::getConfiguredThroughputOfAccociatedSTAs() {
    double configuredThroughputSTAs = 0;
    double configuredThroughputAPP = 0;

    APPList appListBuff;
    bool appActive = false;

    EV << " 909091 --- --- " << " \n";
    EV << " 909091 ap name: " << this->getFullPath() << " \n";
    EV << " 909091 staList.size: " << staList.size() << endl;
    // get associated STAs
    for (std::map<MACAddress, STAInfo, MAC_compare>::iterator iter =
            staList.begin(); iter != staList.end(); ++iter) {
        EV << " 909091 the sta status: " << (*iter).second.status << endl;

        if ((*iter).second.status != ASSOCIATED) {
            EV << " 909091 the sta is not associated!" << endl;
            continue;
        }
        if ((*iter).second.status == NOT_AUTHENTICATED) {
            EV << " 909091 the sta is not authenticated!" << endl;
            continue;
        }

        EV << " 909091 associated staList : " << (*iter).first << ": "
                  << (*iter).second << endl;

        // find this host in the staConfiguredList
        auto it = staConfiguredList.find((*iter).first);
        if (it == staConfiguredList.end()) {
            EV << " 909091 configured STA with MAC address not found "
                      << (*iter).first << " \n";
        } else {
            EV << " 909091 configured STA with MAC address found "
                      << (*iter).first << " \n";
            // get the configured throughtput of this sta
            appListBuff = it->second.appList;

            // go through all apps and get configured throughput
            for (std::map<int, APPInfo>::iterator iter2 = appListBuff.begin();
                    iter2 != appListBuff.end(); ++iter2) {
                // check the if the app time
                appActive == false;

                if (iter2->second.startTime < 1
                        && iter2->second.stopTime > simTime().dbl()) {
                    appActive = true;
                } else if (iter2->second.startTime < simTime().dbl()
                        && iter2->second.stopTime < 1) {
                    appActive = true;
                } else if (iter2->second.startTime < 1
                        && iter2->second.stopTime < 1) {
                    appActive = true;
                } else if (iter2->second.startTime < simTime().dbl()
                        && iter2->second.stopTime > simTime().dbl()) {
                    // application is active
                    appActive = true;
                } else {
                    appActive = false;
                }

                if (appActive == true) {
                    configuredThroughputAPP = (iter2->second.messageLength
                            / iter2->second.sendInterval) * 8;
                    configuredThroughputSTAs = configuredThroughputSTAs
                            + configuredThroughputAPP;
                    EV << " 909091 configuredThroughputAPP of app "
                              << configuredThroughputAPP << " \n";
                }

            } // for app

        } // for mobile node
        EV << " 909091 configuredThroughputSTAs " << configuredThroughputSTAs
                  << " \n";

        // get throughput generated by node with the given mac

        // get configured throughput for each STA

        // get all application

        // get application time window

        // get sendInterval and packetSize

        // calculate bitrate

    }

    // get throughput of associated STA

    return configuredThroughputSTAs;
}

void Ieee80211MgmtAP::sendBeacon() {
    EV << "Sending beacon\n";

    Ieee80211BeaconFrame *frame = new Ieee80211BeaconFrame("Beacon");
    Ieee80211BeaconFrameBody& body = frame->getBody();
    body.setSSID(ssid.c_str());
    body.setSupportedRates(supportedRates);
    body.setBeaconInterval(beaconInterval);
    body.setChannelNumber(channelNumber);
    body.setBodyLength(
            8 + 2 + 2 + (2 + ssid.length()) + (2 + supportedRates.numRates));

    frame->setByteLength(28 + body.getBodyLength());
    frame->setReceiverAddress(MACAddress::BROADCAST_ADDRESS);
    frame->setFromDS(true);

    // get all wireless hosts inclusive their apps and put all the info into staConfiguredList
    getWirelessHostsAndTheirApps();
    // calculate the throughput generated by all associated STAs
    // staList
    configuredThroughputAssocSTAs = getConfiguredThroughputOfAccociatedSTAs();
    supportedRates.rate[7] = configuredThroughputAssocSTAs;

    EV << " 909091 configuredThroughputAssocSTAs from send beacon: "
              << configuredThroughputAssocSTAs << endl;

    sendOrEnqueue(frame);
}

void Ieee80211MgmtAP::handleDataFrame(Ieee80211DataFrame *frame) {
// check toDS bit
    if (!frame->getToDS()) {
        // looks like this is not for us - discard
        EV << "Frame is not for us (toDS=false) -- discarding\n";
        delete frame;
        return;
    }

// handle broadcast/multicast frames
    if (frame->getAddress3().isMulticast()) {
        EV << "Handling multicast frame\n";

        if (isConnectedToHL)
            sendToUpperLayer(frame->dup());

        distributeReceivedDataFrame(frame);
        return;
    }

// look up destination address in our STA list
    auto it = staList.find(frame->getAddress3());
    if (it == staList.end()) {
        // not our STA -- pass up frame to relayUnit for LAN bridging if we have one
        if (isConnectedToHL) {
            sendToUpperLayer(frame);
        } else {
            EV
                      << "Frame's destination address is not in our STA list -- dropping frame\n";
            delete frame;
        }
    } else {
        // dest address is our STA, but is it already associated?
        if (it->second.status == ASSOCIATED)
            distributeReceivedDataFrame(frame); // send it out to the destination STA
        else {
            EV
                      << "Frame's destination STA is not in associated state -- dropping frame\n";
            delete frame;
        }
    }
}

void Ieee80211MgmtAP::handleAuthenticationFrame(
        Ieee80211AuthenticationFrame *frame) {
    int frameAuthSeq = frame->getBody().getSequenceNumber();
    EV << "Processing Authentication frame, seqNum=" << frameAuthSeq << "\n";

// create STA entry if needed
    STAInfo *sta = lookupSenderSTA(frame);
    if (!sta) {
        MACAddress staAddress = frame->getTransmitterAddress();
        sta = &staList[staAddress];    // this implicitly creates a new entry
        sta->address = staAddress;
        sta->status = NOT_AUTHENTICATED;
        sta->authSeqExpected = 1;
    }

// reset authentication status, when starting a new auth sequence
// The statements below are added because the L2 handover time was greater than before when
// a STA wants to re-connect to an AP with which it was associated before. When the STA wants to
// associate again with the previous AP, then since the AP is already having an entry of the STA
// because of old association, and thus it is expecting an authentication frame number 3 but it
// receives authentication frame number 1 from STA, which will cause the AP to return an Auth-Error
// making the MN STA to start the handover process all over again.
    if (frameAuthSeq == 1) {
        if (sta->status == ASSOCIATED)
            sendDisAssocNotification(sta->address);
        sta->status = NOT_AUTHENTICATED;
        sta->authSeqExpected = 1;
    }

// check authentication sequence number is OK
    if (frameAuthSeq != sta->authSeqExpected) {
        // wrong sequence number: send error and return
        EV << "Wrong sequence number, " << sta->authSeqExpected
                  << " expected\n";
        Ieee80211AuthenticationFrame *resp = new Ieee80211AuthenticationFrame(
                "Auth-ERROR");
        resp->getBody().setStatusCode(SC_AUTH_OUT_OF_SEQ);
        sendManagementFrame(resp, frame->getTransmitterAddress());
        delete frame;
        sta->authSeqExpected = 1;    // go back to start square
        return;
    }

// station is authenticated if it made it through the required number of steps
    bool isLast = (frameAuthSeq + 1 == numAuthSteps);

// send OK response (we don't model the cryptography part, just assume
// successful authentication every time)
    EV << "Sending Authentication frame, seqNum=" << (frameAuthSeq + 1) << "\n";
    Ieee80211AuthenticationFrame *resp = new Ieee80211AuthenticationFrame(
            isLast ? "Auth-OK" : "Auth");
    resp->getBody().setSequenceNumber(frameAuthSeq + 1);
    resp->getBody().setStatusCode(SC_SUCCESSFUL);
    resp->getBody().setIsLast(isLast);
// XXX frame length could be increased to account for challenge text length etc.
    sendManagementFrame(resp, frame->getTransmitterAddress());

    delete frame;

// update status
    if (isLast) {
        if (sta->status == ASSOCIATED)
            sendDisAssocNotification(sta->address);
        sta->status = AUTHENTICATED;  // XXX only when ACK of this frame arrives
        EV << "STA authenticated\n";
    } else {
        sta->authSeqExpected += 2;
        EV << "Expecting Authentication frame " << sta->authSeqExpected << "\n";
    }
}

void Ieee80211MgmtAP::handleDeauthenticationFrame(
        Ieee80211DeauthenticationFrame *frame) {
    EV << "Processing deauthentication frame on AP\n";

    STAInfo *sta = lookupSenderSTA(frame);
    delete frame;

    if (sta) {
        // mark STA as not authenticated; alternatively, it could also be removed from staList
        if (sta->status == ASSOCIATED)
            sendDisAssocNotification(sta->address);
        sta->status = NOT_AUTHENTICATED;
        sta->authSeqExpected = 1;
    }
}

void Ieee80211MgmtAP::handleAssociationRequestFrame(
        Ieee80211AssociationRequestFrame *frame) {
    EV << "Processing AssociationRequest frame\n";

// "11.3.2 AP association procedures"
    STAInfo *sta = lookupSenderSTA(frame);
    if (!sta || sta->status == NOT_AUTHENTICATED) {
        // STA not authenticated: send error and return
        Ieee80211DeauthenticationFrame *resp =
                new Ieee80211DeauthenticationFrame("Deauth");
        resp->getBody().setReasonCode(RC_NONAUTH_ASS_REQUEST);
        sendManagementFrame(resp, frame->getTransmitterAddress());
        delete frame;
        return;
    }

    delete frame;

// mark STA as associated
    if (sta->status != ASSOCIATED)
        sendAssocNotification(sta->address);
    sta->status = ASSOCIATED; // XXX this should only take place when MAC receives the ACK for the response

// send OK response
    Ieee80211AssociationResponseFrame *resp =
            new Ieee80211AssociationResponseFrame("AssocResp-OK");
    Ieee80211AssociationResponseFrameBody& body = resp->getBody();
    body.setStatusCode(SC_SUCCESSFUL);
    body.setAid(0);    //XXX
    body.setSupportedRates(supportedRates);
    sendManagementFrame(resp, sta->address);
}

void Ieee80211MgmtAP::handleAssociationResponseFrame(
        Ieee80211AssociationResponseFrame *frame) {
    dropManagementFrame(frame);
}

void Ieee80211MgmtAP::handleReassociationRequestFrame(
        Ieee80211ReassociationRequestFrame *frame) {
    EV << "Processing ReassociationRequest frame\n";

// "11.3.4 AP reassociation procedures" -- almost the same as AssociationRequest processing
    STAInfo *sta = lookupSenderSTA(frame);
    if (!sta || sta->status == NOT_AUTHENTICATED) {
        // STA not authenticated: send error and return
        Ieee80211DeauthenticationFrame *resp =
                new Ieee80211DeauthenticationFrame("Deauth");
        resp->getBody().setReasonCode(RC_NONAUTH_ASS_REQUEST);
        sendManagementFrame(resp, frame->getTransmitterAddress());
        delete frame;
        return;
    }

    delete frame;

// mark STA as associated
    sta->status = ASSOCIATED; // XXX this should only take place when MAC receives the ACK for the response

// send OK response
    Ieee80211ReassociationResponseFrame *resp =
            new Ieee80211ReassociationResponseFrame("ReassocResp-OK");
    Ieee80211ReassociationResponseFrameBody& body = resp->getBody();
    body.setStatusCode(SC_SUCCESSFUL);
    body.setAid(0);    //XXX
    body.setSupportedRates(supportedRates);
    sendManagementFrame(resp, sta->address);
}

void Ieee80211MgmtAP::handleReassociationResponseFrame(
        Ieee80211ReassociationResponseFrame *frame) {
    dropManagementFrame(frame);
}

void Ieee80211MgmtAP::handleDisassociationFrame(
        Ieee80211DisassociationFrame *frame) {
    EV << "Handle disassociation on AP" << endl;

    STAInfo *sta = lookupSenderSTA(frame);
    delete frame;

    if (sta) {
        if (sta->status == ASSOCIATED)
            sendDisAssocNotification(sta->address);
        sta->status = AUTHENTICATED;
    }
}

void Ieee80211MgmtAP::handleBeaconFrame(Ieee80211BeaconFrame *frame) {
    dropManagementFrame(frame);
}

void Ieee80211MgmtAP::handleProbeRequestFrame(
        Ieee80211ProbeRequestFrame *frame) {
    EV << "Processing ProbeRequest frame\n";

    if (strcmp(frame->getBody().getSSID(), "") != 0
            && strcmp(frame->getBody().getSSID(), ssid.c_str()) != 0) {
        EV << "SSID `" << frame->getBody().getSSID()
                  << "' does not match, ignoring frame\n";
        dropManagementFrame(frame);
        return;
    }

    MACAddress staAddress = frame->getTransmitterAddress();
    delete frame;

    EV << "Sending ProbeResponse frame\n";
    Ieee80211ProbeResponseFrame *resp = new Ieee80211ProbeResponseFrame(
            "ProbeResp");
    Ieee80211ProbeResponseFrameBody& body = resp->getBody();
    body.setSSID(ssid.c_str());
    body.setSupportedRates(supportedRates);
    body.setBeaconInterval(beaconInterval);
    body.setChannelNumber(channelNumber);
    sendManagementFrame(resp, staAddress);
}

void Ieee80211MgmtAP::handleProbeResponseFrame(
        Ieee80211ProbeResponseFrame *frame) {
    dropManagementFrame(frame);
}

void Ieee80211MgmtAP::sendAssocNotification(const MACAddress& addr) {
    NotificationInfoSta notif;
    notif.setApAddress(myAddress);
    notif.setStaAddress(addr);
    emit(NF_L2_AP_ASSOCIATED, &notif);
}

void Ieee80211MgmtAP::sendDisAssocNotification(const MACAddress& addr) {
    NotificationInfoSta notif;
    notif.setApAddress(myAddress);
    notif.setStaAddress(addr);
    emit(NF_L2_AP_DISASSOCIATED, &notif);
}

void Ieee80211MgmtAP::start() {
    Ieee80211MgmtAPBase::start();
    scheduleAt(simTime() + uniform(0, beaconInterval), beaconTimer);
}

void Ieee80211MgmtAP::stop() {
    cancelEvent(beaconTimer);
    staList.clear();
    Ieee80211MgmtAPBase::stop();
}

} // namespace ieee80211

} // namespace inet

