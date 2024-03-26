#include "fclib/Event.h"
#include "fclib/Logging.h"

using namespace FCLIB;

namespace FCLIB
{

    EventHandler::EventHandler(EventType type, void *sender, EventHandlerCallback handler)
    {
        this->type = type;
        this->sender = sender;
        this->handler = handler;
    }

    EventHandler::~EventHandler()
    {
    }

    bool EventHandler::match(EventType type, void *sender)
    {
        if (this->type != EventType::ANY && this->type != type)
        {
            return false;
        }
        if (this->sender != NULL && this->sender != sender)
        {
            return false;
        }
        return true;
    }

    void EventHandler::handle(Event *event)
    {
        this->handler(event);
    }

}