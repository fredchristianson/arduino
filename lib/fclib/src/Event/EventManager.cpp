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
        if (!get()->hasListener(listener))
        {
            get()->listeners.add(listener);
        }
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
        get()->add(event);
    }

    EventManager::EventManager() : log("EventManager")
    {
        currentEventList = 0;
    }
    EventManager::~EventManager()
    {
    }

    bool EventManager::hasListener(EventListener *listener)
    {
        for (int i = 0; i < listeners.size(); i++)
        {
            if (listener == listeners[i])
            {
                return true;
            }
        }
        return false;
    }

    void EventManager::add(Event *event)
    {
        log.never("add event %d %lx", currentEventList, event);

        // currentEventList is the one events can be added to.
        // processEvents() may be using the other one
        LinkedList<Event *> &cur = events[currentEventList];
        if (!event->mayHaveMultiple)
        {
            // remove existing messages from sender/type unless multiple allowed
            for (int i = 0; i < cur.size(); i++)
            {
                if (cur[i]->sender == event->sender && cur[i]->type == event->type)
                {
                    cur.remove(i);
                    i--;
                }
            }
        }
        cur.add(event);
    }

    void EventManager::processEvents()
    {
        EventManager *em = get();
        LinkedList<Event *> *events = &(em->events[em->currentEventList]);
        em->currentEventList = (em->currentEventList == 0) ? 1 : 0;
        if (events->size() == 0)
        {
            return;
        }
        Event *event;
        while ((event = events->pop()) != NULL)
        {
            em->log.debug("process event 0x%lx", event);
            for (int i = 0; i < em->listeners.size(); i++)
            {
                EventListener *listener = em->listeners[i];
                em->log.debug("\tcall listener 0x%lx", listener);
                listener->processEvent(event);
                em->log.debug("\tdelete event");
            }
            delete event;
        }
    }
}