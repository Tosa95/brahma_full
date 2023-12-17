#include <Arduino.h>

#include "../lib/metrics_extensions/prometheus/prometheus_metric.h"
#include "../lib/wifi/wifi_manager.hpp"
#include "WiFi.h"
#include "controllers/all_controllers.h"
#include "fs/filesystem.h"
#include "logging/logging.h"
#include "lwip/etharp.h"
#include "metrics/metrics.h"
#include "ota/ota.h"
#include "rtc/rtc.h"
#include "sensors/all_sensors.h"
#include "settings/settings.h"
#include "version.h"
#include "webserver/webserver.h"

GenericWiFi::WiFiData wifis[10];
GenericWiFi::WiFiManager manager;

ip4_addr_t localNetwork;

void yieldFunc() {}

void setup() {
    Serial.begin(115200);
    FileSystem::init();
    loadSettings();

    analogSetAttenuation(ADC_11db);

    Controllers::init();

    manager.setLocalIP(settings.wifi.network.ip);
    manager.setGateway(settings.wifi.network.gateway);
    manager.setSubnet(settings.wifi.network.subnet);
    manager.setDns1(settings.wifi.network.dns1);
    manager.setDns2(settings.wifi.network.dns2);

    manager.mode(true, true);
    manager.setupNetwork();
    manager.turnOnAccessPoint(settings.wifi.ap.ssid, settings.wifi.ap.password);
    manager.connectToNetwork(settings.wifi.credentials.ssid,
                             settings.wifi.credentials.password);

    Sensors::init();
    WebServer::setup();
    RTC::setup();
    OTA::begin();
    Metrics::init();

    Logging::info(Logging::SYSTEM, F("Boot complete. Software version: %s"),
                  VERSION);
}

void exposeMetrics(float temp) {
    Metrics::Temperature::metric->add(temp);
    Metrics::Heater::metric->add(Controllers::heaterController.getState() == Controllers::HeaterState::HEATING ? 1.0 : 0.0);
}

void loop() {
    Controllers::restartController.signalTaskIsAlive();
    if (manager.getAPConnectedDevices() > 0) {
        Serial.println("Disabling restart");
        Controllers::restartController.disableRestart();
    } else {
        Serial.println("Enabling restart");
        Controllers::restartController.enableRestart();
    }
    Controllers::restartController.update();

    float temp = Sensors::ntcController->read();
    Controllers::chronothermo.update();
    if (Controllers::chronothermo.targetTempChanged()) {
        Controllers::heaterController.setTarget(Controllers::chronothermo.getTargetTemp(), settings.thermostat.settings.hysteresisSize);
    }
    Controllers::heaterController.update(temp);
    RTC::update();
    OTA::loop();
    Sensors::arpCache.update();
    Sensors::personSensor.update();
    Metrics::personSender.update();

    exposeMetrics(temp);

    delay(1000);
}