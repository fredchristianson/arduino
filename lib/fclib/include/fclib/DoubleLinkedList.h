#ifndef __FCLIB_DOUBLE_LINKED_LIST_H_
#define __FCLIB_DOUBLE_LINKED_LIST_H_
#include <c_types.h>
namespace FCLIB
{
    template <typename NodeType>
    class DoubleLinkedList;

    template <typename NodeType>
    class DoubleLinkedListNode
    {
    public:
        NodeType *getNext() { return nextNode; }
        NodeType *getPrev() { return prevNode; }

    protected:
        friend DoubleLinkedList<NodeType>;
        NodeType *nextNode;
        NodeType *prevNode;
    };

    template <typename NodeType>
    class DoubleLinkedList
    {
    public:
        DoubleLinkedList()
        {
            first = NULL;
            last = NULL;
        }

        void add(NodeType *node)
        {
            if (node == NULL)
            {
                return;
            }
            node->prevNode = last;
            node->nextNode = NULL;
            if (last != NULL)
            {
                last->nextNode = node;
            }
            if (first == NULL)
            {
                first = node;
            }
            last = node;
        }

        void remove(NodeType *node)
        {
            if (node == NULL)
            {
                return;
            }
            if (last == node)
            {
                last = node->prevNode;
            }
            if (first == node)
            {
                first = node->nextNode;
            }
            if (node->prevNode != NULL)
            {
                node->prevNode->nextNode = node->nextNode;
            }
            if (node->nextNode != NULL)
            {
                node->nextNode->prevNode = node->prevNode;
            }
        }

        NodeType *getFirst() { return first; }
        NodeType *getLast() { return last; }

    protected:
        NodeType *first;
        NodeType *last;
    };
}
#endif