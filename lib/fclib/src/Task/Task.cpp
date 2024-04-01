#include "fclib/Task.h"
#include "fclib/Event.h"

using namespace FCLIB;

namespace FCLIB
{

    TimerTask *Task::once(SimpleCallable callback)
    {
        return new OneTimeTask(callback);
    }
    TimerTask *Task::repeat(SimpleCallable callback, long repeatCount)
    {
        return new RepeatingTask(callback, repeatCount);
    }

    TimerTask *Task::once(TaskAction *action)
    {
        return new OneTimeTask(action);
    }
    TimerTask *Task::repeat(TaskAction *action, long repeatCount)
    {
        return new RepeatingTask(action, repeatCount);
    }

    LoopTask *Task::onLoop(TaskAction *action)
    {
        return new LoopTask(action);
    }
    LoopTask *Task::onLoop(SimpleCallable callback)
    {
        return new LoopTask(callback);
    }

    Task::Task(TaskAction *action) : log("Task")
    {
        this->action = action;
        TaskQueue::onTaskCreate(this);
    }

    Task::~Task()
    {
        TaskQueue::onTaskDestroy(this);
    }
}