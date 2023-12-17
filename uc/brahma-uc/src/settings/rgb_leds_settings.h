#ifndef RGBLeds_STRUCTS_H
#define RGBLeds_STRUCTS_H

    
    #include "stdint.h"
    #include "../../lib/time/types.h"

    namespace RGBLeds {
        namespace Settings { 
            struct RGBLeds { 
                int32_t r = 100;
                int32_t g = 100;
                int32_t b = 100;
            };
        }
    }

#endif /* RGBLeds_STRUCTS_H */