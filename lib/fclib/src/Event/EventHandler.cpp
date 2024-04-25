#include "fclib/Event.h"
#include "fclib/Logging.h"

using namespace FCLIB;

namespace FCLIB
{
    EventHandler::EventHandler(EventType type,
                               EventHandlerCallback handler) : log("EventHandler")
    {
        log.debug("handler for any source");
        this->type = type;
        this->sender = NULL;
        this->handler = handler;
        this->senderIsEventSource = false;
    }
    EventHandler::EventHandler(EventType type, IEventSource *sender, EventHandlerCallback handler) : log("EventHandler")
    {
        log.debug("handler for IEventSource");
        this->type = type;
        this->sender = sender;
        this->handler = handler;
        this->senderIsEventSource = true;
    }
    EventHandler::EventHandler(EventType type, void *sender, EventHandlerCallback handler) : log("EventHandler")
    {
        log.debug("handler for sender that is not IEventSource");

        this->type = type;
        this->sender = sender;
        this->handler = handler;
        this->senderIsEventSource = false;
    }

    EventHandler::~EventHandler()
    {
    }

    bool EventHandler::match(EventType type, const void *sender)
    {
        if (this->type != EventType::ANY && this->type != type)
        {
            return false;
        }
        if (sender == NULL && type == this->type)
        {
            return true;
        }
        if (this->senderIsEventSource && sender != NULL)
        {
            return ((IEventSource *)sender)->match(sender);
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