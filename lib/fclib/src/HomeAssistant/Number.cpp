#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    Number::Number(Device *device, const char *name, float minval, float maxval, float val) : CommandEntity(name == NULL ? "Number" : name, device, ComponentType::NUMBER)
    {
        log.setModuleName("HA::Number");
        // this->deviceClass = "distance";
        componentName = "number";
        minValue = minval;
        maxValue = maxval;
        value = val;
    }

    Number::~Number()
    {
    }

    void Number::setupCapabilities(JsonDocument &doc)
    {

        doc["min"] = minValue;
        doc["max"] = maxValue;
        doc["mode"] = "box";
    }

    void Number::publishState()
    {
        log.debug("publish Number state");
        JsonDocument doc;
        String state(value);
        doc["state"] = value;
        ha()->publishState(baseTopic() + "/set", doc);
    }

    void Number::onCommand(const char *payload)
    {
        log.always("got command: %s", payload);
        String text(payload);
        text.trim();
        if (text.length() == 0)
        {
            return;
        }
        if (isdigit(text[0]))
        {
            float v = atof(text.c_str());
            log.info("got value %f", v);
            if (value != v)
            {
                value = v;
                this->publishState();
                Event::trigger(EventType::CHANGE_EVENT, this, value);
            }
        }
        else if (text[0] == '{')
        {
            log.warn("need to parse json for value");
        }
    }
}
