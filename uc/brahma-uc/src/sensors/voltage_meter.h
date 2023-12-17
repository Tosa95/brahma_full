#ifndef C8DEA982_B5A7_4D31_A6B8_B50DE39C49E1
#define C8DEA982_B5A7_4D31_A6B8_B50DE39C49E1

#include <stdint.h>
#include <Arduino.h>

namespace Sensors {
    class VoltageMeter {
        private:
            uint8_t pin;
            float maxVoltage;
            uint32_t maxRead;
            float partitionFactor;

        public:
            VoltageMeter(uint8_t pin, float maxVoltage, uint32_t maxRead, float partitionFactor = 1): pin(pin), maxVoltage(maxVoltage), maxRead(maxRead), partitionFactor(partitionFactor) {

            }

            uint32_t readRaw() {
                return analogRead(pin);
            }

            float readVoltage() {
                return ((readRaw() * maxVoltage) / maxRead) * partitionFactor;
            }
    };
}

#endif /* C8DEA982_B5A7_4D31_A6B8_B50DE39C49E1 */
