#include "fclib/Persistance.h"

using namespace FCLIB::TEST;
using namespace FCLIB::TEST::PERSIST;

namespace FCLIB::TEST::PERSIST
{
    void testStr(TestResult &result)
    {
        Persist::set("test", "str", "abc");
        result.equal("abc", Persist::get("test", "str", "---").c_str(), "get() returned correct chatr* value");
        Persist::freeMemory(); // release memory
    }

    void testInt(TestResult &result)
    {
        Persist::get();
        // Persist::set("test", "int", 1);
        //   result.equal(1, Persist::get("test", "int", 0), "get() returned correct int value");
        Persist::freeMemory(); // release memory
    }

    void testFloat(TestResult &result)
    {
        Persist::set("test", "float", 1.234f);
        result.equal(1.234, Persist::get("test", "float", 1.234f), "get() returned correct float value");
        Persist::freeMemory(); // release memory
    }

    void testBool(TestResult &result)
    {
        Persist::set("test", "bool", true);
        result.equal(true, Persist::get("test", "bool", false), "get() returned correct bool value");
        Persist::freeMemory(); // release memory
    }

    PersistTestSuite::PersistTestSuite() : TestSuite("PersistTestSuite")
    {
        Persist::get(); // loads the file
    }

    PersistTestSuite::~PersistTestSuite()
    {
    }

    void PersistTestSuite::runTests()
    {
        // test("set str", testStr);
        test("set int", testInt);
        // test("set float", testFloat);
        // test("set bool", testBool);
    }

    void PersistTestSuite::prepare()
    {
        // free then load persisted vals before starting tests
        Persist::freeMemory();
        Persist::get();
    }
    void PersistTestSuite::cleanup()
    {
        Persist::freeMemory(); // release memory
    }
}
