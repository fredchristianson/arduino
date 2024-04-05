
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
    LOG.debug("Neo color %d %x", count, neoColor);
    this->controller->fill(neoColor, 0, this->count);
}

uint32 FCLIB::NeoPixelStrip::getNeoPixelColor(const Color &color) const
{
    if (color.getType() == ColorType::HSV)
    {
        return this->getNeoPixelColorFromHSV(color.toHSV());
    }
    else if (color.getType() == ColorType::RGB)
    {
        return this->getNeoPixelColorFromRGB(color.toRGB());
    }
    else if (color.getType() == ColorType::TEMP)
    {
        return this->getNeoPixelColorFromTemp(color.toTemp());
    }
    return 0;
}

uint32 FCLIB::NeoPixelStrip::getNeoPixelColorFromRGB(const Color::RGB &color) const
{

    return this->controller->Color(color.red(), color.green(), color.blue());
}

uint32 FCLIB::NeoPixelStrip::getNeoPixelColorFromHSV(const Color::HSV &color) const
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

uint32 FCLIB::NeoPixelStrip::getNeoPixelColorFromTemp(const Color::Temp &color) const
{
    Color::RGB rgb = Color::TempToRGB(color);
    uint32 neoColor = this->controller->Color(rgb.red(), rgb.green(), rgb.blue());
    return neoColor;
}

void FCLIB::NeoPixelStrip::onCountChange(uint16 oldCount, uint16 newCount)
{
    controller->clear();
    controller->show();
    controller->updateLength(newCount);
}

void FCLIB::NeoPixelStrip::setPin(uint8_t pin)
{
    LOG.info("Pin change to %d", pin);
    controller->clear();
    controller->show();
    this->pin = pin;
    this->controller->setPin(pin);
    LOG.info("\tPin changed", pin);
}
