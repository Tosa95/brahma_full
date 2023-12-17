#ifndef A10A7B15_0732_4430_9385_C74313D52064
#define A10A7B15_0732_4430_9385_C74313D52064

#include <Arduino.h>
#include <esp_task_wdt.h>

#include "../../lib/time/types.h"

namespace Controllers {
class RestartController {
   private:
    uint32_t wdtTimeoutSeconds;
    Time::millisecond_t lastSuccessfulInternetConnection;
    Time::millisecond_t maxTimeWithNoInternetConnection;
    Time::millisecond_t restartAnywayAfter;
    bool canRestart;

   public:
    RestartController(uint32_t wdtTimeoutSeconds,
                      Time::millisecond_t maxTimeWithNoInternetConnection,
                      Time::millisecond_t restartAnywayAfter)
        : wdtTimeoutSeconds(wdtTimeoutSeconds),
          canRestart(true),
          maxTimeWithNoInternetConnection(maxTimeWithNoInternetConnection),
          restartAnywayAfter(restartAnywayAfter) {}

    void init() {
        lastSuccessfulInternetConnection = millis();
        esp_task_wdt_init(wdtTimeoutSeconds, true);
        esp_task_wdt_add(NULL);  // TODO: add a way to register also other
                                 // tasks, not just the current one
    }

    void update() {
        Time::millisecond_t timeSinceLastSuccessfulConnection =
            millis() - lastSuccessfulInternetConnection;

        if (timeSinceLastSuccessfulConnection >
            maxTimeWithNoInternetConnection) {
            if (canRestart) {
                Serial.printf(
                    "Restarting due to lack of internet connection for %lld ms "
                    "(max: %lld ms)\n",
                    timeSinceLastSuccessfulConnection,
                    maxTimeWithNoInternetConnection);
                esp_restart();
            } else {
                if (!(timeSinceLastSuccessfulConnection > restartAnywayAfter)) {
                    Serial.printf(
                        "Should restart due to lack of internet connection for "
                        "%lld ms "
                        "(max: %lld ms), but restarting is disabled. Will be "
                        "restarting anyway in %lld ms\n",
                        timeSinceLastSuccessfulConnection,
                        maxTimeWithNoInternetConnection, restartAnywayAfter);
                } else {
                    Serial.printf(
                        "Restarting due to lack of internet connection for "
                        "%lld ms "
                        "(max: %lld ms, restart anyway after: %lld ms), even if restarting is disabled\n",
                        timeSinceLastSuccessfulConnection,
                        maxTimeWithNoInternetConnection, restartAnywayAfter);
                    esp_restart();
                }
            }
        }

        Serial.printf(
            "Time since last internet connection: %lld ms (max: %lld ms)\n",
            timeSinceLastSuccessfulConnection, maxTimeWithNoInternetConnection);

        // char data[500];
        // vTaskList(data);

        // Serial.println(data);
    }

    void signalSuccesfulInternetConnection() {
        lastSuccessfulInternetConnection = millis();
    }

    void disableRestart() { canRestart = false; }

    void enableRestart() { canRestart = true; }

    void signalTaskIsAlive() { esp_task_wdt_reset(); }
};
}  // namespace Controllers

#endif /* A10A7B15_0732_4430_9385_C74313D52064 */
