#ifndef PrometheusSettings_ROUTES_H
#define PrometheusSettings_ROUTES_H
    #include "../../settings.h"
    #include "../utils.h"

    #include <ESPAsyncWebServer.h>
    #include <functional>
    #include "AsyncJson.h"
    #include "ArduinoJson.h"

    

    using namespace WebServer;

    namespace Handlers {
        namespace PrometheusSettingsSettingsRoutes {
            void attachHandlers(AsyncWebServer *server) {
                
                addHandler(server, "/api/prometheus", HTTP_GET, [](AsyncWebServerRequest *request) {
                    AsyncResponseStream *response = request->beginResponseStream("text/json");

                    DynamicJsonDocument doc(1000);
                    
                    doc["pushGatewayUrl"] = settings.prometheus.pushGatewayUrl;
                    doc["instanceName"] = settings.prometheus.instanceName;
                    doc["__schema__"]["pushGatewayUrl"]["type"] = "string";
                    doc["__schema__"]["pushGatewayUrl"]["displayed_name"] = "Push Gateway Url";
                    doc["__schema__"]["instanceName"]["type"] = "string";
                    doc["__schema__"]["instanceName"]["displayed_name"] = "Instance Name";

                    serializeJsonPretty(doc, *response);

                    request->send(response);
                });
                addHandler(server, "/api/prometheus", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
                    const JsonObject &jsonObj = json.as<JsonObject>();
                    bool needsReboot = true;

                    
                    jsonToSettingStr(settings.prometheus.pushGatewayUrl, "pushGatewayUrl", jsonObj);
                    jsonToSettingStr(settings.prometheus.instanceName, "instanceName", jsonObj);
                    
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

#endif /* PrometheusSettings_ROUTES_H */