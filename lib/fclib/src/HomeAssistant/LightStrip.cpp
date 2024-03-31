#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    LightStrip::LightStrip(Device *device, HomeAssistantSceneRenderer *render, const char *name) : CommandEntity(name == NULL ? "LightStrip" : name, device, ComponentType::LIGHT)
    {
        log.setModuleName("HA::LightStrip");
        log.setLevel(DEBUG_LEVEL);
        this->renderer = render;
        this->deviceClass = "light";
        componentName = "light";
    }

    LightStrip::~LightStrip()
    {
    }

    void LightStrip::publishState()
    {
        log.debug("publish LightStrip state");
        JsonDocument doc;
        doc["state"] = renderer->isRunning() ? "ON" : "OFF";
        doc["brightness"] = renderer->getBrightness();
        doc["color"] = JsonObject();
        doc["color"]["r"] = renderer->getRGB().red();
        doc["color"]["g"] = renderer->getRGB().green();
        doc["color"]["b"] = renderer->getRGB().blue();
        doc["color_mode"] = "rgb";
        ha()->publishState(baseTopic() + "/set", doc);
        // ha()->publishState(baseTopic() + "/set", "{\"state\":\"ON\"}");
    }

    void LightStrip::setupCommandTopics(JsonDocument &doc)
    {
        doc["command_topic"] = "~/command";
    }
    void LightStrip::setupCapabilities(JsonDocument &doc)
    {
        // doc["effect"] = true;
        // doc["effect_list"] = JsonArray();
        // doc["effect_list"].add("effect1");
        // doc["effect_list"].add("effect1");
        // doc["effect_list"].add("effect2");
        // doc["effect_list"].add("effect3");

        doc["brightness"] = true;
        doc["brightness_value_template"] = "{{ value_json.brightness }}";
        doc["rgb_value_template"] = "{{ value_json.rgb }}";
        doc["white_value"] = true;
        doc["supported_color_modes"] = JsonArray();
        doc["supported_color_modes"].add("color_temp");
        doc["supported_color_modes"].add("rgb");
        // doc["supported_color_modes"].add("white");

        // HA mired range is about 140-500.  Map to 2-6k when received or HA slider looks wrong
        doc["min_mireds"] = 140;
        doc["max_mireds"] = 500;
        doc["color_mode"] = true;
    }

    void LightStrip::onCommand(const char *payload)
    {
        log.debug("got command: %s", payload);
        JsonDocument doc;
        deserializeJson(doc, payload);
        String state = doc["state"];
        if (doc.containsKey("color"))
        {
            JsonObject color = doc["color"];
            log.debug("got rgb");
            if (color.containsKey("r") && color.containsKey("g") && color.containsKey("b"))
            {
                int r = color["r"].as<int>();
                int g = color["g"].as<int>();
                int b = color["b"].as<int>();
                log.debug("set RGB: %d,%d,%d", r, g, b);
                renderer->setRGB(ColorRGB(r, g, b));
            }
        }
        else if (doc.containsKey("color_temp"))
        {
            int temp = doc["color_temp"].as<int>();
            log.debug("temp %d", temp);
        }
        if (doc.containsKey("brightness"))
        {
            int b = doc["brightness"].as<int>();
            renderer->setBrightness(b);
        }

        if (state.equals("ON"))
        {
            log.debug("turn on");
            renderer->start();
            this->lastBoolState = true;
        }
        else
        {
            log.debug("turn off");
            renderer->stop();
            this->lastBoolState = false;
        }
        Event::trigger(EventType::CHANGE_EVENT, this);
        this->publishState();
    }

    void LightStrip::subscribe(Mqtt *mqtt)
    {

        mqtt->subscribe(baseTopic() + "/command", [this](const char *payload)
                        { this->onCommand(payload); });
    }
}
