#ifndef _FCLIB_RENDERER_H_
#define _FCLIB_RENDERER_H_

#include "fclib/Logging.h"
#include "fclib/LedStrip.h"
#include "fclib/Util.h"
#include "fclib/Task.h"
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
        virtual uint count() const;
        virtual const Color &color() const = 0;
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
        virtual const Color &color() const;

    protected:
        ColorRGB elementColor;
    };

    class PatternElementHSV : public PatternElement
    {
    public:
        PatternElementHSV(ColorHSV &color, uint count, PositionUnit unit);
        virtual ~PatternElementHSV();
        virtual const Color &color() const;

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
        List<PatternElement> elements;
        Positioning positioning;
    };

    enum class SceneMode
    {
        MODE_OFF,
        MODE_RGB,
        MODE_HSV,
        MODE_WHITE,
        MODE_TEMPERATURE
    };

    class SceneRenderer
    {
    public:
        SceneRenderer(LedStrip *strip = NULL);
        ~SceneRenderer();
        void setStrip(LedStrip *strip);
        void setBrightness(uint8 brightness);

        void start();
        void stop();
        bool isRunning() { return task != NULL; }
        uint8 getBrightness() { return brightness; }

    protected:
        virtual void runRenderers() = 0;
        Logger log;
        LedStrip *strip;
        virtual void render();
        uint8 brightness;

    private:
        Task *task;
    };

    class HomeAssistantSceneRenderer : public SceneRenderer
    {
    public:
        HomeAssistantSceneRenderer(LedStrip *strip = NULL);
        ~HomeAssistantSceneRenderer();

        void setMode(SceneMode mode);
        void setRGB(const ColorRGB &rgb);
        void setHSV(const ColorHSV &hsv);
        void setTemperature(uint temp);
        void setTransitionMsecs(uint msecs);

        const ColorRGB &getRGB() { return rgb; }

    protected:
        SceneMode mode;
        ColorRGB rgb;
        ColorHSV hsv;
        float temperature; // 0.5 = no adjust.  0, max blue.  1 max red
        virtual void runRenderers();
    };

    class CompositeSceneRenderer : public SceneRenderer
    {
    public:
        CompositeSceneRenderer(LedStrip *strip = NULL);
        ~CompositeSceneRenderer();
        void addRenderer(LedRenderer *renderer);

    protected:
        virtual void runRenderers();
        LedStrip *strip;
        List<LedRenderer> renderers;
        virtual void onLoop();
        LoopTask *task;
    };
}

#endif