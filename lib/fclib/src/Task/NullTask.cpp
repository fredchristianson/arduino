#include "fclib/Task.h"

using namespace FCLIB;

namespace FCLIB
{
    NullTask::NullTask() : TaskBase(NULL, NULL_TASK, 0)
    {
    }

    NullTask::~NullTask()
    {
    }

}