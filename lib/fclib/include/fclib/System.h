#ifndef __FCSYSTEM_H__
#define __FCSYSTEM_H__

#include <stddef.h>
#include <WString.h>
namespace FCLIB
{

    class Board
    {

    public:
        static Board *get();
        Board();
        virtual unsigned long getFreeContStack() = 0;

        virtual unsigned long getFreeHeap() = 0;

        virtual unsigned long getMaxFreeBlockSize() = 0;

        virtual unsigned long getHeapFragmentation() = 0;

        virtual unsigned long currentMsecs() = 0;

        virtual void delayMsecs(size_t msecs) = 0;

        virtual const char *getDeviceId() { return deviceId.c_str(); }
        virtual void feedWatchdog() = 0;

    protected:
        String deviceId;
    };

    class EspBoardClass : public Board
    {
    public:
        EspBoardClass();
        virtual unsigned long getFreeContStack() override;

        virtual unsigned long getFreeHeap() override;

        virtual unsigned long getMaxFreeBlockSize() override;

        virtual unsigned long getHeapFragmentation() override;

        virtual unsigned long currentMsecs() override;

        virtual void delayMsecs(size_t msecs) override;
        void feedWatchdog() override;
    };

    extern Board *THE_BOARD;
};
#endif // __SYSTEM_H__