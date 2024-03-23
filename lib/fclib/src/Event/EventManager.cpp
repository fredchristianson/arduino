#include "fclib/Event.h"

using namespace FCLIB;

namespace FCLIB
{

    EventManager *singletonEventManager;

    EventManager *EventManager::get()
    {
        if (singletonEventManager == NULL)
        {
            singletonEventManager = new EventManager();
        }
        return singletonEventManager;
    }
    void EventManager::addListener(EventListener *listener)
    {
        EventManager::get()->listeners.add(listener);
    }
    void EventManager::removeListener(EventListener *listener)
    {
        EventManager *em = get();
        for (int i = 0; i < em->listeners.size(); i++)
        {
            EventListener *l = em->listeners[i];
            if (l == listener)
            {
                em->listeners.remove(i);
                return;
            }
        }
    }

    void EventManager::addEvent(Event *event)
    {
        get()->events.add(event);
    }
    void EventManager::removeEvent(Event *event)
    {
        EventManager *em = get();
        for (int i = 0; i < em->events.size(); i++)
        {
            Event *e = em->events[i];
            if (e == event)
            {
                em->events.remove(i);
                return;
            }
        }
    }

    EventManager::EventManager() : log("EventManager")
    {
    }
    EventManager::~EventManager()
    {
    }

    void EventManager::processEvents()
    {
        EventManager *em = get();

        if (em->events.size() == 0)
        {
            return;
        }
        LinkedList<Event *> todo(em->events.size(), NULL);
        Event *event;
        while ((event = todo.pop()) != NULL)
        {
            em->log.debug("process event 0x%lx", event);
            for (int i = 0; i < em->listeners.size(); i++)
            {
                EventListener *listener = em->listeners[i];
                listener->processEvent(event);
                delete event;
            }
        }
    }
}