#ifndef WiFiSettings_STRUCTS_H
#define WiFiSettings_STRUCTS_H

    
    #include "stdint.h"
    #include "../../lib/time/types.h"
    #include "config.h"

    namespace WiFiSettings {
        namespace Settings { 
            struct AccessPoint { 
                char ssid[32] = "BRAHMA-AP";
                char password[32] = "BRAHMA-ESP32";
            };
            struct Network { 
                char ip[32] = "192.168.1.182";
                char gateway[32] = "192.168.1.1";
                char subnet[32] = "255.255.255.0";
                char dns1[32] = "192.168.1.1";
                char dns2[32] = "8.8.8.8";
            };
            struct Credentials { 
                char ssid[32] = DEFAULT_WIFI_SSID;
                char password[32] = DEFAULT_WIFI_PASS;
            };
        }
    }

#endif /* WiFiSettings_STRUCTS_H */