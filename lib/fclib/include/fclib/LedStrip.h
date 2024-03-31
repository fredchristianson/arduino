#ifndef _FCLIB_LED_H_
#define _FCLIB_LED_H_
#include <stdint.h>
#include <Adafruit_NeoPixel.h>
#include "./List.h"
#include "./Logging.h"

namespace FCLIB
{
    // modes can be
    enum Positioning : uint16_t
    {
        REPEAT = (1 << 0),
        WRAP = (1 << 1),
        STRETCH = (1 << 2),
        TRUNCATE = (1 << 3),
        COVER = (WRAP + STRETCH)
    };

    enum PositionUnit
    {
        PIXEL,
        PERCENT,
        FR // fraction of remaining space
    };

    typedef enum
    {
        DEFOP, // default operator for the LedStrip
        INHERIT,
        REPLACE,
        ADD,
        SUBTRACT,
        MIN,
        MAX,
        AVG
    } LedOp_t;

    enum ColorType : uint8_t
    {
        RED = (1 << 0),
        GREEN = (1 << 1),
        BLUE = (1 << 2),
        HUE = (1 << 3),
        SATURATION = (1 << 4),
        VALUE = (1 << 5),
        RGB = (RED + GREEN + BLUE),
        HSV = (HUE + SATURATION + VALUE)
    };

    class Color
    {
    public:
        Color(ColorType type);
        ~Color();

        virtual bool isRgb() const;
        virtual bool isHsv() const;

    protected:
        ColorType type;
    };

    class ColorRGB : public Color
    {
    public:
        ColorRGB(uint8 red = 0, uint8 blue = 0, uint8 green = 0);
        ColorRGB(const ColorRGB &other);
        ~ColorRGB();
        uint8 red() const { return r; }

        uint8 green() const { return g; }

        uint8 blue() const { return b; }

        void red(uint8 v) { r = v; }

        void green(uint8 v) { g = v; }

        void blue(uint8 v) { b = v; }

        virtual bool isRgb() const;
        virtual bool isHsv() const;

        static ColorRGB BLACK;
        static ColorRGB WHITE;
        static ColorRGB RED;
        static ColorRGB GREEN;
        static ColorRGB BLUE;

        ColorRGB &operator=(const ColorRGB &other)
        {
            this->r = other.r;
            this->g = other.g;
            this->b = other.b;
            return *this;
        }

    protected:
        uint8 r;
        uint8 g;
        uint8 b;
    };

    class ColorHSV : public Color
    {
    public:
        ColorHSV(uint16 h = 0, uint8 s = 100, uint8 v = 100);
        ColorHSV(const ColorHSV &other);
        ~ColorHSV();
        void dump();
        uint16 hue() const { return h; }       // 0-360: color wheel
        uint8 saturation() const { return s; } // 0-100%
        uint8 value() const { return v; }      // 0-100%
        virtual bool isHsv() const;
        virtual bool isRgb() const;

        void hue(uint16 v) { h = v; }

        void saturation(uint8 v) { s = v; }

        void value(uint8 v) { this->v = v; }
        static ColorHSV BLACK;
        static ColorHSV WHITE;
        static ColorHSV FIRST;
        static ColorHSV LAST;

        static ColorHSV RED;
        static ColorHSV RED_START;
        static ColorHSV RED_END;
        static ColorHSV ORANGE;
        static ColorHSV YELLOW;
        static ColorHSV CHARTREUSE;
        static ColorHSV GREEN;
        static ColorHSV SPRING;
        static ColorHSV CYAN;
        static ColorHSV AZURE;
        static ColorHSV BLUE;
        static ColorHSV VIOLET;
        static ColorHSV MAGENTA;
        static ColorHSV ROSE;

    protected:
        uint16 h;
        uint8 s;
        uint8 v;
    };

    class LedStrip
    {
    public:
        LedStrip(uint16 count = 0);
        virtual ~LedStrip();

        virtual uint16 length();
        virtual void set(int pos, const Color &color, LedOp_t op = DEFOP) = 0;

        virtual void clear() = 0;
        virtual void fill(const Color &color, LedOp_t op = DEFOP) = 0;

        virtual void show() {} // phsyical strips need this.  others may use it
        virtual void setBrightness(uint8 level);
        virtual void setCount(int newCount);

    protected:
        virtual void onCountChange(uint16 oldCount, uint16 newCount) {}
        virtual void setLength(uint16 len);

        Logger LOG;
        uint16_t count;
    };

    class NeoPixelStrip : public LedStrip
    {
    public:
        NeoPixelStrip(uint8_t pin, uint16 count, uint8 brightness = 100);
        virtual ~NeoPixelStrip();

        virtual void set(int pos, const Color &color, LedOp_t op = DEFOP);

        virtual void show() override;

        virtual void clear() override;
        virtual void fill(const Color &color, LedOp_t op = DEFOP) override;

        void setBrightness(uint8 level) override;
        void setPin(uint8_t pin);

    protected:
        virtual void onCountChange(uint16 oldCount, uint16 newCount);

        uint32 getNeoPixelColor(const Color &color) const;
        uint32 getNeoPixelColorFromRgb(const ColorRGB &color) const;
        uint32 getNeoPixelColorFromHsv(const ColorHSV &color) const;
        uint8_t pin;
        uint8 brightness; // 0 - 255
        Adafruit_NeoPixel *controller;
    };

    class CompositeStrip : public LedStrip
    {
    public:
        CompositeStrip();
        virtual ~CompositeStrip();

        virtual void set(int pos, const Color &color, LedOp_t op = DEFOP);

        virtual void clear();
        virtual void fill(const Color &color, LedOp_t op = DEFOP);

        void add(LedStrip *component);
        void setBrightness(uint8);

    private:
        List<LedStrip> strips;
    };

    class StripModifier : public LedStrip
    {
    public:
        StripModifier(LedStrip *baseStrip = NULL);
        virtual ~StripModifier();

        virtual void set(int pos, const Color &color, LedOp_t op = DEFOP);

        virtual void clear();
        virtual void fill(const Color &color, LedOp_t op = DEFOP);

        virtual void setBase(LedStrip *base);
        void setBrightness(uint8);

    protected:
        LedStrip *baseStrip;

        virtual const ColorHSV &modifyColor(int pos, const ColorHSV &origColor, ColorHSV &modifiedColor, LedOp_t op);
        virtual const ColorRGB &modifyColor(int pos, const ColorRGB &origColor, ColorRGB &modifiedColor, LedOp_t op);

        virtual int modifyPosition(int pos);
        virtual bool isValidPosition(int pos);   // based on original position
        virtual bool isValidColor(Color &color); // based on original color
    };

    class StripLoop : public StripModifier
    {
    public:
        StripLoop(LedStrip *base = NULL);

    protected:
        virtual int modifyPosition(int pos);
    };

    class StripOffset : public StripModifier
    {
    public:
        StripOffset(LedStrip *base = NULL, int offset = 0);

        virtual int getOffset();
        virtual void setOffset(int offset);

    protected:
        virtual int modifyPosition(int pos);
        int offset;
    };

    class StripLength : public StripModifier
    {
    public:
        StripLength(LedStrip *base = NULL, uint length = 0);

        virtual int getLength();
        virtual void setLength(int length);

    protected:
        virtual bool isValidPosition(int pos); // based on original position
    };

    class StripSegment : public StripModifier
    {
    public:
        StripSegment(LedStrip *base = NULL, int offset = 0, uint length = 0);

        virtual int getLength();
        virtual void setLength(int length);

        virtual int getOffset();
        virtual void setOffset(int offset);

    protected:
        int offset;
        virtual bool isValidPosition(int pos); // based on original position
        virtual int modifyPosition(int pos);
    };

    class StripRotate : public StripModifier
    {
    public:
        StripRotate(LedStrip *base = NULL, uint count = 0);

        void setRotation(int count);

    protected:
        virtual int modifyPosition(int pos);

    private:
        int pixelCount;
    };

    class StripMirror : public StripModifier
    {
    public:
        StripMirror(LedStrip *base = NULL);
        virtual ~StripMirror();

        virtual void set(int pos, const Color &color, LedOp_t op = DEFOP);

        virtual void clear();
        virtual void fill(const Color &color, LedOp_t op = DEFOP);

        void setBase(LedStrip *base);

    protected:
    };
}
#endif //_FCLIB_LED_H_