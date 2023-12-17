#ifndef E540BD1A_537A_4BED_BF76_6BD59C3A934EF
#define E540BD1A_537A_4BED_BF76_6BD59C3A934EF

    #include <WiFiUdp.h>
    #include "../../lib/NTPClient/NTPClient.h"

    #include "../settings/settings.h"

    namespace RTC {

        extern WiFiUDP ntpUDP;
        extern NTPClient timeClient;

        void setup();

        void update();
    }

#endif /* E540BD1A_537A_4BED_BF76_6BD59C3A934E */
