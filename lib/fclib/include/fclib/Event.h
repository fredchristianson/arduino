#ifndef _FCLIB_EVENT_H_
#define _FCLIB_EVENT_H_

namespace FCLIB
{
    enum EventType
    {
        CUSTOM = 1000
    };

    class Event
    {
    public:
        Event(EventType type);
        virtual ~Event();

    protected:
        EventType type;
    };
}

#endif