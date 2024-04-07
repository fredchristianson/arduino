#include "fclib/Config.h"
#include "fclib/Event.h"

using namespace FCLIB::TEST;
using namespace FCLIB::TEST::CONFIG;

Logger log("ConfigTest");
void testCreate(TestResult &result)
{
    ConfigFile *file = new ConfigFile();
    result.success("created ConfigFile");
    delete file;
    result.success("deleted ConfigFile");
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
    delete file;
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

    log.showMemory("new ConfigSection");
    ConfigSection *section = new ConfigSection(NULL, "test");
    log.showMemory("delete ConfigSection");
    delete section;
    log.showMemory("deleted");

    result.success("section created and deleted");
    log.showMemory("set result");
}
void testSectionSet(TestResult &result)
{
    ConfigSection *section = new ConfigSection(NULL, "test");
    section->set("test1", 1);
    delete section;
    result.success("section created and deleted");
}

void testSectionMultiSet(TestResult &result)
{
    Logger log("multiset");
    ConfigSection *section = new ConfigSection(NULL, "test");
    log.never("start");
    section->set("test1", 1);
    log.never("1");
    section->set("test1", 2);
    log.never("2");
    section->set("test1", 3);
    log.never("3");
    section->set("test4", 4);
    log.never("4");
    section->set("test5", 5);
    log.never("5");
    delete section;
    log.never("6");
    result.success("section created and deleted");
}

void testNothing(TestResult&result){
    result.success("nothing tested");
}

ConfigTestSuite::ConfigTestSuite() : TestSuite("ConfigTestSuite")
{
}

ConfigTestSuite::~ConfigTestSuite()
{
}

void ConfigTestSuite::runTests()
{
    test("Nothing",testNothing);
    test("Create Section", testCreateSection);
    test("Create Section", testCreateSection);
    test("Section set value", testSectionSet);
    test("Section set multiple values", testSectionMultiSet);
    // test("Create Config", testCreate);
    // test("TestLoad", testLoad);
    // test("TestSet", testSet);
    // test("TestSet", testSet);
    // test("TestSetMultiple", testSetMultiple);
    // test("TestSave", testSave);
    // test("TestSaveLoad", testSaveLoad);
}