#include "fclib/LedStrip.h"
#include "fclib/Logging.h"
#include "fclib/Util.h"

using namespace FCLIB;
using namespace FCLIB::Util;

Logger colorLogger("Color");
#define LOG colorLogger

FCLIB::Color::Color(ColorType type)
{
    this->type = type;
}

FCLIB::Color::~Color()
{
}

bool FCLIB::Color::isRgb() const
{
    return false;
}

bool FCLIB::Color::isHsv() const
{
    return false;
}

FCLIB::ColorRGB::ColorRGB(uint8 r, uint8 g, uint8 b) : Color(RGB)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

FCLIB::ColorRGB::ColorRGB(const ColorRGB &other) : Color(other.type)
{
    this->r = other.r;
    this->g = other.g;
    this->b = other.b;
}

FCLIB::ColorRGB::~ColorRGB()
{
}

bool FCLIB::ColorRGB::isRgb() const
{
    return true;
}

bool FCLIB::ColorRGB::isHsv() const
{
    return false;
}

FCLIB::ColorHSV::ColorHSV(uint16 h, uint8 s, uint8 v) : Color(HSV)
{
    LOG.conditional(h > 360, WARN_LEVEL, "Hue out of range (0-360): %d", h);
    LOG.conditional(s > 100, WARN_LEVEL, "Saturation out of range (0-100): %d", s);
    LOG.conditional(v > 100, WARN_LEVEL, "Value out of range (0-100): %d", v);
    this->h = clamp(h, 0, 360);
    this->s = clamp(s, 0, 100);
    this->v = clamp(v, 0, 100);
}

FCLIB::ColorHSV::ColorHSV(const ColorHSV &other) : Color(other.type)
{
    this->h = other.h;
    this->s = other.s;
    this->v = other.v;
}

void FCLIB::ColorHSV::dump()
{
    LOG.never("HSV: %d, %d, %d", h, s, v);
}

bool FCLIB::ColorHSV::isRgb() const
{
    return false;
}
bool FCLIB::ColorHSV::isHsv() const
{
    return true;
}
FCLIB::ColorHSV::~ColorHSV()
{
}

ColorRGB ColorRGB::BLACK(0, 0, 0);
ColorRGB ColorRGB::WHITE(255, 255, 255);
ColorRGB ColorRGB::RED(255, 0, 0);
ColorRGB ColorRGB::GREEN(0, 255, 0);
ColorRGB ColorRGB::BLUE(0, 0, 255);

ColorHSV ColorHSV::BLACK(0, 0, 0);
ColorHSV ColorHSV::WHITE(0, 0, 100);
ColorHSV ColorHSV::FIRST(0, 100, 100);
ColorHSV ColorHSV::LAST(360, 100, 100);
ColorHSV ColorHSV::RED_START(0, 100, 100);
ColorHSV ColorHSV::RED(0, 100, 100);
ColorHSV ColorHSV::ORANGE(30, 100, 100);
ColorHSV ColorHSV::YELLOW(60, 100, 100);
ColorHSV ColorHSV::CHARTREUSE(90, 100, 100);
ColorHSV ColorHSV::GREEN(120, 100, 100);
ColorHSV ColorHSV::SPRING(150, 100, 100);
ColorHSV ColorHSV::CYAN(180, 100, 100);
ColorHSV ColorHSV::AZURE(210, 100, 100);
ColorHSV ColorHSV::BLUE(240, 100, 100);
ColorHSV ColorHSV::VIOLET(270, 100, 100);
ColorHSV ColorHSV::MAGENTA(300, 100, 100);
ColorHSV ColorHSV::ROSE(330, 100, 100);
ColorHSV ColorHSV::RED_END(360, 100, 100);
