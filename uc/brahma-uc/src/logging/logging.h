#ifndef BA191D46_6917_4C66_9C2D_A4E0F52D0928
#define BA191D46_6917_4C66_9C2D_A4E0F52D0928

    #include "stdint.h"
    #include "../../lib/logging/logging.h"
    #include "../../lib/logging/formatters/json_formatter.h"
    #include "../../lib/logging/formatters/standard_formatter.h"

    namespace Logging {
        const uint8_t SYSTEM = 0;
        const uint8_t WIFI = 1;
        const uint8_t WEBSERVER = 2;
        const uint8_t OTA = 3;
        const uint8_t METRICS = 4;
        const uint8_t CAMERA = 5;

        const Logging::LogLevel serialLevel = Logging::LogLevel::DEBUG;
        const Logging::LogLevel metricLevel = Logging::LogLevel::DEBUG;
        const Logging::LogLevel telegramLevel = Logging::LogLevel::DEBUG;


        extern JsonFormatter jf;
        extern StandardFormatter sf;


        extern char *loggingBuffer;
        extern size_t loggingBufferSize;

        void enableAllLoggers();
        void flushAllLoggers();
    }

#endif /* BA191D46_6917_4C66_9C2D_A4E0F52D0928 */
