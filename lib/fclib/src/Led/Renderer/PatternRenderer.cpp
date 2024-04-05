#include "fclib/Render.h"

using namespace FCLIB;
using namespace FCLIB::Util;

FCLIB::PatternRenderer::PatternRenderer(Positioning positioning)
{
    this->positioning = positioning;
}

FCLIB::PatternRenderer::~PatternRenderer()
{
    this->elements.forEach([](PatternElement *element)
                           { delete element; });
}

PatternRenderer &FCLIB::PatternRenderer::addElement(uint count, Color &color, PositionUnit unit)
{
    elements.add(new PatternElement(color, count, unit));

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
            const PatternElement *elem = elements[idx];
            for (uint c = 0; pos < length && c < elem->count(); c++)
            {
                strip.set(pos, elem->color(), op);
                pos++;
            }
        }
    }
}

FCLIB::PatternElement::PatternElement(const Color &color, uint count, PositionUnit unit)
{
    this->elementColor = color;
    this->elementCount = count;
    this->posUnit = unit;
}

FCLIB::PatternElement::~PatternElement()
{
}
uint FCLIB::PatternElement::count() const { return this->elementCount; }
PositionUnit FCLIB::PatternElement::unit() { return this->posUnit; }
