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
            bool fail(const char *message);
            bool equal(const char *val, const char *expect, const char *testMessage, ResultType errorType = FAIL);
            bool equal(float val, float expect, const char *testMessage, ResultType errorType = FAIL);
            bool equal(int val, int expect, const char *testMessage, ResultType errorType = FAIL);
            bool equal(void *val, void *expect, const char *testMessage, ResultType errorType = FAIL);
            bool notEqual(int val, int expect, const char *testMessage, ResultType errorType = FAIL);
            bool notNull(void *val, const char *testMessage, ResultType errorType = FAIL);
            bool null(void *val, const char *testMessage, ResultType errorType = FAIL);
            bool test(bool isSuccess, const char *testMessage, ResultType errorType = FAIL) { return equal(isSuccess, true, testMessage, errorType); }
            bool isTrue(bool isSuccess, const char *testMessage, ResultType errorType = FAIL) { return equal(isSuccess, true, testMessage, errorType); }
            bool isFalse(bool isSuccess, const char *testMessage, ResultType errorType = FAIL) { return notEqual(isSuccess, true, testMessage, errorType); }

            bool isSuccess() const { return type != FAIL; }
            bool isFail() const { return type == FAIL; }
            bool isWarn() const { return type == WARN; }

            void success(const char *message);
            void failure(const char *message, ResultType err = FAIL);
            void warning(const char *message);

            Logger *getLog() { return log; }

        protected:
            // String message;
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
            // prepare() and cleanup() can be used to do things that are outside of
            // memory tracking.  Mostly related to singletons created as part of normal
            // activity.
            virtual void prepare() {}
            virtual void cleanup() {}
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