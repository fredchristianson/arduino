#include "fclib/LedStrip.h"

using namespace FCLIB;

FCLIB::StripMirror::StripMirror(LedStrip *base)
{
    LOG.setModuleName("StripLoop");
    this->setLength(base == NULL ? 0 : base->length() / 2);
}

FCLIB::StripMirror::~StripMirror()
{
}

void FCLIB::StripMirror::set(int pos, const Color &color, LedOp_t op)
{
    if (this->baseStrip == NULL || pos < 0 || pos >= length())
    {
        return;
    }
    baseStrip->set(pos, color, op);
    baseStrip->set(2 * length() - pos - 1, color, op);
}
void FCLIB::StripMirror::clear()
{
    if (this->baseStrip == NULL)
    {
        return;
    }
    baseStrip->clear();
}
void FCLIB::StripMirror::fill(const Color &color, LedOp_t op)
{
    if (this->baseStrip == NULL)
    {
        return;
    }
    baseStrip->fill(color, op);
}
void FCLIB::StripMirror::setBase(LedStrip *base)
{
    this->baseStrip = base;
    this->setLength(base == NULL ? 0 : base->length() / 2);
}
