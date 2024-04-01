#include <stdlib.h>
#include "fclib/App.h"
#include "fclib/System.h"
#include <Esp.h>
#include <WiFiManager.h>

namespace FCLIB
{

    Mqtt *Network::singletonMqtt = NULL;
    TimeClient *Network::singletonTimeClient = NULL;
    Network *singletonNetwork = NULL;
    Mqtt *Network::getMqtt() { return singletonMqtt; }

    bool savePortalParameters = false;
    void saveWifiConfig(WiFiSetup *setup)
    {
        Logger log("Network");
        log.debug("params changed");
        savePortalParameters = true; // params changed so need to save;
    }

    Network::Network() : log("Network")
    {
        this->ntpOffsetMinutes = 0;
        resetWifi = false;
    }

    Network::~Network()
    {
    }

    bool Network::setup(Config *config)
    {
        log.debug("network setup");
        deviceName = config->get("device_name", "FCLIB_DEVICE");
        log.always("deviceName: %s", deviceName.c_str());
        ConfigSection *wifi = config->getSection("wifi");

        resetWifi = wifi->get("reset", false);
        ConfigSection *ntp = config->getSection("ntp");
        if (ntp != NULL)
        {
            this->ntpOffsetMinutes = ntp->get("tz_offset_minutes", 0);
        }
        ConfigSection *mqtt = config->getSection("mqtt");
        if (mqtt)
        {
            this->mqttPassword = mqtt->get("mqtt_password", "");
            this->mqttUser = mqtt->get("mqtt_user", "");
            this->mqttServer = mqtt->get("mqtt_server", "");
            this->mqttDeviceName = mqtt->get("mqtt_device_name", deviceName.c_str());
            log.debug("use mqtt %s %s %s %s", mqttServer.c_str(), mqttDeviceName.c_str(), mqttUser.c_str(), mqttPassword.c_str());
        }
        return true;
    }

    bool Network::connect()
    {
        log.info("Connect Network");

        while (!connectWifi())
        {
            log.error("wifi failed.  Waiting 10 seconds to retry");
            delay(10000);
            log.info("retrying wifi connection");
        }
        // getConfig()->set("reset", false, "wifi");

        bool success = true;

        success &= this->connectTimeClient();

        success &= this->connectMqtt();
        return true;
    }

    bool Network::connectWifi()
    {
        savePortalParameters = false;
        log.info("Connecting wifi");
        WiFiSetup wifi;
        WiFiPortalParameter mqttServerParam("mqttserver", "MQTT IP address", mqttServer.c_str(), 40);
        WiFiPortalParameter mqttUserParam("mqttuser", "MQTT user", mqttUser.c_str(), 40);
        WiFiPortalParameter mqttPasswordParam("mqttpassword", "MQTT password", mqttPassword.c_str(), 40);
        wifi.addParameter(mqttServerParam);
        wifi.addParameter(mqttUserParam);
        wifi.addParameter(mqttPasswordParam);
        wifi.setSaveConfigCallback(saveWifiConfig);
        bool connected = false;
        if (this->resetWifi)
        {
            log.info("Reseting wifi");

            connected = wifi.startPortal(deviceName.c_str());
            log.info("portal done");
        }
        else
        {
            log.info("start wifi connect");
            connected = wifi.connect(deviceName.c_str());
            log.info("wifi connect done %d", connected);
        }
        if (savePortalParameters)
        {
            log.debug("Update mqtt params: %s %s %s", mqttUserParam.getValue(), mqttUserParam.getValue(), mqttServerParam.getValue());

            Config *config = App::THE_APP->getConfig();
            config->set("mqtt_user", mqttUserParam.getValue(), "mqtt");
            config->set("mqtt_password", mqttPasswordParam.getValue(), "mqtt");
            config->set("mqtt_server", mqttServerParam.getValue(), "mqtt");
            config->set("reset", false, "wifi");
        }
        else
        {
            log.debug("savePortalParameters %d", savePortalParameters);
        }
        return connected;
    }

    bool Network::connectTimeClient()
    {
        int tz_offset = this->ntpOffsetMinutes;
        log.debug("tz_offset: %d", tz_offset);
        singletonTimeClient = TimeClient::run(tz_offset);
        return true;
    }

    bool Network::connectMqtt()
    {
        if (singletonMqtt != NULL)
        {
            delete singletonMqtt;
        }
        singletonMqtt = new Mqtt();
        return singletonMqtt->configure(mqttServer.c_str(), mqttDeviceName.c_str(), mqttUser.c_str(), mqttPassword.c_str());
    }
}