#ifndef _FCLIB_MATRIX_H_
#define _FCLIB_MATRIX_H_
#include <stdint.h>
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include "./LinkedList.h"
#include "./Logging.h"
#include "./LedStrip.h"

using namespace FCLIB;

namespace FCLIB
{
    class Matrix
    {
    public:
        Matrix(uint16 rows, uint16 cols, LedStrip *strip);
        virtual ~Matrix();

    protected:
        LedStrip *strip;
        uint16 rows;
        uint16 cols;
    };

}
#endif //_FCLIB_MATRIX_H_