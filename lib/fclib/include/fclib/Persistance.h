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
        // get()/freeMemory() mainly for testing.
        static Persist *get();
        static void freeMemory(); // mainly for tests to free any allocated memory

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
        EventListener listener;
        Logger log;
        AppCounterStat persistCount;
    };

    namespace TEST
    {
        namespace PERSIST
        {
            class PersistTestSuite : public TestSuite
            {
            public:
                PersistTestSuite();
                virtual ~PersistTestSuite();

            protected:
                virtual void runTests() override;
                virtual void prepare();
                virtual void cleanup();
                // void run();
            };
        }
    }
}
#endif