#include <Wire.h>

#include "../lib/metrics_extensions/prometheus/prometheus_metric.h"
#include "../lib/wifi/wifi_manager.hpp"
#include "DHT.h"
#include "SparkFun_SGP40_Arduino_Library.h"  // Click here to get the library: http://librarymanager/All#SparkFun_SGP40
#include "fs/filesystem.h"
#include "metrics/metrics.h"
#include "settings/settings.h"
#include "rtc/rtc.h"
#include "ota/ota.h"
#include "../controllers/all_controllers.h"

#include "settings/data_to_fs.h"
#include "fs/filesystem.h"
#include "webserver/webserver.h"
#include "pinout/pinout.h"

#include "sensors/ntc.h"

#define DHTTYPE DHT22
#define SAVE_EVERY_MS 1800000 // Saves params every 30m
// #define SAVE_EVERY_MS 10000 // Saves params every 30m
#define VOC_PARAMS_FILE "/voc_params_file.bin"

DHT tempHumSensor(Pinout::DHT, DHTTYPE);
SGP40 vocSensor;  // create an object of the SGP40 class
GenericWiFi::WiFiData wifis[10];
GenericWiFi::WiFiManager manager;

ip4_addr_t localNetwork;

void yieldFunc() {}

void setup() {
    Serial.begin(115200);

    FileSystem::init();
    loadSettings();
    Controllers::restartController.init();

    pinMode(Pinout::NTC, INPUT);

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
    
    Wire.begin();
    tempHumSensor.begin();

    //   mySensor.enableDebugging(); // Uncomment this line to print useful
    //   debug messages to Serial

    // Initialize sensor
    VocAlgorithmParams vocParams;
    Serial.println("Loading VOC params");
    bool res = DataToFs::load(VOC_PARAMS_FILE, &vocParams, sizeof(vocParams),
                *FileSystem::mainFS, false);
    

    if (vocSensor.begin() == false) {
        Serial.println(F("SGP40 not detected. Check connections. Freezing..."));
        while (1)
            ;  // Do nothing more
    }

    if (res) {
        vocSensor.setVOCAlgorithmParams(vocParams);
        Serial.println("Loaded VOC params");
    } else {
        Serial.println("No VOC params found");
    }
}

unsigned long lastSaved = 0;

void updateRestartController(GenericWiFi::WiFiManager manager) {
    Controllers::restartController.signalTaskIsAlive();
    Controllers::restartController.update();
    if (manager.getAPConnectedDevices() > 0) {
        Serial.println("Disabling restart");
        Controllers::restartController.disableRestart();
    } else {
        Serial.println("Enabling restart");
        Controllers::restartController.enableRestart();
    }
}

void loop() {
    RTC::update();
    OTA::loop();

    updateRestartController(manager);    

    float h = tempHumSensor.readHumidity();
    float t = tempHumSensor.readTemperature();
    
    float et=Sensors::ntcController->read();
    int32_t voc = vocSensor.getVOCindex(h, t);

    Serial.printf("%.2f°C %.2F%% %d VOC\n", t, h, voc);

    VocAlgorithmParams &vocParams = vocSensor.getVOCAlgorithmParams();

    Metrics::Temperature::metric->add(t);
    Metrics::ExtTemperature::metric->add(et);
    Metrics::Humidity::metric->add(h);
    Metrics::VOC::metric->add(voc);

    if ((millis() - lastSaved) >= SAVE_EVERY_MS) {
        Serial.println("Saving VOC params");
        DataToFs::save(VOC_PARAMS_FILE, &vocParams, sizeof(vocParams),
                   *FileSystem::mainFS);
        Serial.println("Saved VOC params");
        lastSaved=millis();
    }

    delay(1000);  // Wait 1 second - the Sensirion VOC algorithm expects a
                  // sample rate of 1Hz
}