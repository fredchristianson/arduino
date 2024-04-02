#include "fclib/Animation.h"
#include "fclib/System.h"

using namespace FCLIB;
namespace FCLIB
{

    Animator::Animator(float start, float end, Calculate<float> easingFunction)
    {
        this->startValue = start;
        this->endValue = end;
        this->easing = easingFunction;
        this->startMsecs = THE_BOARD->currentMsecs();
        this->durationMsecs = 0;
        this->complete = true;
        this->currentValue = startValue;
    }
    Animator::~Animator() {}

    float Animator::update()
    {
        int now = THE_BOARD->currentMsecs();
        float change = (now - startMsecs) * 100;
        float pct = change / (durationMsecs * 100.0);
        if (pct >= 1)
        {
            complete = true;
            pct = 1;
        }
        float rangePct = easing(pct);
        float range = endValue - startValue;
        float result = startValue + range * rangePct;
        this->currentValue = result;
        if (result > endValue)
        {
            result = endValue;
        }
        // Logger log("Animator");
        // log.always("Animator %d-%d  ==> %f->%f.  %f, %f, %f", startMsecs, startMsecs + durationMsecs, startValue, endValue, pct, rangePct, result);
        return result;
    }
}
