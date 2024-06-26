#ifndef _FCLIB_EVENT_H_
#define _FCLIB_EVENT_H_
#include <functional>
#include "fclib/Logging.h"
#include "fclib/List.h"

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

        TASK_CREATED = 300,
        TASK_DONE = 301,

        ANY = 0xFFFF
    };

    class IEventSource
    {
    public:
        virtual bool match(const void *sender) const
        {
            return sender == getTriggerSender();
        }
        virtual const void *getTriggerSender() const { return this; }
    };

    class EventHandler
    {

    private:
        EventHandler(EventType type, EventHandlerCallback handler);
        EventHandler(EventType type, IEventSource *sender, EventHandlerCallback handler);
        EventHandler(EventType type, void *sender, EventHandlerCallback handler);
        virtual ~EventHandler();

    protected:
        friend EventManager;
        friend EventListener;

        virtual bool match(EventType type, const void *sender = NULL);
        virtual bool match(const void *sender = NULL) { return match(EventType::ANY, sender); }
        virtual void handle(Event *);

        EventType type;
        void *sender;
        bool senderIsEventSource;
        EventHandlerCallback handler;
        Logger log;
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

        bool hasHandler(EventType type);
        void removeSender(void *sender);

    private:
        friend EventManager;
        void processEvent(Event *event);
        List<EventHandler> handlers;
    };

    union EventData_t
    {
        EventData_t() { memset(this, 0, sizeof(EventData_t)); }
        EventData_t(const EventData_t &other)
        {
            memcpy(this, &other, sizeof(EventData_t));
        }

        const EventData_t operator=(const EventData_t &other)
        {
            memcpy(this, &other, sizeof(EventData_t));
            return *this;
        }
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
        static void trigger(EventType type, void *sender, void *custom);
        static void trigger(EventType type, void *sender, const EventData_t &data);

        const EventType getType() const { return this->type; }
        const void *getSender() const { return this->sender; }
        const EventData_t &getData() { return data; }
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
        static void add(EventListener *listener);
        static void remove(EventListener *listener);
        static void add(Event *event);
        static void processEvents();
        static bool hasListener(EventType type);
        static EventManager *get();

    private:
        void addListener(EventListener *listener);
        void removeListener(EventListener *listener);
        bool hasListener(EventListener *listener);
        void addEvent(Event *event);
        EventManager();
        virtual ~EventManager();
        List<Event> events[2];   // one is current and one is a queue for next loop
        List<Event> *eventQueue; // points to one of the events[2] lists.  new events added here
        List<EventListener> listeners;
        void process();
        Logger log;
    };
}

#endif