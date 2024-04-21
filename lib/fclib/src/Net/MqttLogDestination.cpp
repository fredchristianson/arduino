#include "fclib/Net.h"

using namespace FCLIB;

namespace FCLIB
{
    Mqtt::LogDestination::LogDestination(Mqtt *mqtt, const char *topic, LogLevel level)
    {
        this->mqtt = mqtt;
        this->topic = topic;
        this->level = level;
    }
    void Mqtt::LogDestination::write(int level, const char *message) const
    {
        if (this->level < level || message == NULL)
        {
            return;
        }
        this->mqtt->send(this->topic, message, true);
    }

}