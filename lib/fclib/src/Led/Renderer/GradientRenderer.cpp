#include "fclib/Render.h"

using namespace FCLIB;

FCLIB::GradientRenderer::GradientRenderer(const Color &initStart, const Color &initEnd)
    : startColor(initStart), endColor(initEnd)
{
}

FCLIB::GradientRenderer::~GradientRenderer()
{
}

void FCLIB::GradientRenderer::draw(LedStrip &strip, LedOp_t op)
{
    if (startColor.getType() == ColorType::HSV)
    {
        Color::HSV start = startColor.toHSV();
        Color::HSV end = endColor.toHSV();
        drawHSV(start, end, strip, op);
    }
    else
    {
        Color::RGB start = startColor.toRGB();
        Color::RGB end = endColor.toRGB();
        drawRGB(start, end, strip, op);
    }
}

void FCLIB::GradientRenderer::drawRGB(Color::RGB start, Color::RGB end, LedStrip &strip, LedOp_t op)
{
    uint count = strip.length();
    if (count == 0)
    {
        return;
    }
    float diffR = end.red() - start.red();
    float diffG = end.green() - start.green();
    float diffB = end.blue() - start.blue();
    float stepR = diffR / count;
    float stepG = diffG / count;
    float stepB = diffB / count;
    Color::RGB pixel;
    for (uint i = 0; i < count; i++)
    {
        pixel.red(start.red() + i * stepR);
        pixel.green(start.green() + i * stepG);
        pixel.blue(start.blue() + i * stepB);
        Color color(pixel);
        strip.set(i, color, op);
    }
}

void FCLIB::GradientRenderer::drawHSV(Color::HSV start, Color::HSV end, LedStrip &strip, LedOp_t op)
{
    float count = strip.length();
    if (count == 0)
    {
        return;
    }
    LOG.never("Start %d,%d,%d    End %d,%d,%d",
              start.hue(), start.saturation(), start.value(),
              end.hue(), end.saturation(), end.value());

    float diffH = end.hue() - start.hue();
    float diffS = end.saturation() - start.saturation();
    float diffV = end.value() - start.value();
    LOG.never("%f %f %f %f", count, diffH, diffS, diffV);
    float stepH = diffH / count;
    float stepS = diffS / count;
    float stepV = diffV / count;
    Color::HSV pixel;
    LOG.never("HSV gradient steps %f %f %f", stepH, stepS, stepV);
    for (uint i = 0; i < count; i++)
    {
        pixel.hue(start.hue() + i * stepH);
        pixel.saturation(start.saturation() + i * stepS);
        pixel.value(start.value() + i * stepV);
        Color color(pixel);
        strip.set(i, color, op);
    }
}