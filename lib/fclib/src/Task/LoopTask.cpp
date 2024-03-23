#include "fclib/Task.h"
#include "fclib/App.h"

using namespace FCLIB;

namespace FCLIB
{
    LoopTask *LoopTask::create(TaskCallback callback, LoopStage stage, uint8 priority)
    {
        LoopTask *task = new LoopTask(new TaskCallbackAction(callback), stage, priority);
        task->ownAction = true;
        return task;
    }

    LoopTask *LoopTask::create(TaskAction *action, LoopStage stage, uint8 priority)
    {
        return new LoopTask(action, stage, priority);
    }
    LoopTask::LoopTask(TaskAction *action, LoopStage stage, uint8 priority) : TaskBase(action, LOOP_TASK, priority)
    {
        log.setModuleName("LoopTask");
        log.debug("Created LoopTask 0x%lx", this);
        this->stage = stage;
        this->status = TASK_READY;
        AppLoop::addTask(this);
        ownAction = false;
    }

    LoopTask::~LoopTask()
    {
        AppLoop::removeTask(this);
        if (ownAction)
        {
            delete action;
        }
    }
}