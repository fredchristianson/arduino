#ifndef APP_H
#define APP_H
#include <c_types.h>
#include <stdlib.h>
#include "./System.h"
#include "./Timer.h"

namespace FCLIB
{
    class App
    {
    public:
        static App *THE_APP;
        static Board *THE_BOARD;

        App();

        virtual ~App();

        virtual void beginLoop();
        virtual void doLoop();
        virtual void endLoop();
        void setSpeed(uint16 framesPerSecond);

    protected:
        virtual void beforeLoop() {}
        virtual void loopExecute() {}
        virtual void afterLoop() {}

        void setLoopTimer(Timer *timer);

    private:
        Timer *loopTimer;
        Logger LOG;
    };
}
#endif
