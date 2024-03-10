#include <Arduino.h>
#include "fclib/Logging.h"
#include "fclib/Net.h"

using namespace FCLIB;

FCLIB::Logger *logger = new FCLIB::Logger("main");

const int pin = 4;
const int pir = 5;
int state = -1;
WiFiSetup wifi;

void setup()
{
  logger->always("running");
  pinMode(pin, OUTPUT);
  pinMode(pir, INPUT);
  if (wifi.connect("HallLights"))
  {
    logger->always("connected %s", wifi.getIP());
  }
}

void loop()
{
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
