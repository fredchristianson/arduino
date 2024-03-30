#include "fclib/Task.h"

using namespace FCLIB;

namespace FCLIB
{
    TaskQueue::TaskQueue() : log("TaskQueue")
    {
        first = new NullTask();
        last = new NullTask();
        first->next = last;
        last->prev = first;
        first->priority = 0;
        last->priority = 101;
        if (!isHealthy())
        {
            log.error("TaskQueue not initialized propertly");
        }
        log.never("TaskQueue 0x%lx.  first=0x%lx.   last=0x%lx", this, first, last);
    }

    TaskQueue::~TaskQueue()
    {
        TaskBase *iter = first;
        while (iter != NULL)
        {
            TaskBase *next = iter->next;
            delete iter;
            iter = next;
        }
    }

    bool TaskQueue::isHealthy()
    {
        log.never("isHealthy() 0x%lx.  first=0x%lx.   last=0x%lx", this, first, last);

        if (first == NULL)
        {
            log.error("No first task");
            return false;
        }
        if (last == NULL)
        {
            log.error("No last task");
            return false;
        }

        if (first->next == NULL || last->prev == NULL)
        {
            log.error("first and last have null");
            return false;
        }
        TaskBase *walker = first;
        int count = 0;
        log.never("Walk tasks from 0x%lx,   first=0x%lx.   last=0x%lx", walker, first, last);
        while (walker != last && walker != NULL && count < 1000)
        {
            walker = walker->next;
            count++;
            log.never("\tnext (%d) 0x%lx", count, walker);
        }
        if (walker == NULL)
        {
            log.error("walker found NULL");
            return false;
        }
        if (count >= 1000)
        {
            log.warn("too many tasks");
        }
        return true;
    }

    void TaskQueue::add(TaskBase *task)
    {

        log.never("Add task 0x%lx", task);

        for (TaskBase *p = first; p != last; p = p->next)
        {
            log.never("\t\t 0x%lx   0x%lx 0x%lx", p, p->prev, p->next);
        }
        log.never("\tpriority %d", task->getPriority());
        if (!isHealthy())
        {
            return;
        }
        TaskBase *pos = first;
        uint8 p = task->getPriority();
        while (pos->next != last && p >= pos->next->getPriority())
        {
            if (pos == task)
            {
                log.warn("task already in queue");
                return;
            }
            pos = pos->next;
        }
        task->prev = pos;
        task->next = pos->next;
        pos->next->prev = task;
        pos->next = task;
        log.never("\tadded 0x%lx   0x%lx 0x%lx", pos, pos->prev, pos->next);

        for (TaskBase *p = first; p != last; p = p->next)
        {
            log.never("\t\tfound 0x%lx   0x%lx 0x%lx", p, p->prev, p->next);
        }
    }

    void TaskQueue::removeTask(TaskBase *task)
    {
        log.never("remove task 0x%lx", task);
        TaskBase *pos = first->next;
        while (pos != NULL && pos != task)
        {
            pos = pos->next;
        }
        if (pos == NULL)
        {
            log.debug("task not found in remove(task).  may be ok.");
            return;
        }
        log.never("\tfound 0x%lx   0x%lx 0x%lx", pos, pos->prev, pos->next);
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        for (TaskBase *p = first; p != last; p = p->next)
        {
            log.never("\t\tfound 0x%lx   0x%lx 0x%lx", p, p->prev, p->next);
        }
    }

    void TaskQueue::remove(TaskBase *task)
    {
        task->setStatus(TASK_COMPLETE);
    }
    void TaskQueue::updateTaskStatus()
    {
        TaskBase *task = first->next;
        while (task != last)
        {
            TaskBase *next = task->next;
            TaskStatus status = task->updateStatus();
            log.never("task status: %d", status);
            if (status == TASK_COMPLETE)
            {
                log.info("Task complete.  Remove 0x%lx", task);
                removeTask(task);
                delete task;
            }
            task = next;
        }
    }
    void TaskQueue::runTasks()
    {
        log.never("Run tasks");
        TaskBase *task = first->next;
        while (task != last)
        {
            TaskBase *next = task->next;
            log.never("Check task %d", task->getStatus());
            if (task->getStatus() == TASK_READY)
            {
                log.never("Run task 0x%lx", task);
                task->doTask();
            }

            task = next;
        }
    }
}