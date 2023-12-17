#ifndef A18D9E22_B900_4B1F_A998_A2D56594A964
#define A18D9E22_B900_4B1F_A998_A2D56594A964

#include <ESPAsyncWebServer.h>
#include <string.h>

#include <functional>

#include "../../rtc/rtc.h"
#include "../../sensors/all_sensors.h"
#include "../../controllers/all_controllers.h"
#include "../../settings.h"
#include "../utils.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"

using namespace WebServer;

namespace Handlers {
namespace Chronothermo {
void attachHandlers(AsyncWebServer *server) {
    addHandler(server, "/api/chronothermo/intervals/state", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("text/json");

        DynamicJsonDocument doc(1000); 
        char keyString[10];

        for (int i = 0; i < Controllers::chronothermo.getIntervalStates().size(); i++) {
            Controllers::chronothermoIntervalStateKeyPair_t &interval = Controllers::chronothermo.getIntervalStates().getKeyPairByIndex(i);
            DynamicJsonDocument intervalState(200);

            intervalState["active"] = interval.value.active;
            sprintf(keyString, "%d", interval.key);

            doc[keyString] = intervalState;
        }
        
        serializeJsonPretty(doc, *response);

        request->send(response);
    });

    addHandler(
        server, "/api/chronothermo/intervals", HTTP_GET,
        [](AsyncWebServerRequest *request) {
            AsyncResponseStream *response =
                request->beginResponseStream("text/json");

            DynamicJsonDocument doc(5000);

            JsonArray intervalsArray = doc.to<JsonArray>();

            for (int i = 0; i < settings.thermostat.intervals.size(); i++) {
                DynamicJsonDocument interval(300);
                DynamicJsonDocument people(200);
                JsonArray peopleArray = people.to<JsonArray>();
                DynamicJsonDocument days(200);
                JsonArray daysArray = days.to<JsonArray>();
                Controllers::chronothermoIntervalKeyPair_t &intervalData =
                    settings.thermostat.intervals.getKeyPairByIndex(i);

                for (int j = 0; j < intervalData.value.people.size(); j++) {
                    peopleArray.add(intervalData.value.people[j]);
                }

                for (int j = 0; j < N_DAYS; j++) {
                    daysArray.add(intervalData.value.days[j]);
                }

                interval["people"] = peopleArray;
                interval["days"] = daysArray;
                interval["from"] = intervalData.value.from;
                interval["to"] = intervalData.value.to;
                interval["temp"] = intervalData.value.temp;
                interval["key"] = intervalData.key;

                intervalsArray.add(interval);
            }

            serializeJsonPretty(doc, *response);

            request->send(response);
        });

    addHandler(
        server, "/api/chronothermo/intervals", HTTP_POST,
        [](AsyncWebServerRequest *request, JsonVariant &json) {
            const JsonObject &jsonObj = json.as<JsonObject>();

            if (!jsonObj.containsKey("people") ||
                !jsonObj.containsKey("days") || !jsonObj.containsKey("from") ||
                !jsonObj.containsKey("to") || !jsonObj.containsKey("temp")) {
                request->send(400, "text/json",
                              "{\"msg\":\"Each interval should have fields "
                              "people, days, from, to and temp\"}");
            }

            if (settings.thermostat.intervals.full()) {
                request->send(400, "text/json",
                              "{\"msg\":\"Can't add more intervals, please "
                              "remove at least one interval and retry\"}");
            }

            Controllers::ChronothermoInterval newInterval;

            JsonArray peopleArray = jsonObj["people"].as<JsonArray>();

            for (int i = 0; i < peopleArray.size(); i++) {
                newInterval.people.push_back(peopleArray[i].as<uint16_t>());
            }

            JsonArray daysArray = jsonObj["days"].as<JsonArray>();

            for (int i = 0; i < daysArray.size(); i++) {
                newInterval.days[i] = daysArray[i].as<bool>();
            }

            newInterval.from = jsonObj["from"].as<Time::millisecond_t>();
            newInterval.to = jsonObj["to"].as<Time::millisecond_t>();
            newInterval.temp = jsonObj["temp"].as<float>();

            settings.thermostat.intervals.add(newInterval);

            saveSettings();

            request->send(200, "text/json", "{\"msg\":\"OK\"}");
        });

    addHandler(
        server, "/api/chronothermo/intervals", HTTP_DELETE,
        [](AsyncWebServerRequest *request, JsonVariant &json) {
            const JsonObject &jsonObj = json.as<JsonObject>();

            if (!jsonObj.containsKey("key")) {
                request->send(
                    400, "text/json",
                    "{\"msg\":\"Need to know the key to remove a interval\"}");
            }

            int key = jsonObj["key"].as<int>();

            if (!settings.thermostat.intervals.contains(key)) {
                request->send(400, "text/json",
                              "{\"msg\":\"Interval not found\"}");
            }

            settings.thermostat.intervals.remove(key);

            saveSettings();

            request->send(200, "text/json", "{\"msg\":\"OK\"}");
        });

    addHandler(
        server, "/api/chronothermo/intervals", HTTP_PUT,
        [](AsyncWebServerRequest *request, JsonVariant &json) {
            const JsonObject &jsonObj = json.as<JsonObject>();

            if (!jsonObj.containsKey("people") ||
                !jsonObj.containsKey("days") || !jsonObj.containsKey("from") ||
                !jsonObj.containsKey("to") || !jsonObj.containsKey("temp") ||
                !jsonObj.containsKey("key")) {
                request->send(400, "text/json",
                              "{\"msg\":\"Each interval should have fields "
                              "people, days, from, to, temp and key\"}");
            }

            uint16_t key = jsonObj["key"].as<uint16_t>();

            if (!settings.thermostat.intervals.contains(key)) {
                request->send(400, "text/json",
                              "{\"msg\":\"Interval not found\"}");
            }

            Controllers::ChronothermoInterval *interval =
                settings.thermostat.intervals.get(key);

            JsonArray peopleArray = jsonObj["people"].as<JsonArray>();

            interval->people.clear();

            for (int i = 0; i < peopleArray.size(); i++) {
                interval->people.push_back(peopleArray[i].as<uint16_t>());
            }

            JsonArray daysArray = jsonObj["days"].as<JsonArray>();

            for (int i = 0; i < daysArray.size(); i++) {
                interval->days[i] = daysArray[i].as<bool>();
            }

            jsonToSetting(interval->from, "from", jsonObj);
            jsonToSetting(interval->to, "to", jsonObj);
            jsonToSetting(interval->temp, "temp", jsonObj);

            saveSettings();

            request->send(200, "text/json", "{\"msg\":\"OK\"}");
        });
}
}  // namespace Chronothermo
}  // namespace Handlers

#endif /* A18D9E22_B900_4B1F_A998_A2D56594A964 */
