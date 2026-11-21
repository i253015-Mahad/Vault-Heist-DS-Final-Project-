#pragma once

class MaxHeap
{
private:
    int* heap;
    int capacity;
    int Size;
    int operationCount;

public:
    // Constructors and Destructor
    MaxHeap();
    MaxHeap(int* arr, int cap);
    MaxHeap(const MaxHeap& other);
    ~MaxHeap();

    // Heap Construction
    void buildMaxHeap();

    // Heap Operations
    void siftUp(int index);
    void siftDown(int index);
    int extractMax();

    // Heap Sort
    void heapSort();

    // Validation
    bool isMaxHeap() const;
    bool isEmpty() const;
    bool isFull() const;
    bool isSortedAscending() const;

    // Getters
    int getMax() const;
    int getSize() const;
    int getCapacity() const;
    int getOperationCount() const;
    int getElement(int index) const;
    int max(int a, int b) const;

    // Setters
    void insert(int value);
    void setElement(int index, int value);
    void resetOperationCount();

    // working
    void swapElements(int i, int j);
    void display() const;
};

void testMaxHeap();