#include "fclib/Animation.h"
#include "fclib/System.h"

using namespace FCLIB;
namespace FCLIB
{
    AnimateColor::AnimateColor(float start, float end, Calculate<float> easing) : AnimationBase(start, end, easing)
    {
    }

    bool AnimateColor::update()
    {
        bool change = false;
        if (from.getType() == ColorType::RGB)
        {
            Color::RGB frgb = from.toRGB();
            Color::RGB trgb = to.toRGB();
            Color::RGB crgb = current.toRGB();
            float r = animator.calcCurrent(frgb.red(), trgb.red());
            float g = animator.calcCurrent(frgb.green(), trgb.green());
            float b = animator.calcCurrent(frgb.blue(), trgb.blue());
            current = Color::fromRGB(r, g, b);
            change = r != crgb.red() || g != crgb.green() || b != crgb.blue();
        }
        else if (from.getType() == ColorType::HSV)
        {
            Color::HSV fHSV = from.toHSV();
            Color::HSV tHSV = to.toHSV();
            Color::HSV cHSV = current.toHSV();
            float h = animator.calcCurrent(fHSV.hue(), tHSV.hue());
            float s = animator.calcCurrent(fHSV.saturation(), tHSV.saturation());
            float v = animator.calcCurrent(fHSV.value(), tHSV.value());
            current = Color::fromHSV(h, s, v);
            change = cHSV.hue() != h || cHSV.saturation() != s || cHSV.value() != v;
        }
        else if (from.getType() == ColorType::TEMP)
        {
            Color::Temp ftemp = from.toTemp();
            Color::Temp ttemp = to.toTemp();
            Color::Temp old = current.toTemp();
            float m = animator.calcCurrent(ftemp.mireds(), ttemp.mireds());
            current = Color::fromMired(m);
            change = old.mireds() != m;
        }
        if (change)
        {
            callback(current);
        }
        if (animator.isComplete() && doneCallback)
        {
            doneCallback();
            task->end();
            task = NULL;
        }
        return animator.isComplete();
    }
}