#ifndef RTCSettings_ROUTES_H
#define RTCSettings_ROUTES_H
    #include "../../settings.h"
    #include "../utils.h"

    #include <ESPAsyncWebServer.h>
    #include <functional>
    #include "AsyncJson.h"
    #include "ArduinoJson.h"

    

    using namespace WebServer;

    namespace Handlers {
        namespace RTCSettingsSettingsRoutes {
            void attachHandlers(AsyncWebServer *server) {
                
                addHandler(server, "/api/rtc/rtc", HTTP_GET, [](AsyncWebServerRequest *request) {
                    AsyncResponseStream *response = request->beginResponseStream("text/json");

                    DynamicJsonDocument doc(1000);
                    
                    doc["ntpHost"] = settings.rtc.ntpHost;
                    doc["utcOffsetHours"] = settings.rtc.utcOffsetHours;
                    doc["__schema__"]["ntpHost"]["type"] = "string";
                    doc["__schema__"]["ntpHost"]["displayed_name"] = "NtpHost";
                    doc["__schema__"]["utcOffsetHours"]["type"] = "int";
                    doc["__schema__"]["utcOffsetHours"]["displayed_name"] = "UTCOffsetHours";

                    serializeJsonPretty(doc, *response);

                    request->send(response);
                });
                addHandler(server, "/api/rtc/rtc", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
                    const JsonObject &jsonObj = json.as<JsonObject>();
                    bool needsReboot = true;

                    
                    jsonToSettingStr(settings.rtc.ntpHost, "ntpHost", jsonObj);
                    jsonToSetting(settings.rtc.utcOffsetHours, "utcOffsetHours", jsonObj);
                    
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

#endif /* RTCSettings_ROUTES_H */