#ifndef E667FE28_279E_4CA6_AAA5_6CEFDA8A35C0
#define E667FE28_279E_4CA6_AAA5_6CEFDA8A35C0

    #include "../../settings.h"
    #include "../utils.h"

    #include <ESPAsyncWebServer.h>
    #include <functional>
    #include "AsyncJson.h"
    #include "ArduinoJson.h"
    #include "../../sensors/all_sensors.h"
    #include "../../rtc/rtc.h"
    #include <string.h>

    using namespace WebServer;

    namespace Handlers {
        namespace People {
            void attachHandlers(AsyncWebServer *server) {
                
                addHandler(server, "/api/people/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
                    AsyncResponseStream *response = request->beginResponseStream("text/json");

                    DynamicJsonDocument doc(1000);
                    DynamicJsonDocument people(1000);

                    JsonArray peopleArray = people.to<JsonArray>(); 
                    
                    for (int i = 0; i < settings.people.configs.size(); i++) {
                        DynamicJsonDocument person(200);
                        Collections::KeyedArrayEntry<uint16_t, Networking::PersonConfig> &personConf = settings.people.configs.getKeyPairByIndex(i);
                        person["name"] = personConf.value.name;
                        person["mac"] = personConf.value.macAddress;
                        person["considerInAfter"] = personConf.value.considerInAfter;
                        person["wakeUpFrom"] = personConf.value.wakeUpFrom;
                        person["wakeUpTo"] = personConf.value.wakeUpTo;
                        person["key"] = personConf.key;
                        peopleArray.add(person);
                    }

                    doc["people"] = peopleArray;

                    serializeJsonPretty(doc, *response);

                    request->send(response);
                });

                addHandler(server, "/api/people/state", HTTP_GET, [](AsyncWebServerRequest *request) {
                    AsyncResponseStream *response = request->beginResponseStream("text/json");

                    DynamicJsonDocument doc(1000); 
                    char keyString[10];
                    
                    for (int i = 0; i < Sensors::personSensor.getPeople()->size(); i++) {
                        Networking::PersonEntry &person = Sensors::personSensor.getPeople()->get(i);
                        DynamicJsonDocument personState(200);

                        personState["present"] = person.present;
                        personState["inForTheNight"] = person.inForTheNight;

                        sprintf(keyString, "%d", person.key);

                        doc[keyString] = personState;
                    }
                    
                    serializeJsonPretty(doc, *response);

                    request->send(response);
                });

                addHandler(server, "/api/people/settings/person", HTTP_POST, [](AsyncWebServerRequest *request, JsonVariant &json) {
                    const JsonObject &jsonObj = json.as<JsonObject>();
                    bool needsReboot = false;

                    if (!jsonObj.containsKey("name") || !jsonObj.containsKey("mac")) {
                        request->send(400, "text/json", "{\"msg\":\"Each person should have a name and a mac address\"}");
                    }

                    if (settings.people.configs.full()) {
                        request->send(400, "text/json", "{\"msg\":\"Can't add more people, please remove at least one person and retry\"}");
                    }

                    Networking::PersonConfig newPerson;

                    jsonToSettingStr(newPerson.name, "name", jsonObj);
                    jsonToSetting(newPerson.macAddress, "mac", jsonObj);
                    jsonToSetting(newPerson.considerInAfter, "considerInAfter", jsonObj);
                    jsonToSetting(newPerson.wakeUpFrom, "wakeUpFrom", jsonObj);
                    jsonToSetting(newPerson.wakeUpTo, "wakeUpTo", jsonObj);

                    settings.people.configs.add(newPerson);
                    
                    saveSettings();

                    request->send(200, "text/json", "{\"msg\":\"OK\"}");
                });

                addHandler(server, "/api/people/settings/person", HTTP_DELETE, [](AsyncWebServerRequest *request, JsonVariant &json) {
                    const JsonObject &jsonObj = json.as<JsonObject>();
                    bool needsReboot = false;

                    if (!jsonObj.containsKey("key")) {
                        request->send(400, "text/json", "{\"msg\":\"Need to know the key to remove a person\"}");
                    }

                    int key = jsonObj["key"].as<int>();

                    if (!settings.people.configs.contains(key)) {
                        request->send(400, "text/json", "{\"msg\":\"Person not found\"}");
                    }

                    settings.people.configs.remove(key);
                    
                    saveSettings();

                    request->send(200, "text/json", "{\"msg\":\"OK\"}");
                });

                addHandler(server, "/api/people/settings/person", HTTP_PUT, [](AsyncWebServerRequest *request, JsonVariant &json) {
                    const JsonObject &jsonObj = json.as<JsonObject>();
                    bool needsReboot = false;

                    if (!jsonObj.containsKey("name") || !jsonObj.containsKey("mac") || !jsonObj.containsKey("key")) {
                        request->send(400, "text/json", "{\"msg\":\"Need to know the key, name and mac to update a person\"}");
                    }

                    uint16_t key = jsonObj["key"].as<uint16_t>();

                    if (!settings.people.configs.contains(key)) {
                        request->send(400, "text/json", "{\"msg\":\"Person not found\"}");
                    }

                    Networking::PersonConfig *person = settings.people.configs.get(key);

                    jsonToSettingStr(person->name, "name", jsonObj);
                    jsonToSetting(person->macAddress, "mac", jsonObj);
                    jsonToSetting(person->considerInAfter, "considerInAfter", jsonObj);
                    jsonToSetting(person->wakeUpFrom, "wakeUpFrom", jsonObj);
                    jsonToSetting(person->wakeUpTo, "wakeUpTo", jsonObj);
                    
                    saveSettings();

                    request->send(200, "text/json", "{\"msg\":\"OK\"}");
                });

            }
        }
    }

#endif /* E667FE28_279E_4CA6_AAA5_6CEFDA8A35C0 */
