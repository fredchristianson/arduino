#include "fclib/System.h"
#include "fclib/Task.h"

using namespace FCLIB;

namespace FCLIB
{
    TaskQueue *TaskQueue::singleton;

    void TaskQueue::configure(Config *config)
    {
        // nothing to do.  but should be called to create singleton
        get();
    }

    TaskQueue *TaskQueue::get()
    {
        if (singleton == NULL)
        {
            singleton = new TaskQueue();
        }
        return singleton;
    }

    void TaskQueue::destroy()
    {
        if (singleton != NULL)
        {
            delete singleton;
        }
        singleton = NULL;
    }
    void TaskQueue::process()
    {
        if (singleton == NULL)
        {
            singleton = new TaskQueue();
        }
        singleton->runTasks();
    }

    TaskQueue::TaskQueue() : log("TaskQueue")
    {
        singleton = this;
    }

    void TaskQueue::onTaskCreate(Task *task)
    {
        if (singleton != NULL)
        {
            singleton->add(task);
        }
    }
    void TaskQueue::onTaskDestroy(Task *task)
    {
        if (singleton != NULL)
        {

            singleton->remove(task);
        }
    }

    TaskQueue::~TaskQueue()
    {
        singleton = NULL;
        log.never("delete TaskQueue size=%d", tasks.size());
        tasks.forEach([this](Task *task)
                      { 
        log.never("delete queued Task");
                        delete task; 
                        log.never("task deleted"); });
        log.never("queue deleted");
    }

    void TaskQueue::add(Task *task)
    {
        tasks.add(task);
    }

    void TaskQueue::remove(Task *task)
    {
        tasks.remove(task);
    }

    int maxQueueTime = 0;
    void TaskQueue::runTasks()
    {
        int startTime = THE_BOARD->currentMsecs();
        int cnt = tasks.size();
        for (int i = 0; i < tasks.size(); i++)
        {
            Task *task = tasks[i];
            log.never("Check task %d %x", i, task);

            TaskStatus status = task->updateStatus();
            log.never("\tstatus %d", status);
            if (status == TASK_READY)
            {
                log.never("Run task 0x%lx", task);
                task->doTask();
            }
            else if (status == TASK_COMPLETE)
            {
                log.never("delete task %x", task);
                tasks.remove(task);
                delete task;
                log.never("deleted.  size=%d", tasks.size());
                i--;
            }
        }
        int endTime = THE_BOARD->currentMsecs();
        int diff = endTime - startTime;
        if (diff > maxQueueTime)
        {
            log.always("max queue time: %d", diff);
            maxQueueTime = diff;
        }
        // if (diff > 10)
        // {
        //     log.always("%d tasks took msecs: %d", cnt, diff);
        // }
    }
}