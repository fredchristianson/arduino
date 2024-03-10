#include "fclib/LedStrip.h"

using namespace FCLIB;

FCLIB::StripSegment::StripSegment(LedStrip *base, int offset, uint length) : StripModifier(base)
{
    LOG.setModuleName("StripSegment");
    this->offset = offset;
    this->count = length;
}

void FCLIB::StripSegment::setLength(int length)
{
    this->count = length;
}

int FCLIB::StripSegment::getOffset()
{
    return this->offset;
}

void FCLIB::StripSegment::setOffset(int offset)
{
    this->offset = offset;
}

int FCLIB::StripSegment::getLength()
{
    return this->count;
}

bool FCLIB::StripSegment::isValidPosition(int pos)
{
    return pos <= this->count;
}

int FCLIB::StripSegment::modifyPosition(int pos)
{
    LOG.never("seg %d %d", pos, this->offset);
    return pos + this->offset;
}
