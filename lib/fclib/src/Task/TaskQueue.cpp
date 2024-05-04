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

    void TaskQueue::process()
    {
        if (singleton == NULL)
        {
            singleton = new TaskQueue();
        }
        singleton->runTasks();
    }

    TaskQueue::TaskQueue() : log("TaskQueue"), taskCount("TaskCount"), processTimer("TaskProcessTimer")
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
        LoopTime::check("TaskQueue::~TaskQueue()");
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
        taskCount.value(tasks.size());
        processTimer.start();
        for (int i = 0; i < tasks.size() && LoopTime::ok(); i++)
        {
            Task *task = tasks[i];
            log.debug("Check task %d %x", i, task);

            TaskStatus status = task->updateStatus();
            log.debug("\tstatus %d", status);
            if (status == TASK_READY)
            {
                log.debug("Run task 0x%lx", task);
                task->doTask();
            }
            else if (status == TASK_COMPLETE)
            {
                log.debug("delete task %x", task);
                tasks.remove(task);
                delete task;
                log.debug("deleted.  size=%d", tasks.size());
                i--;
            }
        }
        processTimer.end();
        LoopTime::check("TaskQueue::runTasks()");
    }
}