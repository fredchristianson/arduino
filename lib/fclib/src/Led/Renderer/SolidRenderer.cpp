#include "fclib/Render.h"

using namespace FCLIB;

FCLIB::RGBRenderer::RGBRenderer(uint8 r, uint8 g, uint8 b) : color(r, g, b)
{
}

FCLIB::RGBRenderer::RGBRenderer(ColorRGB &initColor) : color(initColor)
{
}

FCLIB::RGBRenderer::~RGBRenderer()
{
}

void FCLIB::RGBRenderer::draw(LedStrip &strip, LedOp_t op)
{
    strip.fill(this->color, op);
}

FCLIB::HSVRenderer::HSVRenderer(uint16 h, uint8 s, uint8 v) : color(h, s, v)
{
}

FCLIB::HSVRenderer::HSVRenderer(const ColorHSV &initColor) : color(initColor)
{
}

FCLIB::HSVRenderer::~HSVRenderer()
{
}

void FCLIB::HSVRenderer::draw(LedStrip &strip, LedOp_t op)
{
    strip.fill(this->color, op);
}