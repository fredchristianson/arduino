#include "fclib/Event.h"
#include "fclib/Logging.h"

using namespace FCLIB;

namespace FCLIB
{
    void Event::trigger(EventType type, void *sender)
    {
        Logger log("Event::trigger");
        log.debug("create event");
        Event *event = new Event(type, sender);
        log.debug("queue avent");
        EventManager::addEvent(event);
        log.debug("event added");
    }
    void Event::trigger(EventType type, void *sender, bool boolState)
    {
        Logger log("Event::trigger");
        log.debug("create event");
        Event *event = new Event(type, sender);
        event->data.boolValue = boolState;
        log.debug("queue avent");
        EventManager::addEvent(event);
        log.debug("event added");
    }

    Event::Event(EventType type, void *sender)
    {
        this->type = type;
        this->sender = sender;
    }

    Event::~Event()
    {
    }

}