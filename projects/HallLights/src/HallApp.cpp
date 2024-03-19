
#include "./HallApp.h"

using namespace FCLIB;

bool HallAppSetup::beginSetup()
{
    log.debug("HallAppSetup::beginSetup");

    this->getLoop()->setSpeed(1);
    return true;
}

bool HallAppLoop::loopExecute()
{
    log.debug("hall app loop");
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


AppLoop* HallApp::createLoop(){
    return new HallAppLoop();
}
AppSetup* HallApp::createSetup(){
    return new HallAppSetup();
}