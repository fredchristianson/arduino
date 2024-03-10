
#include "fclib/LedStrip.h"
#include "fclib/Timer.h"
#include "fclib/Util.h"

using namespace FCLIB;
using namespace FCLIB::Util;

IntervalTimer *logTimer = IntervalTimer::createByFrequency(5000);
FCLIB::NeoPixelStrip::NeoPixelStrip(uint8_t pin, uint16 count, uint8 brightness) : LedStrip(count)
{
    LOG.setModuleName("NeoPixelStrip");
    this->pin = pin;
    this->brightness = brightness;
    this->controller = new Adafruit_NeoPixel(count, pin, NEO_GRB + NEO_KHZ800);
    this->controller->begin();

    this->clear();
}

FCLIB::NeoPixelStrip::~NeoPixelStrip()
{
}

void FCLIB::NeoPixelStrip::set(int pos, const Color &color, LedOp_t op)
{
    uint32 neoColor = this->getNeoPixelColor(color);
    this->controller->setPixelColor(pos, neoColor);
}

void FCLIB::NeoPixelStrip::show()
{
    this->controller->show();
}

void FCLIB::NeoPixelStrip::setBrightness(uint8 b)
{
    this->brightness = b;
    this->controller->setBrightness(b);
}

void FCLIB::NeoPixelStrip::clear()
{
    this->controller->clear();
    this->controller->setBrightness(this->brightness);
}

void FCLIB::NeoPixelStrip::fill(const Color &color, LedOp_t op)
{
    // todo: use op
    uint32 neoColor = this->getNeoPixelColor(color);
    LOG.debug("Neo color %d %d", color.isHsv(), color.isRgb());
    this->controller->fill(neoColor, 0, this->count);
}

uint32 FCLIB::NeoPixelStrip::getNeoPixelColor(const Color &color) const
{
    if (color.isHsv())
    {
        return this->getNeoPixelColorFromHsv((ColorHSV &)color);
    }
    else if (color.isRgb())
    {
        return this->getNeoPixelColorFromRgb((ColorRGB &)color);
    }
    LOG.error("unknown color type %d %d", color.isHsv(), color.isRgb());
    return 0;
}

uint32 FCLIB::NeoPixelStrip::getNeoPixelColorFromRgb(const ColorRGB &color) const
{

    return this->controller->Color(color.red(), color.green(), color.blue());
}

uint32 FCLIB::NeoPixelStrip::getNeoPixelColorFromHsv(const ColorHSV &color) const
{
    // todo: convert to rainbow instead of spectrum RGB.
    LOG.debug("HSV %d %d %d", color.hue(), color.saturation(), color.value());
    uint16 hue = 65535.0 * (color.hue() % 360) / 360.0; // convert ColorHSV value (0-360) to NeoPixel range
    uint sat = ((uint)color.saturation() * 255) / 100;  // convert 0-100% to (0-255)
    uint value = ((uint)color.value() * 255) / 100;     // convert 0-100% to (0-255)
    uint32 neoColor = this->controller->ColorHSV(hue, sat, value);
    LOG.debug("HSV %d %d %d  => %lx", hue, sat, value, neoColor);
    return neoColor;
}
