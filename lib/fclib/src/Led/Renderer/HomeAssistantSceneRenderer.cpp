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
    }

    HomeAssistantSceneRenderer::~HomeAssistantSceneRenderer()
    {
    }
    void HomeAssistantSceneRenderer::setMode(SceneMode mode)
    {
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
    void HomeAssistantSceneRenderer::setTransitionMsecs(uint msecs)
    {
        // todo: implement transition.  brightness? color?
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

}