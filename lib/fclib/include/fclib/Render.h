#ifndef _FCLIB_RENDERER_H_
#define _FCLIB_RENDERER_H_

#include "fclib/Logging.h"
#include "fclib/LedStrip.h"
#include "fclib/Util.h"
#include "fclib/Task.h"
#include "fclib/Animation.h"
#include "fclib/Persistance.h"
#include "fclib/Animation.h"
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
        SolidRenderer(const Color &color);
        virtual ~SolidRenderer();
        void draw(LedStrip &strip, LedOp_t op = DEFOP) override;

    protected:
        Color color;
    };

    class GradientRenderer : public LedRenderer
    {
    public:
        GradientRenderer(const Color &startColor, const Color &endColor);
        virtual ~GradientRenderer();

        void draw(LedStrip &strip, LedOp_t op = DEFOP) override;

    protected:
        void drawHSV(Color::HSV start, Color::HSV end, LedStrip &strip, LedOp_t op = DEFOP);
        void drawRGB(Color::RGB start, Color::RGB end, LedStrip &strip, LedOp_t op = DEFOP);
        const Color startColor;
        const Color endColor;
    };

    class PatternElement
    {
    public:
        PatternElement(const Color &color, uint count, PositionUnit unit);
        virtual ~PatternElement();
        virtual uint count() const;
        virtual const Color &color() const { return elementColor; };
        virtual PositionUnit unit();

    protected:
        Color elementColor;
        uint elementCount;
        PositionUnit posUnit;
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
        MODE_ON,
        MODE_TRANSITION
    };

    struct SceneState
    {
    public:
        SceneState();
        SceneState(const SceneState &other);
        SceneState &operator=(const SceneState &other);
        SceneMode mode;
        Color color;
        uint8 brightness;
    };

    struct SceneTransistion
    {
    public:
        SceneTransistion();
        SceneState from;
        SceneState to;
        uint16_t transistionMsecs;
    };

    class SceneRenderer
    {
    public:
        SceneRenderer(LedStrip *strip = NULL);
        ~SceneRenderer();
        void setStrip(LedStrip *strip);
        virtual void setBrightness(uint8 brightness);

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

        void setSceneState(const SceneState &state, uint16 transitionMSecs);
        const SceneState getSceneState();

        void turnOn(int transitionSeconds);
        void turnOff(int transitionSeconds);

    protected:
        SceneState currentState;
        SceneTransistion transition;
        virtual void runRenderers();
        void saveState(SceneState &state);
        void loadState(SceneState &state);
        void startTransition();
        AnimateColor animateColor;
        AnimateInt animateBrightness;
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