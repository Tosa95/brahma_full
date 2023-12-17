#ifndef F854EF16_81F4_4789_8487_B89BE14097FA
#define F854EF16_81F4_4789_8487_B89BE14097FA

#include "../../lib/networking/person_sensor.h"
#include "../../lib/time/types.h"
#include "../settings/thermostat_settings.h"
#include "chronothermo_settings.h"

namespace Controllers {

struct ChronothermoIntervalState {
    bool active;

    ChronothermoIntervalState() : active(false) {}
    ChronothermoIntervalState(bool active) : active(active) {}
};

typedef Collections::KeyedArrayEntry<key_t, ChronothermoIntervalState>
    chronothermoIntervalStateKeyPair_t;
typedef Collections::SeqKeyedArray<key_t, ChronothermoIntervalState,
                                   MAX_CHRONOTHERMO_PEOPLE>
    chronothermoIntervalStateArray_t;

class Chronothermo {
   private:
    chronothermoIntervalArray_t *intervals;
    chronothermoIntervalStateArray_t intervalStates;
    Time::getDatetimeFn_t *getDatetime;
    Networking::PersonSensor *personSensor;
    float temp = 0;
    bool tempChanged = true;
    Thermostat::Settings::Thermostat *settings;

   public:
    Chronothermo(Thermostat::Settings::Thermostat *settings,
                 chronothermoIntervalArray_t *intervals,
                 Networking::PersonSensor *personSensor,
                 Time::getDatetimeFn_t *getDatetime)
        : settings(settings),
          intervals(intervals),
          getDatetime(getDatetime),
          personSensor(personSensor) {}

    void update() {
        Time::DateTime dt = getDatetime();

        int wdayCorrected = (int)dt.wday - 2;
        if (wdayCorrected < 0) {wdayCorrected+=7;};

        dt.wday = wdayCorrected;

        intervalStates.clear();

        for (int i = 0; i < intervals->size(); i++) {
            chronothermoIntervalKeyPair_t &interval =
                intervals->getKeyPairByIndex(i);
            bool atLeastOnePersonPresent = false;

            for (int j = 0; j < interval.value.people.size(); j++) {
                if (personSensor->personPresent(interval.value.people[j])) {
                    atLeastOnePersonPresent = true;
                    break;
                }
            }

            intervalStates.add(
                ChronothermoIntervalState(interval.value.contains(dt) &&
                                          atLeastOnePersonPresent),
                interval.key);
        }

        float temp = settings->baseTemp;

        if (dt.fullMs >
            settings->overrideStart + settings->overrideFor * 1000) {
            settings->override = false;
            settings->overrideStart = 0;
        }

        if (!settings->override) {
            for (int i = 0; i < intervals->size(); i++) {
                chronothermoIntervalKeyPair_t &interval =
                    intervals->getKeyPairByIndex(i);
                ChronothermoIntervalState *state = intervalStates[interval.key];

                if (state != nullptr && state->active &&
                    interval.value.temp > temp) {
                    temp = interval.value.temp;
                }
            }
        } else {
            temp = settings->target;
        }

        tempChanged = this->temp != temp;

        this->temp = temp;
    }

    float getTargetTemp() { return temp; }

    bool targetTempChanged() { return tempChanged; }

    chronothermoIntervalStateArray_t &getIntervalStates() {
        return intervalStates;
    }
};
}  // namespace Controllers

#endif /* F854EF16_81F4_4789_8487_B89BE14097FA */
