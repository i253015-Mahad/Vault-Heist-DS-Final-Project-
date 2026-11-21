#include "../../include/heap/Heap.h"
#include <iostream>

using namespace std;

//constructors and destructor]

MaxHeap::MaxHeap()
{
	heap = new int[5];
	for (int i = 0; i < 5; i++)
		heap[i] = 0;
    capacity = 5;
    Size = operationCount = 0;
}

MaxHeap::MaxHeap(int* arr, int cap)
{
	heap = new int[cap];
	for (int i = 0; i < cap; i++)
		heap[i] = arr[i];

	capacity = cap;
	Size = cap;
	operationCount = 0;
}

MaxHeap::MaxHeap(const MaxHeap& other)
{
	if (other.heap == nullptr)
	{
		heap = nullptr;
		capacity = Size = operationCount = 0;
		return;
	}

	capacity = other.capacity;
	Size = other.Size;
	operationCount = other.operationCount;
	heap = new int[capacity];
	for (int i = 0; i < Size; i++)
		heap[i] = other.heap[i];
}

MaxHeap::~MaxHeap()
{
	if (heap == nullptr)
		return;
	delete[] heap;
	heap = nullptr;
}

//Heap Construction

void MaxHeap::buildMaxHeap()
{
	if (isEmpty())
		return;

	for (int i = Size / 2 - 1; i >= 0; i--)
		siftDown(i);
}

// Heap Operations

void MaxHeap::siftUp(int index)
{
	if (isEmpty())
		return;

	int i = index;
	while (i > 0)
	{
		int j = (i - 1) / 2;
		operationCount++;
		if (heap[i] <= heap[j])
			break;

		int temp = heap[i];
		heap[i] = heap[j];
		heap[j] = temp;
		operationCount++;
		i = j;
	}
}

void MaxHeap::siftDown(int index)
{
	if (isEmpty())
		return;

	int i = index, j = max(2 * i + 1, 2 * i + 2), temp;

	while ( j != -9999 && heap[i] < heap[j] && i != Size-1)
	{
		operationCount++;
		temp = heap[i];
		heap[i] = heap[j];
		heap[j] = temp;
		i = j;
		j = max(2 * i + 1, 2 * i + 2);
		operationCount++;
	}
}

int MaxHeap::extractMax()
{
	if (isEmpty())
		return - 9999;

	int temp = heap[0];
	heap[0] = heap[Size - 1];
	heap[Size - 1] = temp;
	operationCount++;
	Size--;
	siftDown(0);
	return heap[Size];
}

// Heap Sort

void MaxHeap::heapSort()
{
	if (isEmpty() || !isMaxHeap())
		return;

	buildMaxHeap();
	int i = Size;
	while (Size != 0)
		extractMax();

	Size = i;
}

// Validation

bool MaxHeap::isMaxHeap() const
{
	if (isEmpty())
		return true;

	int i = Size - 1, j = (i - 1) / 2;

	while (i != 0)
	{
		if (heap[i] > heap[j])
			return false;

		i--;
		j = (i - 1) / 2;
	}
	return true;
}

bool MaxHeap::isEmpty() const
{
	if (Size == 0 || heap == nullptr)
		return true;

	return false;
}

bool MaxHeap::isFull() const
{
	if (Size == capacity)
		return true;

	return false;
}

bool MaxHeap::isSortedAscending() const
{
	if (isEmpty())
		return false;

	for (int i = 0; i < Size-1; i++)
	{
		if (heap[i] > heap[i + 1])
			return false;
	}
	return true;
}

// Getters

int MaxHeap::getMax() const { return (isEmpty()) ? -9999 : heap[0]; }
int MaxHeap::getSize() const { return Size; }
int MaxHeap::getCapacity() const { return capacity; }
int MaxHeap::getOperationCount() const { return operationCount; }
int MaxHeap::getElement(int index) const { return (heap == nullptr || index < 0 || index >= capacity) ? -9999 : heap[index]; }
int MaxHeap::max(int a, int b) const
{
	if (a >= Size && b >= Size)
		return -9999;
	else if (b >= Size)
		return a;
	else if (a >= Size)
		return b;
	return (heap[a] >= heap[b]) ? a : b;
}

// Setters

void MaxHeap::insert(int value)
{
	if (!isEmpty() && isFull())
		return;

	heap[Size] = value;
	Size++;
	siftUp(Size - 1);
}

void MaxHeap::setElement(int index, int value)
{
	if (isEmpty() || index >= Size)
		return;

	heap[index] = value;
}

void MaxHeap::resetOperationCount() { operationCount = 0; }

void MaxHeap::setSize(int newSize) { Size = newSize; }

// working

void MaxHeap::swapElements(int i, int j)
{
	if (isEmpty() || i >= Size || j >= Size )
		return;

	int temp = heap[i];
	heap[i] = heap[j];
	heap[j] = temp;
}

void MaxHeap::display() const
{
	if (isEmpty())
		return;
	for (int i = 0; i < Size; i++)
		cout << heap[i] << " ";

	cout << "\n";
}
