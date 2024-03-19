#include <stdlib.h>
#include "fclib/App.h"
#include "fclib/System.h"
#include <Esp.h>

namespace FCLIB
{

    AppDevices::AppDevices() : AppComponent(), log("AppDevices")
    {
    }

    AppDevices::~AppDevices()
    {
    }

    bool AppDevices::setup(Config *config)
    {
        log.debug("device setup");
        return true;
    }

}