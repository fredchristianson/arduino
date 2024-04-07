
#include "./HallApp.h"
#include "fclib/Hardware.h"
#include "fclib/Timer.h"
#include "fclib/LedStrip.h"
#include "fclib/Render.h"
#include "fclib/Task.h"
#include "fclib/Net.h"

using namespace FCLIB;

HallApp::HallApp()
{
    log.setModuleName("HallApp");

    log.showMemory("HallApp Created");
}

HallApp::~HallApp()
{
}

void HallApp::setupComplete()
{
    Task::onLoop([this]()
                 { this->doTask(); });
    Task::repeat([this]()
                 { this->log.showMemory(); })
        ->delaySeconds(5);

    log.showMemory("create hardware");
    Config *config = getConfig();
    ConfigSection *hardwareConfig = config->getSection("hardware");
    motion.setPin(hardwareConfig->get("motion_pin", 4));
    int ledCount = hardwareConfig->get("led_count", 7);
    int ledPin = hardwareConfig->get("led_pin", 5);
    int motionPin = hardwareConfig->get("motion_pin", 4);
    NeoPixelStrip *strip = new NeoPixelStrip(ledPin,
                                             ledCount,
                                             hardwareConfig->get("led_brightness", 50));
    this->ledStrip = strip;

    log.showMemory("create HA entities");
    String deviceName = getConfig()->get("device_name", "FCLIB Device");
    ha = new HA::HomeAssistant(Network::getMqtt());
    haDevice = ha->createDevice(deviceName.c_str());
    haMotion = new HA::MotionSensor(&motion, "Motion Sensor");
    log.debug("create HA::LightStrip");
    haLight = new HA::LightStrip(&renderer, "LedStrip");
    log.debug("created HA::LightStrip %x", haLight);
    haLedCount = new HA::Number("LED Count", 0, 900, ledCount);
    haLedPin = new HA::Number("LED Strip Pin", 0, 16, ledPin);
    haMotionPin = new HA::Number("Motion Pin", 0, 16, motionPin);
    haDevice->add(haMotion);
    haDevice->add(haLight);
    haDevice->add(haLedCount);

    haDevice->add(haLedPin);
    haDevice->add(haMotionPin);
    renderer.setStrip(this->ledStrip);
    ha->publishConfig();

    log.showMemory("setup listeners");
    listener.handle(EventType::CHANGE_EVENT, &motion, [this](Event *event)
                    { this->onMotionChange(event); });
    listener.handle(EventType::CHANGE_EVENT, haLedCount, [this](Event *event)
                    { this->onLedCountChange(haLedCount->asInt()); });
    listener.handle(EventType::CHANGE_EVENT, haLedPin, [this](Event *event)
                    { this->onLedPinChange(haLedCount->asInt()); });
    listener.handle(EventType::CHANGE_EVENT, haMotionPin, [this](Event *event)
                    { this->onMotionPinChange(haMotionPin->asInt()); });

    listener.handleChange(config, [this, config, hardwareConfig](Event *event)
                          {
               this->log.debug("save config");
               config->save(); });
    listener.handleChange(haLight, [this](Event *event)
                          { this->onLightStateChange(event); });
    renderer.start();
}

void HallApp::doTask()
{
}

void HallApp::onLightStateChange(Event *event)
{
    log.debug("LightStrip change");
}
void HallApp::onMotionChange(Event *event)
{
    log.never("handle event 0x%lx %d", event, event->getType());

    if (event->getSender() == &(motion))
    {
        // led.toggle();
        log.debug("got motion event: %s", motion.isDetected() ? "motion" : "no motion");
    }
}

void HallApp::onLedCountChange(int count)
{
    ledStrip->setCount(count);
    Config *config = getConfig();
    ConfigSection *hardwareConfig = config->getSection("hardware");
    hardwareConfig->set("led_count", count);
}

void HallApp::onLedPinChange(int pin)
{
    Config *config = getConfig();
    ConfigSection *hardwareConfig = config->getSection("hardware");
    hardwareConfig->set("led_pin", pin);
    ledStrip->setPin(pin);
}

void HallApp::onMotionPinChange(int pin)
{
    Config *config = getConfig();
    ConfigSection *hardwareConfig = config->getSection("hardware");
    hardwareConfig->set("motion_pin", pin);
    motion.setPin(pin);
}