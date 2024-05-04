#ifndef _FCLIB_APP_H
#define _FCLIB_APP_H
#include <c_types.h>
#include <stdlib.h>
#include "./System.h"
#include "./Timer.h"
#include "./Config.h"
#include "./Net.h"
#include "./Logging.h"
#include "./AppStat.h"

namespace FCLIB
{

    class App
    {
    public:
        static App *THE_APP;
        static Board *THE_BOARD;

        App();
        virtual ~App();

        bool setup(Config *config);
        void loop();
        Config *getConfig();

        void writeStats();
        void resetStats();
        void addStat(AppStat *stat);
        void removeStat(AppStat *stat);

    protected:
        virtual void beforeLoop();
        virtual void runTasks();
        virtual void afterLoop();

        virtual void setupComplete(); // called after successful setup

        virtual bool beginSetup();
        virtual bool endSetup();
        virtual bool setupNetwork();
        virtual bool setupDevices();
        virtual bool setupLogging();

        Logger log;

    private:
        List<AppStat> stats;
        Network *network;
        bool running;
        Config *config;
    };
}
#endif
