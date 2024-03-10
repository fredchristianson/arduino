#include "fclib/LedStrip.h"
using namespace FCLIB;

FCLIB::StripRotate::StripRotate(LedStrip *base, uint count)
{
}

void FCLIB::StripRotate::setRotation(int count)
{
    this->pixelCount = count;
}

int FCLIB::StripRotate::modifyPosition(int pos)
{
    if (baseStrip == NULL || baseStrip->length() == 0)
    {
        return 0;
    }
    return (pos + pixelCount) % baseStrip->length();
}