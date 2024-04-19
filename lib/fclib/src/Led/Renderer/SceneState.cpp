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
        ledCount = -1;
    }

    SceneState::SceneState(const SceneState &other)
    {
        this->mode = other.mode;
        this->color = other.color;
        this->brightness = other.brightness;
        this->effect = other.effect;
        this->ledCount = other.ledCount;
    }

    SceneState &SceneState::operator=(const SceneState &other)
    {
        this->mode = other.mode;
        this->color = other.color;
        this->brightness = other.brightness;
        this->ledCount = other.ledCount;
        this->effect = other.effect;
        return *this;
    }
}