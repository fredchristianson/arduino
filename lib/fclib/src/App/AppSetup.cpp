#include <stdlib.h>
#include "fclib/App.h"
#include "fclib/System.h"
#include <Esp.h>

namespace FCLIB
{

    // todo: setup watchdog timer
    AppSetup::AppSetup() : AppComponent(), log("AppSetup")
    {
    }

    AppSetup::~AppSetup()
    {
    }

    bool AppSetup::setup(Config *config)
    {
        log.always("AppSetup::setup");
        this->config = config;
        return this->setupLogging() &&
               this->beginSetup() &&
               this->endSetup() &&
               this->setupDevices();
        return true;
    }

    bool AppSetup::beginSetup()
    {
        log.debug("begin setup");
        return true;
    }

    bool AppSetup::endSetup()
    {
        log.debug("end setup");
        return true;
    }

    bool AppSetup::setupLogging()
    {
        log.always("AppSetup::setupLogging");
        configureLogging(this->config->getSection("logging"));
        return true;
    }

    bool AppSetup::setupNetwork()
    {
        return true;
    }

    bool AppSetup::setupDevices()
    {
        return true;
    }

}