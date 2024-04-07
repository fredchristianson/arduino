#include "fclib/List.h"

using namespace FCLIB::TEST;
using namespace FCLIB::TEST::LIST;

namespace FCLIB::TEST::LIST
{
    class Item
    {
    public:
        Item(int v = 0) { val = v; }

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
        for (int i = 0; i < 67; i++)
        {
            list.add(&item);
        }

        result.equal(list.size(), 67, "Forced grow");
    }

    void testContains(TestResult &result)
    {
        List<Item> list;
        for (int i = 0; i < 55; i++)
        {
            list.add(new Item(i));
        }
        result.equal(list.size(), 55, "Forced grow");
        Item notInList;
        result.isFalse(list.contains(&notInList), "contains() item not in list");
        result.isTrue(list.contains(list[0]), "contains() item is in list");
        result.isTrue(list.contains(list[50]), "contains() item is in list");
        list.deleteAll();
    }

    void testIndexOf(TestResult &result)
    {
        List<Item> list;
        for (int i = 0; i < 55; i++)
        {
            list.add(new Item(i));
        }
        Item notInList;
        result.equal(list.indexOf(&notInList), -1, "indexOf() item not in list");
        result.equal(list.indexOf(list[0]), 0, "indexOf() first item is 0");
        result.equal(list.indexOf(list[50]), 50, "indexOf() 50th item is 50");
        result.equal(list.indexOf(list[54]), 54, "indexOf() 54th item is 54");
        list.deleteAll();
    }

    void testRemoveAt(TestResult &result)
    {
        List<Item> list;
        for (int i = 0; i < 55; i++)
        {
            list.add(new Item(i));
        }
        result.equal(list.size(), 55, "size is 55");

        result.null(list.removeAt(-1), "RemoveAt -1 return NULL");
        result.null(list.removeAt(55), "RemoveAt 55 return NULL");
        result.notNull(list.getAt(54), "item 54 is not NULL");

        Item *item = (Item *)list.removeAt(0);
        result.notNull(item, "removeAt(0) is not NULL");
        result.equal(item->val, 0, "removeAt(0) value is 0");
        delete item;
        result.equal(list.size(), 54, "size is 54");
        result.null(list.getAt(54), "item 54 is  NULL");
        result.notNull(list.getAt(53), "item 53 is not  NULL");

        result.equal(list.getAt(0)->val, 1, "First item is now #1");
        result.equal(list.getAt(50)->val, 51, "50th item is now #51");
        result.equal(list.getAt(53)->val, 54, "53rd item is now #54");

        item = (Item *)list.removeAt(53);
        result.equal(list.size(), 53, "size is 53");
        result.notNull(item, "removeAt(53) is not NULL");
        result.equal(item->val, 54, "removeAt(53) value is 54");
        result.equal(list.size(), 53, "List size is 53");
        delete item;

        list.deleteAll();
    }

    void testRemoveIf(TestResult &result)
    {
        List<Item> list;
        List<Item> all;
        for (int i = 0; i < 55; i++)
        {
            Item *item = new Item(i);
            list.add(item);
            all.add(item);
        }
        list.removeIf([](Item *item)
                      { return (item->val % 2) == 0; });
        Item *match = list.first([](Item *item)
                                 { return item->val == 9; });
        result.isTrue(match != NULL && match->val == 9, "Found item #9");

        Item *even = list.first([](Item *item)
                                { return (item->val % 2) == 0; });
        result.null(even, "No even items found");
        all.deleteAll();
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
        test("Contains", testContains);
        test("IndexOf", testIndexOf);
        test("RemoveAt", testRemoveAt);
        test("RemoveIf", testRemoveIf);
    }
}
