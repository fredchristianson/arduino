#ifndef _FCLIB_LIST_H_
#define _FCLIB_LIST_H_
#include <c_types.h>
#include <fclib/Callable.h>
#include "fclib/Test.h"

using namespace FCLIB;

namespace FCLIB
{
    class ListBase
    {
    public:
        ListBase(uint16 initSize = 20);
        virtual ~ListBase();

        uint16 size();
        bool contains(void *item) const;
        sint16 indexOf(void *item) const;
        void *removeAt(uint16 index);
        void clear();

    protected:
        void insert_item(void *item);
        void remove_item(void *item);
        void *get_item_at(uint16 index) const;
        void **items;
        uint16 capacity;
        uint16 used;
    };

    template <typename T>
    class List : public ListBase
    {
    public:
        List(uint16 initSize = 20) : ListBase(initSize) {}
        virtual ~List() {}

        void add(T *item) { insert_item(item); }
        void remove(T *item) { remove_item(item); }
        T *remove(uint16 index)
        {
            T *old = (T *)getAt(index);
            removeAt(index);
            return old;
        }
        T *getAt(uint16 index) { return (T *)get_item_at(index); }
        T *operator[](uint16 index) { return getAt(index); }
        void forEach(Callback<T *> callback)
        {
            for (int i = 0; i < size(); i++)
            {
                callback((T *)getAt(i));
            };
        };

        T *first(TestCallback<T *> match)
        {
            for (uint16 i = 0; i < size(); i++)
            {
                if (match((T *)getAt(i)))
                {
                    return (T *)getAt(i);
                }
                return NULL;
            }
        }

        void removeIf(TestCallback<T *> match)
        {
            for (uint16 i = 0; i < size(); i++)
            {
                if (match((T *)getAt(i)))
                {
                    removeAt(i);
                    i--;
                }
            }
        }
    };
    namespace TEST
    {
        namespace LIST
        {
            class ListTestSuite : public TestSuite
            {
            public:
                ListTestSuite();
                virtual ~ListTestSuite();

            protected:
                virtual void runTests() override;
                void run();
            };
        }
    }
}
#endif