#ifndef _FCLIB_HOMEASSISTANT_H_
#define _FCLIB_HOMEASSISTANT_H_
#include <ArduinoJson.h>
#include <c_types.h>
#include "fclib/Logging.h"
#include "fclib/LinkedList.h"
#include "fclib/Hardware.h"
#include "fclib/Net.h"
#include "fclib/Event.h"
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
        SWITCH
    };

    // https://developers.home-assistant.io/docs/core/entity/#generic-properties
    class Entity
    {
    public:
        Entity(const char *name, Device *device, ComponentType type);
        virtual ~Entity();

        virtual const char *getComponentName() { return componentName.c_str(); }
        virtual const char *getName() { return name.c_str(); }
        virtual const char *getDeviceClass() { return deviceClass.c_str(); }
        virtual const char *getUniqueName() { return uniqueName.c_str(); }

    protected:
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
        String baseTopic;
        String id;
        bool defaultEntity;
        EventListener events;

        HomeAssistant *ha();
    };

    // Button has limited HA use.  Binary_sensor and Switch are normally what is wanted.
    class Button : public Entity
    {
    public:
        Button(Device *device, const char *name = "Button");
        virtual ~Button();
        const char *getComponentName() override { return "button"; }
    };

    // BinarySensor cannot be changed by HA.  Only sends state.  Switch is 2-way state change
    class BinarySensor : public Entity
    {
    public:
        BinarySensor(Device *device, HW::InputPinComponent *binary, const char *name = "Binary Sensor");
        virtual ~BinarySensor();
        const char *getComponentName() override { return "binary_sensor"; }

    protected:
        HW::InputPinComponent *hardware;
    };

    // Switch is a binary device that can send and receive state
    class Switch : public Entity
    {
    public:
        Switch(Device *device, HW::InputPinComponent *binary, const char *name = "Switch");
        virtual ~Switch();
        const char *getComponentName() override { return "switch"; }

    protected:
        void onHardwareChange();
        HW::InputPinComponent *hardware;
    };

    // a light that can be turned on/off by HA
    class Led : public Entity
    {
    public:
        Led(Device *device, HW::OutputPinComponent *led, const char *name = "LED");
        virtual ~Led();
        const char *getComponentName() override { return "light"; }

    protected:
        HW::OutputPinComponent *hardware;
    };

    class Device
    {
    public:
        virtual const char *getName() { return name.c_str(); }
        friend Entity;
        friend HomeAssistant;
        void add(Entity *entity);
        void remove(Entity *entity);

    private:
        Device(const char *name);
        virtual ~Device();
        friend HomeAssistant;
        friend Entity;
        void getJson(JsonObject &json);
        Logger log;
        LinkedList<Entity *> entities;
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
        void publishState(Entity *entity, JsonDocument &payload);
        void publishState(Entity *entity, const char *payload);
        Device *createDevice(const char *name);

    private:
        void publishDeviceConfig(Device *device);
        void publishEntityConfig(Entity *entity);
        Logger log;
        Mqtt *mqtt;
        EventListener listener;
        LinkedList<Device *> devices;
    };

}
#endif