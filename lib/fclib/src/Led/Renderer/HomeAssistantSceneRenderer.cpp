#include "fclib/Render.h"
#include "fclib/Task.h"
#include "fclib/App.h"

using namespace FCLIB;

namespace FCLIB
{

    HomeAssistantSceneRenderer::HomeAssistantSceneRenderer(LedStrip *strip) : SceneRenderer(strip)
    {
        log.setModuleName("HomeAssistantSceneRenderer");
        loadState(currentState);
    }

    HomeAssistantSceneRenderer::~HomeAssistantSceneRenderer()
    {
    }

    void HomeAssistantSceneRenderer::runRenderers()
    {
        // log.conditional(it.isComplete(), NEVER_LEVEL, "runRenderers");
        strip->setBrightness(currentState.brightness);
        strip->fill(currentState.color);
    }

    void HomeAssistantSceneRenderer::saveState(SceneState &state)
    {
        Persist::set("hascene", "brightness", state.brightness);
        Color::RGB rgb = state.color.toRGB();
        Persist::set("hascene", "r", rgb.red());
        Persist::set("hascene", "g", rgb.green());
        Persist::set("hascene", "b", rgb.blue());
    }
    void HomeAssistantSceneRenderer::loadState(SceneState &state)
    {
        state.brightness = Persist::get("hascene", "brightness", 255);
        int r = Persist::get("hascene", "r", 255);
        int g = Persist::get("hascene", "g", 255);
        int b = Persist::get("hascene", "b", 255);
        state.color = Color::fromRGB(r, g, b);
    }

    void HomeAssistantSceneRenderer::setSceneState(const SceneState &state, uint16 transitionMsecs)
    {
        transition.from = currentState;
        transition.to = state;
        transition.transistionMsecs = transitionMsecs;
        startTransition();
    }
    const SceneState HomeAssistantSceneRenderer::getSceneState() { return currentState; }
    void HomeAssistantSceneRenderer::turnOff(int transitionSeconds)
    {
        if (currentState.mode == SceneMode::MODE_OFF)
        {
            saveState(currentState);
        }
        if (currentState.mode != SceneMode::MODE_OFF)
        {
            transition.from = currentState;
            transition.to = currentState;
            transition.to.mode = SceneMode::MODE_OFF;
            transition.to.brightness = 0;
            transition.transistionMsecs = transitionSeconds * 1000;
            startTransition();
        }
    }
    void HomeAssistantSceneRenderer::startTransition()
    {
        animateColor.onChange([this](const Color &newColor)
                              { this->currentState.color = newColor; })
            .startColor(transition.from.color)
            .endColor(transition.to.color)
            .msecs(transition.transistionMsecs, AnimationTimeType::SET)
            .run();
        animateBrightness.onChange([this](int val)
                                   { this->currentState.brightness = val; })
            .start(transition.from.brightness)
            .end(transition.to.brightness)
            .msecs(transition.transistionMsecs, AnimationTimeType::SET)
            .onDone([this]()
                    { this->currentState.mode = this->transition.to.mode; })
            .run();
        currentState.mode = SceneMode::MODE_TRANSITION;
    }
    void HomeAssistantSceneRenderer::turnOn(int transitionSeconds)
    {
        if (currentState.mode == SceneMode::MODE_OFF)
        {
            transition.from = currentState;
            loadState(transition.to);
            transition.to.mode = SceneMode::MODE_ON;
            transition.transistionMsecs = transitionSeconds * 1000;
            startTransition();
        }
    }
}