#include <Arduino.h>
#include "fclib/Logging.h"
#include "fclib/Net.h"
#include "fclib/File.h"
#include "fclib/Config.h"
#include "test/Test.h"
#include "fclib/System.h"
#include "./HallApp.h"

using namespace FCLIB;

FCLIB::Logger *logger = new FCLIB::Logger("main");
HallApp hallApp;

WiFiSetup wifi;
ConfigFile config;
String mqttServer;
String mqttUser;
String mqttPassword;
String deviceName;
bool testsPass = true;
Board *board = Board::get();

bool FORCE_WIFI_PORTAL = false;

void setup()
{
    setDefaultLoggerLevel(DEBUG_LEVEL);
    setModuleLoggerLevel("ConfigFile", WARN_LEVEL);
    setModuleLoggerLevel("FileReader", WARN_LEVEL);
    hallApp.runTests(HALLLIGHTS_TEST::runTests, true);

    testsPass = HALLLIGHTS_TEST::runTests();
    if (!testsPass)
    {
        logger->error("tests failed");
        return;
    }

    logger->debug("Load config");
    config.load("/config.ini");
    mqttServer = config.get("mqtt_server", "");
    mqttPassword = config.get("mqtt_password", "");
    mqttUser = config.get("mqtt_user", "");
    deviceName = config.get("device_name", "HallLights");
    WiFiPortalParameter mqttServerParam("mqtt_server", "MQTT IP address", mqttServer.c_str(), 40);
    WiFiPortalParameter mqttUserParam("mqtt_user", "MQTT user", mqttUser.c_str(), 40);
    WiFiPortalParameter mqttPasswordParam("mqtt_password", "MQTT password", mqttPassword.c_str(), 40);
    wifi.addParameter(mqttServerParam);
    wifi.addParameter(mqttUserParam);
    wifi.addParameter(mqttPasswordParam);
    if (FORCE_WIFI_PORTAL)
    {
        wifi.startPortal("HallLights");
    }
    else
    {
        wifi.connect("HallLights");
    }
    logger->always("connected %s", wifi.getIP());
    logger->always("mac %s", WiFi.macAddress().c_str());
    // logger->always("MQTT IP %s", mqttServer.c_str());
    //  config.set("mqtt_server", mqttServerParam.getValue());
    //  config.set("mqtt_user", mqttUserParam.getValue());
    //  config.set("mqtt_password", mqttPasswordParam.getValue());

    // if (config.isChanged())
    // {
    //     config.save("/config.ini");
    // }
    hallApp.setup(&config);
}

void loop()
{
    hallApp.loop();
}
