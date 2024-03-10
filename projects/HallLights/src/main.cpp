#include <Arduino.h>
#include "fclib/Logging.h"

using namespace FCLIB;

FCLIB::Logger *logger = new FCLIB::Logger("main");

const int pin = 4;
const int pir = 5;
int state = -1;

void setup()
{
  logger->always("running");
  pinMode(pin, OUTPUT);
  pinMode(pir, INPUT);
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
