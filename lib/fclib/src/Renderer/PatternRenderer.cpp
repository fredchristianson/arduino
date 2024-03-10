#include "fclib/Render.h"

using namespace FCLIB;
using namespace FCLIB::Util;

FCLIB::PatternRenderer::PatternRenderer(Positioning positioning)
{
    this->positioning = positioning;
}

FCLIB::PatternRenderer::~PatternRenderer()
{
    forEach(this->elements, [](PatternElement *element)
            { delete element; });
}

PatternRenderer &FCLIB::PatternRenderer::addElement(uint count, Color &color, PositionUnit unit)
{
    if (color.isHsv())
    {
        elements.add(new PatternElementHSV((ColorHSV &)color, count, unit));
    }
    else if (color.isRgb())
    {
        elements.add(new PatternElementRGB((ColorRGB &)color, count, unit));
    }
    return *this;
}

void FCLIB::PatternRenderer::draw(LedStrip &strip, LedOp_t op)
{
    // todo:  handle positioning and units.  just repeats now
    uint length = strip.length();
    uint pos = 0;
    while (pos < length)
    {
        for (int idx = 0; idx < elements.size(); idx++)
        {
            PatternElement *elem = elements[idx];
            Color &color = elem->color();
            for (int c = 0; pos < length && c < elem->count(); c++)
            {
                if (color.isRgb())
                {
                    ColorRGB &rgb = (ColorRGB &)color;
                }
                strip.set(pos, elem->color(), op);
                pos++;
            }
        }
    }
}

FCLIB::PatternElement::PatternElement(uint count, PositionUnit unit)
{
    this->elementCount = count;
    this->posUnit = unit;
}

FCLIB::PatternElement::~PatternElement()
{
}
uint FCLIB::PatternElement::count() { return this->elementCount; }
PositionUnit FCLIB::PatternElement::unit() { return this->posUnit; }

FCLIB::PatternElementHSV::PatternElementHSV(ColorHSV &color, uint count, PositionUnit unit) : PatternElement(count, unit), elementColor(color)
{
}

FCLIB::PatternElementHSV::~PatternElementHSV()
{
}

Color &FCLIB::PatternElementHSV::color()
{
    return this->elementColor;
}

FCLIB::PatternElementRGB::PatternElementRGB(ColorRGB &color, uint count, PositionUnit unit) : PatternElement(count, unit), elementColor(color)
{
}

FCLIB::PatternElementRGB::~PatternElementRGB()
{
}

Color &FCLIB::PatternElementRGB::color()
{
    return this->elementColor;
}