

//This code was taken from github.com/SomethingWithComputers
//It is a Binary Heap wrapper for UE's TArray
//https://gist.github.com/SomethingWithComputers/48e32b281d1bf8e662412e4113df43dc

#pragma once

#include "CoreMinimal.h"


    template <typename InElementType>
    struct TPriorityQueueNode {
        InElementType Element;
        float Priority;

        TPriorityQueueNode()
        {
        }

        TPriorityQueueNode(InElementType InElement, double InPriority)
        {
            Element = InElement;
            Priority = InPriority;
        }

        bool operator<(const TPriorityQueueNode<InElementType> Other) const
        {
            return Priority < Other.Priority;
        }
    };

    template <typename InElementType>
    class TPriorityQueue {
    public:
        TPriorityQueue()
        {
            Array.Heapify();
        }

    public:
        // Always check if IsEmpty() before Pop-ing!
        InElementType Pop()
        {
            TPriorityQueueNode<InElementType> Node;
            Array.HeapPop(Node);
            return Node.Element;
        }

        TPriorityQueueNode<InElementType> PopNode()
        {
            TPriorityQueueNode<InElementType> Node;
            Array.HeapPop(Node);
            return Node;
        }

        void Push(InElementType Element, double Priority)
        {
            Array.HeapPush(TPriorityQueueNode<InElementType>(Element, Priority));
        }

        bool IsEmpty() const
        {
            return Array.Num() == 0;
        }


    private:
        TArray<TPriorityQueueNode<InElementType>> Array;
    };

