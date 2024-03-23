#ifndef _FCLIB_HOMEASSISTANT_H_
#define _FCLIB_HOMEASSISTANT_H_
#include <c_types.h>
#include "fclib/Logging.h"
#include "fclib/DoubleLinkedList.h"
#include "fclib/LinkedList.h"
#include "fclib/Hardware.h"
#include "fclib/Net.h"
using namespace FCLIB;

namespace FCLIB::HA
{
    class Device;
    // https://www.home-assistant.io/integrations/mqtt/
    enum ComponentType
    {
        BINARY_SENSOR,
        BUTTON,
        LIGHT,
        SWITCH
    };

    // https://developers.home-assistant.io/docs/core/entity/#generic-properties
    class Entity : public DoubleLinkedListNode<Entity>
    {
    public:
        Entity(ComponentType type);
        virtual ~Entity();

    protected:
        Logger log;
        ComponentType type;
    };

    // Button has limited HA use.  Binary_sensor and Switch are normally what is wanted.
    class Button : public Entity
    {
    public:
        Button();
        virtual ~Button();
    };

    // BinarySensor cannot be changed by HA.  Only sends state.  Switch is 2-way state change
    class BinarySensor : public Entity
    {
    public:
        BinarySensor(Device *device, HW::BinaryReadComponent *binary);
        virtual ~BinarySensor();

    protected:
        HW::BinaryReadComponent *hardware;
    };

    // Switch is a binary device that can send and receive state
    class Switch : public Entity
    {
    public:
        Switch();
        virtual ~Switch();
    };

    // a light that can be turned on/off by HA
    class Led : public Entity
    {
    public:
        Led(Device *device, HW::Led *led);
        virtual ~Led();

    protected:
        HW::BinaryWriteComponent *hardware;
    };

    class Device
    {
    public:
        Device(const char *name);
        virtual ~Device();

        void add(Entity *entity);
        void remove(Entity *entity);

    protected:
        Logger log;
        DoubleLinkedList<Entity> entities;
        String name;
    };

    class HomeAssistant
    {
    public:
        HomeAssistant(Mqtt *mqtt);
        virtual ~HomeAssistant();

        void addDevice(Device *device);

    private:
        Logger log;
        Mqtt *mqtt;
        EventListener listener;
        LinkedList<Device *> devices;
    };

}
#endif