#ifndef _FCLIB_RENDERER_H_
#define _FCLIB_RENDERER_H_

#include "./Logging.h"
#include "./LedStrip.h"
#include "./Util.h"
#include <stdint.h>

using namespace FCLIB;

namespace FCLIB
{

    class LedRenderer
    {
    public:
        LedRenderer();
        virtual ~LedRenderer();

        virtual void draw(LedStrip &strip, LedOp_t op = DEFOP) = 0;

    protected:
        Logger LOG;
    };

    class SolidRenderer : public LedRenderer
    {
    public:
        SolidRenderer() {}
        virtual ~SolidRenderer() {}

    protected:
    };

    class RGBRenderer : public SolidRenderer
    {
    public:
        RGBRenderer(uint8 r, uint8 g, uint8 b);
        RGBRenderer(ColorRGB &initColor = ColorRGB::BLACK);
        virtual ~RGBRenderer();
        virtual void draw(LedStrip &strip, LedOp_t op = DEFOP) override;

    protected:
        ColorRGB color;
    };

    class HSVRenderer : public SolidRenderer
    {
    public:
        HSVRenderer(uint16 h, uint8 s, uint8 v);
        HSVRenderer(const ColorHSV &initColor);
        virtual ~HSVRenderer();
        virtual void draw(LedStrip &strip, LedOp_t op = DEFOP) override;

    protected:
        ColorHSV color;
    };

    class RGBGradientRenderer : public LedRenderer
    {
    public:
        RGBGradientRenderer(const ColorRGB &startColor, const ColorRGB &endColor);
        virtual ~RGBGradientRenderer();

        virtual void draw(LedStrip &strip, LedOp_t op = DEFOP) override;

    protected:
        const ColorRGB startColor;
        const ColorRGB endColor;
    };

    class HSVGradientRenderer : public LedRenderer
    {
    public:
        HSVGradientRenderer(const ColorHSV &startColor, const ColorHSV &endColor);
        virtual ~HSVGradientRenderer();

        virtual void draw(LedStrip &strip, LedOp_t op = DEFOP) override;

    protected:
        const ColorHSV startColor;
        const ColorHSV endColor;
    };

    class PatternElement
    {
    public:
        PatternElement(uint count, PositionUnit unit);
        virtual ~PatternElement();
        virtual uint count();
        virtual Color &color() = 0;
        virtual PositionUnit unit();

    protected:
        uint elementCount;
        PositionUnit posUnit;
    };

    class PatternElementRGB : public PatternElement
    {
    public:
        PatternElementRGB(ColorRGB &color, uint count, PositionUnit unit);
        virtual ~PatternElementRGB();
        virtual Color &color();

    protected:
        ColorRGB elementColor;
    };

    class PatternElementHSV : public PatternElement
    {
    public:
        PatternElementHSV(ColorHSV &color, uint count, PositionUnit unit);
        virtual ~PatternElementHSV();
        virtual Color &color();

    protected:
        ColorHSV elementColor;
    };

    class PatternRenderer : public LedRenderer
    {
    public:
        PatternRenderer(Positioning positioning = REPEAT);
        virtual ~PatternRenderer();

        PatternRenderer &addElement(uint count, Color &color, PositionUnit unit = PIXEL);
        virtual void draw(LedStrip &strip, LedOp_t op = DEFOP) override;

    protected:
        LinkedList<PatternElement *> elements;
        Positioning positioning;
    };
}

#endif