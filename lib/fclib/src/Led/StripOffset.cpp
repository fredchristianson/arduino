#include "fclib/LedStrip.h"

using namespace FCLIB;

FCLIB::StripOffset::StripOffset(LedStrip *base, int offset)
{
    LOG.setModuleName("StripOffset");
}

int FCLIB::StripOffset::getOffset()
{
    return this->offset;
}

void FCLIB::StripOffset::setOffset(int offset)
{
    this->offset = offset;
}

int FCLIB::StripOffset::modifyPosition(int pos)
{
    return (pos + this->offset);
}
