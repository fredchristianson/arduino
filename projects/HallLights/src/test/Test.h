#ifndef HALLLIGHTS_TEST
#define HALLLIGHTS_TEST

#include "fclib/Config.h"
#include "fclib/Test.h"
#include "fclib/Logging.h"

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
        return runner.isSuccess();
    }
}

#endif