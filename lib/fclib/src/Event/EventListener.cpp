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
        Logger log("EventListener");
        log.debug("create handler");
        EventHandler *ehandler = new EventHandler(type, handler);
        log.never("handler=%x", ehandler);
        handlers.add(ehandler);
        log.debug("handler added");
        return ehandler;
    }
    EventListener::EventListener()
    {
        EventManager::add(this);
    }
    EventListener::~EventListener()
    {
        handlers.forEach([](EventHandler *h)
                         { 
                            Logger log("~EventListener");
                            log.never("delete handler %x",h);
                            delete h; });
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

    bool EventListener::hasHandler(EventType type)
    {
        EventHandler *handler = handlers.first([type](EventHandler *h)
                                               { return h->match(type); });
        return handler != NULL;
    }

    void EventListener::removeSender(void *sender)
    {
        handlers.removeIf([sender](EventHandler *handler)
                          { return handler->match(EventType::ANY, sender); });
    }
}