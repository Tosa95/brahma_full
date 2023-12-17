#ifndef PersonSensorSettings_STRUCTS_H
#define PersonSensorSettings_STRUCTS_H

    
    #include "stdint.h"
    #include "../../lib/time/types.h"

    namespace PersonSensorSettings {
        namespace Settings { 
            struct PersonSensor { 
                uint16_t persistence = 30;
            };
        }
    }

#endif /* PersonSensorSettings_STRUCTS_H */