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
    class App;
    class AppSetup;
    class AppLoop;
    class AppDevices;
    class AppNetwork;

    class AppComponent
    {
    public:
        AppComponent();
        virtual ~AppComponent();

        App *getApp();
        virtual bool setup(Config *config) { return true; }

        AppSetup *getSetup();
        AppDevices *getDevices();
        AppNetwork *getNetwork();
        AppLoop *getLoop();
        Config *getConfig();

    protected:
        friend App;
        App *app;
    };

    class AppSetup : public AppComponent
    {
    public:
        AppSetup();
        virtual ~AppSetup();

        bool setup(Config *appConfig);

    protected:
        virtual bool beginSetup();
        virtual bool endSetup();
        virtual bool setupNetwork();
        virtual bool setupDevices();
        virtual bool setupLogging();
        Config *config;

        Logger log;
    };

    class AppDevices : public AppComponent
    {
    public:
        AppDevices();
        virtual ~AppDevices();

        bool setup(Config *config);

    protected:
        Logger log;
    };

    class AppLoop : public AppComponent
    {
    public:
        AppLoop();
        virtual ~AppLoop();
        void setSpeed(uint16 framesPerSecond);
        void runOnce();
        bool setup(Config *appConfig);

    protected:
        virtual bool beforeLoop() { return true; }
        virtual bool loopExecute() { return true; }
        virtual bool afterLoop() { return true; }
        void setLoopTimer(Timer *timer);
        Logger log;

    private:
        Timer *loopTimer;
        bool running;
    };

    class AppNetwork : public AppComponent
    {
    public:
        AppNetwork();
        virtual ~AppNetwork();

        virtual bool setup(Config *config);
        virtual bool connect();

    protected:
        bool connectWifi();
        bool connectMqtt();
        bool connectNtp();

        bool useWifi;
        bool resetWifi;
        bool useNtp;
        bool useMqtt;

        String mqttUser;
        String mqttPassword;
        String mqttServer;

        int ntpOffsetMinutes;
        String deviceName;

    private:
        Logger log;
    };

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
        AppSetup *getSetup();
        AppDevices *getDevices();
        AppNetwork *getNetwork();
        AppLoop *getLoop();

    protected:
        Logger log;

        virtual AppSetup *createSetup();
        virtual AppDevices *createDevices();
        virtual AppNetwork *createNetwork();
        virtual AppLoop *createLoop();

    private:
        LinkedList<Event *> events;
        bool running;
        AppSetup *appSetup;
        AppLoop *appLoop;
        AppDevices *appDevices;
        AppNetwork *appNetwork;
        Config *config;
    };
}
#endif
