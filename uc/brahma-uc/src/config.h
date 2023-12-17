#ifndef B9C58FDF_0D9C_48A7_A3BD_133650BBE51E
#define B9C58FDF_0D9C_48A7_A3BD_133650BBE51E

// #define PRODUCT__THERMOSTAT
#define PRODUCT__THERMOSTAT_V2
// #define PRODUCT__AIR_QUALITY_SENSOR
// #define PRODUCT__RGB_LED_CONTROLLER
// #define PRODUCT__TEST

#ifdef PRODUCT__THERMOSTAT
    // Thermostat feature flags
    #define METRICS_HEATER
    #define METRICS_TEMPERATURE
    #define DEFAULT_INSTANCE_NAME "esp-32"

    #define ROUTES_THERMOSTAT
    #define ROUTES_PEOPLE_SENSOR

    #define USES_NTC
    #define HARDWARE_NTC_10K
    #define HARDWARE_NTC_R_PART 15

#endif

#ifdef PRODUCT__THERMOSTAT_V2
    // Thermostat feature flags
    #define METRICS_HEATER
    #define METRICS_TEMPERATURE
    #define DEFAULT_INSTANCE_NAME "thermostat"

    #define ROUTES_THERMOSTAT
    #define ROUTES_PEOPLE_SENSOR

#endif

#ifdef PRODUCT__AIR_QUALITY_SENSOR
    // Air quality sensor feature flags

    #define METRICS_TEMPERATURE
    #define METRICS_EXT_TEMPERATURE
    #define METRICS_HUMIDITY
    #define METRICS_VOC

    #define DEFAULT_INSTANCE_NAME "air-quality-sensor"

    #define USES_NTC
    #define HARDWARE_NTC_100K
    #define HARDWARE_NTC_R_PART 1
#endif

#ifdef PRODUCT__RGB_LED_CONTROLLER
    // RGB LED controller feature flags
    #define DEFAULT_INSTANCE_NAME "rgb-led-controller"

    #define R_CHANNEL 0
    #define G_CHANNEL 1
    #define B_CHANNEL 2

    #define PWM_FREQ 5000

    #define ROUTES_RGB_LEDS
#endif

#ifdef PRODUCT__TEST
    // RGB LED controller feature flags
    #define DEFAULT_INSTANCE_NAME "test-esp"
#endif


#define HOME__MILAN
// #define HOME__CAVRIANA

#ifdef HOME__MILAN
    #define DEFAULT_WIFI_SSID "TIM-TD"
    #define DEFAULT_WIFI_PASS "WiFi@Tosatto.9173?!--++--"
#endif

#ifdef HOME__CAVRIANA
    #define DEFAULT_WIFI_SSID "Tosatto"
    #define DEFAULT_WIFI_PASS "WiFi@Tosatto.9173?!--++--"
#endif

#endif /* B9C58FDF_0D9C_48A7_A3BD_133650BBE51E */
