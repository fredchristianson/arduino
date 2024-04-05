#include "fclib/Render.h"
#include "fclib/Task.h"
#include "fclib/App.h"

using namespace FCLIB;

namespace FCLIB
{

    SceneState::SceneState()
    {
        mode = SceneMode::MODE_OFF;
        brightness = 0;
    }

    SceneState::SceneState(const SceneState &other)
    {
        this->mode = other.mode;
        this->color = other.color;
        this->brightness = other.brightness;
    }

    SceneState &SceneState::operator=(const SceneState &other)
    {
        this->mode = other.mode;
        this->color = other.color;
        this->brightness = other.brightness;

        return *this;
    }
}