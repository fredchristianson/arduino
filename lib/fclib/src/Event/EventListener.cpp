#include "fclib/Event.h"

using namespace FCLIB;

namespace FCLIB
{
    EventListener::EventListener()
    {
    }

    EventListener::~EventListener()
    {
        stop();
    }

    void EventListener::start()
    {
        EventManager::addListener(this);
    }
    void EventListener::stop()
    {
        EventManager::removeListener(this);
    }

    void EventListener::processEvent(Event *event)
    {
        if (match(event->getType(), event->getSender()))
        {
            handle(event);
        }
    }
}