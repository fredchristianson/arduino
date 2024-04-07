#pragma once
#ifndef TESTS__TEST
#define TESTS_TEST

#include "fclib/Config.h"
#include "fclib/Task.h"
#include "fclib/Test.h"
#include "fclib/Logging.h"
#include "fclib/List.h"
#include "fclib/Animation.h"
#include "fclib/Persistance.h"

using namespace FCLIB;
using namespace FCLIB::TEST;

bool runTests()
{
    Logger log("Test");
    TestRunner runner("Tests", log);

    LIST::ListTestSuite list;
    // TASK::TaskTestSuite task;
    // CONFIG::ConfigTestSuite config;
    // PERSIST::PersistTestSuite persist;
    // ANIMATION::AnimationTestSuite animate;

    runner.run(list);

    // runner.run(task);
    // runner.run(config);
    // runner.run(persist);
    // runner.run(animate);
    return runner.isSuccess();
}
#endif