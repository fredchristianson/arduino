#include "fclib/Config.h"
#include "fclib/Event.h"
#include "fclib/File.h"

using namespace FCLIB::TEST;
using namespace FCLIB::TEST::CONFIG;

namespace FCLIB::TEST::CONFIG
{
    Logger log("ConfigTest");
    const int nameCount = 14;
    const char *valueNames[nameCount] = {
        "test1", "test2", "test3", "test4", "test5", "test6",
        "test7", "test8", "test9", "test10", "test11", "test12", "test13", "test14"};

    void testCreate(TestResult &result)
    {
        Config config;
        config.set("test", "default value");
        config.set("test1", "default value 1");
        config.set("test", "section value", "section1");
        result.equal("default value", config.get("test", "xxx"), "default value correct");
        result.equal("section value", config.get("section1", "test", "xxx"), "section value correct");
        result.equal("default value 1", config.get("section1", "test1", "xxx"), "default value if not in section");
    }

    void testLoad(TestResult &result)
    {
        ConfigFile *file = new ConfigFile();
        file->load("/file-does-not-exist");
        result.success("loaded ConfigFile");
        delete file;
    }

    void testSet(TestResult &result)
    {
        ConfigFile *file = new ConfigFile();
        file->set("test", 1);
        result.success("created ConfigFile");
        delete file;
    }

    void testSetMultiple(TestResult &result)
    {
        ConfigFile *file = new ConfigFile();
        file->set("test1", 1);
        file->set("test2", 2);
        file->set("test3", 3);
        file->set("test4", 4);
        result.success("created ConfigFile");
        delete file;
    }
    void testSave(TestResult &result)
    {
        ConfigFile *file = new ConfigFile();
        file->set("test", 1);
        file->save("/test-create-if-needed.ini");
        result.success("created ConfigFile");
        log.never("delete file");
        delete file;
        log.never("testSave done");
    }
    void testSaveLoad(TestResult &result)
    {
        ConfigFile *file = new ConfigFile();
        file->set("test", 1);
        file->save("/test-create-if-needed.ini");
        result.success("created ConfigFile");
        delete file;
        file = new ConfigFile();
        file->load("/test-create-if-needed.ini");
        result.equal(file->get("test", 0), 1, "loaded value matched saved value");
    }

    void testCreateSection(TestResult &result)
    {

        ConfigSection *section = new ConfigSection(NULL, "test");
        delete section;

        result.success("section created and deleted");
    }
    void testSectionSet(TestResult &result)
    {
        ConfigSection *section = new ConfigSection(NULL, "test");
        section->set("test1", 1);
        result.equal(section->get("test1", 0), 1, "Value is correct");
        delete section;
    }

    void testSectionMultiSet(TestResult &result)
    {
        ConfigSection *section = new ConfigSection(NULL, "test");
        for (int i = 0; i < nameCount; i++)
        {
            section->set(valueNames[i], "abc");
        }

        for (int i = 0; i < nameCount; i++)
        {
            result.equal("abc", section->get(valueNames[i], "wrong"), "correct value 'abc'");
        }

        for (int i = 0; i < nameCount; i++)
        {
            section->set(valueNames[i], i);
        }

        for (int i = 0; i < nameCount; i++)
        {
            result.equal(i, section->get(valueNames[i], -1), "correct value i");
        }

        delete section;
    }

    void testNothing(TestResult &result)
    {
        result.success("nothing tested");
    }

    void testValues(TestResult &result)
    {
        ConfigValue val(NULL, "test", "1.234");
        result.isFalse(val.isChanged(), "New value should not be changed");
        result.equal(val.toString(), "1.234", "Text value correct");
        result.equal(val.toInt(), 1, "Convert to int");
        result.equal(val.toFloat(), 1.234, "Convert to float");
        result.equal(val.toBool(), true, "Convert to bool");

        val.set("abc");
        result.isTrue(val.isChanged(), "Should not be changed");
        val.clearChanged();
        result.isFalse(val.isChanged(), "Change cleared");

        result.equal(val.toInt(), 0, "Text is not a number");
        val.set(2.35);
        // log.warn("tostring: %s", val.toString());
        result.equal("2.35", val.toString(), "Float converted to text");
    }

    ConfigTestSuite::ConfigTestSuite() : TestSuite("ConfigTestSuite")
    {
    }

    ConfigTestSuite::~ConfigTestSuite()
    {
    }

    void ConfigTestSuite::prepare()
    {
        FileSystem::get();
    }
    void ConfigTestSuite::cleanup()
    {
        FileSystem::get()->unmount();
    }

    void ConfigTestSuite::runTests()
    {
        test("ConfigValue", testValues);
        test("Create Section", testCreateSection);
        test("Create Section", testCreateSection);
        test("Section set value", testSectionSet);
        test("Section set multiple values", testSectionMultiSet);
        test("Create Config", testCreate);
        test("TestLoad Config File", testLoad);
        test("TestSet Config File", testSet);
        test("TestSetMultiple", testSetMultiple);
        test("TestSave", testSave);
        // test("TestSaveLoad", testSaveLoad);
    }
}