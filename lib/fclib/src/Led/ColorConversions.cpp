#include "fclib/LedStrip.h"
#include "fclib/Logging.h"
#include "fclib/Util.h"

using namespace FCLIB;
using namespace FCLIB::Util;

namespace FCLIB
{
    /* These are allo approximate conversions.  Trying to get something close  in
    cases where there is a need to convert color types.  Primarily needed when source
    color is HSV or Temp because led strips need rgb.

    Conversions can be done other ways: NeoPixelStrip uses the neopixel HSV-to-RGB*/
    Color::RGB Color::HSVToRGB(const HSV &hsv)
    { // Clamp HSV values to valid ranges
        double H = hsv.hue() % 360;
        double S = hsv.saturation() / 100.0;
        double V = hsv.value();
        H = H < 0 ? 0 : H;
        S = S < 0 ? 0 : S > 1 ? 1
                              : S;
        V = V < 0 ? 0 : V > 1 ? 1
                              : V;

        double C = V * S;
        double X = C * (1 - std::abs(fmod(H / 60.0, 2) - 1));
        double m = V - C;

        double r, g, b;
        if (H >= 0 && H < 60)
        {
            r = C + m;
            g = X + m;
            b = m;
        }
        else if (H >= 60 && H < 120)
        {
            r = X + m;
            g = C + m;
            b = m;
        }
        else if (H >= 120 && H < 180)
        {
            r = m;
            g = C + m;
            b = X + m;
        }
        else if (H >= 180 && H < 240)
        {
            r = m;
            g = X + m;
            b = C + m;
        }
        else if (H >= 240 && H < 300)
        {
            r = X + m;
            g = m;
            b = C + m;
        }
        else
        {
            r = C + m;
            g = m;
            b = X + m;
        }

        return RGB(r, g, b);
    }
}
Color::Temp Color::HSVToTemp(const HSV &hsv)
{
    return RGBToTemp(HSVToRGB(hsv));
}
Color::HSV Color::RGBToHSV(const RGB &rgb)
{
    // Normalize RGB values (0.0 - 1.0)
    double rf = rgb.red() / 255.0;
    double gf = rgb.green() / 255.0;
    double bf = rgb.blue() / 255.0;

    // Find the maximum and minimum RGB values
    double max_value = std::max({rf, gf, bf});
    double min_value = std::min({rf, gf, bf});

    // Calculate Value (V)
    double V = max_value;

    // Calculate Saturation (S) using a simple approach
    double S = 0;
    if (V > 0.0)
    {
        S = 1.0 - min_value / max_value;
    }

    // Calculate approximate Hue (H) based on dominant color component
    double H;
    if (max_value == rf)
    {
        H = 60.0 * (gf - bf) / (max_value - min_value) + 360.0;
    }
    else if (max_value == gf)
    {
        H = 60.0 * (bf - rf) / (max_value - min_value) + 120.0;
    }
    else
    {
        H = 60.0 * (rf - gf) / (max_value - min_value) + 240.0;
    }

    // Wrap Hue around the circle (0-360 degrees)
    H = fmod(H, 360.0);

    return Color::HSV(H, S, V);
}

// approximate RGB values for kelvin 2000-6500 (mired 500-150)
const Color::RGB approxMiredToColorRGB[8] = {

    Color::RGB(0xd6, 0xdf, 0xff),
    Color::RGB(0xe2, 0xe7, 0xff),
    Color::RGB(0xf4, 0xf2, 0xff),
    Color::RGB(0xff, 0xf3, 0xf0),
    Color::RGB(0xff, 0xe6, 0xcf),
    Color::RGB(0xff, 0xd3, 0xa5),
    Color::RGB(0xff, 0xb8, 0x6f),
    Color::RGB(0xff, 0x8c, 0x19)

};

int rgbDiff(const Color::RGB &a, const Color::RGB &b)
{
    int diff = abs((a.red() - b.red())) + abs((a.green() - b.green())) + abs((a.blue() - b.blue()));
    return diff;
}

Color::Temp Color::RGBToTemp(const RGB &rgb)
{
    const Color::RGB *match = &approxMiredToColorRGB[0];
    int diff = rgbDiff(rgb, *match);
    int idx = 0;
    while (idx < 7 && rgbDiff(rgb, approxMiredToColorRGB[idx + 1]) < diff)
    {
        idx++;
        diff = rgbDiff(rgb, approxMiredToColorRGB[idx + 1]);
    }
    return idx * 43 + 150;
}

Color::RGB Color::TempToRGB(const Temp &temp)
{
    const Color::RGB *rgb = &approxMiredToColorRGB[0];
    uint16 mired = temp.mireds();
    if (mired < 183)
    {
        rgb = &approxMiredToColorRGB[0];
    }
    else if (mired > 460)
    {
        rgb = &approxMiredToColorRGB[7];
    }
    else
    {
        uint16 pos = mired - 140;
        uint16 span = 45;
        uint16 idx = round(1.0 * pos / span);
        // todo: find gradient between entries
        rgb = &approxMiredToColorRGB[idx];
    }
    return *rgb;
}
Color::HSV Color::TempToHSV(const Temp &temp)
{
    return RGBToHSV(TempToRGB(temp));
}