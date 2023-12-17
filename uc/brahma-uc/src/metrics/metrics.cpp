#include "metrics.h"
#include "../../lib/metrics/queue_based/queue_metric_variable_interval.h"
#include "../../lib/metrics/queue_based/queue_metric_fixed_interval.h"
#include "../logging/logging.h"
#include "../../lib/metrics/aggregation/aggregators.h"
#include "../../lib/metrics/aggregation/fixed_interval_aggregation_metric.h"
#include "../../lib/metrics/alerts/hysteresis_alert.h"
#include "../../lib/metrics/alerts/callbacks/logging_callback.h"
#include "../../lib/metrics/routing/fan_out.h"
#include "../../lib/metrics_extensions/prometheus/prometheus_metric.h"
#include "../../lib/metrics_extensions/prometheus/factories.h"
#include "../../lib/metrics_extensions/prometheus/collectors/direct_push_collector.h"
#include "../sensors/all_sensors.h"
#include "../controllers/all_controllers.h"
#include "../rtc/rtc.h"
#include "../settings.h"

#define METRICS_N_SAMPLES 300
#define METRICS_REFRESH_INTERVAL 60.0

using namespace Metrics::Extensions::Prometheus;
namespace Metrics {
    

    Metrics::Extensions::Prometheus::Collector *prometheusCollector = new Metrics::Extensions::Prometheus::DirectPushCollector(
        settings.prometheus.pushGatewayUrl,
        [](){
            Logging::info(0, F("onSuccessfulUpdate event received"));
            Controllers::restartController.signalSuccesfulInternetConnection();
        }
    );
    PrometheusPersonSender personSender(&Sensors::arpCache, &Sensors::personSensor, prometheusCollector);

    // --------------------------- LOGGING -----------------------

    namespace LoggingM {
        QueueMetricVariableInterval<Logging::LogEntry> baseMetric(
            100, 
            [](Logging::LogEntry &e){return e.timestamp;},
            [](Logging::LogEntry &e){e.deleteData();}
        ); 
        Metric<Logging::LogEntry,Logging::LogEntry> *metric = &baseMetric;

        JsonMetric *metricJson = new LogEntryJsonMetric(metric, &Logging::jf);
    }
    

    
    // --------------------------- MEMORY ---------------------------
    namespace Memory {
        Metrics::InstantMetric<int, int> *metric;
        JsonMetric *metricJson;

        void init() {
            metric = Metrics::Extensions::Prometheus::getPrometheusAndLocalMetric(
                "free_memory",
                new ArrayLabelContainer (
                    new LabelAndValue* [3] {
                    new LabelAndValue("job","node"),
                    new LabelAndValue("kind","primary"),
                    new LabelAndValue("instance",settings.prometheus.instanceName, false)
                },
                3
                ),
                MetricType::GAUGE,
                new Metrics::MinFolder<int>(),
                prometheusCollector,
                [](){return RTC::timeClient.getEpochTimeMs();},
                settings.metrics.nSamples,
                (int)(settings.metrics.sampleInterval * 1000)
            );

            metricJson = new IntJsonMetric(metric);
        }
    }

    #ifdef METRICS_TEMPERATURE
        namespace Temperature {
            Metrics::InstantMetric<float, float> *metric;
            JsonMetric *metricJson;

            void init() {
                metric = Metrics::Extensions::Prometheus::getPrometheusAndLocalMetric(
                    "temperature",
                    new ArrayLabelContainer (
                        new LabelAndValue* [3] {
                        new LabelAndValue("job","node"),
                        new LabelAndValue("kind","primary"),
                        new LabelAndValue("instance",settings.prometheus.instanceName, false)
                    },
                    3
                    ),
                    MetricType::GAUGE,
                    new Metrics::AvgFolder<float, float>(),
                    prometheusCollector,
                    [](){return RTC::timeClient.getEpochTimeMs();},
                    settings.metrics.nSamples,
                    (int)(settings.metrics.sampleInterval * 1000)
                );

                metricJson = new FloatJsonMetric<float>(metric);
            }
        }
    #endif

     #ifdef METRICS_EXT_TEMPERATURE
        namespace ExtTemperature {
            Metrics::InstantMetric<float, float> *metric;
            JsonMetric *metricJson;

            void init() {
                metric = Metrics::Extensions::Prometheus::getPrometheusAndLocalMetric(
                    "ext_temperature",
                    new ArrayLabelContainer (
                        new LabelAndValue* [3] {
                        new LabelAndValue("job","node"),
                        new LabelAndValue("kind","primary"),
                        new LabelAndValue("instance",settings.prometheus.instanceName, false)
                    },
                    3),
                    MetricType::GAUGE,
                    new Metrics::AvgFolder<float, float>(),
                    prometheusCollector,
                    [](){return RTC::timeClient.getEpochTimeMs();},
                    settings.metrics.nSamples,
                    (int)(settings.metrics.sampleInterval * 1000)
                );

                metricJson = new FloatJsonMetric<float>(metric);
            }
        }
    #endif

    #ifdef METRICS_HUMIDITY
        namespace Humidity {
            Metrics::InstantMetric<float, float> *metric;
            JsonMetric *metricJson;

            void init() {
                metric = Metrics::Extensions::Prometheus::getPrometheusAndLocalMetric(
                    "humidity",
                    new ArrayLabelContainer (
                        new LabelAndValue* [3] {
                        new LabelAndValue("job","node"),
                        new LabelAndValue("kind","primary"),
                        new LabelAndValue("instance",settings.prometheus.instanceName, false)
                    },
                    3
                    ),
                    MetricType::GAUGE,
                    new Metrics::AvgFolder<float, float>(),
                    prometheusCollector,
                    [](){return RTC::timeClient.getEpochTimeMs();},
                    settings.metrics.nSamples,
                    (int)(settings.metrics.sampleInterval * 1000)
                );

                metricJson = new FloatJsonMetric<float>(metric);
            }
        }
    #endif

    #ifdef METRICS_VOC
        namespace VOC {
            Metrics::InstantMetric<float, float> *metric;

            JsonMetric *metricJson;

            void init() {
                metric = Metrics::Extensions::Prometheus::getPrometheusAndLocalMetric(
                    "voc",
                    new ArrayLabelContainer (
                        new LabelAndValue* [3] {
                        new LabelAndValue("job","node"),
                        new LabelAndValue("kind","primary"),
                        new LabelAndValue("instance",settings.prometheus.instanceName, false)
                    },
                    3
                    ),
                    MetricType::GAUGE,
                    new Metrics::AvgFolder<float, float>(),
                    prometheusCollector,
                    [](){return RTC::timeClient.getEpochTimeMs();},
                    settings.metrics.nSamples,
                    (int)(settings.metrics.sampleInterval * 1000)
                );

                metricJson = new FloatJsonMetric<float>(metric);
            }
        }
    #endif

    #ifdef METRICS_HEATER
        namespace Heater {
            Metrics::InstantMetric<float, float> *metric;

            JsonMetric *metricJson;

            void init () {
                metric = Metrics::Extensions::Prometheus::getPrometheusAndLocalMetric(
                    "heater",
                    new ArrayLabelContainer (
                        new LabelAndValue* [3] {
                        new LabelAndValue("job","node"),
                        new LabelAndValue("kind","primary"),
                        new LabelAndValue("instance",settings.prometheus.instanceName, false)
                    },
                    3
                    ),
                    MetricType::GAUGE,
                    new Metrics::IdentityFolder<float>(),
                    prometheusCollector,
                    [](){return RTC::timeClient.getEpochTimeMs();},
                    settings.metrics.nSamples,
                    (int)(settings.metrics.sampleInterval * 1000)
                );

                metricJson = new FloatJsonMetric<float>(metric);
            }
        }
    #endif

    namespace RSSI {
        Metrics::InstantMetric<int, int> *metric;
        JsonMetric *metricJson;

        void init() {
            metric = Metrics::Extensions::Prometheus::getPrometheusAndLocalMetric(
                "rssi",
                new ArrayLabelContainer (
                    new LabelAndValue* [2] {
                    new LabelAndValue("job","node"),
                    new LabelAndValue("instance",settings.prometheus.instanceName, false)
                },
                2
                ),
                MetricType::GAUGE,
                new Metrics::MinFolder<int>(),
                prometheusCollector,
                [](){return RTC::timeClient.getEpochTimeMs();},
                settings.metrics.nSamples,
                (int)(settings.metrics.sampleInterval * 1000)
            );

            metricJson = new IntJsonMetric(metric);
        }
    }

    // --------------------------- EXPOSED --------------------------
    ExposedMetricsEntry _exposedMetrics[] = {
        ExposedMetricsEntry {
            {.name="logs"},
            {.unit="str"},
            .metric=&LoggingM::metricJson
        },
        ExposedMetricsEntry {
            {.name="mem"},
            {.unit="B"},
            .metric=&Memory::metricJson
        },
        ExposedMetricsEntry {
            {.name="rssi"},
            {.unit="dB"},
            .metric=&RSSI::metricJson
        },

        #ifdef METRICS_TEMPERATURE
            ExposedMetricsEntry {
                {.name="temp"},
                {.unit="°C"},
                .metric=&Temperature::metricJson
            },
        #endif

        #ifdef METRICS_EXT_TEMPERATURE
            ExposedMetricsEntry {
                {.name="ext_temp"},
                {.unit="°C"},
                .metric=&ExtTemperature::metricJson
            },
        #endif
            
        #ifdef METRICS_HUMIDITY
            ExposedMetricsEntry {
                {.name="humidity"},
                {.unit="%"},
                .metric=&Humidity::metricJson
            },
        #endif

        #ifdef METRICS_VOC
            ExposedMetricsEntry {
                {.name="voc"},
                {.unit=""},
                .metric=&VOC::metricJson
            },
        #endif

        #ifdef METRICS_HEATER
        ExposedMetricsEntry {
            {.name="heater"},
            {.unit=""},
            .metric=&Heater::metricJson
        }
        #endif
    };

    ExposedMetricsEntry *exposedMetrics = _exposedMetrics;

    size_t exposedMetricsNumber = sizeof(_exposedMetrics) / sizeof(ExposedMetricsEntry);

    void init() {
        Memory::init();
        RSSI::init();

        #ifdef METRICS_TEMPERATURE 
            Temperature::init(); 
        #endif

        #ifdef METRICS_EXT_TEMPERATURE
            ExtTemperature::init();
        #endif

        #ifdef METRICS_HUMIDITY
            Humidity::init();
        #endif

        #ifdef METRICS_VOC
            VOC::init();
        #endif

        #ifdef METRICS_HEATER
            Heater::init();
        #endif


    }
}
