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
        free(items);
    }

    uint16 ListBase::size()
    {
        return used;
    }
    bool ListBase::contains(void *item)
    {
        return indexOf(item) >= 0;
    }
    uint16 ListBase::indexOf(void *item)
    {
        for (int i = 0; i < used; i++)
        {
            if (items[i] == item)
            {
                return i;
            }
        }
        return -1;
    }
    void *ListBase::removeAt(uint16 index)
    {
        void *result = NULL;
        if (index + 1 < used)
        {
            result = items[index];
            memcpy(items + index * LIST_ITEM_PTR_SIZE, items + (index + 1) * LIST_ITEM_PTR_SIZE, (used - index - 1) * LIST_ITEM_PTR_SIZE);
        }
        used -= 1;
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
            free(items);
            items = newItems;
        }
        items[used++] = item;
    }

    void ListBase::remove_item(void *item)
    {
        for (int i = 0; i < used; i++)
        {
            if (items[i] == item)
            {
                removeAt(i);
                return;
            }
        }
    }
    void *ListBase::get_item_at(uint16 index)
    {
        if (index >= 0 && index < used)
        {
            return items[index];
        }
        return NULL;
    }
}