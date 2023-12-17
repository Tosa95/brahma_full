#ifndef RGBLeds_ROUTES_H
#define RGBLeds_ROUTES_H
    #include "../../settings.h"
    #include "../utils.h"

    #include <ESPAsyncWebServer.h>
    #include <functional>
    #include "AsyncJson.h"
    #include "ArduinoJson.h"

    

    using namespace WebServer;

    namespace Handlers {
        namespace RGBLedsSettingsRoutes {
            void attachHandlers(AsyncWebServer *server) {
                
                addHandler(server, "/api/rgb_leds/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
                    AsyncResponseStream *response = request->beginResponseStream("text/json");

                    DynamicJsonDocument doc(1000);
                    
                    doc["r"] = settings.rgbLeds.r;
                    doc["g"] = settings.rgbLeds.g;
                    doc["b"] = settings.rgbLeds.b;
                    doc["__schema__"]["r"]["type"] = "int";
                    doc["__schema__"]["r"]["displayed_name"] = "R (0-255)";
                    doc["__schema__"]["g"]["type"] = "int";
                    doc["__schema__"]["g"]["displayed_name"] = "G (0-255)";
                    doc["__schema__"]["b"]["type"] = "int";
                    doc["__schema__"]["b"]["displayed_name"] = "B (0-255)";

                    serializeJsonPretty(doc, *response);

                    request->send(response);
                });
                addHandler(server, "/api/rgb_leds/settings", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
                    const JsonObject &jsonObj = json.as<JsonObject>();
                    bool needsReboot = false;

                    
                    jsonToSetting(settings.rgbLeds.r, "r", jsonObj);
                    jsonToSetting(settings.rgbLeds.g, "g", jsonObj);
                    jsonToSetting(settings.rgbLeds.b, "b", jsonObj);
                    
                    saveSettings();

                    request->send(200, "text/json", "{\"msg\":\"OK\"}");

                    if (needsReboot) {
                        delay(1000);
                        ESP.restart();
                    }
                });

            }
        }
    }

#endif /* RGBLeds_ROUTES_H */