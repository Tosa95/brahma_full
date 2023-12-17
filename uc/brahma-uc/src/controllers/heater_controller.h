#ifndef B348B7D1_640A_440B_B71F_23B86D73FCD4
#define B348B7D1_640A_440B_B71F_23B86D73FCD4
#ifndef B961153F_53FC_432B_AFD1_C7A43663C386
#define B961153F_53FC_432B_AFD1_C7A43663C386

#include <Arduino.h>

#include "../../lib/math_utils/hysteresis.h"
#include "../../lib/math_utils/ema.h"
#include "../../lib/ntc/ntc.h"

namespace Controllers {
    enum HeaterState : int { HEATING, OFF };

    class HeaterController {
    private:
        Hysteresis::HysteresisController<float> hysteresis;
        Math::EMA<float> currentTemp;
        uint8_t onPin;
        uint8_t offPin;
        bool enabled;
        float tempStabilizationAlpha;
        HeaterState state;

        void sendSignal(uint8_t pin) {
            digitalWrite(pin, HIGH);
            delay(100);
            digitalWrite(pin, LOW);

            if (pin == onPin) {
                state = HeaterState::HEATING;
            } else {
                state = HeaterState::OFF;
            }
        }

    public:
        HeaterController(uint8_t onPin, uint8_t offPin, bool enabled = false, float lowTh = 9.5, float highTh = 10.5, float tempStabilizationAlpha = 0.9)
            : onPin(onPin), offPin(offPin), enabled(enabled), hysteresis(lowTh, highTh), currentTemp(tempStabilizationAlpha) {
        }

        void init() {
            pinMode(onPin, OUTPUT);
            pinMode(offPin, OUTPUT);

            sendSignal(offPin);
        }

        void update(float temp) {
            currentTemp.set(temp);

            if (enabled) {
                hysteresis.update(currentTemp.get());
                switch (hysteresis.getState())
                {
                case Hysteresis::HysteresisState::ABOVE:
                case Hysteresis::HysteresisState::UNDEFINED:
                    sendSignal(offPin);
                    break;
                case Hysteresis::HysteresisState::BELOW:
                    sendSignal(onPin);
                    break;
                }
            } else {
                sendSignal(offPin);
            }
        }

        Hysteresis::HysteresisController<float> &getHysteresis() {
            return hysteresis;
        }

        float getCurrentTemp() {
            return currentTemp.get();
        }

        void setTarget(float target, float hyst) {
            hysteresis.setHighThreshold(target + hyst/2);
            hysteresis.setLowThreshold(target - hyst/2);
        }

        HeaterState getState() {
            return state;
        }
    };
}

#endif /* B961153F_53FC_432B_AFD1_C7A43663C386 */


#endif /* B348B7D1_640A_440B_B71F_23B86D73FCD4 */
