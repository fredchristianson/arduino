
#include "./CupboardApp.h"
#include "fclib/Hardware.h"
#include "fclib/Timer.h"
#include "fclib/LedStrip.h"
#include "fclib/Render.h"
#include "fclib/Task.h"
#include "fclib/Net.h"

using namespace FCLIB;

WiFiServer rebootServer(1234);
Logger rebootLogger("RebootServer");
const char *motionOptions[] = {"Either", "Both", NULL};
void checkReboot()
{
    WiFiClient client = rebootServer.accept();
    if (client)
    {
        client.setTimeout(20);
        String c = client.readString();
        if (c.indexOf("reboot") >= 0)
        {
            rebootLogger.warn("Rebooting in 5 seconds");
            client.write("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length:15\n\nrebooting..........");
            client.flush();
            client.stop(100);
            for (int i = 0; i < 5; i++)
            {
                delay(1000);
                rebootLogger.warn("\t%d...", (5 - i));
            }
            delay(1000);
            THE_BOARD->reboot();
        }
        client.write("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length:10\n\ndone..........");
        client.flush();
        client.stop(100);
    }
}

CupboardApp::CupboardApp()
{
    log.setModuleName("CupboardApp");
    log.setLevel(DEBUG_LEVEL);
    device = NULL;
    haLight = NULL;
    haMotion = NULL;
    motion1Pin = NULL;
    motion2Pin = NULL;
    led1Pin = NULL;
    led2Pin = NULL;
    led1Count = NULL;
    led2Count = NULL;
    log.showMemory("CupboardApp Created");
}

CupboardApp::~CupboardApp()
{
}

void CupboardApp::setupComplete()
{
    rebootServer.begin();
    log.info("listening for reboot on %d", rebootServer.port());
    Network::getMqtt()->startLogger("log/cupboard", DEBUG_LEVEL);
    log.info("==================================");
    log.info("Cupboard App Starting");
    log.info("==================================");
    // Task::onLoop([this]()
    //              { this->doTask(); });
    Task::repeat(checkReboot)->delayMsecs(100);
    Task::repeat([this]()
                 {
                     // this->writeStats();
                 })
        ->delaySeconds(60);

    Config *config = getConfig();
    motion.addPin(config->get("motion1", "pin", -1));
    motion.addPin(config->get("motion2", "pin", -1));
    strip1.setPin(config->get("led1", "pin", -1));
    strip2.setPin(config->get("led2", "pin", -1));
    strip1.setCount(config->get("led1", "led_count", -1));
    strip2.setCount(config->get("led2", "led_count", -1));
    strips.setOrig(&strip1);
    strips.setCopy(&strip2);
    log.showMemory("setup listeners");

    ha = new HA::HomeAssistant(Network::getMqtt());
    device = ha->createDevice("Kitchen Cupboard Lights");
    haLight = new HA::LightStrip(&haRenderer, "Kitchen Cupboard Lights");
    haMotion = new HA::MotionSensor(&motion);
    motion1Pin = new HA::Number("Left Motion Pin", 0, 16, config->get("motion1", "pin", -1));
    motion2Pin = new HA::Number("Right Motion Pin", 0, 16, config->get("motion2", "pin", -1));
    led1Pin = new HA::Number("Left led Pin", 0, 16, config->get("led1", "pin", -1));
    led2Pin = new HA::Number("Right led Pin", 0, 16, config->get("led2", "pin", -1));
    led1Count = new HA::Number("Left led Count", 0, 900, config->get("led1", "led_count", 10));
    led2Count = new HA::Number("Right led Count", 0, 900, config->get("led2", "led_count", 15));

    int motionType = config->get("motion", "type", 0);
    if (motionType == 0)
    {
        motion.any();
    }
    else
    {
        motion.all();
    }
    motionOption = new HA::Select("Motion Type", motionOptions, motionType, [this](const char *val)
                                  { this->setMotionType(val); });
    device->add(haLight)
        .add(haMotion)
        .add(motion1Pin)
        .add(motion2Pin)
        .add(led1Pin)
        .add(led2Pin)
        .add(led1Count)
        .add(led2Count)
        .add(motionOption);
    ha->publishConfig();
    ha->subscribeToTopics();

    haRenderer.setStrip(&strips);

    // listener.handle(EventType::MQTT_RECONNECTED, [this](Event *event)
    //                 this->haLight->publishState(); });

    listener.handle(EventType::CHANGE_EVENT, &motion, [this](Event *event)
                    { this->onMotionChange(event); });

    listener.handleChange(config, [this, config](Event *event)
                          {
        this->log.debug("save config");
        config->save(); });

    listener.handleChange(motion1Pin, [this, config](Event *event)
                          {
        config->set("pin", motion1Pin->asInt(), "motion1");
        this->motion.removeAll();
        motion.addPin(config->get("motion1", "pin", -1));
        motion.addPin(config->get("motion2", "pin", -1)); });
    listener.handleChange(motion2Pin, [this, config](Event *event)
                          {
        config->set("pin", motion2Pin->asInt(), "motion2");
        this->motion.removeAll();
        motion.addPin(config->get("motion1", "pin", -1));
        motion.addPin(config->get("motion2", "pin", -1)); });
    listener.handleChange(led1Pin, [this, config](Event *event)
                          {
        config->set("pin", led1Pin->asInt(), "led1");
        strip1.setPin(led1Pin->asInt()); });
    listener.handleChange(led2Pin, [this, config](Event *event)
                          {
        config->set("pin", led2Pin->asInt(), "led2");
        strip2.setPin(led2Pin->asInt()); });
    listener.handleChange(led1Count, [this, config](Event *event)
                          {
        config->set("led_count", led1Count->asInt(), "led1");
        strip1.setCount(led1Count->asInt()); });
    listener.handleChange(led2Count, [this, config](Event *event)
                          {
        config->set("led_count", led2Count->asInt(), "led2");
        strip2.setCount(led2Count->asInt()); });
    haRenderer.turnOff(0);
    haRenderer.start();
}

void CupboardApp::doTask()
{
}

void CupboardApp::onMotionChange(Event *event)
{
    // log.never("handle event 0x%lx %d", event, event->getType());
    log.never("Motion %s detected.", motion.isDetected() ? " is " : " is not ");
}

void CupboardApp::setMotionType(const char *option)
{
    Config *config = getConfig();

    if (strcasecmp(option, "both") == 0)
    {
        config->set("motionType", 1);
        motion.all();
    }
    else
    {
        config->set("motionType", 0);

        motion.any();
    }
    log.always("setMotionType %s", option);
}