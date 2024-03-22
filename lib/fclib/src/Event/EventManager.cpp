#include "fclib/Event.h"

using namespace FCLIB;

namespace FCLIB
{

    EventManager *singletonEventManager = NULL;

    void EventManager::addListener(EventListener *listener)
    {
        singletonEventManager->listeners.add(listener);
    }
    void EventManager::removeListener(EventListener *listener)
    {
        singletonEventManager->listeners.remove(listener);
    }

    void EventManager::addEvent(Event *event)
    {
        singletonEventManager->events.add(event);
    }
    void EventManager::removeEvent(Event *event)
    {
        singletonEventManager->events.remove(event);
    }

    EventManager::EventManager() : log("EventManager")
    {
        singletonEventManager = this;
    }
    EventManager::~EventManager()
    {
        singletonEventManager = NULL;
    }

    void EventManager::processEvents()
    {
        Event *event = events.getFirst();
        while (event != NULL)
        {
            log.debug("process event 0x%lx", event);
            EventListener *listener = listeners.getFirst();
            while (listener != NULL)
            {
                log.debug("check listener 0x%lx", listener);

                if (listener->match(event->getType(), event->getSender()))
                {
                    listener->handle(event);
                }
                listener = listener->getNext();
            }
            Event *next = event->getNext();
            log.debug("remove event %lx", event);
            events.remove(event);
            log.debug("delete event");
            delete event;
            log.debug("process next");
            event = next;
        }
    }
}