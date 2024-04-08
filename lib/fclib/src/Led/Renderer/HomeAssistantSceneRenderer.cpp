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

    // IntervalTimer it(5000, TIME_MSECS);
    void HomeAssistantSceneRenderer::runRenderers()
    {
        if (strip != NULL)
        {
            // log.conditional(it.isComplete(), ALWAYS_LEVEL, "brightness %d", currentState.brightness);
            strip->setBrightness(currentState.brightness);
            strip->fill(currentState.color);
        }
    }

    void HomeAssistantSceneRenderer::saveState(SceneState &state)
    {
        log.showMemory("Before psersist");
        Persist::set("hascene", "brightness", state.brightness);
        log.showMemory("After  psersist");
        Color::RGB rgb = state.color.toRGB();
        Persist::set("hascene", "r", rgb.red());
        Persist::set("hascene", "g", rgb.green());
        Persist::set("hascene", "b", rgb.blue());
        Persist::set("hascene", "on", state.mode != SceneMode::MODE_OFF);
    }
    void HomeAssistantSceneRenderer::loadState(SceneState &state)
    {
        state.brightness = Persist::get("hascene", "brightness", 255);
        int r = Persist::get("hascene", "r", 255);
        int g = Persist::get("hascene", "g", 255);
        int b = Persist::get("hascene", "b", 255);
        state.color = Color::fromRGB(r, g, b);
        bool on = Persist::get("hascene", "on", false);
        state.mode = on ? SceneMode::MODE_ON : SceneMode::MODE_OFF;
    }

    void HomeAssistantSceneRenderer::setSceneState(const SceneState &state, uint16 transitionMsecs)
    {
        transition.from = currentState;
        transition.to = state;
        transition.transistionMsecs = transitionMsecs;
        if (state.mode == SceneMode::MODE_OFF)
        {
            transition.to.brightness = 0;
        }
        else if (transition.to.brightness == 0)
        {
            transition.to.brightness = Persist::get("hascene", "brightness", 255);
        }
        if (transition.from.mode == SceneMode::MODE_OFF)
        {
            transition.from.brightness = 0;
        }
        startTransition();
    }
    const SceneState HomeAssistantSceneRenderer::getSceneState()
    {
        return currentState;
    }
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
        start();
        log.never("start transition: %d %d,%d,%d %d-%d %d", transition.to.color.getType(),
                  transition.to.color.toRGB().red(),
                  transition.to.color.toRGB().green(),
                  transition.to.color.toRGB().blue(),
                  transition.from.brightness,
                  transition.to.brightness,
                  transition.transistionMsecs);
        animateColor.onChange([this](const Color &newColor)
                              {
                                   Color::RGB rgb = newColor.toRGB();
                                   this->currentState.color = newColor; })
            .startColor(transition.from.color)
            .endColor(transition.to.color)
            .msecs(transition.transistionMsecs, AnimationTimeType::SET)
            .run();

        animateBrightness.onChange([this](int val)
                                   {
           // this->log.never("brightness change %d", val);
            this->currentState.brightness = val;
            Event::trigger(EventType::CHANGE_EVENT, this); })
            .start(transition.from.brightness)
            .end(transition.to.brightness)
            .msecs(transition.transistionMsecs, AnimationTimeType::SET)
            .onDone([this]()
                    {
                        //log.debug("transition done");
            this->currentState = this->transition.to;
            Event::trigger(EventType::TASK_DONE, this);

            if (this->currentState.mode == SceneMode::MODE_OFF)
            {
                stop();
            } else {
            this->saveState(this->currentState);
            } })
            .run();

        currentState.mode = SceneMode::MODE_TRANSITION;
    }
    void HomeAssistantSceneRenderer::turnOn(int transitionSeconds)
    {
        if (currentState.mode == SceneMode::MODE_OFF)
        {

            currentState.brightness = 0;

            transition.from = currentState;
            loadState(transition.to);
            transition.to.mode = SceneMode::MODE_ON;
            transition.transistionMsecs = transitionSeconds * 1000;
            startTransition();
        }
    }
}