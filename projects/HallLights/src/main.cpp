#include <Arduino.h>
#include "fclib/Logging.h"
#include "fclib/Net.h"
#include "fclib/File.h"
#include "fclib/Config.h"
#include "test/Test.h"
#include "fclib/System.h"

using namespace FCLIB;

FCLIB::Logger *logger = new FCLIB::Logger("main");

const int pin = 4;
const int pir = 5;
int state = -1;
WiFiSetup wifi;
WiFiPortalParameter *mqttParameter;
ConfigFile config;
String mqttServer;
bool testsPass = true;
Board *board = Board::get();

bool FORCE_WIFI_PORTAL = false;

void setup()
{
  setDefaultLoggerLevel(DEBUG_LEVEL);
  setModuleLoggerLevel("ConfigFile", WARN_LEVEL);
  setModuleLoggerLevel("FileReader", WARN_LEVEL);

  logger->always("running");
  logger->always("mac %s", board->getDeviceId());

  testsPass = HALLLIGHTS_TEST::runTests();
  if (!testsPass)
  {
    logger->error("tests failed");
    return;
  }
  pinMode(pin, OUTPUT);
  pinMode(pir, INPUT);

  logger->debug("Load config");
  config.load("/config.ini");
  mqttServer = config.get("mqtt_server", "<need-mqtt-server>");
  mqttParameter = new WiFiPortalParameter("mqtt_erver", "MQTT IP address", mqttServer.c_str(), 40);
  wifi.addParameter(*mqttParameter);
  if (FORCE_WIFI_PORTAL)
  {
    wifi.startPortal("HallLights");
    logger->always("MQTT IP %s", mqttParameter->getValue());
    mqttServer = mqttParameter->getValue();
    config.set("mqtt_server", mqttServer.c_str());
    config.save("/config.ini");
  }
  else
  {
    wifi.connect("HallLights");
  }
  logger->always("connected %s", wifi.getIP());
  logger->always("mac %s", WiFi.macAddress().c_str());
  logger->always("MQTT IP %s", mqttServer.c_str());
}

void loop()
{
  if (!testsPass)
  {
    return;
  }
  int pirState = digitalRead(pir);
  if (pirState == HIGH && state != 0)
  {
    digitalWrite(pin, HIGH);
    state = 0;
    logger->always("On");
  }
  else if (pirState == LOW && state != 1)
  {
    digitalWrite(pin, LOW);
    state = 1;
    logger->always("Off");
  }
}
