
#include "./HallApp.h"
#include "fclib/EpochTime.h"

    using namespace FCLIB;

bool HallAppSetup::beginSetup()
{
    log.debug("HallAppSetup::beginSetup");

    this->getLoop()->setSpeed(1);
    return true;
}

bool HallAppLoop::loopExecute()
{
    EpochTime &et = EpochTime::getInstance();
    log.debug("hall app loop %ld", et.getEpochSeconds());
    delay(500);
    return true;
}

HallApp::HallApp()
{
    log.setModuleName("HallApp");

    log.info("Created");
}

HallApp::~HallApp()
{
}

AppLoop *HallApp::createLoop()
{
    return new HallAppLoop();
}
AppSetup *HallApp::createSetup()
{
    return new HallAppSetup();
}