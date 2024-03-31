#include <Arduino.h>
#include "fclib/Logging.h"
#include "fclib/Net.h"
#include "fclib/File.h"
#include "fclib/Config.h"
#include "test/Test.h"
#include "fclib/System.h"
#include "./HallApp.h"

using namespace FCLIB;

FCLIB::Logger *logger = new FCLIB::Logger("main", INFO_LEVEL);
HallApp hallApp;
ConfigFile config;
bool testsPass = true;

void setup()
{
    testsPass = HALLLIGHTS_TEST::runTests();

    if (!testsPass)
    {
        logger->error("tests failed");
        return;
    }

    logger->debug("Load config");
    config.load("/config.ini");
    hallApp.setup(&config);
}

int cnt = 0;
void loop()
{
    // if ((cnt++ % 1000) == 0)
    // {
    //     logger->showMemory("main");
    // }
    if (testsPass)
    {
        hallApp.loop();
    }
}
