#include <stdlib.h>
#include "fclib/App.h"
#include "fclib/System.h"
#include <Esp.h>
#include <WiFiManager.h>

namespace FCLIB
{

    bool savePortalParameters = false;
    void saveWifiConfig(WiFiSetup *setup)
    {
        Logger log("AppNetwork");
        log.debug("params changed");
        savePortalParameters = true; // params changed so need to save;
    }

    AppNetwork::AppNetwork() : AppComponent(), log("AppNetwork")
    {

        this->useWifi = true;
        this->useNtp = true;
        this->useMqtt = true;
        this->ntpOffsetMinutes = 0;
    }

    AppNetwork::~AppNetwork()
    {
    }

    bool AppNetwork::setup(Config *config)
    {
        log.debug("network setup");
        deviceName = config->get("device_name", "FCLIB_DEVICE");
        ConfigSection *wifi = config->getSection("wifi");
        useWifi = (wifi != NULL);
        if (useWifi)
        {
            useWifi = wifi->get("connect", true);
            resetWifi = wifi->get("reset", false);
            log.debug("use wifi %d.  reset %d", useWifi, resetWifi);
        }
        ConfigSection *ntp = config->getSection("ntp");
        if (ntp != NULL)
        {
            this->useNtp = ntp->get("connect", true);
            this->ntpOffsetMinutes = ntp->get("tz_offset_minutes", 0);
            log.debug("use ntp %d", useNtp);
        }
        ConfigSection *mqtt = config->getSection("mqtt");
        this->useMqtt = (mqtt != NULL);
        if (this->useMqtt)
        {
            this->mqttPassword = mqtt->get("mqtt_password", "");
            this->mqttUser = mqtt->get("mqtt_user", "");
            this->mqttServer = mqtt->get("mqtt_server", "");
            log.debug("use mqtt %s %s %s", mqttServer.c_str(), mqttUser.c_str(), mqttPassword.c_str());
        }
        return true;
    }

    bool AppNetwork::connect()
    {
        log.info("Connect Network");

        if (!this->useWifi)
        {
            return false;
        }
        else
        {
            while (!connectWifi())
            {
                log.error("wifi failed.  Waiting 10 seconds to retry");
                delay(10000);
                log.info("retrying wifi connection");
            }
            // getConfig()->set("reset", false, "wifi");
        }

        if (this->useMqtt)
        {
            this->connectMqtt();
        }
        if (this->useNtp)
        {
            this->connectNtp();
        }
        return true;
    }

    bool AppNetwork::connectWifi()
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
            log.always("Update mqtt params: %s %s %s", mqttUserParam.getValue(), mqttUserParam.getValue(), mqttServerParam.getValue());

            Config *config = getConfig();
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

    bool AppNetwork::connectNtp()
    {
        int tz_offset = this->ntpOffsetMinutes;
        log.debug("tz_offset: %d", tz_offset);
        TimeClient::run(tz_offset);
        return true;
    }

    bool AppNetwork::connectMqtt()
    {
        return true;
    }
}