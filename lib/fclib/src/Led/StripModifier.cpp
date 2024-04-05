#include "fclib/LedStrip.h"
using namespace FCLIB;

FCLIB::StripModifier::StripModifier(LedStrip *base) : LedStrip()
{
    LOG.setModuleName("StripModifier");
    this->baseStrip = base;
    this->setLength(base == NULL ? 0 : base->length());
}

FCLIB::StripModifier::~StripModifier()
{
}

void FCLIB::StripModifier::set(int pos, const Color &color, LedOp_t op)
{
    if (this->baseStrip == NULL)
    {
        LOG.warn("StripModifier needs a base strip");
        return;
    }
    int newPos = this->modifyPosition(pos);

    baseStrip->set(newPos, this->modifyColor(newPos, color, op), op);
}

void FCLIB::StripModifier::clear()
{
    if (this->baseStrip == NULL)
    {
        LOG.warn("StripModifier needs a base strip");
        return;
    }
    this->baseStrip->clear();
}

void FCLIB::StripModifier::fill(const Color &color, LedOp_t op)
{
    if (this->baseStrip == NULL)
    {
        LOG.warn("StripModifier needs a base strip");
        return;
    }
    int len = this->length();

    for (int pos = 0; pos < len; pos++)
    {
        int mod_pos = this->modifyPosition(pos);
        Color useColor = this->modifyColor(mod_pos, color, op);

        this->baseStrip->set(mod_pos, useColor, op);
    }
}

void FCLIB::StripModifier::setBase(LedStrip *base)
{
    this->baseStrip = base;
    if (base != NULL)
    {
        this->setLength(base->length());
    }
    else
    {
        this->setLength(0);
    }
}

int FCLIB::StripModifier::modifyPosition(int pos)
{
    return pos;
}

bool FCLIB::StripModifier::isValidPosition(int pos)
{
    return true;
}

bool FCLIB::StripModifier::isValidColor(Color &color)
{
    return true;
}

void FCLIB::StripModifier::setBrightness(uint8 brightness)
{
    this->baseStrip->setBrightness(brightness);
}