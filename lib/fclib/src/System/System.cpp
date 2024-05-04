

#include <Arduino.h>
#include "fclib/System.h"
#include <esp.h>
#include <chrono>
#include <map>
#include <stddef.h>
#include <ESP8266WiFi.h>

using namespace std::chrono;
auto processStartTime = std::chrono::high_resolution_clock::now();

using namespace FCLIB;

namespace FCLIB
{

    EspBoardClass::EspBoardClass()
    {
        deviceId = WiFi.macAddress();
        deviceId.replace(":", "");
    }

    void EspBoardClass::reboot()
    {
        return EspClass::restart();
    }
    unsigned long EspBoardClass::getFreeContStack() { return EspClass::getFreeContStack(); }

    unsigned long EspBoardClass::getFreeHeap() { return EspClass::getFreeHeap(); }

    unsigned long EspBoardClass::getMaxFreeBlockSize() { return EspClass::getMaxFreeBlockSize(); }

    unsigned long EspBoardClass::getHeapFragmentation() { return EspClass::getHeapFragmentation(); }

    void EspBoardClass::delayMsecs(size_t msecs) { return delay(msecs); }

    unsigned long EspBoardClass::currentMsecs()
    {
        return millis();
    }

    void EspBoardClass::feedWatchdog()
    {
        EspClass::wdtFeed();
    }

    Board::Board() {}

    Board *Board::get()

    {
        return THE_BOARD;
    }
}

Board *FCLIB::THE_BOARD = new EspBoardClass();
