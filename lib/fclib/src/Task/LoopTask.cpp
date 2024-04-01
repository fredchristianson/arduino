#include "fclib/Task.h"
#include "fclib/App.h"

using namespace FCLIB;

namespace FCLIB
{
    LoopTask::LoopTask(TaskAction *action) : Task(action)
    {
        log.setModuleName("LoopTask");
        this->status = TASK_READY;
        this->action = action;
        this->callback = NULL;
    }

    LoopTask::LoopTask(SimpleCallable callable) : Task(this)
    {
        log.setModuleName("LoopTask");
        this->status = TASK_READY;
        this->action = NULL;
        this->callback = callable;
    }

    LoopTask::~LoopTask()
    {
    }

    void LoopTask::doTask()
    {
        if (this->action != NULL)
        {
            this->action->doTask();
        }
        else if (this->callback)
        {
            this->callback();
        }
    }
}