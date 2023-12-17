#ifndef Metrics_ROUTES_H
#define Metrics_ROUTES_H
    #include "../../settings.h"
    #include "../utils.h"

    #include <ESPAsyncWebServer.h>
    #include <functional>
    #include "AsyncJson.h"
    #include "ArduinoJson.h"

    

    using namespace WebServer;

    namespace Handlers {
        namespace MetricsSettingsRoutes {
            void attachHandlers(AsyncWebServer *server) {
                
                addHandler(server, "/api/metrics/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
                    AsyncResponseStream *response = request->beginResponseStream("text/json");

                    DynamicJsonDocument doc(1000);
                    
                    doc["nSamples"] = settings.metrics.nSamples;
                    doc["sampleInterval"] = settings.metrics.sampleInterval;
                    doc["__schema__"]["nSamples"]["type"] = "int";
                    doc["__schema__"]["nSamples"]["displayed_name"] = "Number of samples kept in memory";
                    doc["__schema__"]["sampleInterval"]["type"] = "float";
                    doc["__schema__"]["sampleInterval"]["displayed_name"] = "Sample Interval (s)";

                    serializeJsonPretty(doc, *response);

                    request->send(response);
                });
                addHandler(server, "/api/metrics/settings", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
                    const JsonObject &jsonObj = json.as<JsonObject>();
                    bool needsReboot = true;

                    
                    jsonToSetting(settings.metrics.nSamples, "nSamples", jsonObj);
                    jsonToSetting(settings.metrics.sampleInterval, "sampleInterval", jsonObj);
                    
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

#endif /* Metrics_ROUTES_H */