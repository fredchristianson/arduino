#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "fclib/Net.h"
#include "fclib/Logging.h"

using namespace FCLIB;

WiFiManager wifiManager;
String ipAddress;

FCLIB::WiFiSetup::WiFiSetup() : log("WiFiSetup", DEBUG_LEVEL)
{
}

FCLIB::WiFiSetup::~WiFiSetup()
{
}

bool FCLIB::WiFiSetup::connect(const char *name)
{
    log.debug("Connect");

    wifiManager.autoConnect(name);
    return WiFi.isConnected();
}

bool FCLIB::WiFiSetup::startPortal(const char *name)
{
    log.debug("Start Portal");
    wifiManager.startConfigPortal(name);
    return WiFi.isConnected();
}

const char *FCLIB::WiFiSetup::getIP()
{
    ipAddress = WiFi.localIP().toString();
    return ipAddress.c_str();
}