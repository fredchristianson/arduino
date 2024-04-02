#include "fclib/Animation.h"
#include "fclib/System.h"

using namespace FCLIB;
namespace FCLIB
{
    AnimationBase::AnimationBase(float startValue, float endValue, Calculate<float> easingFunction) : animator(startValue, endValue, easingFunction), log("Animation")
    {
        task = NULL;
        doneCallback = NULL;
        task = Task::repeat([this]()
                            { this->update(); })
                   ->delayMsecs(1);
    }
    AnimationBase &AnimationBase::seconds(long secs)
    { // adds sec to lenght of animation
        animator.durationMsecs += secs * 1000;
        return *this;
    }
    AnimationBase &AnimationBase::msecs(long msecs)
    { // adds milleseconds  to lenght of animation
        animator.durationMsecs += msecs;
        return *this;
    }
    AnimationBase &AnimationBase::minutes(long minutes)
    { // adds minutes to lenght of animation
        animator.durationMsecs += minutes * 60 * 1000;
        return *this;
    }
    AnimationBase &AnimationBase::startTime(long msecs)
    { // default to create time
        animator.startMsecs = msecs;
        animator.complete = false;

        return *this;
    }
    AnimationBase &AnimationBase::duration(long msecs)
    {
        animator.durationMsecs = msecs;
        animator.complete = false;

        return *this;
    }
    AnimationBase &AnimationBase::endTime(long msecs)
    { // must be called after startTime if startTime changes
        animator.durationMsecs = msecs - animator.startMsecs;
        animator.complete = false;
        return *this;
    }
    AnimationBase &AnimationBase::start(float start)
    {
        animator.startValue = start;
        return *this;
    }
    AnimationBase &AnimationBase::end(float end)
    {
        animator.endValue = end;
        animator.complete = false;

        return *this;
    }

    AnimationBase &AnimationBase::onDone(SimpleCallable callback)
    {
        doneCallback = callback;
        return *this;
    }

    AnimationBase &AnimationBase::easing(Calculate<float> calc)
    {
        animator.easing = calc;
        return *this;
    }

    float AnimationBase::get()
    {
        update();

        return animator.currentValue;
    }

    bool AnimationBase::isComplete() { return animator.complete; }

    // update and return true if changed
    bool AnimationBase::update()
    {
        if (isComplete())
        {
            return false;
        }

        float old = animator.currentValue;
        animator.update();

        bool change = (old != animator.currentValue);
        if (change)
        {
            changed(animator.currentValue);
        }
        if (isComplete() && doneCallback)
        {
            doneCallback();
        }
        return change;
    }
    AnimationBase::~AnimationBase()
    {
        if (task != NULL)
        {
            task->end();
        }
    }
}