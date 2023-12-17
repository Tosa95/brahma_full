#ifndef AD38B594_9D60_49B9_9953_56465AC52031
#define AD38B594_9D60_49B9_9953_56465AC52031

#ifdef USES_NTC
    #include "ntc.h"
#endif

#include "../../lib/networking/arp_cache.h"
#include "../../lib/networking/person_sensor.h"

 namespace Sensors {
     extern Networking::ArpCache arpCache;
     extern Networking::PersonSensor personSensor;

     void init();
 }

#endif /* AD38B594_9D60_49B9_9953_56465AC52031 */
