#pragma once
#ifndef __FCLIB_LOOPTIME_H_
#define __FCLIB_LOOPTIME_H_

namespace FCLIB
{
    class LoopTime
    {
    public:
        static bool over();
        static bool ok() { return !LoopTime::over(); }
        static void startLoop();
        static void endLoop();
        static void setMaxLoopMSecs(unsigned long msecs) { LoopTime::maxMSecs = msecs; }
        static void check(const char *msg);

    private:
        static unsigned long startMSecs;
        static unsigned long maxMSecs;
        static bool loggedOver;
    };
}
#endif