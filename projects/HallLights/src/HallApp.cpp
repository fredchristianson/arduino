
#include "./HallApp.h"
#include "fclib/Hardware.h"
#include "fclib/Timer.h"
#include "fclib/LedStrip.h"
#include "fclib/Render.h"

using namespace FCLIB;

HallApp::HallApp()
{
    log.setModuleName("HallApp");

    log.info("Created");
}

HallApp::~HallApp()
{
}

void HallApp::setupComplete()
{
    LoopTask::create([this]()
                     { this->doTask(); });

    Config *config = getConfig();
    ConfigSection *hardwareConfig = config->getSection("hardware");
    motion.setPin(hardwareConfig->get("motion_pin", 4));
    int ledCount = hardwareConfig->get("led_count", 7);
    int ledPin = hardwareConfig->get("led_pin", 5);
    NeoPixelStrip *strip = new NeoPixelStrip(ledPin,
                                             ledCount,
                                             hardwareConfig->get("led_brightness", 50));
    this->ledStrip = strip;

    listener.handle(EventType::CHANGE_EVENT, &motion, [this](Event *event)
                    { this->onMotionChange(event); });

    String deviceName = getConfig()->get("device_name", "FCLIB Device");
    ha = new HA::HomeAssistant(getNetwork()->getMqtt());
    haDevice = ha->createDevice(deviceName.c_str());
    haMotion = new HA::MotionSensor(haDevice, &motion, "Motion Sensor");
    haLight = new HA::LightStrip(haDevice, &renderer, "LedStrip");
    haLedCount = new HA::Number(haDevice, "LED Count", 0, 900, ledCount);
    haLedPin = new HA::Number(haDevice, "LED Strip Pin", 0, 16, ledPin);
    renderer.setStrip(this->ledStrip);

    ha->publishConfig();
    ConfigSection *s = getConfig()->getSection("renderer");
    renderer.setBrightness(s->get("brightness", 50));
    log.always("Config RGB %d,%d,%d", s->get("red", 255), s->get("green", 255), s->get("blue", 0));
    renderer.setRGB(ColorRGB(s->get("red", 255), s->get("green", 255), s->get("blue", 0)));

    Task::repeat([this]()
                 { this->log.showMemory(); })
        ->delaySeconds(15);
    listener.handle(EventType::CHANGE_EVENT, haLedCount, [this](Event *event)
                    { this->onLedCountChange(haLedCount->asInt()); });
    listener.handle(EventType::CHANGE_EVENT, haLedPin, [this](Event *event)
                    { this->onLedPinChange(haLedCount->asInt()); });

    listener.handleChange(config, [this, config, hardwareConfig](Event *event)
                          {
               this->log.always("save config");
               config->save(); });
    listener.handleChange(haLight, [this](Event *event)
                          { this->onLightStateChange(event); });
}

void HallApp::doTask()
{
}

void HallApp::onLightStateChange(Event *event)
{
    log.debug("LightStrip change");
    const ColorRGB &rgb = renderer.getRGB();
    ConfigSection *s = getConfig()->getSection("renderer");
    s->set("red", rgb.red());
    s->set("green", rgb.green());
    s->set("blue", rgb.blue());
    s->set("brightness", renderer.getBrightness());
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