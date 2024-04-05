#ifndef _FCLIB_PERSISTANCE_H_
#define _FCLIB_PERSISTANCE_H_
#include "fclib/Config.h"
#include "fclib/Event.h"
#include "fclib/Logging.h"

using namespace FCLIB;
namespace FCLIB
{
    class Persist : public ConfigFile
    {
    public:
        static String get(const char *owner, const char *name, const char *defaultValue);

        static int get(const char *owner, const char *name, int defaultValue);

        static float get(const char *owner, const char *name, float defaultValue);

        static bool get(const char *owner, const char *name, bool defaultValue);

        static void set(const char *owner, const char *name, const char *value);
        static void set(const char *owner, const char *name, int value);
        static void set(const char *owner, const char *name, float value);
        static void set(const char *owner, const char *name, bool value);

        static void store(); // usually should not need this but can force a save
    private:
        Persist();
        virtual ~Persist();

        void persistChanges();

        static Persist *singleton;
        static Persist *get();
        EventListener listener;
        Logger log;
    };
}
#endif