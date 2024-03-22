#include "fclib/Task.h"

using namespace FCLIB;

namespace FCLIB
{
    TimerTask::TimerTask(TaskAction *action, long repeatCount) : LoopTask(action), timer(0, TIME_MSECS)
    {
        log.setModuleName("TimerTask");
        this->repeatCount = repeatCount;
        this->status = TASK_WAITING;
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
            action->doTask();
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

    OneTimeTask::OneTimeTask(TaskAction &action) : TimerTask(&action, 1)
    {
        log.setModuleName("OneTimeTask");
        log.debug("OneTimeTask created 0x%lx", this);
    }
    OneTimeTask::~OneTimeTask() {}

    RepeatingTask::RepeatingTask(TaskAction &action, long repeatCount) : TimerTask(&action, repeatCount)
    {
        log.setModuleName("RepeatingTask");
        log.debug("Task created 0x%lx", this);
    }
    RepeatingTask::~RepeatingTask() {}

    TimerTask *Task::once(TaskAction &action) { return new OneTimeTask(action); }
    TimerTask *Task::repeat(TaskAction &action, long count) { return new RepeatingTask(action, count); }
}