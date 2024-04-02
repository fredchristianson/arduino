#include "fclib/List.h"

using namespace FCLIB::TEST;
using namespace FCLIB::TEST::LIST;

namespace FCLIB::TEST::LIST
{
    class Item
    {
    public:
        Item() {}

        int val;
    };

    void testAdd(TestResult &result)
    {
        List<Item> list;
        Item items[20];
        for (int i = 0; i < 20; i++)
        {
            items[i].val = i;
            list.add(&items[i]);
        }
        result.equal(list.size(), 20, "Added 20 items");
        bool success = true;
        for (int i = 0; success && i < 20; i++)
        {
            success = success && list[i]->val == i;
        }
        result.equal(success, true, "Item has correct value");
    }

    void testRemove(TestResult &result)
    {
        List<Item> list;
        Item items[20];
        for (int i = 0; i < 20; i++)
        {
            list.add(&items[i]);
        }
        for (int i = 0; i < 20; i++)
        {
            list.removeAt(0);
        }
        result.equal(list.size(), 0, "Removed 20 items");
    }

    void testGrow(TestResult &result)
    {
        List<Item> list;
        Item item;
        for (int i = 0; i < 40; i++)
        {
            list.add(&item);
        }

        result.equal(list.size(), 40, "Forced grow");
    }

    ListTestSuite::ListTestSuite() : TestSuite("ListTestSuite")
    {
    }

    ListTestSuite::~ListTestSuite()
    {
    }

    void ListTestSuite::runTests()
    {
        test("AddTest", testAdd);
        test("RemoveTest", testRemove);
        test("Grow", testGrow);
    }
}
