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

    AnimationBase &AnimationBase::seconds(long secs, AnimationTimeType type)
    { // adds sec to lenght of animation
        if (type == AnimationTimeType::ADD)
        {
            animator.durationMsecs += secs * 1000;
        }
        else
        {
            animator.durationMsecs = secs * 1000;
        }
        return *this;
    }
    AnimationBase &AnimationBase::msecs(long msecs, AnimationTimeType type)
    { // adds milleseconds  to lenght of animation
        if (type == AnimationTimeType::ADD)
        {
            animator.durationMsecs += msecs;
        }
        else
        {
            animator.durationMsecs = msecs;
        }
        return *this;
    }
    AnimationBase &AnimationBase::minutes(long minutes, AnimationTimeType type)
    { // adds minutes to lenght of animation
        if (type == AnimationTimeType::ADD)
        {
            animator.durationMsecs += minutes * 60 * 1000;
        }
        else
        {
            animator.durationMsecs = minutes * 60 * 1000;
        }
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
    AnimationBase &AnimationBase::run()
    {

        if (task != NULL)
        {
            log.never("end task %x", task);
            task->end();
        }
        task = Task::repeat([this]()
                            { this->update(); })
                   ->delayMsecs(1);

        animator.restart();
        return *this;
    }

    float AnimationBase::get()
    {
        update();

        return animator.currentValue;
    }

    bool AnimationBase::isComplete() const { return animator.complete; }

    // update and return true if changed
    bool AnimationBase::update()
    {
        if (isComplete())
        {
            return false;
        }

        float old = animator.currentValue;
        animator.update();

        bool change = isDifferent(old, animator.currentValue);
        if (change)
        {
            changed(animator.currentValue);
        }
        if (isComplete() && doneCallback)
        {
            doneCallback();
            task->end();
            task = NULL;
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