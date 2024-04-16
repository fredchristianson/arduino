#include <Arduino.h>
#include "fclib/Logging.h"
#include "fclib/Net.h"
#include "fclib/File.h"
#include "fclib/Config.h"
#ifdef RUN_TESTS
#include "test/Test.h"
#endif
#include "fclib/System.h"
#include "./HallApp.h"

using namespace FCLIB;

FCLIB::Logger *logger = new FCLIB::Logger("main", INFO_LEVEL);
HallApp hallApp;
ConfigFile config;
bool testsPass = true;

void setup()
{
    int test = 0;
    logger->info("stack %lx", &test);
    logger->showMemory("setup");
#ifdef RUN_TESTS
    wdt_disable(); // some tests may take a while
    testsPass = HALLLIGHTS_TEST::runTests();
#endif
    if (!testsPass)
    {
        logger->error("tests failed");
        return;
    }

    wdt_enable(WDTO_1S);

    logger->debug("Load config");
    config.load("/config.ini");
    hallApp.setup(&config);
}

void loop()
{

    if (testsPass)
    {
        hallApp.loop();
    }
}
