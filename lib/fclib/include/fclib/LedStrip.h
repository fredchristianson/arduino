#ifndef _FCLIB_LED_H_
#define _FCLIB_LED_H_
#include <stdint.h>
#include <Adafruit_NeoPixel.h>
#include "./List.h"
#include "./Logging.h"
#include "./Color.h"

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
        NeoPixelStrip(uint8_t pin = -1, uint16 count = 0, uint8 brightness = 100);
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
        uint32 getNeoPixelColorFromRGB(const Color::RGB &color) const;
        uint32 getNeoPixelColorFromHSV(const Color::HSV &color) const;
        uint32 getNeoPixelColorFromTemp(const Color::Temp &color) const;
        uint8_t pin;
        uint8 brightness; // 0 - 255
        Adafruit_NeoPixel *controller;
    };

    class NullStrip : public LedStrip
    {
    public:
        static NullStrip *get();
        NullStrip() {}
        virtual ~NullStrip() {}

        virtual void set(int pos, const Color &color, LedOp_t op = DEFOP) {}

        virtual void show() override {}

        virtual void clear() override {}
        virtual void fill(const Color &color, LedOp_t op = DEFOP) override {}

    private:
        static NullStrip *singleton;
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
        void show() override;

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

        virtual Color modifyColor(int pos, const Color &origColor, LedOp_t op);

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

    class MirrorStrip : public LedStrip
    {
    public:
        MirrorStrip(LedStrip *orig = NULL, LedStrip *copy = NULL);
        virtual ~MirrorStrip();

        virtual void set(int pos, const Color &color, LedOp_t op = DEFOP);
        MirrorStrip &setOrig(LedStrip *orig);
        MirrorStrip &setCopy(LedStrip *copy);
        void show() override;
        virtual void clear();
        virtual void fill(const Color &color, LedOp_t op = DEFOP);

    protected:
        LedStrip *orig;
        LedStrip *copy;
    };
}
#endif //_FCLIB_LED_H_