#include "fclib/LedStrip.h"
#include "fclib/Timer.h"
#include "fclib/Util.h"

using namespace FCLIB;
using namespace FCLIB::Util;

namespace FCLIB
{
    CompositeStrip::CompositeStrip() : LedStrip(0)
    {
        LOG.setModuleName("CompositStrip");
    }

    CompositeStrip::~CompositeStrip()
    {
    }

    void CompositeStrip::set(int compositePos, const Color &color, LedOp_t op)
    {
        uint idx = 0;
        int pos = compositePos;
        while (idx < (int)strips.size() && pos >= strips.get(idx)->length())
        {
            pos -= strips.get(idx)->length();
            idx++;
        }
        if (idx < 0 || idx >= strips.size())
        {
            LOG.warn("composit pos %d ignored. strip count=%d.   Composite length=%d", strips.size(), this->length());
            return;
        }
        strips.get(idx)->set(pos, color, op);
    }

    void CompositeStrip::clear()
    {
        forEach<LedStrip *>(strips, [](LedStrip *&strip)
                            { strip->clear(); });
    }

    void CompositeStrip::fill(const Color &color, LedOp_t op)
    {
        LOG.debug("fill color %d %d", color.isHsv(), color.isRgb());
        Logger *l = &LOG;
        for (int i = 0; i < strips.size(); i++)
        {
            LedStrip *strip = strips[i];
            l->debug("Strip %lx %d %d", strip, color.isHsv(), color.isRgb());
            strip->fill(color, op);
        }
    }

    void CompositeStrip::add(LedStrip *component)
    {
        strips.add(component);
        this->setLength(sum(strips, [](LedStrip *s)
                            { return s->length(); }));
        LOG.never("Composite length %d", this->length());
    }

    void CompositeStrip::setBrightness(uint8 brightness)
    {
        for (int i = 0; i < strips.size(); i++)
        {
            strips[i]->setBrightness(brightness);
        }
    }
}