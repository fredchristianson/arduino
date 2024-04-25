#pragma once
#ifndef _FCLIB_COLOR_H_
#define _FCLIB_COLOR_H_
#include <c_types.h>
namespace FCLIB
{
    enum ColorType : uint8_t
    {
        RGB = 0,
        HSV = 1,
        TEMP = 2
    };

    class Color
    {
    public:
        struct RGB
        {
        public:
            RGB(uint8 r = 0, uint8 g = 0, uint8 b = 0)
            {
                this->r = r;
                this->g = g;
                this->b = b;
            }
            RGB(const Color::RGB &other)
            {
                this->r = other.r;
                this->g = other.g;
                this->b = other.b;
            }
            RGB &operator=(const Color::RGB &other)
            {
                this->r = other.r;
                this->g = other.g;
                this->b = other.b;
                return *this;
            }
            uint8 red() const { return r; }

            uint8 green() const { return g; }

            uint8 blue() const { return b; }

            void red(uint8 v) { r = v; }

            void green(uint8 v) { g = v; }

            void blue(uint8 v) { b = v; }
            String toString() const
            {
                String s = "RGB(";
                s.concat(r);
                s.concat(",");
                s.concat(g);
                s.concat(",");
                s.concat(b);
                s.concat(")");
                return s;
            }
            static RGB BLACK;
            static RGB WHITE;
            static RGB RED;
            static RGB GREEN;
            static RGB BLUE;

        private:
            uint8 r;
            uint8 g;
            uint8 b;
        };
        struct HSV
        {
        public:
            HSV(uint16 h = 0, uint8 s = 100, uint8 v = 100)
            {
                this->h = h;
                this->s = s, this->v = v;
            }
            HSV(const HSV &other)
            {
                this->h = other.h;
                this->s = other.s;
                this->v = other.v;
            }

            HSV &operator=(const HSV &other)
            {
                this->h = other.h;
                this->s = other.s;
                this->v = other.v;
                return *this;
            }
            uint16 hue() const { return h; }       // 0-360: color wheel
            uint8 saturation() const { return s; } // 0-100%
            uint8 value() const { return v; }      // 0-100%
            String toString() const
            {
                String s = "HSV(";
                s.concat(h);
                s.concat(",");
                s.concat(s);
                s.concat(",");
                s.concat(v);
                s.concat(")");
                return s;
            }
            void hue(uint16 v) { h = v; }

            void saturation(uint8 v) { s = v; }

            void value(uint8 v) { this->v = v; }
            static HSV BLACK;
            static HSV WHITE;
            static HSV FIRST;
            static HSV LAST;

            static HSV RED;
            static HSV RED_START;
            static HSV RED_END;
            static HSV ORANGE;
            static HSV YELLOW;
            static HSV CHARTREUSE;
            static HSV GREEN;
            static HSV SPRING;
            static HSV CYAN;
            static HSV AZURE;
            static HSV BLUE;
            static HSV VIOLET;
            static HSV MAGENTA;
            static HSV ROSE;

        protected:
            uint16 h;
            uint8 s;
            uint8 v;
        };

        struct Temp
        {
        public:
            Temp(uint16 mireds = 200) { this->m = mireds; }
            Temp(const Temp &other) { this->m = other.m; }
            Temp &operator=(const Temp &other)
            {
                this->m = other.m;
                return *this;
            }
            uint16 kelvin() const { return m == 0 ? 0 : 1000000 / m; };
            uint16 mireds() const { return m; }
            String toString() const
            {
                String s = "Temp(";
                s.concat(m);
                s.concat(")");
                return s;
            }

        protected:
            uint16 m;
        };

    public:
        static Color fromRGB(uint8 red, uint8 green, uint8 blue);
        static Color fromHSV(uint8 hue, uint8 sat, uint8 value);
        static Color fromMired(uint16 mireds);
        static Color fromKelvin(uint16 kelvin);
        static RGB HSVToRGB(const HSV &hsv);
        static Temp HSVToTemp(const HSV &hsv);
        static HSV RGBToHSV(const RGB &rgb);
        static Temp RGBToTemp(const RGB &rgb);
        static RGB TempToRGB(const Temp &temp);
        static HSV TempToHSV(const Temp &temp);

        Color(const Color &other);
        Color(const Color::RGB &rgb);
        Color(const Color::HSV &hsv);
        Color(const Color::Temp &tmp);
        Color();
        ~Color();

        const Color &operator=(const Color &other);
        ColorType getType() const { return type; }

        // conversions between types are approximate (and maybe just a guess in some cases)
        RGB toRGB() const;
        HSV toHSV() const;
        Temp toTemp() const;
        String toString() const;

    protected:
        ColorType type;
        union
        {
            RGB rgb;
            HSV hsv;
            Temp temp;
        };
    };
}
#endif