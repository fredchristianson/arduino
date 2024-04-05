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
        SceneState scene = renderer->getSceneState();
        Color color = scene.color;
        log.debug("publish LightStrip state");
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
        doc["supported_color_modes"].add("hs");
        // doc["supported_color_modes"].add("white");

        // HA mired range is about 140-500.  Map to 2-6k when received or HA slider looks wrong
        doc["min_mireds"] = 140;
        doc["max_mireds"] = 500;
        doc["color_mode"] = true;
    }

    void LightStrip::onCommand(const char *payload)
    {
        log.always("got command: %s", payload);
        SceneState scene = renderer->getSceneState();

        JsonDocument doc;
        deserializeJson(doc, payload);
        String state = doc["state"];
        int transitionSeconds = 2;
        if (doc.containsKey("transition"))
        {
            transitionSeconds = doc["transition"].as<int>();
        }

        if (doc.containsKey("brightness"))
        {
            int b = doc["brightness"].as<int>();
            scene.brightness = b;
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
        renderer->setSceneState(scene, transitionSeconds);
        Event::trigger(EventType::CHANGE_EVENT, this);
        this->publishState();
    }

    void LightStrip::subscribe(Mqtt *mqtt)
    {

        mqtt->subscribe(baseTopic() + "/command", [this](const char *payload)
                        { this->onCommand(payload); });
    }
}
