#include "all_controllers.h"
#include "../rtc/rtc.h"
#include "../pinout/pinout.h"
#include "../settings/settings.h"
#include "../../lib/time/types.h"
#include "../sensors/all_sensors.h"
#include "config.h"

namespace Controllers {

    RestartController restartController(60, 1 * 60 * 1000, 5 * 60 * 1000);

    #if defined(PRODUCT__THERMOSTAT) || defined(PRODUCT__THERMOSTAT_V2)
        HeaterController heaterController(Pinout::HEATER_ON, Pinout::HEATER_OFF, true, 9.5, 10.5, 0.95);
        Chronothermo chronothermo(&(settings.thermostat.settings), &(settings.thermostat.intervals), &Sensors::personSensor, []()->Time::DateTime {return Time::getTimeFromEpochMs(RTC::timeClient.getEpochTimeMs());});
        

        void init() {
            restartController.init();

            heaterController.init();

            heaterController.setTarget(chronothermo.getTargetTemp(), settings.thermostat.settings.hysteresisSize);

            updateFromSettings();
        }

        void updateFromSettings() {
            // heaterController.setTarget(settings.thermostat.settings.target, settings.thermostat.settings.hysteresisSize);
        }
    #endif
}