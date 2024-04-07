#include "fclib/Test.h"

using namespace FCLIB;
using namespace FCLIB::TEST;

TestResult::TestResult(const char *message, TestSuite *suite, Logger *logger) : log(logger)
{
    // this->message = message;
    this->testSuite = suite;
    this->nextResult = NULL;
    this->type = SUCCESS;
}

TestResult::~TestResult()
{
    delete nextResult;
}

void TestResult::success(const char *message)
{
    log->info("\t\t\tSuccess: %s", message);
    this->type = SUCCESS;
    // this->message = message;
}
void TestResult::failure(const char *message, ResultType err)
{
    log->info("\t\t\tFail: %s", message);
    this->type = err;
    // this->message = message;
}
void TestResult::warning(const char *message)
{
    log->info("\t\t\tWarning: %s", message);
    // if test already failed, to change to warn level
    if (this->type != FAIL)
    {
        this->type = WARN;
    }
    // this->message = message;
}

bool TestResult::fail(const char *msg)
{
    this->failure(msg);
    return false;
}

bool TestResult::equal(int val, int expect, const char *testMessage, ResultType errType)
{
    if (val != expect)
    {
        this->failure(testMessage, errType);
        return errType != ResultType::FAIL;
    }
    else
    {
        this->success(testMessage);
        return true;
    }
}

bool TestResult::equal(float val, float expect, const char *testMessage, ResultType errType)
{
    if (val != expect)
    {
        this->failure(testMessage, errType);
        return errType != ResultType::FAIL;
    }
    else
    {
        this->success(testMessage);
        return true;
    }
}

bool TestResult::equal(const char *val, const char *expect, const char *testMessage, ResultType errType)
{
    if (val == expect)
    {
        this->success(testMessage);
        return true;
    }
    else if (val == NULL || expect == NULL)
    {
        this->failure(testMessage, errType);
        return errType != ResultType::FAIL;
    }
    else if (strcmp(val, expect) != 0)
    {
        this->failure(testMessage, errType);
        return errType != ResultType::FAIL;
    }
    else
    {
        this->success(testMessage);
        return true;
    }
}
bool TestResult::equal(void *val, void *expect, const char *testMessage, ResultType errType)
{
    if (val != expect)
    {
        this->failure(testMessage, errType);
        return errType != ResultType::FAIL;
    }
    else
    {
        this->success(testMessage);
        return true;
    }
}
bool TestResult::notEqual(int val, int expect, const char *testMessage, ResultType errType)
{
    if (val == expect)
    {
        this->failure(testMessage, errType);
        return errType != ResultType::FAIL;
    }
    else
    {
        this->success(testMessage);
        return true;
    }
}

bool TestResult::notNull(void *val, const char *testMessage, ResultType errType)
{
    if (val == NULL)
    {
        this->failure(testMessage, errType);
        return errType != ResultType::FAIL;
    }
    else
    {
        this->success(testMessage);
        return true;
    }
}

bool TestResult::null(void *val, const char *testMessage, ResultType errType)
{
    if (val != NULL)
    {
        this->failure(testMessage, errType);
        return errType != ResultType::FAIL;
    }
    else
    {
        this->success(testMessage);
        return true;
    }
}