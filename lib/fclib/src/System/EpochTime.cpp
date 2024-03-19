#include "fclib/EpochTime.h"

using namespace FCLIB;

namespace FCLIB
{
    EpochTime singletonEpochTime;
    EpochTime &EpochTime::getInstance() { return singletonEpochTime; }
}