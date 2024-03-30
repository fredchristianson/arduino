#ifndef _FCLIB_EVENT_H_
#define _FCLIB_EVENT_H_
#include <functional>
#include "fclib/LinkedList.h"
#include "fclib/Logging.h"

namespace FCLIB
{
    class Event;
    class Config;
    class EventManager;
    class EventListener;

    using EventHandlerCallback = std::function<void(Event *)>;
    enum class EventType
    {
        CHANGE_EVENT = 100,
        ON_EVENT = 101,
        OFF_EVENT = 102,
        PRESS_EVENT = 103,
        LONGPRESS_EVENT = 104,
        MOTION_START_EVENT = 105,
        MOTION_STOP_EVENT = 106,

        CONNECTED = 200,
        DISCONNECTED = 201,

        ANY = 0xFFFF
    };

    class EventHandler
    {

    private:
        EventHandler(EventType type, void *sender, EventHandlerCallback handler);
        virtual ~EventHandler();

    protected:
        friend EventManager;
        friend EventListener;

        virtual bool match(EventType type, void *sender);
        virtual void handle(Event *);

        EventType type;
        void *sender;
        EventHandlerCallback handler;
    };

    class EventListener
    {
    public:
        EventListener();
        ~EventListener();

        EventHandler *handle(EventType type, void *sender, EventHandlerCallback callback);
        EventHandler *handle(EventType type, EventHandlerCallback callback);
        EventHandler *handleChange(EventHandlerCallback callback) { return handle(EventType::CHANGE_EVENT, callback); }
        EventHandler *handleChange(void *sender, EventHandlerCallback callback) { return handle(EventType::CHANGE_EVENT, sender, callback); }

    private:
        friend EventManager;
        void processEvent(Event *event);
        LinkedList<EventHandler *> handlers;
    };

    union EventData_t
    {
        EventData_t() { memset(this, 0, sizeof(EventData_t)); }
        bool boolValue;
        int intValue;
        float floatValue;
        void *custom;
    };

    class Event
    {
    public:
        static int nextId;
        static void trigger(EventType type, void *sender);
        static void trigger(EventType type, void *sender, bool boolState);
        static void trigger(EventType type, void *sender, int intVal);
        static void trigger(EventType type, void *sender, float floatVal);

        EventType getType() { return this->type; }
        void *getSender() { return this->sender; }
        int getId();
        void setAllowMultiple(bool allow = true) { this->mayHaveMultiple = allow; }
        bool isMultipleAllowed() { return this->mayHaveMultiple; }

    protected:
        friend EventManager;
        Event(EventType type, void *sender);
        virtual ~Event();

        EventType type;
        void *sender;
        EventData_t data;
        long id;
        bool mayHaveMultiple; // if true, mutiple messages can be queued from same sender/typ
    };

    class EventManager
    {
    public:
        static void addListener(EventListener *listener);
        static void removeListener(EventListener *listener);
        static void addEvent(Event *event);
        static void processEvents();

        static EventManager *get();

    private:
        bool hasListener(EventListener *listener);
        void add(Event *event);
        EventManager();
        virtual ~EventManager();
        LinkedList<Event *> events[2];
        int currentEventList;
        LinkedList<EventListener *> listeners;
        Logger log;
    };
}

#endif