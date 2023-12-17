#ifndef E79F1791_FAA7_411D_BC89_77CBA8114E1F
#define E79F1791_FAA7_411D_BC89_77CBA8114E1F


    #include "../../lib/logging/formatters/standard_formatter.h"
    #include "../../lib/logging/formatters/light_formatter.h"
    #include "../../lib/logging/sinks/serial_sink.h"
    #include "../../lib/logging/sinks/metric_sink.h"
    #include "../../lib/logging/sinks/level_selecting_sink.h"
    #include "../../lib/logging/sinks/compound_log_sink.h"
    #include "../../lib/logging/sinks/telegram_sink.h"
    #include "../../lib/logging/sinks/buffered_sink.h"
    #include "../../lib/logging/sinks/disablable_sink.h"

    #include <Esp.h>


    #include "../../lib/time/types.h"
    #include "../metrics/metrics.h"
    // #include "../telegram/telegram.h"
    #include "../rtc/rtc.h"
    #include "logging.h"

    #define MAX_LOG_SIZE 300

    namespace Logging {
        char _loggingBuffer[MAX_LOG_SIZE];
        char *loggingBuffer = _loggingBuffer;
        size_t loggingBufferSize = MAX_LOG_SIZE;

        const char *labelsTable[] = {
            "SYSTEM",
            "WIFI",
            "WEBSERVER",
            "OTA",
            "METRICS",
            "CAMERA"
        };

        const size_t numLabels = sizeof(labelsTable)/sizeof(char *);

        StandardFormatter sf(labelsTable, false);
        LightFormatter lf(labelsTable, false);
        JsonFormatter jf(labelsTable, false);

        SerialSink ss(&sf, loggingBuffer);
        LevelSelectingSink lss(serialLevel, &ss);

        MetricSink ms(Metrics::LoggingM::metric);
        LevelSelectingSink mlss(metricLevel, &ms);

        // TelegramSink ts(&lf, loggingBuffer, &Telegram::bot, (const char *) settings.telegram.chatId);
        // BufferedSink tbf(5, &ts);
        // LevelSelectingSink tlss(telegramLevel, &tbf);
        // DisablableSink tds(false, &tlss);

        LogSink *loggers[] = {
            &mlss,
            &lss,
            // &tds
        };

        CompoundLogSink cls(loggers, sizeof(loggers)/sizeof(LogSink *));

        LogSink *logger = &cls;

        Time::millisecond_t getCurrentTime() {
            return RTC::timeClient.getEpochTimeMs();
        }

        void enableAllLoggers() {
            // tds.setEnabled(true);
        }

        void flushAllLoggers() {
            // if ((int)ESP.getFreeHeap() > 15000) {
            //     tbf.flush();
            // }
        }
    }

#endif /* E79F1791_FAA7_411D_BC89_77CBA8114E1F */
