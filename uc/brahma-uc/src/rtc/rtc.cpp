#include "rtc.h"
#include "../logging/logging.h"
#include <Arduino.h>
#include "../../lib/metrics/aggregation/aggregators.h"
#include "../controllers/all_controllers.h"

namespace RTC {

    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, settings.rtc.ntpHost, settings.rtc.utcOffsetHours*3600);

    const int maxFailedUpdatesPerMinute = 10;
    Metrics::TimeDecreasingCounterAggregator<float,int> failCounter(maxFailedUpdatesPerMinute/60.0);

    void setup() {
        timeClient = NTPClient(ntpUDP, settings.rtc.ntpHost, settings.rtc.utcOffsetHours*3600);
        timeClient.begin();
        update();
    }

    void update() {
        bool result = timeClient.update();

        if (result) {
            failCounter.add(0);
            Controllers::restartController.signalSuccesfulInternetConnection();
            // Logging::debug(Logging::SYSTEM, F("Time updated"));
        } else {
            failCounter.add(1);

            if (failCounter.get() > maxFailedUpdatesPerMinute) {
                Logging::warn(Logging::SYSTEM, F("Unable to update time for more than %d times in a minute."), maxFailedUpdatesPerMinute);
                failCounter.clear();
            }
        }
    }
}