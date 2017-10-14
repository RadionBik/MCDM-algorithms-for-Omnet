 
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

cplusplus {{
//#include "inet/linklayer/ieee80211/mgmt/Ieee80211Primitives_m.h"

}}
//message Ieee80211Primitives;


// TODO generated message class
//
class NWinfo //extends Ieee80211Primitives 
{
               int idx;
               double rss;
               double rssthr;
               double SNR;
               double SNRthr;
               double AdDataRate;
               //double AdPckLossRate;
               double xusers;
               double mcost;
               simtime_t nnDelay;
               double jitter;
               string SSID;
               double speed;
               int Thrvolume;
               int Rxvolume;               
               double srcportno=1025;//TODO should be collected from running app
               int Result; // used to pass the result to a response
}

message DInquiry extends NWinfo   //// to be sent to the decider
{
    NWinfo nw1;
    NWinfo nw2;
}

message updateMsg extends DInquiry
{
    int kind;

}




