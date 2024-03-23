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
#include "./Task.h"

namespace FCLIB
{
    class App;
    class AppSetup;
    class AppLoop;
    class AppDevices;
    class AppNetwork;
    class TaskQueue;

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
        static void addTask(LoopTask *task);
        static void removeTask(LoopTask *task);

        AppLoop();
        virtual ~AppLoop();
        void run();
        bool setup(Config *appConfig);

    protected:
        virtual void beforeLoop();
        virtual void runTasks();
        virtual void afterLoop();
        Logger log;

        TaskQueue beforeTasks;
        TaskQueue loopTasks;
        TaskQueue afterTasks;

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
        Mqtt *getMqtt() { return mqtt; }

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
        String mqttDeviceName;

        int ntpOffsetMinutes;
        String deviceName;

    private:
        Mqtt *mqtt;
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

        virtual void setupComplete(); // called after successful setup
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
