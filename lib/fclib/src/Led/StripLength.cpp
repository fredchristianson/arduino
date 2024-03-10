#include "fclib/LedStrip.h"

using namespace FCLIB;

FCLIB::StripLength::StripLength(LedStrip *base, uint length)
{
    LOG.setModuleName("StripLength");
    this->count = length;
}

int FCLIB::StripLength::getLength()
{
    return this->count;
}

void FCLIB::StripLength::setLength(int length)
{
    this->count = length;
}

bool FCLIB::StripLength::isValidPosition(int pos)
{
    return pos >= 0 && pos < this->count;
}
