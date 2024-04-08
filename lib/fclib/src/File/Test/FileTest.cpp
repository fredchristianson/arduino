#include "fclib/File.h"

using namespace FCLIB;
using namespace FCLIB::TEST;
using namespace FCLIB::TEST::FILE;

namespace FCLIB::TEST::FILE
{
    const char *smallFileName = "test_file";
    const char *testTextLine = "abcdefghijklmnopqrstuvwzyz";

    void testWrite(TestResult &result)
    {
        FileWriter writer(smallFileName);
        result.isTrue(writer.writeLine(testTextLine), "Write line");
        String s = testTextLine;
        result.isTrue(writer.writeLine(s), "Write line");
    }

    void testAppend(TestResult &result)
    {
        FileAppender writer(smallFileName);
        result.isTrue(writer.writeLine(testTextLine), "Write line");
        String s = testTextLine;
        result.isTrue(writer.writeLine(s), "Write line");
    }

    void testRead(TestResult &result)
    {
        FileReader reader(smallFileName);
        int count = 0;
        String line;
        while (count < 10 && reader.readLine(line))
        {
            count++;
            result.equal(line.c_str(), testTextLine, "line text is correct");
        }
        result.equal(4, count, "read 4 lines");
    }

    void testDelete(TestResult &result)
    {
        FileBase file(smallFileName);
        result.isTrue(file.exists(), "File exists");
        file.remove();
        result.isFalse(file.exists(), "File was removed");
    }

    FileTestSuite::FileTestSuite() : TestSuite("FileTestSuite")
    {
    }

    FileTestSuite::~FileTestSuite()
    {
    }

    void FileTestSuite::runTests()
    {
        // test("Success", ok);
        test("Write", testWrite);
        test("Append", testAppend);
        test("Read", testRead);
        test("Delete", testDelete);
    }
}