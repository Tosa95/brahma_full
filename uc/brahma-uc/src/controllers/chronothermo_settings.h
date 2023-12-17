#ifndef B3DD2640_0173_4BEA_AB60_59520174CE67
#define B3DD2640_0173_4BEA_AB60_59520174CE67

#define MAX_CHRONOTHERMO_INTERVALS 30
#define MAX_CHRONOTHERMO_PEOPLE 20
#define N_DAYS 7

#include <stdint.h>
#include "../../lib/collections/seq_keyed_array.h"
#include "../../lib/time/types.h"
#include "../../lib/math_utils/intervals.h"

namespace Controllers {

    typedef uint16_t key_t;
    typedef Collections::KeyedArrayEntry<key_t,uint16_t> personKeyPair_t;
    typedef Array<uint16_t, MAX_CHRONOTHERMO_PEOPLE> peopleArray_t;

    struct ChronothermoInterval {
        peopleArray_t people;
        Time::millisecond_t from;
        Time::millisecond_t to;
        float temp;
        bool days[N_DAYS];

        ChronothermoInterval() {
            for (int i = 0; i < N_DAYS; i++) {
                days[i] = false;
            }
        }

        bool contains(Time::DateTime dt) {
            uint64_t currTimeInSeconds = dt.hour * 3600L + dt.minute*60L + dt.second;
            return Math::inCircularInterval<uint64_t>(currTimeInSeconds, from, to, 24L * 60L * 60L) && days[dt.wday];
        }
    };

    typedef Collections::KeyedArrayEntry<key_t,ChronothermoInterval> chronothermoIntervalKeyPair_t;
    typedef Collections::SeqKeyedArray<key_t, ChronothermoInterval, MAX_CHRONOTHERMO_INTERVALS> chronothermoIntervalArray_t;

}

#endif /* B3DD2640_0173_4BEA_AB60_59520174CE67 */
