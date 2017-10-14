//
// Copyright (C) 2006 Andras Varga
//
// This library is free software, you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation;
// either version 2 of the License, or any later version.
// The library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//

package inet.examples.wireless.handover;

import inet.examples.adhoc.hostautoconf.Host;
import inet.examples.inet.ipv4hook.MyRouter;
import inet.examples.wireless.dynamic.DynamicHost;
import inet.node.ethernet.Eth100G;
import inet.node.wireless.AccessPoint;
import inet.node.xmipv6.CorrespondentNode6;
import inet.node.xmipv6.MobileHost6;
import inet.physicallayer.contract.packetlevel.IErrorModel;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.node.inet.WirelessHost;
import inet.physicallayer.common.packetlevel.RadioMedium;
import ned.DatarateChannel;
//import inet.examples.power

network MCDM_Handover_low
{
    @display("bgb=700,700");
    //string errorModelType = default(""); 

    types:
        channel fiberline extends DatarateChannel
        {
            delay = 1us;
            datarate = 512Mbps;
        }
        channel ethline extends DatarateChannel
        {
            delay = 50ns;
            datarate = 100Mbps;
        }
    submodules:
        server: StandardHost {
            @display("p=296,625;i=device/server_l");
        }
        configurator: IPv4NetworkConfigurator {
            @display("p=48,23");
        }
        routerServerFarm: Router {
            @display("p=353,521");
        }
        server2: StandardHost {
            @display("p=386,625;i=device/server;is=l");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=148,38");
        }
        switch: EtherSwitch {
            @display("p=353,390");
        }
        AP1: AccessPoint {
            @display("p=222,112");
        }
        AP2: AccessPoint {
            @display("p=222,362");
        }
        AP3: AccessPoint {
            @display("p=222,612");
        }
        AP4: AccessPoint {
            @display("p=472,112");
        }
        AP5: AccessPoint {
            @display("p=472,362");
        }
        AP6: AccessPoint {
            @display("p=472,612");
        }
        wirelessClient: WirelessHost {
            @display("p=138,161");
        }
        wirelessClient1: WirelessHost {
            @display("p=293,566");
        }
        wirelessClient2: WirelessHost {
            @display("p=197,280");
        }
        wirelessClient3: WirelessHost {
            @display("p=442,253");
        }
        wirelessClient4: WirelessHost {
            @display("p=472,479");
        }
    connections:
        switch.ethg++ <--> Eth100G <--> routerServerFarm.ethg++;
        server.ethg++ <--> Eth100G <--> routerServerFarm.ethg++;
        server2.ethg++ <--> Eth100G <--> routerServerFarm.ethg++;

        AP1.ethg++ <--> ethline <--> switch.ethg++;
        switch.ethg++ <--> ethline <--> AP2.ethg++;
        AP3.ethg++ <--> ethline <--> switch.ethg++;
        AP4.ethg++ <--> ethline <--> switch.ethg++;
        AP5.ethg++ <--> ethline <--> switch.ethg++;
        AP6.ethg++ <--> ethline <--> switch.ethg++;
}

