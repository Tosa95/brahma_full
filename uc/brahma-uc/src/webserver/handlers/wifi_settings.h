#ifndef WiFiSettings_ROUTES_H
#define WiFiSettings_ROUTES_H
    #include "../../settings.h"
    #include "../utils.h"

    #include <ESPAsyncWebServer.h>
    #include <functional>
    #include "AsyncJson.h"
    #include "ArduinoJson.h"

    

    using namespace WebServer;

    namespace Handlers {
        namespace WiFiSettingsSettingsRoutes {
            void attachHandlers(AsyncWebServer *server) {
                
                addHandler(server, "/api/wifi/access_point", HTTP_GET, [](AsyncWebServerRequest *request) {
                    AsyncResponseStream *response = request->beginResponseStream("text/json");

                    DynamicJsonDocument doc(1000);
                    
                    doc["ssid"] = settings.wifi.ap.ssid;
                    doc["password"] = settings.wifi.ap.password;
                    doc["__schema__"]["ssid"]["type"] = "string";
                    doc["__schema__"]["ssid"]["displayed_name"] = "SSID";
                    doc["__schema__"]["password"]["type"] = "string";
                    doc["__schema__"]["password"]["displayed_name"] = "Password";

                    serializeJsonPretty(doc, *response);

                    request->send(response);
                });
                addHandler(server, "/api/wifi/access_point", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
                    const JsonObject &jsonObj = json.as<JsonObject>();
                    bool needsReboot = true;

                    
                    jsonToSettingStr(settings.wifi.ap.ssid, "ssid", jsonObj);
                    jsonToSettingStr(settings.wifi.ap.password, "password", jsonObj);
                    
                    saveSettings();

                    request->send(200, "text/json", "{\"msg\":\"OK\"}");

                    if (needsReboot) {
                        delay(1000);
                        ESP.restart();
                    }
                });
                addHandler(server, "/api/wifi/network", HTTP_GET, [](AsyncWebServerRequest *request) {
                    AsyncResponseStream *response = request->beginResponseStream("text/json");

                    DynamicJsonDocument doc(1000);
                    
                    doc["ip"] = settings.wifi.network.ip;
                    doc["gateway"] = settings.wifi.network.gateway;
                    doc["subnet"] = settings.wifi.network.subnet;
                    doc["dns1"] = settings.wifi.network.dns1;
                    doc["dns2"] = settings.wifi.network.dns2;
                    doc["__schema__"]["ip"]["type"] = "string";
                    doc["__schema__"]["ip"]["displayed_name"] = "IP";
                    doc["__schema__"]["gateway"]["type"] = "string";
                    doc["__schema__"]["gateway"]["displayed_name"] = "Gateway";
                    doc["__schema__"]["subnet"]["type"] = "string";
                    doc["__schema__"]["subnet"]["displayed_name"] = "Subnet";
                    doc["__schema__"]["dns1"]["type"] = "string";
                    doc["__schema__"]["dns1"]["displayed_name"] = "Dns1";
                    doc["__schema__"]["dns2"]["type"] = "string";
                    doc["__schema__"]["dns2"]["displayed_name"] = "Dns2";

                    serializeJsonPretty(doc, *response);

                    request->send(response);
                });
                addHandler(server, "/api/wifi/network", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
                    const JsonObject &jsonObj = json.as<JsonObject>();
                    bool needsReboot = true;

                    
                    jsonToSettingStr(settings.wifi.network.ip, "ip", jsonObj);
                    jsonToSettingStr(settings.wifi.network.gateway, "gateway", jsonObj);
                    jsonToSettingStr(settings.wifi.network.subnet, "subnet", jsonObj);
                    jsonToSettingStr(settings.wifi.network.dns1, "dns1", jsonObj);
                    jsonToSettingStr(settings.wifi.network.dns2, "dns2", jsonObj);
                    
                    saveSettings();

                    request->send(200, "text/json", "{\"msg\":\"OK\"}");

                    if (needsReboot) {
                        delay(1000);
                        ESP.restart();
                    }
                });
                addHandler(server, "/api/wifi/credentials", HTTP_GET, [](AsyncWebServerRequest *request) {
                    AsyncResponseStream *response = request->beginResponseStream("text/json");

                    DynamicJsonDocument doc(1000);
                    
                    doc["ssid"] = settings.wifi.credentials.ssid;
                    doc["password"] = settings.wifi.credentials.password;
                    doc["__schema__"]["ssid"]["type"] = "string";
                    doc["__schema__"]["ssid"]["displayed_name"] = "SSID";
                    doc["__schema__"]["password"]["type"] = "string";
                    doc["__schema__"]["password"]["displayed_name"] = "Password";

                    serializeJsonPretty(doc, *response);

                    request->send(response);
                });
                addHandler(server, "/api/wifi/credentials", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
                    const JsonObject &jsonObj = json.as<JsonObject>();
                    bool needsReboot = true;

                    
                    jsonToSettingStr(settings.wifi.credentials.ssid, "ssid", jsonObj);
                    jsonToSettingStr(settings.wifi.credentials.password, "password", jsonObj);
                    
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

#endif /* WiFiSettings_ROUTES_H */