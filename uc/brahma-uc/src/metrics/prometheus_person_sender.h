#ifndef DC7F1868_957A_48BA_8115_4ED0D985CD67
#define DC7F1868_957A_48BA_8115_4ED0D985CD67

#include "../../lib/metrics_extensions/prometheus/collectors/collector.h"
#include "../../lib/networking/arp_cache.h"
#include "../../lib/networking/person_sensor.h"
#include "../../lib/time/types.h"
#include "../settings/settings.h"

namespace Metrics {
class PrometheusPersonSender {
   private:
    Time::millisecond_t sendEvery;
    Networking::ArpCache *arpCache;
    Networking::PersonSensor *personSensor;
    Metrics::Extensions::Prometheus::Collector *prometheusCollector;
    Time::millisecond_t lastSent;
    char macAddress[20];
    Metrics::Extensions::Prometheus::ArrayLabelContainer *alc =
        new Metrics::Extensions::Prometheus::ArrayLabelContainer(
            new Metrics::Extensions::Prometheus::LabelAndValue *[4] {
                new Metrics::Extensions::Prometheus::LabelAndValue("job",
                                                                   "node"),
                    new Metrics::Extensions::Prometheus::LabelAndValue(
                        "kind", "primary"),
                    new Metrics::Extensions::Prometheus::LabelAndValue(
                        "instance", settings.prometheus.instanceName, false),
                    new Metrics::Extensions::Prometheus::LabelAndValue(
                        "mac", macAddress, false)
            },
            4);

    char personName[100];
    Metrics::Extensions::Prometheus::ArrayLabelContainer *alcPersonPresent =
        new Metrics::Extensions::Prometheus::ArrayLabelContainer(
            new Metrics::Extensions::Prometheus::LabelAndValue *[4] {
                new Metrics::Extensions::Prometheus::LabelAndValue("job",
                                                                   "node"),
                    new Metrics::Extensions::Prometheus::LabelAndValue(
                        "kind", "primary"),
                    new Metrics::Extensions::Prometheus::LabelAndValue(
                        "instance", settings.prometheus.instanceName, false),
                    new Metrics::Extensions::Prometheus::LabelAndValue(
                        "name", personName, false)
            },
            4);

   public:
    PrometheusPersonSender(
        Networking::ArpCache *arpCache, Networking::PersonSensor *personSensor,
        Metrics::Extensions::Prometheus::Collector *prometheusCollector,
        Time::millisecond_t sendEvery = 30 * 1000ULL)
        : arpCache(arpCache),
          personSensor(personSensor),
          sendEvery(sendEvery),
          lastSent(0),
          prometheusCollector(prometheusCollector) {}

    void update() {
        if (millis() - lastSent >= sendEvery || lastSent == 0) {
            for (int i = 0; i < arpCache->getCache().size(); i++) {
                sprintf(macAddress, "%llx",
                        arpCache->getCache().get(i).macAddress);
                prometheusCollector->collect(
                    Metrics::Extensions::Prometheus::Metric(
                        "mac", Metrics::Extensions::Prometheus::GAUGE,
                        arpCache->getCache().get(i).present ? 1.0 : 0.0, alc));
            }

            for (int i = 0; i < personSensor->getPeople()->size(); i++) {
                Networking::PersonEntry &personEntry =
                    personSensor->getPeople()->get(i);
                Networking::PersonConfig *personConfig = personSensor->getPersonConfig(personEntry.key);
                if (personConfig != nullptr) {
                    strcpy(personName, personConfig->name);
                    prometheusCollector->collect(
                        Metrics::Extensions::Prometheus::Metric(
                            "person_present",
                            Metrics::Extensions::Prometheus::GAUGE,
                            personEntry.present ? 1.0 : 0.0, alcPersonPresent));
                }
            }

            lastSent = millis();
        }
    }
};
}  // namespace Metrics

#endif /* DC7F1868_957A_48BA_8115_4ED0D985CD67 */
