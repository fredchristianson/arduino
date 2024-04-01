#ifndef _FCLIB_TASK_H_
#define _FCLIB_TASK_H_
#include <c_types.h>
#include <functional>
#include "fclib/Logging.h"
#include "fclib/Timer.h"
#include "fclib/Test.h"
#include "fclib/Callable.h"
#include "fclib/List.h"
#include "fclib/Event.h"
#include "fclib/Config.h"

using namespace FCLIB;

#define FCLIB_REPEAT_FOREVER -1
namespace FCLIB
{
    class TaskQueue;
    class Task;
    class TimerTask;
    class LoopTask;

    enum TaskStatus
    {
        TASK_READY = 0,
        TASK_WAITING = 1,
        TASK_COMPLETE = 2
    };

    class TaskAction
    {
    public:
        TaskAction() {}
        virtual ~TaskAction() {}
        virtual void doTask() = 0;
    };

    class Task
    {
    public:
        TaskStatus getStatus() { return this->status; }
        virtual TaskStatus updateStatus() = 0;
        virtual void doTask() { action->doTask(); };

        void setStatus(TaskStatus status) { this->status = status; }
        void end() { this->status = TASK_COMPLETE; }

    protected:
        Task(TaskAction *action);
        virtual ~Task();
        TaskStatus status;
        TaskAction *action;
        Logger log;

    private:
        friend TaskQueue;

    public:
        static TimerTask *once(TaskAction *action);
        static TimerTask *repeat(TaskAction *action, long repeatCount = FCLIB_REPEAT_FOREVER);

        static TimerTask *once(SimpleCallable callback);
        static TimerTask *repeat(SimpleCallable callback, long repeatCount = FCLIB_REPEAT_FOREVER);

        static LoopTask *onLoop(TaskAction *action);
        static LoopTask *onLoop(SimpleCallable callback);
    };

    class LoopTask : public Task, public TaskAction
    {
    public:
        virtual TaskStatus updateStatus() { return TASK_READY; }

        void doTask() override;

    protected:
        friend Task;
        LoopTask(TaskAction *action);
        LoopTask(SimpleCallable callback);
        ~LoopTask();
        TaskAction *action; // owned by another object
        SimpleCallable callback;
    };

    class TimerTask : public Task, public TaskAction
    {
    public:
        virtual TaskStatus updateStatus() override;

        TimerTask *delaySeconds(int seconds);
        TimerTask *delayMinutes(int minutes);
        TimerTask *delayMsecs(int msecs);

        void setRepeatCount(long count) { this->repeatCount = count; }
        long getRepeatCount() { return this->repeatCount; }

    protected:
        TimerTask(TaskAction *action, long repeatCount);
        TimerTask(SimpleCallable callback, long repeatCount);
        virtual ~TimerTask();
        void doTask() override;

        Timer timer;
        long repeatCount;        // -1 means forever
        TaskAction *action;      // owned by another object
        SimpleCallable callback; // owned by this object (e.g. delete on descructor)
    };

    class OneTimeTask : public TimerTask
    {
    protected:
        friend Task;
        OneTimeTask(SimpleCallable callback);
        OneTimeTask(TaskAction *action);
        virtual ~OneTimeTask();
    };

    class RepeatingTask : public TimerTask
    {
    protected:
        friend Task;
        RepeatingTask(SimpleCallable callback, long repeatCount = FCLIB_REPEAT_FOREVER);
        RepeatingTask(TaskAction *action, long repeatCount = FCLIB_REPEAT_FOREVER);
        virtual ~RepeatingTask();
    };

    class TaskQueue
    {
    public:
        static void configure(Config *config);
        static void process();
        static TaskQueue *singleton;
        static TaskQueue *get();
        static void destroy(); // only for tests

        void runTasks();
        uint16 size() const { return tasks.size(); }

    protected:
        TaskQueue();
        virtual ~TaskQueue();
        friend Task;
        static void onTaskCreate(Task *task);
        static void onTaskDestroy(Task *task);

    private:
        void add(Task *task);
        void remove(Task *task); // marks for removal
        List<Task> tasks;
        Logger log;
        EventListener listener;
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