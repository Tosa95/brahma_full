#ifndef RTCSettings_STRUCTS_H
#define RTCSettings_STRUCTS_H

    
    #include "stdint.h"
    #include "../../lib/time/types.h"

    namespace RTCSettings {
        namespace Settings { 
            struct RTC { 
                char ntpHost[128] = "pool.ntp.org";
                int16_t utcOffsetHours = 1;
            };
        }
    }

#endif /* RTCSettings_STRUCTS_H */