#include "fclib/Task.h"

using namespace FCLIB;
using namespace FCLIB::TEST;
using namespace FCLIB::TEST::TASK;

class TestAction : public TaskAction
{
public:
    virtual void doTask() {}
};

class TestTask : public TaskBase
{
public:
    TestTask(TestAction *action) : TaskBase(action, NULL_TASK) {}
    TaskStatus updateStatus() { return TASK_WAITING; }
};
void testQueueCreate(TestResult &result)
{
    TaskQueue queue;
    result.success("TaskQueue created");
}

void testQueueNullTasks(TestResult &result)
{
    TaskQueue *queue = new TaskQueue();
    TaskBase *first = queue->getFirstNull();
    TaskBase *last = queue->getLastNull();
    result.notNull(first, "First NULL task  created") &&
        result.notNull(last, "Last NULL task created ") &&
        result.null(first->getPrev(), "first->prev is null ") &&
        result.null(last->getNext(), "last->next is null ") &&
        result.equal(first->getNext(), last, "First->next is last") &&
        result.equal(last->getPrev(), first, "Last->prev == first");
    delete queue;
}

void testQueueAdd(TestResult &result)
{
    TestAction action;
    TaskQueue queue;
    TestTask *task = new TestTask(&action);
    queue.add(task);
}

void testQueueRemove(TestResult &result)
{
    TestAction action;
    TaskQueue queue;
    TestTask *task = new TestTask(&action);
    queue.add(task);
    queue.remove(task);
}

TaskTestSuite::TaskTestSuite() : TestSuite("TaskTestSuite")
{
}

TaskTestSuite::~TaskTestSuite()
{
}

void ok(TestResult &result)
{
    result.equal(1, 1, "1==1");
}
void TaskTestSuite::runTests()
{
    // test("Success", ok);
    test("TaskQueueCreate", testQueueCreate);
    test("TaskQueue NullTasks", testQueueNullTasks);
    test("QueueAdd", testQueueAdd);
    test("QueueRemove", testQueueRemove);
}