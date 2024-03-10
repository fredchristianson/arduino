#ifndef __FCNET_H_
#define __FCNET_H_
#include "fclib/Logging.h"

namespace FCLIB
{

    class WiFiSetup
    {
    public:
        WiFiSetup();
        ~WiFiSetup();

        bool connect(const char *name);
        bool startPortal(const char *name);

        const char *getIP();

    private:
        Logger log;
    };
}

#endif