#ifndef BFB9AD3E_EE4A_4436_817B_053E6952BCEC

    #include "../../lib/metrics/metric.h"
    #include "../../lib/metrics/instant_metric.h"
    #include "../../lib/metrics/json_translation/json_metrics.h"
    #include "../config.h"

    #include "../../lib/logging/log_entry.h"
    #include "../../lib/metrics_extensions/prometheus/collectors/direct_push_collector.h"

    #include "prometheus_person_sender.h"

    #define EXPOSED_METRIC_NAME_SZ 10
    #define UNIT_SZ 4

    namespace Metrics {

        extern  Metrics::Extensions::Prometheus::Collector *prometheusCollector;
        extern  PrometheusPersonSender personSender;

        namespace LoggingM {
            extern Metrics::Metric<Logging::LogEntry,Logging::LogEntry> *metric;
        }

        namespace Memory {
            extern Metrics::InstantMetric<int,int> *metric;
        }

        #ifdef METRICS_TEMPERATURE
            namespace Temperature {
                extern Metrics::InstantMetric<float,float> *metric;
            }
        #endif

        #ifdef METRICS_EXT_TEMPERATURE
            namespace ExtTemperature {
                extern Metrics::InstantMetric<float,float> *metric;
            }
        #endif

        #ifdef METRICS_HUMIDITY
            namespace Humidity {
                extern Metrics::InstantMetric<float,float> *metric;
            }
        #endif

        #ifdef METRICS_VOC
            namespace VOC {
                extern Metrics::InstantMetric<float,float> *metric;
            }
        #endif

        #ifdef METRICS_HEATER
            namespace Heater {
                extern Metrics::InstantMetric<float,float> *metric;
            }
        #endif

        namespace RSSI {
            extern Metrics::InstantMetric<int,int> *metric;
        }

        struct ExposedMetricsEntry {
            char name[EXPOSED_METRIC_NAME_SZ];
            char unit[UNIT_SZ];
            Metrics::JsonMetric **metric;
        };

        extern ExposedMetricsEntry *exposedMetrics;
        extern size_t exposedMetricsNumber;

        void init();
    }

#define BFB9AD3E_EE4A_4436_817B_053E6952BCEC
#endif /* BFB9AD3E_EE4A_4436_817B_053E6952BCEC */
