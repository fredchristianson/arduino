#include "fclib/Render.h"

using namespace FCLIB;

FCLIB::RGBGradientRenderer::RGBGradientRenderer(const ColorRGB &initStart, const ColorRGB &initEnd)
    : startColor(initStart), endColor(initEnd)
{
}

FCLIB::RGBGradientRenderer::~RGBGradientRenderer()
{
}

void FCLIB::RGBGradientRenderer::draw(LedStrip &strip, LedOp_t op)
{
    uint count = strip.length();
    if (count == 0)
    {
        return;
    }
    float diffR = endColor.red() - startColor.red();
    float diffG = endColor.green() - startColor.green();
    float diffB = endColor.blue() - startColor.blue();
    float stepR = diffR / count;
    float stepG = diffG / count;
    float stepB = diffB / count;
    ColorRGB pixelColor;
    for (uint i = 0; i < count; i++)
    {
        pixelColor.red(startColor.red() + i * stepR);
        pixelColor.green(startColor.green() + i * stepG);
        pixelColor.blue(startColor.blue() + i * stepB);
        strip.set(i, pixelColor, op);
    }
}

FCLIB::HSVGradientRenderer::HSVGradientRenderer(const ColorHSV &initStart, const ColorHSV &initEnd)
    : startColor(initStart), endColor(initEnd)
{
}

FCLIB::HSVGradientRenderer::~HSVGradientRenderer()
{
}

void FCLIB::HSVGradientRenderer::draw(LedStrip &strip, LedOp_t op)
{
    float count = strip.length();
    if (count == 0)
    {
        return;
    }
    LOG.never("Start %d,%d,%d    End %d,%d,%d",
              startColor.hue(), startColor.saturation(), startColor.value(),
              endColor.hue(), endColor.saturation(), endColor.value());

    float diffH = endColor.hue() - startColor.hue();
    float diffS = endColor.saturation() - startColor.saturation();
    float diffV = endColor.value() - startColor.value();
    LOG.never("%f %f %f %f", count, diffH, diffS, diffV);
    float stepH = diffH / count;
    float stepS = diffS / count;
    float stepV = diffV / count;
    ColorHSV pixelColor;
    LOG.never("HSV gradient steps %f %f %f", stepH, stepS, stepV);
    for (uint i = 0; i < count; i++)
    {
        pixelColor.hue(startColor.hue() + i * stepH);
        pixelColor.saturation(startColor.saturation() + i * stepS);
        pixelColor.value(startColor.value() + i * stepV);
        strip.set(i, pixelColor, op);
    }
}