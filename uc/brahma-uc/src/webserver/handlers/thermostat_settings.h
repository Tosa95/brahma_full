#ifndef Thermostat_ROUTES_H
#define Thermostat_ROUTES_H
    #include "../../settings.h"
    #include "../utils.h"

    #include <ESPAsyncWebServer.h>
    #include <functional>
    #include "AsyncJson.h"
    #include "ArduinoJson.h"

    
    #include "../../controllers/all_controllers.h"

    using namespace WebServer;

    namespace Handlers {
        namespace ThermostatSettingsRoutes {
            void attachHandlers(AsyncWebServer *server) {
                
                addHandler(server, "/api/thermostat/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
                    AsyncResponseStream *response = request->beginResponseStream("text/json");

                    DynamicJsonDocument doc(1000);
                    
                    doc["target"] = settings.thermostat.settings.target;
                    doc["baseTemp"] = settings.thermostat.settings.baseTemp;
                    doc["override"] = settings.thermostat.settings.override;
                    doc["overrideStart"] = settings.thermostat.settings.overrideStart;
                    doc["overrideFor"] = settings.thermostat.settings.overrideFor;
                    doc["hysteresisSize"] = settings.thermostat.settings.hysteresisSize;
                    doc["__schema__"]["target"]["type"] = "float";
                    doc["__schema__"]["target"]["displayed_name"] = "Target (°C)";
                    doc["__schema__"]["baseTemp"]["type"] = "float";
                    doc["__schema__"]["baseTemp"]["displayed_name"] = "Base Temp (°C)";
                    doc["__schema__"]["override"]["type"] = "boolean";
                    doc["__schema__"]["override"]["displayed_name"] = "Override";
                    doc["__schema__"]["overrideStart"]["type"] = "int";
                    doc["__schema__"]["overrideStart"]["displayed_name"] = "Override Start";
                    doc["__schema__"]["overrideFor"]["type"] = "int";
                    doc["__schema__"]["overrideFor"]["displayed_name"] = "Override For";
                    doc["__schema__"]["hysteresisSize"]["type"] = "float";
                    doc["__schema__"]["hysteresisSize"]["displayed_name"] = "Hysteresis Size";

                    serializeJsonPretty(doc, *response);

                    request->send(response);
                });
                addHandler(server, "/api/thermostat/settings", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
                    const JsonObject &jsonObj = json.as<JsonObject>();
                    bool needsReboot = false;

                    
                    jsonToSetting(settings.thermostat.settings.target, "target", jsonObj);
                    jsonToSetting(settings.thermostat.settings.baseTemp, "baseTemp", jsonObj);
                    jsonToSetting(settings.thermostat.settings.override, "override", jsonObj);
                    jsonToSetting(settings.thermostat.settings.overrideStart, "overrideStart", jsonObj);
                    jsonToSetting(settings.thermostat.settings.overrideFor, "overrideFor", jsonObj);
                    jsonToSetting(settings.thermostat.settings.hysteresisSize, "hysteresisSize", jsonObj);
                    
                    saveSettings();
                    Controllers::updateFromSettings();

                    request->send(200, "text/json", "{\"msg\":\"OK\"}");

                    if (needsReboot) {
                        delay(1000);
                        ESP.restart();
                    }
                });

            }
        }
    }

#endif /* Thermostat_ROUTES_H */