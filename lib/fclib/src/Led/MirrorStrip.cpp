#include "fclib/LedStrip.h"

using namespace FCLIB;

FCLIB::MirrorStrip::MirrorStrip(LedStrip *orig, LedStrip *copy)
{
    LOG.setModuleName("MirrorStrip");
    if (orig == NULL)
    {
        this->orig = NullStrip::get();
    }
    else
    {
        this->orig = orig;
    }
    if (copy == NULL)
    {
        this->copy = NullStrip::get();
    }
    else
    {
        this->copy = copy;
    }
    this->setLength(this->orig->length());
}

FCLIB::MirrorStrip::~MirrorStrip()
{
}

void FCLIB::MirrorStrip::set(int pos, const Color &color, LedOp_t op)
{
    orig->set(pos, color, op);

    if (copy->length() > 0)
    {
        float ratio = 1.0 * copy->length() / orig->length();
        float start = ratio * pos;
        float end = ratio * (pos + 1);
        for (int i = start; i < end; i++)
        {
            copy->set(i, color, op);
        }
    }
}
void FCLIB::MirrorStrip::clear()
{
    orig->clear();
    copy->clear();
}
void FCLIB::MirrorStrip::fill(const Color &color, LedOp_t op)
{
    orig->fill(color, op);
    copy->fill(color, op);
}
MirrorStrip &FCLIB::MirrorStrip::setOrig(LedStrip *base)
{
    if (base == NULL)
    {
        orig = NullStrip::get();
    }
    else
    {
        orig = base;
    }
    setLength(orig->length());
    return *this;
}
MirrorStrip &FCLIB::MirrorStrip::setCopy(LedStrip *base)
{
    if (base == NULL)
    {
        copy = NullStrip::get();
    }
    else
    {
        copy = base;
    }
    return *this;
}
void FCLIB::MirrorStrip::show()
{
    orig->show();
    copy->show();
}
