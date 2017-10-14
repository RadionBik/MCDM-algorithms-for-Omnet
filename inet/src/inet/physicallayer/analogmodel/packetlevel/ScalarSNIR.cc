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

#include "inet/physicallayer/analogmodel/packetlevel/ScalarSNIR.h"
#include "inet/physicallayer/analogmodel/packetlevel/ScalarNoise.h"
#include "inet/physicallayer/contract/packetlevel/IRadioSignal.h"

namespace inet {

namespace physicallayer {

ScalarSNIR::ScalarSNIR(const IReception *reception, const INoise *noise) :
        SNIRBase(reception, noise), minSNIR(NaN)
        {
        }

std::ostream& ScalarSNIR::printToStream(std::ostream& stream, int level) const {
    stream << "ScalarSNIR";
    if (level >= PRINT_LEVEL_DETAIL)
        stream << ", minSNIR = " << minSNIR;
    return stream;
}

double ScalarSNIR::computeMin() const {
    const IScalarSignal *scalarSignalAnalogModel = check_and_cast<
            const IScalarSignal *>(reception->getAnalogModel());
    const ScalarNoise *scalarNoise = check_and_cast<const ScalarNoise *>(noise);

    double rxPower = 0;
    double snir = 0;
    //rxPower = unit(scalarSignalAnalogModel->getPower()/1).get();
    //EV_INFO << "scalarSignalAnalogModel rxPower" << rxPower << " .\n";

    EV_INFO << "scalarSignalAnalogModel->getPower()"
                   << scalarSignalAnalogModel->getPower() << " .\n";
    //EV_TRACE << "scalarSignalAnalogModel RxPower dBm=" << 10 * log10(unit(scalarSignalAnalogModel->getPower()).get() * pow(10, -14) / 1000) << "dBm" << endl;
    EV_INFO
                   << " ... scalarNoise->computeMaxPower(reception->getStartTime(), reception->getEndTime())"
                   << scalarNoise->computeMaxPower(reception->getStartTime(),
                           reception->getEndTime()) << " .\n";

    rxPower = scalarSignalAnalogModel->getPower().get();
    EV_INFO << "... scalarSignalAnalogModel rxPower" << rxPower << " .\n";
    //EV_TRACE << "scalarSignalAnalogModel rxPower dBm=" << 10 * log10(rxPower * pow(10, -14) / 1000) << "dBm" << endl;
    EV_INFO << "... scalarSignalAnalogModel rxPower dBm="
                   << 10 * log10(rxPower) + 30 << "dBm" << endl;

    snir = unit(
            scalarSignalAnalogModel->getPower()
                    / scalarNoise->computeMaxPower(reception->getStartTime(),
                            reception->getEndTime())).get();
    EV_INFO << "... scalarSignalAnalogModel snir=" << snir << endl;
    snir = 10
            * log10(
                    unit(
                            scalarSignalAnalogModel->getPower()
                                    / scalarNoise->computeMaxPower(
                                            reception->getStartTime(),
                                            reception->getEndTime())).get());
    EV_INFO << "... scalarSignalAnalogModel snir=" << snir << "dB" << endl;

    // orig
    return unit(
            scalarSignalAnalogModel->getPower()
                    / scalarNoise->computeMaxPower(reception->getStartTime(),
                            reception->getEndTime())).get();
    //return unit(scalarSignalAnalogModel->getPower() / 1).get();
    //return unit(scalarNoise->computeMaxPower(reception->getStartTime(), reception->getEndTime())).get();
}

double ScalarSNIR::getMin() const {
    if (isNaN(minSNIR)) {
        minSNIR = computeMin();
        EV_INFO << "minSNIR:" << minSNIR << " .\n";
    }
    return minSNIR;
}

/*double ScalarSNIR::getSNIR() {
    double SNIR;
    const IScalarSignal *scalarSignalAnalogModel = check_and_cast<
            const IScalarSignal *>(reception->getAnalogModel());
    const ScalarNoise *scalarNoise = check_and_cast<const ScalarNoise *>(noise);
    SNIR = unit(
            scalarSignalAnalogModel->getPower()
                    / scalarNoise->computeMaxPower(reception->getStartTime(),
                            reception->getEndTime())).get();

    EV_INFO << "SNIR:" << SNIR << " .\n";

    return SNIR;
}

double ScalarSNIR::getRSS() {
    double RSS;
    const IScalarSignal *scalarSignalAnalogModel = check_and_cast<
            const IScalarSignal *>(reception->getAnalogModel());

    RSS = scalarSignalAnalogModel->getPower().get();
    EV_INFO << "RSS:" << RSS << " .\n";
    RSS = 10 * log10(RSS) + 30;
    EV_INFO << "RSS dBm=" << RSS << "dBm" << endl;

    return RSS;
}*/

} // namespace physicallayer

} // namespace inet

