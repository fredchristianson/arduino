#include "fclib/Event.h"

using namespace FCLIB;

namespace FCLIB
{
    EventHandler *EventListener::handle(EventType type, void *sender, EventHandlerCallback handler)
    {
        EventManager::addListener(this);

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
    }
    EventListener::~EventListener()
    {
        EventManager::removeListener(this);
    }

    void EventListener::processEvent(Event *event)
    {
        for (int i = 0; i < handlers.size(); i++)
        {
            EventHandler *h = handlers[i];
            if (h->match(event->getType(), event->getSender()))
            {
                h->handle(event);
            }
        }
    }

}