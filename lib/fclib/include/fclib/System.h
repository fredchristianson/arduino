#ifndef __FCSYSTEM_H__
#define __FCSYSTEM_H__

#include <stddef.h>

namespace FCLIB
{


    class Board
    {

    public:
        static Board* get();
        Board();
        virtual unsigned long getFreeContStack() = 0;

        virtual unsigned long getFreeHeap() = 0;

        virtual unsigned long getMaxFreeBlockSize() = 0;

        virtual unsigned long getHeapFragmentation() = 0;

        virtual unsigned long currentMsecs() = 0;

        virtual void delayMsecs(size_t msecs) = 0;

    private:
    };

    class EspBoardClass : public Board
    {
        public:
        EspBoardClass();
        virtual unsigned long getFreeContStack();

        virtual unsigned long getFreeHeap();

        virtual unsigned long getMaxFreeBlockSize();

        virtual unsigned long getHeapFragmentation();

        virtual unsigned long currentMsecs();

        virtual void delayMsecs(size_t msecs);
    };

    extern Board* THE_BOARD;
};
#endif // __SYSTEM_H__