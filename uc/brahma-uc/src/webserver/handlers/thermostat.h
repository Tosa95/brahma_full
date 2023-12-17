#ifndef ThermostatState_ROUTES_H
#define ThermostatState_ROUTES_H
    #include "../../settings.h"
    #include "../utils.h"

    #include <ESPAsyncWebServer.h>
    #include <functional>
    #include "AsyncJson.h"
    #include "ArduinoJson.h"
    #include "../../controllers/all_controllers.h"
    #include "../../rtc/rtc.h"

    using namespace WebServer;

    namespace Handlers {
        namespace Thermostat {
            void attachHandlers(AsyncWebServer *server) {
                
                addHandler(server, "/api/thermostat/temps", HTTP_GET, [](AsyncWebServerRequest *request) {
                    AsyncResponseStream *response = request->beginResponseStream("application/octet-stream");

                    response->write(settings.thermostat.temps, N_TEMPS);

                    request->send(response);
                });


                addHandler(server, "/api/thermostat/temps", HTTP_POST, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {

                    for (int i = 0; i < len; i++) {
                        settings.thermostat.temps[index + i] = data[i];
                    }

                    saveSettings();

                    request->send(200, "text/json", "{\"msg\":\"OK\"}");

                });

                addHandler(server, "/api/thermostat/state", HTTP_GET, [](AsyncWebServerRequest *request) {
                    AsyncResponseStream *response = request->beginResponseStream("text/json");

                    DynamicJsonDocument doc(1000);
                    
                    doc["temp"] = Controllers::heaterController.getCurrentTemp();
                    doc["heating"] = Controllers::heaterController.getState() == Controllers::HeaterState::HEATING;
                    doc["time"] = RTC::timeClient.getEpochTimeMs();
                    doc["lowTh"] = Controllers::heaterController.getHysteresis().getLowThreshold();
                    doc["highTh"] = Controllers::heaterController.getHysteresis().getHighThreshold();
                    doc["currTh"] = Controllers::heaterController.getHysteresis().getCurrentThreshold();

                    serializeJsonPretty(doc, *response);

                    request->send(response);
                });

            }
        }
    }

#endif