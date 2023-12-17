#include "ota.h"
#include <ArduinoOTA.h>
#include <Esp.h>
#include "../logging/logging.h"
#include "../version.h"
#include "../webserver/webserver.h"

namespace OTA {

    bool updating = false;

    bool updateInProgress() {
        return updating;
    }

    void begin() {
        ArduinoOTA.onStart([]() {
            Logging::info(Logging::OTA, F("Begin of software update. Current software version is: %s"), VERSION);
            // WebServer::server->end();
            updating = true;
        });
        ArduinoOTA.onEnd([]() {
            Logging::info(Logging::OTA, F("Software update completed with success! The system will now reboot"));
            ESP.restart();
        });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            // static unsigned int prev = 101;
            // unsigned int perc = progress / (total / 100);
            // if (perc % 2 == 0 && perc != prev) {
            //     Logging::info(Logging::OTA, F("OTA update progress: %u%"), (progress / (total / 100)));
            //     prev = perc;
            // }
        });
        ArduinoOTA.onError([](ota_error_t error) {
            const char *error_str = "Generic Error";

            if (error == OTA_AUTH_ERROR) error_str = "Auth Failed";
            else if (error == OTA_BEGIN_ERROR) error_str = "Begin Failed";
            else if (error == OTA_CONNECT_ERROR) error_str = "Connect Failed";
            else if (error == OTA_RECEIVE_ERROR) error_str = "Receive Failed";
            else if (error == OTA_END_ERROR) error_str = "End Failed";

            Logging::info(Logging::OTA, F("OTA error: %s"), error_str);

            ESP.restart();
        });
        ArduinoOTA.begin();
    }

    void loop() {
        ArduinoOTA.handle();
    }
}
