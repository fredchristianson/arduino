#include <Arduino.h>
#include "fclib/Animation.h"

using namespace FCLIB::TEST;
using namespace FCLIB::TEST::ANIMATION;

namespace FCLIB::TEST::ANIMATION
{
    void testFloatAnimation(TestResult &result)
    {
        TaskQueue::get();
        // wdt_disable();
        AnimateFloat a(0, 10);

        a.duration(200);

        while (!a.isComplete())
        {
            delay(1);
            a.update();
        }
        // wdt_enable(WDTO_1S);
        TaskQueue::process();

        result.equal(a.value(), 10.0, "end value correct");
        TaskQueue::destroy();
    }

    void testCallbacks(TestResult &result)
    {
        TaskQueue::get();
        AnimateInt a(0, 10000);
        bool changeCallback = false;
        bool doneCallback = false;
        a.onChange([&changeCallback](int val)
                   { changeCallback = true; })
            .onDone([&doneCallback]()
                    { doneCallback = true; })
            .duration(200);
        while (!a.isComplete())
        {
            TaskQueue::process();
        }
        result.test(changeCallback, "got changeCallback");
        result.test(doneCallback, "got doneCallback");
        result.equal(a.value(), 10000, "final value correct");
        TaskQueue::destroy();
    }

    void testRestart(TestResult &result)
    {
        TaskQueue::get();
        AnimateInt a(0, 10000);
        a.duration(200);
        a.run().run();

        while (!a.isComplete())
        {
            TaskQueue::process();
        }

        result.equal(a.value(), 10000, "final value correct");
        TaskQueue::destroy();
    }

    AnimationTestSuite::AnimationTestSuite() : TestSuite("AnimationTestSuite")
    {
    }

    AnimationTestSuite::~AnimationTestSuite()
    {
    }

    void AnimationTestSuite::runTests()
    {
        test("Float animation", testFloatAnimation);
        test("Callbacks", testCallbacks);
        test("Restart", testRestart);
    }
}