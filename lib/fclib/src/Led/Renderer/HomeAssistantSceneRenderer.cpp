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
        effects.add("None");
        effects.add("Grow");
    }

    HomeAssistantSceneRenderer::~HomeAssistantSceneRenderer()
    {
    }

    const List<const char> &HomeAssistantSceneRenderer::getEffects() const { return effects; }
    // IntervalTimer it(5000, TIME_MSECS);
    void HomeAssistantSceneRenderer::runRenderers()
    {
        if (strip != NULL)
        {
            // log.conditional(it.isComplete(), ALWAYS_LEVEL, "brightness %d", currentState.brightness);
            strip->setBrightness(currentState.brightness);

            // strip->fill(currentState.color);
            // if (currentState.ledCount == 0)
            // {
            //     log.never("no length");
            // }
            // if (currentState.brightness == 0)
            // {
            //     log.never("no brightness");
            // }
            for (int i = 0; i < currentState.ledCount; i++)
            {
                strip->set(i, currentState.color);
            }
        }
    }

    void HomeAssistantSceneRenderer::saveState(SceneState &state)
    {
        log.never("save state b=%d", state.brightness);

        Persist::set("hascene", "brightness", state.brightness);
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
        log.never("Loaded state b=%d", state.brightness);
        state.mode = on ? SceneMode::MODE_ON : SceneMode::MODE_OFF;
    }

    void HomeAssistantSceneRenderer::setSceneState(const SceneState &state, uint16 transitionMsecs)
    {
        transition.from = currentState;
        transition.to = state;
        transition.transistionMsecs = transitionMsecs;
        if (state.mode == SceneMode::MODE_OFF)
        {
            log.never("transistion to 0 brightness");
            transition.to.brightness = 0;
        }
        else if (transition.to.brightness == 0)
        {
            transition.to.brightness = Persist::get("hascene", "brightness", 255);
            log.never("transistion to %d saved brightness", transition.to.brightness);
        }
        if (transition.from.mode == SceneMode::MODE_OFF)
        {
            log.never("transition from off brightness to %d", transition.to.brightness);
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
        log.never("turnOff()");
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
        String &effect = transition.to.effect;
        transition.to.ledCount = strip->length();
        if (effect.equalsIgnoreCase("grow") ||
            (lastEffect.equalsIgnoreCase("grow") && transition.to.mode == SceneMode::MODE_OFF))
        {
            int startLeds = 0;
            int endLeds = strip->length();
            if (transition.to.mode == SceneMode::MODE_OFF)
            {
                int tmp = startLeds;
                startLeds = endLeds;
                endLeds = tmp;
            }
            this->currentState.ledCount = 0;
            log.never("Grow %d->%d over %d msecs", startLeds, endLeds, transition.transistionMsecs);
            ledLength.onChange([this](int len)
                               { 
                                log.never("new len %d",len);
                                this->currentState.ledCount = len; })
                .start(startLeds)
                .end(endLeds)
                .msecs(transition.transistionMsecs, AnimationTimeType::SET)
                .run();
        }
        else if (transition.to.mode == SceneMode::MODE_ON)
        {
            this->currentState.ledCount = strip->length();
        }
        animateColor.onChange([this](const Color &newColor)
                              { this->currentState.color = newColor; })
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
                        log.always("Trigger task done %x",this);
                         Event::trigger(EventType::TASK_DONE, this);
                        this->currentState.mode = this->transition.to.mode;
                        if (this->currentState.mode != SceneMode::MODE_OFF)
                        {

                        this->saveState(this->currentState);
                        } })
            .run();

        currentState.mode = SceneMode::MODE_TRANSITION;
        lastEffect = effect;
    }
    void HomeAssistantSceneRenderer::turnOn(int transitionSeconds)
    {
        log.never("turnOn()");
        if (currentState.mode == SceneMode::MODE_OFF)
        {

            currentState.brightness = 0;

            transition.from = currentState;
            loadState(transition.to);
            transition.to.mode = SceneMode::MODE_ON;
            if (transition.to.brightness == 0)
            {
                transition.to.brightness = 255;
            }
            transition.transistionMsecs = transitionSeconds * 1000;
            startTransition();
        }
    }
}