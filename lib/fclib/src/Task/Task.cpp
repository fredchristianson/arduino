#include "fclib/Task.h"

using namespace FCLIB;

namespace FCLIB
{
    TaskBase::TaskBase(TaskAction *action, TaskType type, uint8 priority) : log("TaskBase")
    {
        this->action = action;
        this->taskType = type;
        this->priority = priority;
        next = NULL;
        prev = NULL;
        log.debug("TaskBase: 0x%lx 0x%lx %d %d", this, this->action, this->taskType, this->priority);
    }

    TaskBase::~TaskBase()
    {
    }

}