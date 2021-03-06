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


package inet.linklayer.ieee80211.mgmt;

//
// Used in 802.11 infrastructure mode: in a station (STA), this module
// controls channel scanning, association and handovers, by sending commands
// (e.g. ~Ieee80211Prim_ScanRequest) to the management module (~Ieee80211MgmtSTA).
//
// @author Andras Varga
//
simple Ieee80211AgentSTA
{
    parameters:
        string interfaceTableModule;   // The path to the InterfaceTable module
        bool activeScan = default(true);  // selects between active and passive scanning
        string channelsToScan = default("");  // list of channel numbers to scan (space delimited); empty means all channels
        double probeDelay @unit(s) = default(100ms); // delay before sending a probe request during active scanning
        double minChannelTime @unit(s) = default(150ms); // min interval to spend on a channel during active scanning
        double maxChannelTime @unit(s) = default(300ms); // channel time for passive scanning, and max channel time for active scanning
        double authenticationTimeout @unit(s) = default(5s); // timeout for the authentication procedure
        double associationTimeout @unit(s) = default(5s); // timeout for the association procedure
        double startingTime @unit("s") = default(-1s); // agent starting time.
        string default_ssid = default("SSID"); // default ssid
        // EEW
        double regularScanTimeout = default(2); // default timeout for regular scan for the collection of crit param for the handove decision alg.
        string decisionAlgorithm = default("RSS"); // following algorithms are implemented RSS,SAW,WPM,TOPSIS,VOKIR,GRA,AHP
        string trafficType = default("conversational");
        string mcdmAlgConfigFile = default(""); // config file for the external java mcdmAlg library
        string userName = default(""); // user name or user ID
        
        @display("i=block/control");
        @signal[rcvdPk](type=cPacket);
        @signal[sentRequest](type=long); // ENUM! ~Ieee80211PrimRequestCode
        @signal[acceptConfirm](type=long); // ENUM! ~Ieee80211PrimConfirmCode
        @signal[dropConfirm](type=long); // ENUM! ~Ieee80211PrimConfirmCode
        @signal[NF_L2_ASSOCIATED_NEWAP](type=inet::InterfaceEntry);
        @signal[NF_L2_ASSOCIATED_OLDAP](type=inet::InterfaceEntry);
        @signal[NF_L2_DISASSOCIATED](type=inet::InterfaceEntry);
        @statistic[sentRequest](title="sent requests"; record=count,vector); // ENUM! ~Ieee80211PrimRequestCode
        @statistic[acceptConfirm](title="accepted confirms"; record=count,vector); // ENUM! ~Ieee80211PrimConfirmCode
        @statistic[dropConfirm](title="dropped confirms"; record=count,vector); // ENUM! ~Ieee80211PrimConfirmCode
        
        // statistics for the multi-criteria handover
        @statistic[RSS](title="RSS"; source="RSS"; record=vector,stats);
        @statistic[availBitrate](title="availBitrate"; source="availBitrate"; record=vector,stats);        
        @statistic[cost](title="cost"; source="cost"; record=vector,stats);
        @statistic[apID](title="apID"; source="apID"; record=vector,stats);
        @statistic[numberOfHO](title="numberOfHO"; source="numberOfHO"; record=vector,stats);
        @statistic[currentEchoAppsBitrate](title="currentEchoAppsBitrate"; source="currentEchoAppsBitrate"; record=vector,stats);
        @statistic[relOverfOfAvailBitrate](title="relOverfOfAvailBitrate"; source="relOverfOfAvailBitrate"; record=vector,stats);
        @statistic[packetLoss](title="packetLoss"; source="packetLoss"; record=vector,stats);        
    gates:
        input mgmtIn;
        output mgmtOut;
}
