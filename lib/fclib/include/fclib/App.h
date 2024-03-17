#ifndef _FCLIB_APP_H
#define _FCLIB_APP_H
#include <c_types.h>
#include <stdlib.h>
#include "./System.h"
#include "./Timer.h"
#include "./Config.h"
#include "./Event.h"
#include "./LinkedList.h"
#include "./Net.h"
#include "./Logging.h"

namespace FCLIB
{
    class App
    {
    public:
        static App *THE_APP;
        static Board *THE_BOARD;

        App();

        virtual ~App();

        void loop();
        void setSpeed(uint16 framesPerSecond);

        bool setup(Config *appConfig);

        bool runTests(bool (*runFunc)(), bool stopIfFail = true);

    protected:
        virtual bool beginSetup();
        virtual bool endSetup();
        virtual bool setupNetwork();
        virtual bool setupDevices();
        virtual bool setupLogging();
        virtual bool connectNetwork();

        virtual bool beforeLoop() { return true; }
        virtual bool loopExecute() { return true; }
        virtual bool afterLoop() { return true; }

        void setLoopTimer(Timer *timer);

    protected:
        Config *config;
        Logger log;

        bool useNtp;
        bool useMqtt;

    private:
        LinkedList<Event *> events;
        Timer *loopTimer;
        bool stopped;
    };
}
#endif
