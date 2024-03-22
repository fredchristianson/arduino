#ifndef _FCLIB_EVENT_H_
#define _FCLIB_EVENT_H_

#include "fclib/DoubleLinkedList.h"
#include "fclib/Logging.h"

namespace FCLIB
{
    class Event;
    class Config;
    class EventManager;

    enum EventType
    {
        CHANGE_EVENT = 100,
        ON_EVENT = 101,
        OFF_EVENT = 102,
        PRESS_EVENT = 103,
        LONGPRESS_EVENT = 104
    };

    class EventData
    {
    };

    class EventBooleanData : EventData
    {
    public:
        EventBooleanData() { this->state = false; }
        bool isTrue() { return state; }
        bool isFalse() { return !state; }

        void setState(bool state) { this->state = state; }

    private:
        bool state;
    };

    class EventSender
    {
    public:
        EventSender() {}
    };

    class EventListener : public DoubleLinkedListNode<EventListener>
    {
    public:
        EventListener();
        virtual ~EventListener();

        void start();
        void stop();

    protected:
        friend EventManager;
        void processEvent(Event *event);

        virtual bool match(EventType type, EventSender *sender) { return true; }
        virtual void handle(Event *) = 0;
    };

    class Event : public DoubleLinkedListNode<Event>
    {
    public:
        static void trigger(EventSender *sender, EventType type, bool boolState);

        EventType getType() { return this->type; }
        EventSender *getSender() { return this->sender; }

    protected:
        friend EventManager;
        Event(EventType type, EventSender *sender);
        virtual ~Event();

        EventType type;
        EventSender *sender;
        union
        {
            EventBooleanData boolData;
            void *customData;
        };
    };

    class EventManager
    {
    public:
        static void addListener(EventListener *listener);
        static void removeListener(EventListener *listener);
        static void addEvent(Event *event);
        static void removeEvent(Event *event);
        EventManager();
        virtual ~EventManager();

        void processEvents();

    protected:
        DoubleLinkedList<Event> events;
        DoubleLinkedList<EventListener> listeners;
        Logger log;
    };
}

#endif