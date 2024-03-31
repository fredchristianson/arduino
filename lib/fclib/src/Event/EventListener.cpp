#include "fclib/Event.h"

using namespace FCLIB;

namespace FCLIB
{
    EventHandler *EventListener::handle(EventType type, void *sender, EventHandlerCallback handler)
    {
        Logger log("Event::trigger");
        log.debug("create event");
        EventHandler *ehandler = new EventHandler(type, sender, handler);
        handlers.add(ehandler);
        return ehandler;
    }

    EventHandler *EventListener::handle(EventType type, EventHandlerCallback handler)
    {
        Logger log("Event::trigger");
        log.debug("create event");
        EventHandler *ehandler = new EventHandler(type, NULL, handler);
        log.debug("queue avent");
        handlers.add(ehandler);
        log.debug("event added");
        return ehandler;
    }
    EventListener::EventListener()
    {
        EventManager::add(this);
    }
    EventListener::~EventListener()
    {
        EventManager::remove(this);
    }

    void EventListener::processEvent(Event *event)
    {
        EventType type = event->getType();
        const void *sender = event->getSender();
        handlers.forEach([event, type, sender](EventHandler *handler)
                         {
            if (handler->match(type,sender))
            {
                handler->handle(event);
            } });
    }

}