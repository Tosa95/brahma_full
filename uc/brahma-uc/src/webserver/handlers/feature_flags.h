#ifndef DE20B76E_17C0_45DA_B149_EF4D4FA93D31
#define DE20B76E_17C0_45DA_B149_EF4D4FA93D31

#include <ESPAsyncWebServer.h>

#include <functional>

#include "../../controllers/all_controllers.h"
#include "../../rtc/rtc.h"
#include "../../settings.h"
#include "../utils.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"

using namespace WebServer;

namespace Handlers {
namespace FeatureFlags {
void attachHandlers(AsyncWebServer *server) {

    addHandler(
        server, "/api/feature_flags", HTTP_GET,
        [](AsyncWebServerRequest *request) {
            AsyncResponseStream *response =
                request->beginResponseStream("text/json");

            DynamicJsonDocument doc(1000);

            #if defined(PRODUCT__THERMOSTAT) || defined(PRODUCT__THERMOSTAT_V2)
                doc["PRODUCT__THERMOSTAT"] = true;
            #else 
                doc["PRODUCT__THERMOSTAT"] = false;
            #endif

            #ifdef PRODUCT__AIR_QUALITY_SENSOR
                doc["PRODUCT__AIR_QUALITY_SENSOR"] = true;
            #else 
                doc["PRODUCT__AIR_QUALITY_SENSOR"] = false;
            #endif

            #ifdef PRODUCT__RGB_LED_CONTROLLER
                doc["PRODUCT__RGB_LED_CONTROLLER"] = true;
            #else 
                doc["PRODUCT__RGB_LED_CONTROLLER"] = false;
            #endif

            #ifdef ROUTES_THERMOSTAT
                doc["ROUTES_THERMOSTAT"] = true;
            #else 
                doc["ROUTES_THERMOSTAT"] = false;
            #endif

            #ifdef ROUTES_PEOPLE_SENSOR
                doc["ROUTES_PEOPLE_SENSOR"] = true;
            #else 
                doc["ROUTES_PEOPLE_SENSOR"] = false;
            #endif

            #ifdef ROUTES_RGB_LEDS
                doc["ROUTES_RGB_LEDS"] = true;
            #else 
                doc["ROUTES_RGB_LEDS"] = false;
            #endif

            serializeJsonPretty(doc, *response);

            request->send(response);
        });
}
}
}

#endif /* DE20B76E_17C0_45DA_B149_EF4D4FA93D31 */
