#ifndef Thermostat_STRUCTS_H
#define Thermostat_STRUCTS_H

    
    #include "stdint.h"
    #include "../../lib/time/types.h"

    namespace Thermostat {
        namespace Settings { 
            struct Thermostat { 
                float target = 22.0;
                float baseTemp = 14.0;
                bool override = false;
                uint64_t overrideStart = 0;
                uint64_t overrideFor = 3600;
                float hysteresisSize = 1.0;
            };
        }
    }

#endif /* Thermostat_STRUCTS_H */