
#include <Arduino.h>
#include "fclib/Logging.h"
#include "fclib/Net.h"
#include "fclib/File.h"
#include "fclib/Config.h"
#ifdef RUN_TESTS
#include "test/Test.h"
#endif
#include "fclib/System.h"

using namespace FCLIB;

FCLIB::Logger *logger = new FCLIB::Logger("main", INFO_LEVEL);
bool testsPass = true;

void setup()
{
    Logger::setDefaultLevel(INFO_LEVEL);

    int test = 0;
    logger->info("stack %lx", &test);
    logger->showMemory("setup");
#ifdef RUN_TESTS
    wdt_disable(); // some tests may take a while
    testsPass = runTests();
#endif
    if (!testsPass)
    {
        logger->error("tests failed");
        return;
    }
    else
    {
        logger->info("tests passed");
    }

    wdt_enable(WDTO_1S);
}

void loop()
{
}
