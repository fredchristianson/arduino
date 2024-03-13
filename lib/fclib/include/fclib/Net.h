#ifndef __FCNET_H_
#define __FCNET_H_
#include "fclib/Logging.h"
#include "fclib/LinkedList.h"
#include <WiFiManager.h>

namespace FCLIB
{
    class WiFiPortalParameter : public WiFiManagerParameter
    {
    public:
        WiFiPortalParameter(
            const char *name,
            const char *prompt,
            const char *defaultValue,
            int len) : WiFiManagerParameter(name, prompt, defaultValue, len)
        {
        }
    };

    class WiFiSetup
    {
    public:
        WiFiSetup();
        ~WiFiSetup();

        bool connect(const char *name);
        bool startPortal(const char *name);

        const char *getIP();

        void addParameter(WiFiPortalParameter &param);

    private:
        Logger log;
        LinkedList<WiFiPortalParameter *> params;
    };
}

#endif