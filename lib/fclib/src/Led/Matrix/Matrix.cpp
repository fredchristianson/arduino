#include "fclib/Matrix.h"

Matrix::Matrix(uint16 rows, uint16 cols, LedStrip *strip)
{
    this->strip = strip;
    this->rows = rows;
    this->cols = cols;
}

Matrix::~Matrix()
{
}
