#include "fclib/LedStrip.h"

using namespace FCLIB;

FCLIB::StripLoop::StripLoop(LedStrip *base) : StripModifier(base)
{
    LOG.setModuleName("StripLoop");
    LOG.always("Loop base length %d", base->length());
}

int FCLIB::StripLoop::modifyPosition(int pos)
{
    if (this->baseStrip->length() == 0)
    {
        LOG.warn("base strip length is 0");
        return 0;
    }
    return (pos) % (this->baseStrip->length());
}
