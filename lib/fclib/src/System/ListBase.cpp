#include <cstring>
#include "fclib/List.h"

using namespace FCLIB;

namespace FCLIB
{
    uint16 LIST_ITEM_PTR_SIZE = sizeof(void *);

    ListBase::ListBase(uint16 initSize)
    {
        capacity = initSize;
        used = 0;
        items = (void **)malloc(LIST_ITEM_PTR_SIZE * capacity);
        memset(items, 0, capacity * LIST_ITEM_PTR_SIZE);
    }
    ListBase::~ListBase()
    {
        // Logger log("ListBase");
        // log.showMemory("free list items");
        free(items);
        // log.showMemory("\tfreed list items");
    }

    uint16 ListBase::size() const
    {
        return used;
    }
    bool ListBase::contains(void *item) const
    {
        return indexOf(item) >= 0;
    }

    sint16 ListBase::indexOf(void *item) const
    {
        for (int i = 0; i < used && LoopTime::ok(); i++)
        {
            if (items[i] == item)
            {
                return i;
            }
        }
        LoopTime::check("ListBase::indexOf");
        return -1;
    }
    void *ListBase::removeAt(uint16 index)
    {
        if (index < 0 || index >= used)
        {
            return NULL;
        }
        void *result = items[index];

        // memcpy isn't working.  copy each pointer
        for (int i = index; i + 1 < used && LoopTime::ok(); i++)
        {
            items[i] = items[i + 1];
        }
        // memcpy(items + index * LIST_ITEM_PTR_SIZE, items + (index + 1) * LIST_ITEM_PTR_SIZE, (used - index) * LIST_ITEM_PTR_SIZE);
        used -= 1;
        LoopTime::check("ListBase::removeAt");

        return result;
    }
    void ListBase::clear()
    {
        used = 0;
    }

    void ListBase::insert_item(void *item)
    {
        if (used == capacity)
        {
            capacity += 20;
            void **newItems = (void **)malloc(capacity * LIST_ITEM_PTR_SIZE);
            memcpy(newItems, items, used * LIST_ITEM_PTR_SIZE);
            memset(newItems + used * LIST_ITEM_PTR_SIZE, 0, (capacity - used) * LIST_ITEM_PTR_SIZE);
            Logger log("ListBase");
            free(items);

            items = newItems;
        }
        items[used++] = item;
    }

    void ListBase::remove_item(void *item)
    {
        for (int i = 0; i < used && LoopTime::ok(); i++)
        {
            if (items[i] == item)
            {
                removeAt(i);
                return;
            }
        }
        LoopTime::check("ListBase::remove_item");
    }
    void *ListBase::get_item_at(uint16 index) const
    {
        if (index >= 0 && index < used)
        {
            return items[index];
        }
        return NULL;
    }
}