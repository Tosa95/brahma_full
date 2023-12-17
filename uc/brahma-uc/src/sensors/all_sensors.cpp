#include "all_sensors.h"
#include "../pinout/pinout.h"
#include "../settings/settings.h"
#include "../rtc/rtc.h"

namespace Sensors {
    Networking::ArpCache arpCache(30 * 60 * 1000ULL, 1*1000ULL, 5*1000ULL);
    Networking::PersonSensor personSensor(&arpCache, &(settings.people.configs), []()->Time::DateTime {return Time::getTimeFromEpochMs(RTC::timeClient.getEpochTimeMs());});

    void init() {
        arpCache = Networking::ArpCache(settings.personSensor.persistence * 60 * 1000ULL, 1*1000ULL, 5*1000ULL);
    }
}