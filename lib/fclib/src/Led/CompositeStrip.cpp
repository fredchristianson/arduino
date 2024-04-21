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
        while (idx < (int)strips.size() && pos >= strips[idx]->length())
        {
            pos -= strips[idx]->length();
            idx++;
        }
        if (idx < 0 || idx >= strips.size())
        {
            LOG.warn("composit pos %d ignored. strip count=%d.   Composite length=%d", strips.size(), this->length());
            return;
        }
        strips[idx]->set(pos, color, op);
    }

    void CompositeStrip::clear()
    {
        strips.forEach([](LedStrip *strip)
                       { strip->clear(); });
        LoopTime::check("CompositStrip::clear");
    }

    void CompositeStrip::fill(const Color &color, LedOp_t op)
    {
        for (int i = 0; i < strips.size() && LoopTime::ok(); i++)
        {
            LedStrip *strip = strips[i];
            strip->fill(color, op);
        }
        LoopTime::check("CompositStrip::fill");
    }

    void CompositeStrip::add(LedStrip *component)
    {
        strips.add(component);
        uint16 count = 0;
        for (int i = 0; i < strips.size() && LoopTime::ok(); i++)
        {
            count += strips[i]->length();
        }
        LoopTime::check("CompositStrip::add");

        this->setLength(count);
    }

    void CompositeStrip::setBrightness(uint8 brightness)
    {
        for (int i = 0; i < strips.size() && LoopTime::ok(); i++)
        {
            strips[i]->setBrightness(brightness);
        }
        LoopTime::check("CompositStrip::setBrightness");
    }
    void CompositeStrip::show()
    {
        for (int i = 0; i < strips.size() && LoopTime::ok(); i++)
        {
            strips[i]->show();
        }
        LoopTime::check("CompositStrip::show");
    }
}