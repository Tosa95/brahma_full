#ifndef PrometheusSettings_STRUCTS_H
#define PrometheusSettings_STRUCTS_H

    
    #include "stdint.h"
    #include "../../lib/time/types.h"
    #include "config.h"

    namespace PrometheusSettings {
        namespace Settings { 
            struct Prometheus { 
                char pushGatewayUrl[128] = "http://192.168.1.191:9091";
                char instanceName[32] = DEFAULT_INSTANCE_NAME;
            };
        }
    }

#endif /* PrometheusSettings_STRUCTS_H */