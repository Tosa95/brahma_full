#ifndef D9AF4AF4_2082_4914_9B41_79562EC8A4973
#define D9AF4AF4_2082_4914_9B41_79562EC8A4973

#include <ESPAsyncWebServer.h>

#include <functional>

#include "../../controllers/all_controllers.h"
#include "../../rtc/rtc.h"
#include "../../sensors/all_sensors.h"
#include "../../settings.h"
#include "../utils.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"

using namespace WebServer;

namespace Handlers {
namespace NetworkingUtils {
void attachHandlers(AsyncWebServer *server) {
    addHandler(
        server, "/api/networking/mac", HTTP_GET,
        [](AsyncWebServerRequest *request) {
            uint32_t ip = request->client()->getRemoteAddress();
            ip4_addr_t addr = {.addr=ip};
            Networking::CacheEntry *entry = Sensors::arpCache.getEntryByIp(addr);
            char resp[100];

            if (entry != nullptr && entry->present) {
                sprintf(resp, "{\"msg\":\"OK\",\"mac\":%lld}", entry->macAddress);
                request->send(200, "text/json", resp);
            } else {
                request->send(404, "text/json", "{\"msg\":\"Mac address not in cache\"}");
            }
        });
}
}  // namespace Networking
}  // namespace Handlers


#endif /* D9AF4AF4_2082_4914_9B41_79562EC8A4973 */
