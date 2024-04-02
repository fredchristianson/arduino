#include "fclib/Animation.h"
#include "fclib/System.h"

using namespace FCLIB;
namespace FCLIB
{

    Animator::Animator(float start, float end, Calculate easingFunction)
    {
        this->startValue = start;
        this->endValue = end;
        this->easing = easingFunction;
        this->startMsecs = THE_BOARD->currentMsecs;
        this->durationMsecs = 0;
        this->complete = true;
        this->currentValue = endValue;
    }
    ~Animator() {}
}
