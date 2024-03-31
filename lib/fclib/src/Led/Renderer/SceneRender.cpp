#include "fclib/Render.h"
#include "fclib/Task.h"
#include "fclib/App.h"

using namespace FCLIB;

namespace FCLIB
{

    SceneRenderer::SceneRenderer(LedStrip *strip) : log("SceneRenderer")
    {
        this->strip = strip;
        this->brightness = 50;
        this->task = NULL;
    }

    SceneRenderer::~SceneRenderer()
    {
        AppLoop::removeTask(task);
    }

    void SceneRenderer::start()
    {
        log.never("start rendering");
        if (task != NULL)
        {
            log.warn("already rendering");
            return;
        }
        // task = LoopTask::create([this]()
        //                         { this->onLoop(); });
        task = Task::repeat([this]()
                            { this->onLoop(); })
                   ->delayMsecs(500);
        log.never("created task %lx", task);
    }

    void SceneRenderer::stop()
    {
        log.never("remove task %lx", task);

        if (task != NULL)
        {
            AppLoop::removeTask(task);
            strip->clear();
            strip->show();
            task = NULL;
            log.never("removed");
        }
    }

    void SceneRenderer::setStrip(LedStrip *strip)
    {
        this->strip = strip;
        strip->clear();
        strip->show();
    }

    void SceneRenderer::setBrightness(uint8 b)
    {
        this->brightness = b;
    }

    void SceneRenderer::render()
    {
        log.never("render %lx %d", this, this->brightness);
        strip->clear();
        strip->setBrightness(this->brightness);
        this->runRenderers();
        strip->show();
    }

    void SceneRenderer::onLoop()
    {
        this->render();
    }
}