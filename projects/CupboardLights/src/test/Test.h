#ifndef HALLLIGHTS_TEST
#define HALLLIGHTS_TEST

#include "fclib/Config.h"
#include "fclib/Task.h"
#include "fclib/Test.h"
#include "fclib/Logging.h"
#include "fclib/List.h"
#include "fclib/Animation.h"
#include "fclib/Persistance.h"

using namespace FCLIB;
using namespace FCLIB::TEST;

namespace CUPBOARD_TEST
{

    bool runTests()
    {
        Logger log("Cupboard Test");
        TestRunner runner("Cupboard Tests", log);

        TASK::TaskTestSuite task;
        runner.run(task);
        CONFIG::ConfigTestSuite config;
        runner.run(config);
        LIST::ListTestSuite list;
        runner.run(list);
        PERSIST::PersistTestSuite persist;
        runner.run(persist);
        ANIMATION::AnimationTestSuite animate;
        runner.run(animate);
        return runner.isSuccess();
    }
}

#endif