#include "fclib/Task.h"

using namespace FCLIB;
using namespace FCLIB::TEST;
using namespace FCLIB::TEST::TASK;

class TestAction : public TaskAction
{
public:
    virtual void doTask() {}
};

class TestTask : public Task
{
public:
    static int deleteCount;
    TestTask(TestAction *action) : Task(action) {}
    ~TestTask()
    {
        Logger logger("TestTask");
        log.never("~TestTask");
        deleteCount++;
    }
    TaskStatus updateStatus()
    {
        return status == TASK_COMPLETE ? TASK_COMPLETE : TASK_WAITING;
    }
};

int TestTask::deleteCount = 0;
void testQueueCreate(TestResult &result)
{
    TaskQueue::get();
    result.success("TaskQueue created");
}

void testQueueAdd(TestResult &result)
{
    TestAction action;
    TaskQueue *queue = TaskQueue::get();
    Task *t = new TestTask(&action);
    result.equal(1, queue->size(), "task added to queue");
    t->end();
}

void testQueueRemove(TestResult &result)
{
    TestTask::deleteCount = 0;
    TestAction action;
    TaskQueue *queue = TaskQueue::get();
    TestTask *task = new TestTask(&action);
    task->end();
    queue->runTasks(); // should delete completed task
    result.equal(1, TestTask::deleteCount, "task deleted");
}

TaskTestSuite::TaskTestSuite() : TestSuite("TaskTestSuite")
{
}

TaskTestSuite::~TaskTestSuite()
{
}

void TaskTestSuite::runTests()
{
    // test("Success", ok);
    test("TaskQueueCreate", testQueueCreate);
    test("QueueAdd", testQueueAdd);
    test("QueueRemove", testQueueRemove);
}