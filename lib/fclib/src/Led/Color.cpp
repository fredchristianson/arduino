#include "fclib/LedStrip.h"
#include "fclib/Logging.h"
#include "fclib/Util.h"

using namespace FCLIB;
using namespace FCLIB::Util;

namespace FCLIB
{
    Color Color::fromRGB(uint8 red, uint8 green, uint8 blue)
    {
        RGB rgb(red, green, blue);
        return Color(rgb);
    }
    Color Color::fromHSV(uint8 hue, uint8 sat, uint8 value)
    {
        HSV hsv(hue, sat, value);
        return Color(hsv);
    }
    Color Color::fromMired(uint16 mireds)
    {
        Temp temp(mireds);
        return Color(temp);
    }
    Color Color::fromKelvin(uint16 kelvin)
    {
        Temp temp(1000000 / kelvin);
        return Color(temp);
    }

    Color::Color(const Color &other)
    {
        this->type = other.type;
        switch (this->type)
        {
        case ColorType::RGB:
            this->rgb = other.rgb;
            break;
        case ColorType::HSV:
            this->hsv = other.hsv;
            break;
        case ColorType::TEMP:
            this->temp = other.temp;
            break;
        }
    }
    Color::Color(const Color::RGB &rgb)
    {
        this->type = ColorType::RGB;
        this->rgb = rgb;
    }
    Color::Color(const Color::HSV &hsv)
    {
        this->type = ColorType::HSV;
        this->hsv = hsv;
    }
    Color::Color(const Color::Temp &temp)
    {
        this->type = ColorType::TEMP;
        this->temp = temp;
    }
    Color::Color()
    {
        this->type = ColorType::RGB;
    }
    Color::~Color() {}

    const Color &Color::operator=(const Color &other)
    {
        this->type = other.type;
        switch (this->type)
        {
        case ColorType::RGB:
            this->rgb = other.rgb;
            break;
        case ColorType::HSV:
            this->hsv = other.hsv;
            break;
        case ColorType::TEMP:
            this->temp = other.temp;
            break;
        }
        return *this;
    }

    // conversions between types are approximate (and maybe just a guess in some cases)
    Color::RGB Color::toRGB() const
    {
        if (this->type == ColorType::RGB)
        {
            return rgb;
        }
        else if (this->type == ColorType::HSV)
        {
            return HSVToRGB(hsv);
        }
        else if (this->type == ColorType::TEMP)
        {
            return TempToRGB(temp);
        }
        else
        {
            Logger log("Color");
            log.error("unknown color type %d", this->type);
            return RGB();
        }
    }
    Color::HSV Color::toHSV() const
    {
        if (this->type == ColorType::RGB)
        {
            return RGBToHSV(rgb);
        }
        else if (this->type == ColorType::HSV)
        {
            return hsv;
        }
        else if (this->type == ColorType::TEMP)
        {
            return TempToHSV(temp);
        }
        else
        {
            Logger log("Color");
            log.error("unknown color type %d", this->type);
            return HSV();
        }
    }
    Color::Temp Color::toTemp() const
    {
        if (this->type == ColorType::RGB)
        {
            return RGBToTemp(rgb);
        }
        else if (this->type == ColorType::HSV)
        {
            return HSVToTemp(hsv);
        }
        else if (this->type == ColorType::TEMP)
        {
            return temp;
        }
        else
        {
            Logger log("Color");
            log.error("unknown color type %d", this->type);
            return Temp();
        }
    }
}

Color::RGB Color::RGB::BLACK(0, 0, 0);
Color::RGB Color::RGB::WHITE(255, 255, 255);
Color::RGB Color::RGB::RED(255, 0, 0);
Color::RGB Color::RGB::GREEN(0, 255, 0);
Color::RGB Color::RGB::BLUE(0, 0, 255);

Color::HSV Color::HSV::BLACK(0, 0, 0);
Color::HSV Color::HSV::WHITE(0, 0, 100);
Color::HSV Color::HSV::FIRST(0, 100, 100);
Color::HSV Color::HSV::LAST(360, 100, 100);
Color::HSV Color::HSV::RED_START(0, 100, 100);
Color::HSV Color::HSV::RED(0, 100, 100);
Color::HSV Color::HSV::ORANGE(30, 100, 100);
Color::HSV Color::HSV::YELLOW(60, 100, 100);
Color::HSV Color::HSV::CHARTREUSE(90, 100, 100);
Color::HSV Color::HSV::GREEN(120, 100, 100);
Color::HSV Color::HSV::SPRING(150, 100, 100);
Color::HSV Color::HSV::CYAN(180, 100, 100);
Color::HSV Color::HSV::AZURE(210, 100, 100);
Color::HSV Color::HSV::BLUE(240, 100, 100);
Color::HSV Color::HSV::VIOLET(270, 100, 100);
Color::HSV Color::HSV::MAGENTA(300, 100, 100);
Color::HSV Color::HSV::ROSE(330, 100, 100);
Color::HSV Color::HSV::RED_END(360, 100, 100);
