#include "../../include/fclib/LedStrip.h"

FCLIB::LedStrip::LedStrip(uint16 count) : LOG("LedStrip")
{
    this->count = count;
}

FCLIB::LedStrip::~LedStrip()
{
}

uint16 FCLIB::LedStrip::length()
{
    return this->count;
}

void FCLIB::LedStrip::setLength(uint16 len)
{
    LOG.never("Set strip length %d", len);
    this->count = len;
}

void FCLIB::LedStrip::setBrightness(uint8 level)
{
    // nothing to do in base class
}

void FCLIB::LedStrip::setCount(int newCount)
{
    onCountChange(this->count, newCount);
    setLength(newCount);
}
