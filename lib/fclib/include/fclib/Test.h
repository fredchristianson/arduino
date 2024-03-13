#ifndef _FCLIB_TEST_H_
#define _FCLIB_TEST_H_
#include "fclib/Logging.h"
#include "WString.h"

namespace FCLIB
{

	namespace TEST
	{
		class TestSuite;
		class TestRunner;

		enum ResultType
		{
			SUCCESS = 1,
			FAIL = 0,
			WARN = -1
		};

		class TestResult
		{
		public:
			TestResult(const char *message, TestSuite *suite, Logger *testLogger);
			virtual ~TestResult();
			void fail(const char *message);
			void equal(int val, int expect, const char *testMessage, ResultType errorType = FAIL);
			void notEqual(int val, int expect, const char *testMessage, ResultType errorType = FAIL);

			bool isSuccess() const { return type == SUCCESS; }
			bool isFail() const { return type == FAIL; }
			bool isWarn() const { return type == WARN; }

			void success(const char *message);
			void failure(const char *message, ResultType err = FAIL);
			void warning(const char *message);

		protected:
			String message;
			ResultType type;

			friend TestSuite;
			friend TestRunner;
			TestResult *nextResult;
			TestSuite *testSuite;
			Logger *log;
		};

		class TestSuite
		{
		public:
			TestSuite(const char *name);
			virtual ~TestSuite();

			bool run();

			bool isSuccess() const;
			int successCount() const;
			int failCount() const;
			int warnCount() const;

		protected:
			virtual void runTests() = 0;
			bool test(const char *name, void (*func)(TestResult &result));

			TestResult *firstResult;
			TestSuite *nextSuite;
			Logger *log;
			String name;

			friend TestRunner;
		};

		class TestRunner
		{
		public:
			TestRunner(const char *name, Logger &logger);
			virtual ~TestRunner();

			bool run(TestSuite &suite);

			bool isSuccess() const;
			int successCount() const;
			int failCount() const;
			int warnCount() const;

		protected:
			String name;
			TestSuite *firstSuite;
			Logger &log;
			long startMemory;
		};
	};

} // namespace FCLIB

#endif