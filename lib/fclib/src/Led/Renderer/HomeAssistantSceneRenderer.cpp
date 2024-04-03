#include "fclib/Render.h"
#include "fclib/Task.h"
#include "fclib/App.h"

using namespace FCLIB;

namespace FCLIB
{

    HomeAssistantSceneRenderer::HomeAssistantSceneRenderer(LedStrip *strip) : SceneRenderer(strip)
    {
        log.setModuleName("HomeAssistantSceneRenderer");
        this->mode = SceneMode::MODE_OFF;
        stateChangeDelay = NULL;
    }

    HomeAssistantSceneRenderer::~HomeAssistantSceneRenderer()
    {
    }
    void HomeAssistantSceneRenderer::setMode(SceneMode mode)
    {
        log.always("Set mode %d", mode);
        this->mode = mode;
    }
    void HomeAssistantSceneRenderer::setRGB(const ColorRGB &newRgb)
    {
        this->rgb.red(newRgb.red());
        this->rgb = newRgb;
        log.debug("set RGB %d,%d,%d", this->rgb.red(), this->rgb.green(), this->rgb.blue());
        mode = SceneMode::MODE_RGB;
    }
    void HomeAssistantSceneRenderer::setHSV(const ColorHSV &hsv)
    {
        this->hsv = hsv;
        mode = SceneMode::MODE_HSV;
    }
    void HomeAssistantSceneRenderer::setTemperature(uint temp)
    {
        this->temperature = temp;
    }

    IntervalTimer it(5000);
    void HomeAssistantSceneRenderer::runRenderers()
    {
        // log.conditional(it.isComplete(), NEVER_LEVEL, "runRenderers");
        if (mode == SceneMode::MODE_RGB)
        {
            strip->fill(rgb);
        }
        else if (mode == SceneMode::MODE_HSV)
        {
            strip->fill(hsv);
        }
    }

    void HomeAssistantSceneRenderer::setBrightness(uint8 brightness, int transitionSeconds)
    {
        int start = mode == SceneMode::MODE_RGB ? getBrightness() : 0;
        this->brightness = start;
        log.always("transition brightness from %d to  %d over %d secs", start, brightness, transitionSeconds);
        brightnessAnimation.onChange([this](int b)
                                     { log.never("new brightness %d", b );
                                                  this->brightness = b; })
            .onDone([this]()
                    { log.always("transition done"); })
            .seconds(transitionSeconds, AnimationTimeType::SET)
            .start(start)
            .end(brightness)
            .easing(Ease::cubic)
            .run();
    }

    void HomeAssistantSceneRenderer::turnOff(int transitionSeconds)
    {
        if (stateChangeDelay != NULL)
        {
            stateChangeDelay->end();
        }
        stateChangeDelay = Task::once([this]()
                                      { this->mode = SceneMode::MODE_OFF;this->stateChangeDelay=NULL; });

        stateChangeDelay->delaySeconds(transitionSeconds);
    }
    void HomeAssistantSceneRenderer::turnOn(int transitionSeconds)
    {
        if (stateChangeDelay != NULL)
        {
            stateChangeDelay->end();
            stateChangeDelay = NULL;
        }
        // on needs to happen now so brightness can transition
        this->mode = SceneMode::MODE_RGB;
        if (this->brightness == 0)
        {
            this->brightness = 255;
        }

        // stateChangeDelay = Task::once([this]()
        //                               { this->mode = SceneMode::MODE_RGB;this->stateChangeDelay=NULL; });
        // stateChangeDelay->delaySeconds(transitionSeconds);
    }
}