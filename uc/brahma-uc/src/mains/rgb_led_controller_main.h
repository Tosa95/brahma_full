#ifndef B9C87247_F603_4C49_A8A7_831B79EBC005
#define B9C87247_F603_4C49_A8A7_831B79EBC005
#include "../lib/metrics_extensions/prometheus/prometheus_metric.h"
#include "../lib/wifi/wifi_manager.hpp"
#include "metrics/metrics.h"
#include "settings/settings.h"
#include "rtc/rtc.h"
#include "ota/ota.h"
#include "../controllers/all_controllers.h"

#include "settings/data_to_fs.h"
#include "fs/filesystem.h"
#include "webserver/webserver.h"
#include "pinout/pinout.h"

GenericWiFi::WiFiData wifis[10];
GenericWiFi::WiFiManager manager;

ip4_addr_t localNetwork;

void yieldFunc() {}

void setColors() {
    ledcWrite(R_CHANNEL, settings.rgbLeds.r);
    ledcWrite(G_CHANNEL, settings.rgbLeds.g);
    ledcWrite(B_CHANNEL, settings.rgbLeds.b);
}

void setup() {
    ledcSetup(R_CHANNEL, PWM_FREQ, 8);
    ledcSetup(G_CHANNEL, PWM_FREQ, 8);
    ledcSetup(B_CHANNEL, PWM_FREQ, 8);

    ledcAttachPin(Pinout::R_PIN, R_CHANNEL);
    ledcAttachPin(Pinout::G_PIN, G_CHANNEL);
    ledcAttachPin(Pinout::B_PIN, B_CHANNEL);

    Serial.begin(115200);

    FileSystem::init();
    loadSettings();

    setColors();

    Controllers::restartController.init();



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

    RTC::setup();
    OTA::begin();
    Metrics::init();
    WebServer::setup();
}

unsigned long lastSaved = 0;

void updateRestartController(GenericWiFi::WiFiManager manager) {
    Controllers::restartController.signalTaskIsAlive();
    Controllers::restartController.update();
    if (manager.getAPConnectedDevices() > 0) {
        Controllers::restartController.disableRestart();
    } else {
        Controllers::restartController.enableRestart();
    }
}

int i = 0;

void loop() {
    RTC::update();
    OTA::loop();
    updateRestartController(manager);    

    setColors();

    i++;

    if (i >= 256) {
        i = 0;
    }

    delay(20);
}


#endif /* B9C87247_F603_4C49_A8A7_831B79EBC005 */
