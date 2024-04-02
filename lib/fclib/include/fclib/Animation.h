#ifndef _FCLIB_ANIMATION_H_
#define _FCLIB_ANIMATION_H_
#include <c_types.h>
#include "fclib/Logging.h"
#include "fclib/Callable.h"
#include "fclib/Task.h"

using namespace FCLIB;

namespace FCLIB
{
    template <typename T>
    class AbstractAnimation;

    class Ease
    {
    public:
        static float linear(float domain) { return domain; }
        static float cubic(float domain) { return domain * domain * domain; }
    };

    class Animator
    {
    protected:
        friend template <typename T>
        class AbstractAnimation;
        Animator(float start, float end, Calculate<float> easingFunction);
        virtual ~Animator();
        float update();
        float startValue;
        float endValue;
        float currentValue;
        bool complete;
        int startMsecs;
        int durationMsecs;
        Calculate<float> *easing;
    };

    template <typename T>
    class AbstractAnimation
    {
    public:
        AbstractAnimation(T startValue, T endValue, Calculate<T> easingFunction = Ease::linear) : Animator(startValue, endValue, easingFunction) { task = NULL; }
        AbstractAnimation &seconds(int secs)
        { // adds sec to lenght of animation
            animator.durationMsecs += secs * 1000;
            return *this;
        }
        AbstractAnimation &msecs(int msecs)
        { // adds milleseconds  to lenght of animation
            animator.durationMsecs += msecs;
            return *this;
        }
        AbstractAnimation &minutes(int minutes)
        { // adds minutes to lenght of animation
            animator.durationMsecs += minutes * 60 * 1000;
            return *this;
        }
        AbstractAnimation &startTime(int msecs)
        { // default to create time
            animator.startMsecs = msecs;
            return *this;
        }
        AbstractAnimation &duration(int msecs)
        {
            animator.durationMsecs = msecs;
            return *this;
        }
        AbstractAnimation &endTime(int msecs)
        { // must be called after startTime if startTime changes
            animator.durationMsecs = msecs - animator.startMsecs;
            return *this;
        }
        AbstractAnimation &start(T start)
        {
            animator.startValue = start;
            return *this;
        }
        AbstractAnimation &end(T end)
        {
            animator.endValue = end;
            return *this;
        }

        AbstractAnimation &onChange(Callback<T> callback)
        {
            changeCallback = callback;
            return *this;
        }
        AbstractAnimation &onDone(SimpleCallable callback)
        {
            doneCallback = callback;
            return *this;
        }

        AbstractAnimation &easing(Calculate<T> calc)
        {
            animator.easing = calc;
            return *this;
        }

        T get()
        {
            update();
            return currentValue;
        }
        bool isComplete() { return animator.complete; }

    protected:
        Animator animator;
        virtual bool update()
        {
            animator.update();
            return animator.complete;
        }
        virtual ~AbstractAnimation()
        {
            if (task != NULL)
            {
                task->end();
            }
        }
        Callback<T> changeCallback;
        SimpleCallable doneCallback;
        Task *task;
    };
    class AnimateInt : public AbstractAnimation<int>
    {
    public:
        AnimateInt(int start = 0, int end = 100, Calcualte<float> easing = Ease::linear) : AbstractAnimation<int>(start, end, easing) {}
        virtual ~AnimateInt() {}
    };
    class AnimateFloat : public AbstractAnimation<float>
    {
    public:
        AnimateFloat(float start = 0, float end = 100, Calcualte<float> easing = Ease::linear) : AbstractAnimation<float>(start, end, easing) {}
        virtual ~AnimationFloat() {}
    };
}
#endif