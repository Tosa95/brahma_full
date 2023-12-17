#ifndef BE2270D7_5965_4686_81FF_49F498C44D30
#define BE2270D7_5965_4686_81FF_49F498C44D30

#include "voltage_meter.h"
#include "../../lib/math_utils/limit.h"
#include "../../lib/math_utils/ema.h"

namespace Sensors {
    class BatteryMeter {
        private:
            VoltageMeter voltageMeter;
            float minVoltage;
            float maxVoltage;

            float alpha;
            float lastRead;

            Math::EMA<float> ema;

        public:
            BatteryMeter(VoltageMeter voltageMeter, float minVoltage, float maxVoltage, float alpha): voltageMeter(voltageMeter), minVoltage(minVoltage), maxVoltage(maxVoltage), ema(alpha) {
                
            }

            void update() {
                ema.set(voltageMeter.readVoltage());
            }

            float readVoltage() {
                return ema.get();
            }

            float readPercent() {
                float perc = (readVoltage() - minVoltage) / (maxVoltage - minVoltage) * 100;
                return Math::limit(perc, 0.0f, 100.0f);
            }

    };
}

#endif /* BE2270D7_5965_4686_81FF_49F498C44D30 */
