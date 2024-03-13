#include "fclib/Test.h"

using namespace FCLIB;
using namespace FCLIB::TEST;

TestResult::TestResult(const char *message, TestSuite *suite, Logger *logger) : log(logger)
{
	this->message = message;
	this->testSuite = suite;
	this->nextResult = NULL;
}

TestResult::~TestResult()
{
	delete nextResult;
}

void TestResult::success(const char *message)
{
	log->info("\t\t\tSuccess: %s", message);
	this->type = SUCCESS;
	this->message = message;
}
void TestResult::failure(const char *message, ResultType err)
{
	log->info("\t\t\tFail: %s", message);
	this->type = err;
	this->message = message;
}
void TestResult::warning(const char *message)
{
	log->info("\t\t\tWarning: %s", message);

	this->type = WARN;
	this->message = message;
}

void TestResult::fail(const char *msg)
{
	this->failure(msg);
}

void TestResult::equal(int val, int expect, const char *testMessage, ResultType errType)
{
	if (val != expect)
	{
		this->failure(testMessage, errType);
	}
	else
	{
		this->success(testMessage);
	}
}
void TestResult::notEqual(int val, int expect, const char *testMessage, ResultType errType)
{
	if (val == expect)
	{
		this->failure(testMessage, errType);
	}
	else
	{
		this->success(testMessage);
	}
}