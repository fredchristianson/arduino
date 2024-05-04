#ifndef _FCLIB_HOMEASSISTANT_H_
#define _FCLIB_HOMEASSISTANT_H_
#include <ArduinoJson.h>
#include <c_types.h>
#include "fclib/Logging.h"
#include "fclib/Hardware.h"
#include "fclib/Net.h"
#include "fclib/Event.h"
#include "fclib/Render.h"
#include "fclib/List.h"
#include "Throttle.h"
using namespace FCLIB;

namespace FCLIB::HA
{
    class HomeAssistant;
    class Device;

    // https://www.home-assistant.io/integrations/mqtt/
    enum class ComponentType
    {
        BUTTON,
        BINARY_SENSOR,
        LIGHT,
        SWITCH,
        MOTION_SENSOR,
        NUMBER,
        SELECT

    };

    // https://developers.home-assistant.io/docs/core/entity/#generic-properties
    class Entity
    {
    public:
        Entity(const char *name, ComponentType type);
        virtual ~Entity();

        virtual const char *getComponentName() { return componentName.c_str(); }
        virtual const char *getName() { return name.c_str(); }
        virtual const char *getDeviceClass() { return deviceClass.c_str(); }
        virtual const char *getUniqueName() { return uniqueName.c_str(); }

        virtual void setupStateTopic(JsonDocument &doc);
        virtual void setupCommandTopics(JsonDocument &doc);
        virtual void setupCapabilities(JsonDocument &doc);
        virtual void subscribe(Mqtt *mqtt) {}

        virtual void publishState();

    protected:
        String baseTopic();
        virtual void setBoolState(bool isOn);
        virtual void setStateOn();
        virtual void setStateOff();
        friend HomeAssistant;
        friend Device;
        Logger log;
        ComponentType type;
        Device *device;
        int componentTypeIndex; // if multiple entities have same type 0,1,2,...;
        String name;
        String componentName;
        String deviceClass;
        String uniqueName;
        String id;
        bool defaultEntity;
        EventListener events;
        bool lastBoolState;

        HomeAssistant *ha();
    };

    // an entity with a topic to receive commands
    class CommandEntity : public Entity
    {
    public:
        CommandEntity(const char *name, ComponentType type);
        ~CommandEntity();
        virtual void setupCommandTopics(JsonDocument &doc);
        virtual void onCommand(const char *payload);
        virtual void subscribe(Mqtt *mqtt);
    };
    // Button has limited HA use.  Binary_sensor and Switch are normally what is wanted.
    class Button : public Entity
    {
    public:
        Button(const char *name = "Button");
        virtual ~Button();
        const char *getComponentName() override { return "button"; }
    };

    // BinarySensor cannot be changed by HA.  Only sends state.  Switch is 2-way state change
    class BinarySensor : public Entity
    {
    public:
        BinarySensor(HW::IBinarySensor *binary, const char *name = "Binary Sensor");
        virtual ~BinarySensor();
        const char *getComponentName() override { return "binary_sensor"; }

    protected:
        HW::IBinarySensor *hardware;
    };

    class MotionSensor : public BinarySensor
    {
    public:
        MotionSensor(HW::IMotion *binary, const char *name = "Motion Sensor");
        virtual ~MotionSensor();

    protected:
        void onMotionStart();
        void onMotionStop();
        HW::IMotion *motionSensor;
        Timer pause;
    };

    // Switch is a binary device that can send and receive state
    class Switch : public Entity
    {
    public:
        Switch(HW::IBinarySensor *binary, const char *name = "Switch");
        virtual ~Switch();
        const char *getComponentName() override { return "switch"; }

    protected:
        void onHardwareChange();
        HW::IBinarySensor *hardware;
    };

    // a light that can be turned on/off by HA
    class Led : public Entity
    {
    public:
        Led(HW::OutputPinComponent *led, const char *name = "LED");
        virtual ~Led();
        const char *getComponentName() override { return "light"; }
        virtual void updateState(const char *payload);

    protected:
        HW::OutputPinComponent *hardware;
    };

    // a light that can be turned on/off by HA
    class LightStrip : public CommandEntity
    {
    public:
        LightStrip(HomeAssistantSceneRenderer *renderer, const char *name = "LED Strip");
        virtual ~LightStrip();
        const char *getComponentName() override { return "light"; }
        virtual void setupCommandTopics(JsonDocument &doc);
        virtual void setupCapabilities(JsonDocument &doc);
        virtual void subscribe(Mqtt *mqtt);
        virtual void publishState() override;
        virtual void publishTransitionState();

    protected:
        void onCommand(const char *payload);
        HomeAssistantSceneRenderer *renderer;
        Throttle publishThrottle;
    };

    class Number : public CommandEntity
    {
    public:
        Number(const char *name, float min, float max, float val);
        virtual ~Number();
        virtual void publishState() override;
        virtual void setupCapabilities(JsonDocument &doc);

        float asFloat() { return value; }
        int asInt() { return (int)(value); }

    protected:
        void onCommand(const char *payload);
        float minValue;
        float maxValue;
        float value;
    };

    class Select : public CommandEntity
    {
    public:
        Select(const char *name, const char *options[], int selectedIndex, Callback<const char *> onChange);
        virtual ~Select();
        void select(int index);
        void add(const char *option);
        virtual void publishState() override;
        virtual void setupCapabilities(JsonDocument &doc);

    protected:
        Callback<const char *> changeHandler;
        int selectedIndex;
        void onCommand(const char *payload);
        List<String> options;
    };

    class Device
    {
    public:
        virtual const char *getName() { return name.c_str(); }
        friend Entity;
        friend HomeAssistant;
        Device &add(Entity *entity);
        void remove(Entity *entity);

    private:
        Device(const char *name);
        virtual ~Device();
        friend HomeAssistant;
        friend Entity;
        void getJson(JsonObject &json);
        Logger log;
        List<Entity> entities;
        String name;
        String friendlyName;
        String id;
        String boardId;
        HomeAssistant *ha;
    };

    class HomeAssistant
    {
    public:
        static String nameToId(const char *name);
        static String nameToId(String name);
        static String toUnfriendlyName(const char *name);

        HomeAssistant(Mqtt *mqtt);
        virtual ~HomeAssistant();

        void addDevice(Device *device);
        void publishConfig();
        void subscribeToTopics();
        void publishState(const char *topic, JsonDocument &payload);
        void publishState(const char *topic, const char *payload);
        void publishState(const String &topic, JsonDocument &payload) { publishState(topic.c_str(), payload); }
        void publishState(const String &topic, const char *payload) { publishState(topic.c_str(), payload); }
        Device *createDevice(const char *name);

    private:
        void publishDeviceConfig(Device *device);
        void publishEntityConfig(Entity *entity);
        Logger log;
        Mqtt *mqtt;
        EventListener listener;
        List<Device> devices;
    };

}
#endif