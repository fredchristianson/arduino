#ifndef _FCLIB_LIST_H_
#define _FCLIB_LIST_H_
#include <c_types.h>
#include <fclib/Callable.h>
#include "fclib/Test.h"
#include "fclib/Loop.h"

using namespace FCLIB;

namespace FCLIB
{
    class ListBase
    {
    public:
        ListBase(uint16 initSize = 20);
        virtual ~ListBase();

        uint16 size() const;
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

        void add(T *item) { insert_item((void *)item); }
        void remove(T *item) { remove_item(item); }
        T *remove(uint16 index)
        {
            T *old = (T *)getAt(index);
            removeAt(index);
            return old;
        }
        T *getAt(uint16 index) { return (T *)get_item_at(index); }
        const T *getAt(uint16 index) const { return (T *)get_item_at(index); }
        T *operator[](uint16 index) { return getAt(index); }
        const T *operator[](uint16 index) const { return getAt(index); }
        void forEach(Callback<T *> callback)
        {
            for (int i = 0; i < size() && LoopTime::ok(); i++)
            {
                callback((T *)getAt(i));
            };
            LoopTime::check("forEach");
        };

        void forEach(Callback<T *> callback) const
        {
            for (int i = 0; i < size() && LoopTime::ok(); i++)
            {
                callback((const T *)getAt(i));
            };
            LoopTime::check("forEach");
        };

        T *first(BoolCallback<T *> match)
        {
            for (uint16 i = 0; i < size() && LoopTime::ok(); i++)
            {
                if (match((T *)getAt(i)))
                {
                    return (T *)getAt(i);
                }
            }
            LoopTime::check("first");

            return NULL;
        }

        bool any(BoolCallback<T *> match) const
        {
            for (uint16 i = 0; i < size() && LoopTime::ok(); i++)
            {
                if (match((T *)getAt(i)))
                {
                    return true;
                }
            }
            LoopTime::check("any");

            return false;
        }

        void removeIf(BoolCallback<T *> match, Callback<T *> onRemove = NULL)
        {
            for (uint16 i = 0; i < size() && LoopTime::ok(); i++)
            {
                if (match((T *)getAt(i)))
                {
                    if (onRemove != NULL)
                    {
                        onRemove((T *)getAt(i));
                    }
                    removeAt(i);
                    i--;
                }
            }
            LoopTime::check("removeIf");
        }

        void deleteAll()
        {
            for (uint16 i = 0; i < size() && LoopTime::ok(); i++)
            {
                delete getAt(i);
            }
            LoopTime::check("deleteAll");
            clear();
        }

    private:
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