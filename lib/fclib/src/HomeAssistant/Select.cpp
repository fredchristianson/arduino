#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    Select::Select(const char *name, const char *optList[], int selectedIndex, Callback<const char *> onChange) : CommandEntity(name == NULL ? "Select" : name, ComponentType::SELECT)
    {
        log.setModuleName("HA::OptionsEn");
        // this->deviceClass = "distance";
        componentName = "select";
        this->changeHandler = onChange;
        const char **opt = optList;
        while (*opt != NULL)
        {
            String *s = new String(*opt);
            options.add(s);
            opt++;
        }
        this->selectedIndex = selectedIndex;
    }

    Select::~Select()
    {
    }

    void Select::setupCapabilities(JsonDocument &doc)
    {
        doc["options"] = JsonArray();
        for (int i = 0; i < options.size(); i++)
        {
            doc["options"].add(options[i]->c_str());
        }
    }

    void Select::publishState()
    {
        log.always("publish Select state %d", selectedIndex);
        int s = min(max(0, selectedIndex), (int)options.size());
        JsonDocument doc;
        doc["state"] = options[s]->c_str();
        ha()->publishState(baseTopic() + "/set", doc);
    }

    void Select::onCommand(const char *payload)
    {
        log.always("got command: %s", payload);
        changeHandler(payload);
    }
}
