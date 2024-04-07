#include "fclib/Test.h"
#include "fclib/System.h"
#include "fclib/Event.h"
#include "fclib/Task.h"

using namespace FCLIB;
using namespace FCLIB::TEST;

TestSuite::TestSuite(const char *name)
{
    firstResult = NULL;
    this->name = name;
}

TestSuite::~TestSuite()
{
    delete firstResult;
}

bool TestSuite::run()
{

    log->info("\tRunning test suite %s", name.c_str());
    prepare();
    runTests();
    cleanup();
    log->info("\tSuite Complete:  %s", this->name.c_str());
    log->info("\tSuccess count: %d", successCount());
    log->info("\tFail count: %d", failCount());
    log->info("\tWarning count: %d", warnCount());
    return isSuccess();
}

bool TestSuite::isSuccess() const
{
    TestResult *result = firstResult;
    bool success = true;
    while (success && result != NULL)
    {
        success = result->isSuccess();
        result = result->nextResult;
    }
    return success;
}

int TestSuite::successCount() const
{
    TestResult *result = firstResult;
    int count = 0;
    while (result != NULL)
    {
        count += result->isSuccess() ? 1 : 0;
        result = result->nextResult;
    }
    return count;
}
int TestSuite::failCount() const
{
    TestResult *result = firstResult;
    int count = 0;
    while (result != NULL)
    {
        count += result->isFail() ? 1 : 0;
        result = result->nextResult;
    }
    return count;
}
int TestSuite::warnCount() const
{
    TestResult *result = firstResult;
    int count = 0;
    while (result != NULL)
    {
        count += result->isWarn() ? 1 : 0;
        result = result->nextResult;
    }
    return count;
}

bool TestSuite::test(const char *message, void (*func)(TestResult &result))
{
    TestResult *result = new TestResult(message, this, log);
    result->nextResult = this->firstResult;
    this->firstResult = result;
    log->info("\t\tTest: %s", message);
    int mem = THE_BOARD->getFreeHeap();
    // process events and tasks to since they may free memory
    EventManager::processEvents();
    TaskQueue::process();
    func(*result);
    EventManager::processEvents();
    TaskQueue::process();
    int endMem = THE_BOARD->getFreeHeap();
    if (endMem != mem)
    {
        log->error("\t\t\tMemory leak: %d-%d=%d", mem, endMem, mem - endMem);
        result->warning("Memory leak");
    }
    return result->isSuccess();
}
