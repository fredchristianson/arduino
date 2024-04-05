#include "fclib/Render.h"

using namespace FCLIB;

FCLIB::SolidRenderer::SolidRenderer(const Color &initColor) : color(initColor)
{
}

FCLIB::SolidRenderer::~SolidRenderer()
{
}

void FCLIB::SolidRenderer::draw(LedStrip &strip, LedOp_t op)
{
    strip.fill(this->color, op);
}