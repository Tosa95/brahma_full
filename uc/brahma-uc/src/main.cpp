#include "config.h"

#ifdef PRODUCT__THERMOSTAT
    #include "mains/thermostat_main.h"
#endif

#ifdef PRODUCT__THERMOSTAT_V2
    #include "mains/thermostat_main_v2.h"
#endif

#ifdef PRODUCT__AIR_QUALITY_SENSOR
    #include "mains/air_quality_sensor_main.h"
#endif

#ifdef PRODUCT__RGB_LED_CONTROLLER
    #include "mains/rgb_led_controller_main.h"
#endif

#ifdef PRODUCT__TEST
    #include "mains/test_main.h"
#endif