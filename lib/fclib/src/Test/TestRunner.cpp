#include "fclib/Test.h"
#include "fclib/System.h"
#include "fclib/File.h"
#include "fclib/Event.h"
#include "fclib/Task.h"

using namespace FCLIB;
using namespace FCLIB::TEST;

const char *testRunnerDivider = "==============================================================";
const char *testSuiteDivider = "--------------------------------------------------------------";
TestRunner::TestRunner(const char *name, Logger &logger) : log(logger)
{
    // make sure singletons created for mem leak detecting
    EventManager::processEvents();
    TaskQueue::process();
    Board *board = Board::get();
    log.info(String(40, '=').c_str());

    firstSuite = NULL;
    this->name = name;
    log.info("Running Tests:  %s", this->name.c_str());
    log.showMemory();
    log.info(testRunnerDivider);
    startMemory = board->getFreeHeap();
}

TestRunner::~TestRunner()
{
    Board *board = Board::get();
    log.info(testRunnerDivider);
    log.info("Tests Complete:  %s", this->name.c_str());
    log.info("Success count: %d", successCount());
    log.info("Fail count: %d", failCount());
    log.info("Warning count: %d", warnCount());
    log.showMemory();
    long endMemory = board->getFreeHeap();
    if (startMemory != endMemory)
    {
        log.error("Memory leak: %d bytes", startMemory - endMemory);
    }
    log.info(testRunnerDivider);
}

bool TestRunner::run(TestSuite &suite)
{
    suite.log = &log;
    if (this->firstSuite != NULL)
    {
        log.info("\r\n");
        log.info(testSuiteDivider);
        log.info("\r\n");
    }
    suite.nextSuite = this->firstSuite;
    this->firstSuite = &suite;
    suite.run();
    bool success = isSuccess();

    return success;
}

bool TestRunner::isSuccess() const
{
    TestSuite *suite = firstSuite;
    bool success = true;
    while (success && suite != NULL)
    {
        success = suite->isSuccess();
        suite = suite->nextSuite;
    }
    return success;
}

int TestRunner::successCount() const
{
    TestSuite *suite = firstSuite;
    int count = 0;
    while (suite != NULL)
    {
        count += suite->successCount();
        suite = suite->nextSuite;
    }
    return count;
}
int TestRunner::failCount() const
{
    TestSuite *suite = firstSuite;
    int count = 0;
    while (suite != NULL)
    {
        count += suite->failCount();
        suite = suite->nextSuite;
    }
    return count;
}
int TestRunner::warnCount() const
{
    TestSuite *suite = firstSuite;
    int count = 0;
    while (suite != NULL)
    {
        count += suite->warnCount();
        suite = suite->nextSuite;
    }
    return count;
}
