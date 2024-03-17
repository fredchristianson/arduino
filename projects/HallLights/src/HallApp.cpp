
#include "./HallApp.h"

using namespace FCLIB;

HallApp::HallApp()
{
    log.setModuleName("HallApp");
    log.info("Created");
}

HallApp::~HallApp()
{
}

bool HallApp::beginSetup()
{
    this->setSpeed(1);
    return true;
}
bool HallApp::endSetup()
{
    return true;
}
bool HallApp::setupNetwork()
{
    return true;
}
bool HallApp::setupDevices()
{
    return true;
}

bool HallApp::loopExecute()
{
    log.always("Time: %s", TimeClient::getDisplayTime());
    log.always("Epcoh Time: %d", TimeClient::getEpochTime());
    log.always("offseet: %d", TimeClient::getTimezoneOffsetMinutes());
    delay(100);
    return true;
}