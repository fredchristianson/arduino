#include "fclib/HomeAssistant.h"

using namespace FCLIB;

namespace FCLIB::HA
{
    LightStrip::LightStrip(HomeAssistantSceneRenderer *render, const char *name) : CommandEntity(name == NULL ? "LightStrip" : name, ComponentType::LIGHT)
    {
        log.setModuleName("HA::LightStrip");
        log.setLevel(DEBUG_LEVEL);
        publishThrottle.setMaxFrequency(1000, TIME_MSECS);
        this->renderer = render;
        this->deviceClass = "light";
        componentName = "light";
        log.debug("created HA:LightStrip %x", this);
        events.handle(EventType::CHANGE_EVENT, render, [this](const Event *event)
                      { this->publishTransitionState(); });
        events.handle(EventType::TASK_DONE, render, [this](const Event *event)
                      { this->publishState(); });
        log.never("listen for TASK_DONE %x", render);
        Task::repeat([this]()
                     { this->publishState(); })
            ->delayMinutes(1);
    }

    LightStrip::~LightStrip()
    {
    }

    void LightStrip::publishTransitionState()
    {
        publishThrottle.run([this]()
                            { this->publishState(); });
    }
    void LightStrip::publishState()
    {

        SceneState scene = renderer->getSceneState();
        Color color = scene.color;
        log.never("publish LightStrip state %d", scene.mode);
        JsonDocument doc;
        doc["state"] = scene.mode != SceneMode::MODE_OFF ? "ON" : "OFF";
        doc["brightness"] = scene.brightness;
        if (color.getType() == ColorType::RGB)
        {
            doc["color"] = JsonObject();
            Color::RGB rgb = color.toRGB();
            doc["color"]["r"] = rgb.red();
            doc["color"]["g"] = rgb.green();
            doc["color"]["b"] = rgb.blue();
            doc["color_mode"] = "rgb";
        }
        else if (color.getType() == ColorType::HSV)
        {
            doc["color"] = JsonObject();
            Color::HSV hsv = color.toHSV();
            doc["color_mode"] = "hs";
            doc["color"]["h"] = hsv.hue();
            doc["color"]["s"] = hsv.saturation();
        }
        else if (color.getType() == ColorType::TEMP)
        {
            log.debug("publish temp: %d", color.toTemp().mireds());
            doc["color"] = "color_temp";
            doc["color_temp"] = color.toTemp().mireds();
        }
        ha()->publishState(baseTopic() + "/set", doc);
        // ha()->publishState(baseTopic() + "/set", "{\"state\":\"ON\"}");
    }

    void LightStrip::setupCommandTopics(JsonDocument &doc)
    {
        doc["command_topic"] = "~/command";
    }
    void LightStrip::setupCapabilities(JsonDocument &doc)
    {
        const List<const char> &effects = renderer->getEffects();
        if (effects.size() > 0)
        {
            doc["effect"] = true;
            doc["effect_list"] = JsonArray();
            for (int i = 0; i < effects.size() && LoopTime::ok(); i++)
            {
                doc["effect_list"].add(effects[i]);
            }
        }
        doc["brightness"] = true;
        doc["brightness_value_template"] = "{{ value_json.brightness }}";
        doc["rgb_value_template"] = "{{ value_json.rgb }}";
        doc["white_value"] = true;
        doc["supported_color_modes"] = JsonArray();
        doc["supported_color_modes"].add("color_temp");
        doc["supported_color_modes"].add("rgb");
        doc["supported_color_modes"].add("hs");
        // doc["supported_color_modes"].add("white");

        // HA mired range is about 140-500.  Map to 2-6k when received or HA slider looks wrong
        doc["min_mireds"] = 140;
        doc["max_mireds"] = 500;
        doc["color_mode"] = true;
    }

    void LightStrip::onCommand(const char *payload)
    {
        log.debug("got command: %s", payload);
        SceneState scene = renderer->getSceneState();
        String c = scene.color.toString();
        log.debug("color %s", c.c_str());
        JsonDocument doc;
        deserializeJson(doc, payload);
        String state = doc["state"];
        int transitionMsecs = 250;
        if (doc.containsKey("transition"))
        {
            transitionMsecs = doc["transition"].as<int>() * 1000;
        }

        if (doc.containsKey("brightness"))
        {
            int b = doc["brightness"].as<int>();
            scene.brightness = b;
            log.always("brightness %d", b);
        }
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
                // renderer->setRGB(ColorRGB(r, g, b));
                scene.color = Color::fromRGB(r, g, b);
            }
        }
        else if (doc.containsKey("color_temp"))
        {
            int temp = doc["color_temp"].as<int>();
            scene.color = Color::fromMired(temp);
        }
        if (doc.containsKey("effect"))
        {
            scene.effect = doc["effect"].as<const char *>();
        }

        if (state.equals("ON"))
        {
            log.debug("turn on");
            scene.mode = SceneMode::MODE_ON;
            this->lastBoolState = true;
        }
        else
        {
            log.debug("turn off");
            scene.mode = SceneMode::MODE_OFF;
            this->lastBoolState = false;
        }
        c = scene.color.toString();
        log.debug("to color %s", c.c_str());
        renderer->setSceneState(scene, transitionMsecs);
        Event::trigger(EventType::CHANGE_EVENT, this);
        this->publishState();
    }

    void LightStrip::subscribe(Mqtt *mqtt)
    {
        log.debug("LightStrip subscribe");
        mqtt->subscribe(baseTopic() + "/command", [this](const char *payload)
                        { this->onCommand(payload); });
    }
}
