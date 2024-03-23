#ifndef _FCLIB_TASK_H_
#define _FCLIB_TASK_H_
#include <c_types.h>
#include <functional>
#include "fclib/Logging.h"
#include "fclib/Timer.h"
#include "fclib/Test.h"

#define FCLIB_REPEAT_FOREVER -1
namespace FCLIB
{
    class TaskQueue;
    using TaskCallback = std::function<void()>;

    enum LoopStage
    {
        LOOP_BEGIN = 0,
        LOOP_NORMAL = 1,
        LOOP_END
    };

    enum TaskStatus
    {
        TASK_READY = 0,
        TASK_WAITING = 1,
        TASK_COMPLETE = 2
    };

    enum TaskType
    {
        LOOP_TASK = 0,
        NULL_TASK = -1
    };

    class LoopTasks;

    class TaskAction
    {
    public:
        TaskAction() {}
        virtual ~TaskAction() {}
        virtual void doTask() = 0;
    };

    class TaskCallbackAction : public TaskAction
    {
    public:
        TaskCallbackAction(TaskCallback callback) { this->callback = callback; }
        virtual void doTask() override { (this->callback)(); }

    private:
        TaskCallback callback;
    };

    class TaskBase
    {
    public:
        TaskBase(TaskAction *action, TaskType type, uint8 priority = 50);
        virtual ~TaskBase();

        uint8 getPriority() { return this->priority; }
        TaskStatus getStatus() { return this->status; }
        TaskType getTaskType() { return this->taskType; }
        virtual TaskStatus updateStatus() = 0;
        virtual void doTask() { action->doTask(); };

        TaskBase *getNext() { return next; }
        TaskBase *getPrev() { return prev; }

    protected:
        friend TaskQueue;
        uint8 priority; // 0(low) - 100(high)
        TaskStatus status;
        TaskType taskType;
        TaskAction *action;
        Logger log;

    private:
        friend TaskQueue;
        TaskBase *prev;
        TaskBase *next;
    };

    class LoopTask : public TaskBase
    {
    public:
        static LoopTask *create(TaskCallback callback, LoopStage stage = LOOP_NORMAL, uint8 priority = 50);
        static LoopTask *create(TaskAction *action, LoopStage stage = LOOP_NORMAL, uint8 priority = 50);
        LoopStage getStage() { return this->stage; }
        virtual TaskStatus updateStatus() { return TASK_READY; }

    protected:
        LoopTask(TaskAction *action, LoopStage stage = LOOP_NORMAL, uint8 priority = 50);
        virtual ~LoopTask();
        void setStage(LoopStage stage) { this->stage = stage; }

    protected:
        LoopStage stage;
        bool ownAction; // action created/owned by this object (e.g. delete )
    };

    /* NullTasks are used as placeholders so queues are never empty.*/
    class NullTask : public TaskBase
    {
    public:
        NullTask();
        virtual ~NullTask();
        virtual TaskStatus updateStatus() { return TASK_WAITING; }
        virtual void doTask() {}
    };

    class TimerTask : public LoopTask, public TaskAction
    {
    public:
        TimerTask(TaskCallback callback, long repeatCount);
        virtual ~TimerTask();
        virtual TaskStatus updateStatus() override;

        virtual void doTask() override;
        TimerTask *delaySeconds(int seconds);
        TimerTask *delayMinutes(int minutes);
        TimerTask *delayMsecs(int msecs);

        void setRepeatCount(long count) { this->repeatCount = count; }
        long getRepeatCount() { return this->repeatCount; }

    protected:
        Timer timer;
        long repeatCount; // -1 means forever
        TaskCallback callback;
    };

    class OneTimeTask : public TimerTask
    {
    public:
        OneTimeTask(TaskCallback callback);
        virtual ~OneTimeTask();
    };

    class RepeatingTask : public TimerTask
    {
    public:
        RepeatingTask(TaskCallback callback, long repeatCount = FCLIB_REPEAT_FOREVER);
        virtual ~RepeatingTask();
    };

    class Task
    {
    public:
        static TimerTask *once(TaskAction &action);
        static TimerTask *repeat(TaskAction &action, long repeatCount = FCLIB_REPEAT_FOREVER);

        static TimerTask *once(TaskCallback callback);
        static TimerTask *repeat(TaskCallback callback, long repeatCount = FCLIB_REPEAT_FOREVER);
    };

    class TaskQueue
    {
    public:
        TaskQueue();
        virtual ~TaskQueue();
        void add(TaskBase *task);
        void remove(TaskBase *task);

        void updateTaskStatus();
        void runTasks();

        TaskBase *getFirstNull() { return first; }
        TaskBase *getLastNull() { return last; }

        bool isHealthy();

    protected:
        TaskBase *first;
        TaskBase *last;
        Logger log;
    };

    namespace TEST
    {
        namespace TASK
        {
            class TaskTestSuite : public TestSuite
            {
            public:
                TaskTestSuite();
                virtual ~TaskTestSuite();

            protected:
                virtual void runTests() override;
                void run();
            };
        }
    }
}

#endif