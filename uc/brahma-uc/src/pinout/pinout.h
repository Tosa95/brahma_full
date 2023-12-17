#ifndef B6200A25_0337_4A0E_A6C4_89E5FA72BEF5
#define B6200A25_0337_4A0E_A6C4_89E5FA72BEF5

#include "config.h"

namespace Pinout {

    #if defined(PRODUCT__THERMOSTAT)
        const int HX711_DAT = 26;
        const int HX711_CLK = 25;

        const int RC522_SS = 5;
        const int RC522_RESET = 4;

        const int ENCODER_CLICK = 14;
        const int ENCODER_A = 12;
        const int ENCODER_B = 13;

        const int BATTERY_READ = 34;

        const int NTC = 33;

        const int HEATER_ON = 13;
        const int HEATER_OFF = 14;
    #endif

    #if defined(PRODUCT__THERMOSTAT_V2)
        const int HEATER_ON = 1;
        const int HEATER_OFF = 2;
    #endif

    #ifdef PRODUCT__AIR_QUALITY_SENSOR
        const int DHT = 27;
        const int NTC = 34;
    #endif

    #ifdef PRODUCT__RGB_LED_CONTROLLER
        const int R_PIN = 18;
        const int G_PIN = 19;
        const int B_PIN = 21;
    #endif
}

#endif /* B6200A25_0337_4A0E_A6C4_89E5FA72BEF5 */
