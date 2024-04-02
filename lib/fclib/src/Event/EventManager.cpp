#include "fclib/Event.h"

using namespace FCLIB;

namespace FCLIB
{
    /* EventManager Static */
    /***********************/

    EventManager *singletonEventManager;

    EventManager *EventManager::get()
    {
        if (singletonEventManager == NULL)
        {
            singletonEventManager = new EventManager();
        }
        return singletonEventManager;
    }
    void EventManager::add(EventListener *listener)
    {
        get()->addListener(listener);
    }
    void EventManager::remove(EventListener *listener)
    {
        get()->removeListener(listener);
    }

    void EventManager::add(Event *event)
    {
        get()->addEvent(event);
    }

    void EventManager::processEvents()
    {
        get()->process();
    }
    /**************************/
    /* EventManager end Static*/

    EventManager::EventManager() : log("EventManager")
    {
        eventQueue = &events[0];
    }

    EventManager::~EventManager()
    {
    }
    void EventManager::addListener(EventListener *listener)
    {
        log.never("check contains");
        if (!listeners.contains(listener))
        {
            log.never("not found");
            listeners.add(listener);
        }
    }

    void EventManager::removeListener(EventListener *listener)
    {
        listeners.remove(listener);
    }

    bool EventManager::hasListener(EventListener *listener)
    {
        for (int i = 0; i < listeners.size(); i++)
        {
            if (listener == listeners.getAt(i))
            {
                return true;
            }
        }
        return false;
    }

    void EventManager::addEvent(Event *event)
    {
        log.debug("add event type %d", event->type);
        if (!event->mayHaveMultiple)
        {
            // remove other events matching this type/sender if multiple are not allowed
            eventQueue->removeIf([event](Event *other)
                                 { return event->getType() == other->getType() && event->getSender() == other->getSender(); });
        }
        eventQueue->add(event);
    }

    void EventManager::process()
    {
        // eventQueue is the one events can be added to.
        // the one they have been added to is put in cur
        List<Event> *cur = eventQueue;

        if (cur->size() == 0)
        {
            log.never("no events to process");
            return;
        }
        if (eventQueue == &events[0])
        {
            log.never("switch event queue 1");
            eventQueue = &events[1];
        }
        else
        {
            log.never("switch event queue 0");

            eventQueue = &events[0];
        }
        cur->forEach([this](Event *event)
                     { 
                        log.never("pocess event type %d",event->getType());
                        this->listeners.forEach([this,event](EventListener *listener)
                                               { 
                                                log.never("send to listener %x",listener);
                                                listener->processEvent(event); }); });
        cur->forEach([](Event *event)
                     { delete event; });
        cur->clear();
    }
}