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