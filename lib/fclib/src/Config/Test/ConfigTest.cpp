#include "fclib/Config.h"

using namespace FCLIB::TEST;
using namespace FCLIB::TEST::CONFIG;

void testFail(TestResult &result)
{
	result.fail("force fail");
}

void testEqual(TestResult &result)
{
	result.equal(1, 1, "1 should equal 1");
	result.equal(2, 2, "2 should  equal 2");
}

void testNotEqual(TestResult &result)
{
	result.notEqual(2, 1, "2 should not equal 1");
	result.notEqual(1, 2, "1 should not equal 2");
}

ConfigTestSuite::ConfigTestSuite() : TestSuite("ConfigTestSuite")
{
}

ConfigTestSuite::~ConfigTestSuite()
{
}

void ConfigTestSuite::runTests()
{
	test("Equal tests", testEqual);
	test("Not Equal Tests", testNotEqual);
}