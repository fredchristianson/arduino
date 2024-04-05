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
        this->currentValue = calcCurrent(startValue, endValue);
        return this->currentValue;
    }
    float Animator::calcCurrent(float start, float end)
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
        float range = end - start;
        float result = start + range * rangePct;
        if ((end > start && result > end) || (end < start && result < end))
        {
            result = end;
        }
        // Logger log("Animator");
        // log.debug("Animator %d-%d  ==> %f->%f.  %f, %f, %f", startMsecs, startMsecs + durationMsecs, startValue, endValue, pct, rangePct, result);
        return result;
    }

    void Animator::restart()
    {
        startMsecs = THE_BOARD->currentMsecs();
        // Logger log("Animator");
        // log.debug("restarting animation at %d.  end in %d at %d", startMsecs, durationMsecs, startMsecs + durationMsecs);
        complete = false;
    }
}
