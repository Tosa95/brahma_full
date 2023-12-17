#ifndef E49F14F1_5768_4527_8A2D_6B2D71AD744A
#define E49F14F1_5768_4527_8A2D_6B2D71AD744A

    #include <string.h>
    #include <stdint.h>
    #include <Arduino.h>
    #include <Array.h>
    #include "../../lib/collections/seq_keyed_array.h"
    #include "wifi_settings.h"
    #include "rtc_settings.h"
    #include "thermostat_settings.h"
    #include "person_sensor_settings.h"
    #include "prometheus_settings.h"
    #include "metrics_settings.h"
    #include "../../lib/networking/person_sensor.h"
    #include "../controllers/chronothermo_settings.h"
    #include "rgb_leds_settings.h"

    #define CONTAINER_ID_SIZE 16
    #define MAX_CONTAINERS 100
    #define N_FOODS 3
    #define MAX_PEOPLE_PER_FOOD 6
    #define MAX_DIV_BY 50

    #define N_TEMPS 4*24*7

    #define MAX_TEMP_INTERVALS 30
    #define TEMP_INTERVAL_NAME_LENGTH 50
    
    struct Settings {

        char version = 19;

        struct {
            WiFiSettings::Settings::AccessPoint ap;
            WiFiSettings::Settings::Network network;
            WiFiSettings::Settings::Credentials credentials;
        } wifi;

        struct {
            Collections::SeqKeyedArray<uint16_t, Networking::PersonConfig,MAX_PEOPLE> configs;
        } people;

        PersonSensorSettings::Settings::PersonSensor personSensor;

        RTCSettings::Settings::RTC rtc;

        struct {
            uint8_t temps[N_TEMPS];
            Thermostat::Settings::Thermostat settings;
            Controllers::chronothermoIntervalArray_t intervals;
        } thermostat;
        
        PrometheusSettings::Settings::Prometheus prometheus;

        Metrics::Settings::Metrics metrics;

        RGBLeds::Settings::RGBLeds rgbLeds;

        Settings() {
            for (int i = 0; i < N_TEMPS; i++) {
                thermostat.temps[i] = 28;
            }

            people.configs.add(Networking::PersonConfig("Davide", 0x4ce0db83c13a, 21*60*60, 5*60*60, 11*60*60 + 30*60));
            people.configs.add(Networking::PersonConfig("Claudia", 0x6aa4324c93f8, 21*60*60, 5*60*60, 11*60*60 + 30*60));
        }

    };

    extern Settings settings;

    void saveSettings();

    void loadSettings();

#endif /* E49F14F1_5768_4527_8A2D_6B2D71AD744A */
