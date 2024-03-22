#include <stdlib.h>
#include "fclib/App.h"
#include "fclib/System.h"
#include <Esp.h>

namespace FCLIB
{

    // todo: setup watchdog timer
    AppEventManager::AppEventManager() : AppComponent(), log("AppEventManager")
    {
    }

    AppEventManager::~AppEventManager()
    {
    }

    bool AppEventManager::setup(Config *config)
    {

        return true;
    }

    void AppEventManager::processEvents()
    {
        eventManager.processEvents();
    }

}