//
// Copyright (C) 2013 OpenSim Ltd.
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

#include "inet/physicallayer/base/packetlevel/FlatTransmissionBase.h"
#include "inet/physicallayer/ieee80211/packetlevel/Ieee80211TransmissionBase.h"
#include "inet/physicallayer/ieee80211/packetlevel/errormodel/Ieee80211ErrorModelBase.h"
#include "inet/linklayer/ieee80211/mac/Ieee80211Consts.h"

namespace inet {

namespace physicallayer {

using namespace ieee80211;

Ieee80211ErrorModelBase::Ieee80211ErrorModelBase() {
}

double Ieee80211ErrorModelBase::computePacketErrorRate(
        const ISNIR *snir) const {
    Enter_Method_Silent
    ();

    EV_INFO << " 008 from Ieee80211ErrorModelBase::computePacketErrorRate" << endl;

    const ITransmission *transmission = snir->getReception()->getTransmission();
    const FlatTransmissionBase *flatTransmission = check_and_cast<
            const FlatTransmissionBase *>(transmission);
    const Ieee80211TransmissionBase *ieee80211Transmission = check_and_cast<
            const Ieee80211TransmissionBase *>(transmission);
    const IIeee80211Mode *mode = ieee80211Transmission->getMode();
    // Probability of no bit error in the payload and the header
    EV_INFO << " 008 mode:" << mode->getDetailStringRepresentation() << endl;
    EV_INFO << " 008 flatTransmission->getHeaderBitLength():" << flatTransmission->getHeaderBitLength() << endl;
    EV_INFO << " 008 flatTransmission->getPayloadBitLength():" << flatTransmission->getPayloadBitLength() << endl;
    double succesRate = getSuccessRate(mode,
            flatTransmission->getHeaderBitLength(),
            flatTransmission->getPayloadBitLength(), snir->getMin());
    EV_INFO << " 008 succesRate=" << succesRate << endl;
    return 1 - succesRate;
}

double Ieee80211ErrorModelBase::computeBitErrorRate(const ISNIR *snir) const {
    return NaN;
}

double Ieee80211ErrorModelBase::computeSymbolErrorRate(
        const ISNIR *snir) const {
    return NaN;}

}
// namespace physicallayer

}// namespace inet

