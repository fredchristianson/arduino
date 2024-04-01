#include "fclib/Render.h"
#include "fclib/Task.h"
#include "fclib/App.h"

using namespace FCLIB;

namespace FCLIB
{

    CompositeSceneRenderer::CompositeSceneRenderer(LedStrip *strip) : SceneRenderer(strip)
    {
        this->strip = strip;
        this->brightness = 50;
    }

    CompositeSceneRenderer::~CompositeSceneRenderer()
    {
    }

    void CompositeSceneRenderer::addRenderer(LedRenderer *renderer)
    {
        this->renderers.add(renderer);
    }

    void CompositeSceneRenderer::runRenderers()
    {

        for (int i = 0; i < renderers.size(); i++)
        {
            log.debug("draw");
            renderers[i]
                ->draw(*strip);
        }
    }

}