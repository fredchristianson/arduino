#include "fclib/Event.h"
#include "fclib/Logging.h"

using namespace FCLIB;

namespace FCLIB
{
    int Event::nextId = 1;

    void Event::trigger(EventType type, void *sender)
    {
        Logger log("Event::trigger");
        log.debug("create event");
        Event *event = new Event(type, sender);
        log.debug("queue avent");
        EventManager::add(event);
        log.debug("event added");
    }
    void Event::trigger(EventType type, void *sender, bool boolState)
    {
        Logger log("Event::trigger");
        log.debug("create event");
        Event *event = new Event(type, sender);
        event->data.boolValue = boolState;
        log.debug("queue avent");
        EventManager::add(event);
        log.debug("event added");
    }
    void Event::trigger(EventType type, void *sender, int intVal)
    {
        Logger log("Event::trigger");
        log.debug("create event");
        Event *event = new Event(type, sender);
        event->data.intValue = intVal;
        log.debug("queue avent");
        EventManager::add(event);
        log.debug("event added");
    }
    void Event::trigger(EventType type, void *sender, float floatVal)
    {
        Logger log("Event::trigger");
        log.debug("create event");
        Event *event = new Event(type, sender);
        event->data.floatValue = floatVal;
        log.debug("queue avent");
        EventManager::add(event);
        log.debug("event added");
    }

    Event::Event(EventType type, void *sender)
    {
        this->type = type;
        this->sender = sender;
        this->id = nextId++;
        mayHaveMultiple = false;
    }

    Event::~Event()
    {
    }

    int Event::getId() { return id; }
}