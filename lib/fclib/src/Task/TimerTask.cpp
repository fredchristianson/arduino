#include "fclib/Task.h"

using namespace FCLIB;

namespace FCLIB
{
    TimerTask::TimerTask(TaskCallback callback, long repeatCount) : LoopTask(this), timer(0, TIME_MSECS)
    {
        log.setModuleName("TimerTask");
        this->repeatCount = repeatCount;
        this->status = TASK_WAITING;
        this->callback = callback;
        log.debug("TimerTask 0x%lx %d", this, repeatCount);
    }
    TimerTask::~TimerTask()
    {
    }

    TaskStatus TimerTask::updateStatus()
    {
        log.never("Update status 0x%lx %ld %ld", this, repeatCount, FCLIB_REPEAT_FOREVER);
        if (repeatCount < 1 && repeatCount != FCLIB_REPEAT_FOREVER)
        {
            log.never("complete");
            status = TASK_COMPLETE;
        }
        else
        {
            if (timer.isComplete())
            {
                log.never("ready");
                status = TASK_READY;
            }
            else
            {
                log.never("Waiting");
                status = TASK_WAITING;
            }
        }
        return status;
    }

    void TimerTask::doTask()
    {
        if ((repeatCount > 0 || repeatCount == FCLIB_REPEAT_FOREVER) && timer.isComplete())
        {
            log.debug("Timer triggered");
            callback();
            if (repeatCount != FCLIB_REPEAT_FOREVER)
            {
                repeatCount -= 1;
            }
            timer.reset();
        }
    }

    TimerTask *TimerTask::delaySeconds(int seconds)
    {
        timer.seconds(seconds);
        log.never("timer %ld %ld %ld", this->timer.getDurationMsecs(), this->timer.getStartTimeMsecs(), this->timer.currentTimeMsecs());
        return this;
    }
    TimerTask *TimerTask::delayMinutes(int minutes)
    {
        timer.minutes(minutes);
        return this;
    }
    TimerTask *TimerTask::delayMsecs(int msecs)
    {
        timer.msecs(msecs);
        return this;
    }

    OneTimeTask::OneTimeTask(TaskCallback callback) : TimerTask(callback, 1)
    {
        log.setModuleName("OneTimeTask");
        log.debug("OneTimeTask created 0x%lx", this);
    }
    OneTimeTask::~OneTimeTask() {}

    RepeatingTask::RepeatingTask(TaskCallback callback, long repeatCount) : TimerTask(callback, repeatCount)
    {
        log.setModuleName("RepeatingTask");
        log.debug("Task created 0x%lx", this);
    }
    RepeatingTask::~RepeatingTask() {}

    TimerTask *Task::once(TaskCallback callback)
    {
        return new OneTimeTask(callback);
    }
    TimerTask *Task::repeat(TaskCallback callback, long repeatCount)
    {
        return new RepeatingTask(callback, repeatCount);
    }
}