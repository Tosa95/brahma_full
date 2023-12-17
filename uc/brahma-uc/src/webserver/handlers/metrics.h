#ifndef C0359679_E5C1_4F9B_BB61_57D9CB229331
#define C0359679_E5C1_4F9B_BB61_57D9CB229331
#ifndef FCBE7A87_1A05_4DC3_89B5_E877475B305234
#define FCBE7A87_1A05_4DC3_89B5_E877475B305234

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <stdlib.h>

#include "../../../lib/metrics/json_translation/json_metrics.h"
#include "../../../lib/metrics/json_translation/printer.h"
#include "../../logging/logging.h"
#include "../../metrics/metrics.h"
#include "../../settings.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"

namespace Handlers {
namespace MetricsRoutes {

struct GetMetricParams {
    Time::millisecond_t startTs = 0;
    Time::millisecond_t endTs = 100000000000000;
    Time::millisecond_t interval = 0;
    int maxEntries = 1000;

    char metricName[EXPOSED_METRIC_NAME_SZ];

    bool error = false;
};

GetMetricParams extractParams(AsyncWebServerRequest *request) {
    GetMetricParams res;

    if (request->hasParam(F("name"))) {
        strcpy(res.metricName, request->getParam(F("name"))->value().c_str());
    } else {
        request->send(
            404, "text/json",
            "{\"msg\":\"KO\",\"cause\":\"Must provide the metric name\"}");
        res.error = true;
        return res;
    }

    if (request->hasParam(F("start"))) {
        res.startTs = strtoull(request->getParam(F("start"))->value().c_str(),
                               nullptr, 10);
    }

    if (request->hasParam(F("end"))) {
        res.endTs =
            strtoull(request->getParam(F("end"))->value().c_str(), nullptr, 10);
    }

    if (request->hasParam(F("interval"))) {
        res.interval = strtoull(
            request->getParam(F("interval"))->value().c_str(), nullptr, 10);
    }

    if (request->hasParam(F("maxEntries"))) {
        res.maxEntries = strtod(
            request->getParam(F("maxEntries"))->value().c_str(), nullptr);
    }

    return res;
}

Metrics::ExposedMetricsEntry *searchMetric(GetMetricParams &params) {
    for (size_t i = 0; i < Metrics::exposedMetricsNumber; i++) {
        if (strcmp(Metrics::exposedMetrics[i].name, params.metricName) == 0) {
            return &Metrics::exposedMetrics[i];
        }
    }

    return nullptr;
}

void getMetric(AsyncWebServerRequest *request) {
    GetMetricParams params = extractParams(request);

    if (params.error) {
        return;
    }

    Metrics::ExposedMetricsEntry *exposedMetric = searchMetric(params);

    if (exposedMetric == nullptr) {
        request->send(404, "text/json",
                      "{\"msg\":\"KO\",\"cause\":\"Metric not found\"}");
        return;
    }

    Metrics::JsonMetric *metric = *(exposedMetric->metric);

    Metrics::BufferPrinter printer(Logging::loggingBuffer,
                                   Logging::loggingBufferSize);

    metric->setPrinter(&printer);

    Metrics::Iterator<void *> *it =
        metric->query(params.startTs, params.endTs, params.interval);

    AsyncResponseStream *response = request->beginResponseStream("text/json");

    response->printf(
        "{\"name\":\"%s\",\"unit\":\"%s\",\"time_offset\":%d,\"data\":",
        params.metricName, exposedMetric->unit, settings.rtc.utcOffsetHours);

    response->print("[");
    int i = 0;
    while (it->next() != nullptr &&
           (i < params.maxEntries || params.maxEntries == 0)) {
        if (i > 0) {
            response->print(",");
        }
        response->print(Logging::loggingBuffer);
        i++;
        optimistic_yield(0);
    }
    response->print("]");
    response->printf(",\"current_value\":");
    metric->printCurrentValue();
    response->print(Logging::loggingBuffer);
    response->printf(",\"data_left\":%s}",
                     i >= params.maxEntries ? "true" : "false");

    delete it;

    request->send(response);
}

void getMetricName(AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("text/json");

    response->print("[");
    for (int i = 0; i < Metrics::exposedMetricsNumber; i++) {
        if (i > 0) {
            response->print(",");
        }
        response->printf("\"%s\"", Metrics::exposedMetrics[i].name);
    }
    response->print("]");

    request->send(response);
}

void attachHandlers(AsyncWebServer *server) {
    server->on("/api/metric/name", HTTP_GET, getMetricName);
    server->on("/api/metric", HTTP_GET, getMetric);
}

}  // namespace MetricsRoutes
}  // namespace Handlers

#endif /* FCBE7A87_1A05_4DC3_89B5_E877475B3052 */

#endif /* C0359679_E5C1_4F9B_BB61_57D9CB229331 */
