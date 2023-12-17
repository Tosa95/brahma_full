#ifndef Metrics_STRUCTS_H
#define Metrics_STRUCTS_H

    
    #include "stdint.h"
    #include "../../lib/time/types.h"

    namespace Metrics {
        namespace Settings { 
            struct Metrics { 
                int32_t nSamples = 300;
                float sampleInterval = 60.0;
            };
        }
    }

#endif /* Metrics_STRUCTS_H */