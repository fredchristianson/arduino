#ifndef _FCLIB_ANIMATION_H_
#define _FCLIB_ANIMATION_H_
#include <c_types.h>
#include "fclib/Logging.h"
#include "fclib/Callable.h"
#include "fclib/Task.h"
#include "fclib/Color.h"

using namespace FCLIB;

namespace FCLIB
{
    class AnimationBase;

    enum class AnimationTimeType
    {
        ADD = 0,
        SET = 1
    };
    class Ease
    {
    public:
        static float linear(float domain) { return domain; }
        static float cubic(float domain) { return domain * domain * domain; }
    };

    class Animator
    {
    public:
        float getCurrentValue() const { return currentValue; }
        bool isComplete() const { return complete; }
        float calcCurrent(float start, float end);

    protected:
        friend class AnimationBase;
        Animator(float start, float end, Calculate<float> easingFunction);
        virtual ~Animator();
        float update();
        void restart();
        float startValue;
        float endValue;
        float currentValue;
        bool complete;
        unsigned long startMsecs;
        unsigned long durationMsecs;
        Calculate<float> easing;
    };

    class AnimationBase
    {
    public:
        AnimationBase(float startValue, float endValue, Calculate<float> easingFunction = Ease::linear);
        AnimationBase &seconds(long secs, AnimationTimeType type = AnimationTimeType::ADD);
        AnimationBase &msecs(long msecs, AnimationTimeType type = AnimationTimeType::ADD);
        AnimationBase &minutes(long minutes, AnimationTimeType type = AnimationTimeType::ADD);
        AnimationBase &startTime(long msecs);
        AnimationBase &duration(long msecs);
        AnimationBase &endTime(long msecs);
        AnimationBase &start(float start);
        AnimationBase &end(float end);
        AnimationBase &run(); // start or restart

        AnimationBase &onDone(SimpleCallable callback);

        AnimationBase &easing(Calculate<float> calc);

        bool isComplete() const;
        // update and return true if changed
        virtual bool update();

    protected:
        float get();
        virtual void changed(float newValue) {}
        // if the derived class is not a float, the old/new values may not be different
        virtual bool isDifferent(float oldValue, float newValue) { return oldValue != newValue; }
        Animator animator;

        virtual ~AnimationBase();
        SimpleCallable doneCallback;
        Task *task;
        Logger log;
    };
    class AnimateInt : public AnimationBase
    {
    public:
        AnimateInt(int start = 0, int end = 100, Calculate<float> easing = Ease::linear) : AnimationBase(start, end, easing) { callback = NULL; }
        virtual ~AnimateInt() {}

        AnimateInt &onChange(Callback<int> callback)
        {
            this->callback = callback;
            return *this;
        }

        int value() const { return (int)animator.getCurrentValue(); }

    protected:
        virtual bool isDifferent(float oldValue, float newValue) { return ((int)oldValue) != ((int)newValue); }

        void changed(float newValue) override
        {
            if (callback != NULL)
            {
                callback((int)newValue);
            }
        }

    private:
        Callback<int> callback;
    };
    class AnimateFloat : public AnimationBase
    {
    public:
        AnimateFloat(float start = 0, float end = 100, Calculate<float> easing = Ease::linear) : AnimationBase(start, end, easing) { callback = NULL; }
        virtual ~AnimateFloat() {}
        AnimateFloat &onChange(Callback<float> callback)
        {
            this->callback = callback;
            return *this;
        }

        float value() const { return animator.getCurrentValue(); }

    protected:
        void changed(float newValue) override
        {
            if (callback != NULL)
            {

                callback(newValue);
            }
        }

    private:
        Callback<float> callback;
    };

    class AnimateColor : public AnimationBase
    {
    public:
        AnimateColor(float start = 0, float end = 100, Calculate<float> easing = Ease::linear);
        virtual ~AnimateColor() {}
        AnimateColor &onChange(Callback<const Color &> callback)
        {
            this->callback = callback;
            return *this;
        }
        AnimateColor &startColor(const Color &color)
        {
            current = color;
            from = color;
            return *this;
        }
        AnimateColor &endColor(const Color &color)
        {
            to = color;
            return *this;
        }
        Color value() const { return current; }
        virtual bool update();

    protected:
    private:
        Callback<const Color &> callback;
        Color from;
        Color to;
        Color current;
    };

    namespace TEST
    {
        namespace ANIMATION
        {
            class AnimationTestSuite : public TestSuite
            {
            public:
                AnimationTestSuite();
                virtual ~AnimationTestSuite();

            protected:
                virtual void runTests() override;
                void run();
            };
        }
    }
}
#endif