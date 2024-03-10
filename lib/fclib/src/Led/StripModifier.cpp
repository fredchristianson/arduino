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
    if (color.isHsv())
    {
        ColorHSV mod;
        const ColorHSV &use = this->modifyColor(newPos, (ColorHSV &)color, mod, op);
        LOG.never("set HSV %d,%d,%d", use.hue(), use.saturation(), use.value());
        baseStrip->set(newPos, use, op);
    }
    else
    {
        ColorRGB mod;
        baseStrip->set(newPos, this->modifyColor(newPos, (ColorRGB &)color, mod, op), op);
    }
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
    ColorHSV modHSV;
    ColorRGB modRGB;
    for (int pos = 0; pos < len; pos++)
    {
        const Color *useColor = &color;
        if (color.isHsv())
        {
            useColor = &(this->modifyColor(pos, (ColorHSV &)color, modHSV, op));
        }
        else if (color.isRgb())
        {
            useColor = &(this->modifyColor(pos, (ColorRGB &)color, modRGB, op));
        }
        int mod_pos = this->modifyPosition(pos);
        this->baseStrip->set(mod_pos, *useColor, op);
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

const ColorHSV &FCLIB::StripModifier::modifyColor(int pos, const ColorHSV &origColor, ColorHSV &modifiedColor, LedOp_t op)
{
    return origColor;
}

const ColorRGB &FCLIB::StripModifier::modifyColor(int pos, const ColorRGB &origColor, ColorRGB &modifiedColor, LedOp_t op)
{
    return origColor;
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
