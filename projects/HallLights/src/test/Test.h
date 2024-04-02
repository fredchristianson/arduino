#ifndef HALLLIGHTS_TEST
#define HALLLIGHTS_TEST

#include "fclib/Config.h"
#include "fclib/Task.h"
#include "fclib/Test.h"
#include "fclib/Logging.h"
#include "fclib/List.h"
#include "fclib/Animation.h"

using namespace FCLIB;
using namespace FCLIB::TEST;

namespace HALLLIGHTS_TEST
{

    bool runTests()
    {
        Logger log("Hall Lights Test");
        TestRunner runner("Hall Lights Tests", log);
        CONFIG::ConfigTestSuite config;
        runner.run(config);
        TASK::TaskTestSuite task;
        runner.run(task);
        LIST::ListTestSuite list;
        runner.run(list);
        ANIMATION::AnimationTestSuite animate;
        runner.run(animate);
        return runner.isSuccess();
    }
}

#endif