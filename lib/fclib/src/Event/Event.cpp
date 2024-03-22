#include "fclib/Event.h"
#include "fclib/Logging.h"

using namespace FCLIB;

namespace FCLIB
{
    void Event::trigger(EventSender *sender, EventType type, bool boolState)
    {
        Logger log("Event::trigger");
        log.debug("create event");
        Event *event = new Event(type, sender);
        event->boolData.setState(boolState);
        log.debug("queue avent");
        EventManager::addEvent(event);
        log.debug("event added");
    }

    Event::Event(EventType type, EventSender *sender)
    {
        this->type = type;
        this->sender = sender;
    }

    Event::~Event()
    {
    }

}