#ifndef F2A64BA4_1CCF_4B7B_8710_F92F723B2A24
#define F2A64BA4_1CCF_4B7B_8710_F92F723B2A24
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
    // Serial.println("hoho");
    FileSystem::init();
    loadSettings();

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

    WebServer::setup();
    RTC::setup();
    OTA::begin();
    Metrics::init();

    Logging::info(Logging::SYSTEM, F("Boot complete. Software version: %s"),
                  VERSION);
}

void loop() {
    // Controllers::restartController.signalTaskIsAlive();
    // if (manager.getAPConnectedDevices() > 0) {
    //     Serial.println("Disabling restart");
    //     Controllers::restartController.disableRestart();
    // } else {
    //     Serial.println("Enabling restart");
    //     Controllers::restartController.enableRestart();
    // }

    Serial.println("hoho");
    Logging::info(Logging::SYSTEM, F("HAHAHA"),
                  VERSION);

    delay(1000);
}


#endif /* F2A64BA4_1CCF_4B7B_8710_F92F723B2A23 */
