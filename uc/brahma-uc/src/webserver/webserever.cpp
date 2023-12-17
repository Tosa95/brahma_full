#include "webserver.h"

#ifndef ESP32
    #include <ESP8266WiFi.h>
#else
    #include <WiFi.h>
#endif

#ifndef ESP32
    #include <ESPAsyncTCP.h>
#else
    #include <AsyncTCP.h>
#endif

#include "../config.h"

#include "../fs/filesystem.h"
#include "handlers/feature_flags.h"
#include "handlers/wifi_settings.h"
#include "handlers/metrics.h"
#include "handlers/rtc_settings.h"
#include "handlers/prometheus_settings.h"
#include "handlers/metrics_settings.h"
#include "../logging/logging.h"

#ifdef ROUTES_THERMOSTAT
    #include "handlers/thermostat.h"
    #include "handlers/chronothermo.h"
    #include "handlers/thermostat_settings.h"
#endif

#ifdef ROUTES_PEOPLE_SENSOR
    #include "handlers/people.h"
    #include "handlers/person_sensor_settings.h"
    #include "handlers/networking_utils.h"
#endif

#ifdef ROUTES_RGB_LEDS
    #include "handlers/rgb_leds_settings.h"
#endif


namespace WebServer {
        // Create AsyncWebServer object on port 80
        AsyncWebServer *server = nullptr;

        void setup() {
            if (server != nullptr) {
                server->end();
                delete server;
            }
            server = new AsyncWebServer(80);
            Handlers::FeatureFlags::attachHandlers(server);
            Handlers::WiFiSettingsSettingsRoutes::attachHandlers(server);
            Handlers::MetricsRoutes::attachHandlers(server);
            Handlers::RTCSettingsSettingsRoutes::attachHandlers(server);
            Handlers::PrometheusSettingsSettingsRoutes::attachHandlers(server);
            Handlers::MetricsSettingsRoutes::attachHandlers(server);

            #ifdef ROUTES_THERMOSTAT
                Handlers::Thermostat::attachHandlers(server);
                Handlers::Chronothermo::attachHandlers(server);
                Handlers::ThermostatSettingsRoutes::attachHandlers(server);
            #endif
            
            #ifdef ROUTES_PEOPLE_SENSOR
                Handlers::NetworkingUtils::attachHandlers(server);
                Handlers::People::attachHandlers(server);
                Handlers::PersonSensorSettingsSettingsRoutes::attachHandlers(server);
            #endif

            #ifdef ROUTES_RGB_LEDS
                Handlers::RGBLedsSettingsRoutes::attachHandlers(server);
            #endif
            
            
            server->serveStatic("/", *FileSystem::mainFS, "/");
            server->onNotFound([](AsyncWebServerRequest *request){
                // TODO: transform into an object, manage also non-gz case
                // TODO: also check the extension, should be none or html. If it is not, return 404
                fs::File index = FileSystem::mainFS->open("/index.html.gz");
                AsyncWebServerResponse * response = new AsyncFileResponse(index, "index.html", String(), false, nullptr);
                request->send(response);
                index.close();
            });

            server->begin(); 
            Logging::info(Logging::WEBSERVER, F("HTTP server started"));
        }
    }