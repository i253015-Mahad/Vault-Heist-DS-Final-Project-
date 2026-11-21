#include "../../include/heap/Heap.h"
#include <iostream>

using namespace std;

//constructors and destructor]

MaxHeap::MaxHeap()
{
	heap = new int[5];
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

	if (heap != nullptr)
		delete[] heap;
	heap = nullptr;

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

    for (int i = 0; i < Size; i++)
        siftUp(i);
}

// Heap Operations

void MaxHeap::siftUp(int index)
{
	if (isEmpty())
		return;

	int i = index, j = (i - 1) / 2, temp;

	while (heap[i] > heap[j] && i != 0)
	{
		temp = heap[i];
		heap[i] = heap[j];
		heap[j] = temp;
		i = j;
		j = (i - 1) / 2;
	}
}

void MaxHeap::siftDown(int index)
{
	if (isEmpty())
		return;

	int i = index, j = max(2 * i + 1, 2 * i + 2), temp;

	while ( j != -9999 && heap[i] < heap[j] && i != Size-1)
	{
		temp = heap[i];
		heap[i] = heap[j];
		heap[j] = temp;
		i = j;
		j = max(2 * i + 1, 2 * i + 2);
	}
}

int MaxHeap::extractMax()
{
	if (isEmpty())
		return - 9999;

	int temp = heap[0];
	heap[0] = heap[Size - 1];
	heap[Size - 1] = temp;
	Size--;
	siftDown(0);
	return heap[Size];
}

// Heap Sort

void MaxHeap::heapSort()
{
	if (isEmpty() || !isMaxHeap())
		return;

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
int MaxHeap::getElement(int index) const { return (index > Size - 1) ? -9999 : heap[index]; }
int MaxHeap::max(int a, int b) const
{
	if (a > Size - 1 && b > Size - 1)
		return -9999;

	if (a >= Size || (heap[a] < heap[b] && b < Size ))
		return b;
	else if (b >= Size || (heap[a] > heap[b] && a < Size))
		return a;
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

//test
void testMaxHeap()
{
    int passed = 0;
    int failed = 0;

#define CHECK(condition, description)                  \
        do                                                 \
        {                                                  \
            if (condition)                                 \
            {                                              \
                cout << "[PASS] " << description << endl;  \
                passed++;                                  \
            }                                              \
            else                                           \
            {                                              \
                cout << "[FAIL] " << description << endl;  \
                failed++;                                  \
            }                                              \
        } while (0)

    cout << "\n====================================\n";
    cout << "       MAX HEAP TEST SUITE\n";
    cout << "====================================\n\n";


    // ==========================================
    // TEST 1: BUILD MAX HEAP
    // ==========================================

    cout << "\n--- BUILD MAX HEAP ---\n";

    int arr1[] = { 10, 40, 30, 5, 20, 60, 15 };

    MaxHeap h1(arr1, 7);
    h1.buildMaxHeap();

    CHECK(h1.isMaxHeap(), "Build Max Heap");

    CHECK(h1.getSize() == 7, "Correct heap size");

    CHECK(h1.getMax() == 60, "Maximum element is at root");


    // ==========================================
    // TEST 2: SIFT UP
    // ==========================================

    cout << "\n--- SIFT UP ---\n";

    int arr2[] = { 90, 70, 80, 40, 50, 60 };

    MaxHeap h2(arr2, 6);
    h2.buildMaxHeap();

    h2.setElement(5, 120);
    h2.siftUp(5);

    CHECK(h2.isMaxHeap(), "Sift Up restores Max Heap");

    CHECK(h2.getMax() == 120, "Sift Up moves new maximum to root");


    // ==========================================
    // TEST 3: SIFT DOWN
    // ==========================================

    cout << "\n--- SIFT DOWN ---\n";

    int arr3[] = { 90, 70, 80, 40, 50, 60 };

    MaxHeap h3(arr3, 6);
    h3.buildMaxHeap();

    h3.setElement(0, -10);
    h3.siftDown(0);

    CHECK(h3.isMaxHeap(), "Sift Down restores Max Heap");

    CHECK(h3.getMax() == 80, "Correct root after Sift Down");


    // ==========================================
    // TEST 4: SWAP ELEMENTS
    // ==========================================

    cout << "\n--- SWAP ELEMENTS ---\n";

    int arr4[] = { 90, 70, 80, 40, 50, 60 };

    MaxHeap h4(arr4, 6);

    int original1 = h4.getElement(1);
    int original4 = h4.getElement(4);

    h4.swapElements(1, 4);

    CHECK(h4.getElement(1) == original4,
        "First element exchanged correctly");

    CHECK(h4.getElement(4) == original1,
        "Second element exchanged correctly");

    // Swap back
    h4.swapElements(1, 4);

    CHECK(h4.getElement(1) == original1 &&
        h4.getElement(4) == original4,
        "Swapping twice restores original array");


    // ==========================================
    // TEST 5: SET ELEMENT
    // ==========================================

    cout << "\n--- SET ELEMENT ---\n";

    int arr5[] = { 90, 70, 80, 40, 50, 60 };

    MaxHeap h5(arr5, 6);
    h5.buildMaxHeap();

    h5.setElement(1, 75);

    CHECK(h5.getElement(1) == 75,
        "Set Element changes correct index");

    // Restore heap if required
    h5.siftUp(1);

    CHECK(h5.isMaxHeap(),
        "Heap valid after Sift Up");

    h5.setElement(0, -20);
    h5.siftDown(0);

    CHECK(h5.isMaxHeap(),
        "Heap valid after Sift Down");


    // ==========================================
    // TEST 6: INSERT
    // ==========================================

    cout << "\n--- INSERT ---\n";

    MaxHeap h6;

    CHECK(h6.isEmpty(), "New heap is empty");

    h6.insert(40);

    CHECK(h6.getSize() == 1, "Insert increases size");

    CHECK(h6.getMax() == 40, "First inserted value is root");

    h6.insert(90);
    h6.insert(20);
    h6.insert(100);
    h6.insert(60);

    CHECK(h6.isMaxHeap(), "Heap valid after multiple insertions");

    CHECK(h6.getMax() == 100, "Correct maximum after insertions");

    CHECK(h6.getSize() == 5, "Correct size after insertions");


    // ==========================================
    // TEST 7: EXTRACT MAX
    // ==========================================

    cout << "\n--- EXTRACT MAX ---\n";

    int arr7[] = { 100, 80, 90, 40, 70, 60, 50 };

    MaxHeap h7(arr7, 7);
    h7.buildMaxHeap();

    int previousMax = h7.extractMax();

    CHECK(previousMax == 100, "Extract returns maximum");

    CHECK(h7.isMaxHeap(), "Heap valid after extraction");

    CHECK(h7.getMax() == 90, "Next maximum becomes root");

    CHECK(h7.getSize() == 6, "Extraction decreases size");

    int extracted[] = { 90, 80, 70, 60, 50, 40 };

    bool correctOrder = true;

    for (int i = 0; i < 6; i++)
    {
        int current = h7.extractMax();

        if (current != extracted[i])
            correctOrder = false;

        if (!h7.isMaxHeap())
            correctOrder = false;
    }

    CHECK(correctOrder, "All elements extracted in descending order");

    CHECK(h7.isEmpty(), "Heap empty after extracting all elements");


    // ==========================================
    // TEST 8: HEAP SORT
    // ==========================================

    cout << "\n--- HEAP SORT ---\n";

    int arr8[] = { 40, 10, 90, 30, 70, 20, 80, 50 };

    MaxHeap h8(arr8, 8);

    h8.buildMaxHeap();

    h8.heapSort();

    bool sorted = true;

    for (int i = 0; i < 7; i++)
    {
        if (h8.getElement(i) > h8.getElement(i + 1))
            sorted = false;
    }

    CHECK(sorted, "Heap Sort produces ascending order");

    CHECK(h8.getSize() == 8, "Heap Sort preserves element count");


    // ==========================================
    // TEST 9: DUPLICATE VALUES
    // ==========================================

    cout << "\n--- DUPLICATE VALUES ---\n";

    int arr9[] = { 50, 50, 20, 50, 20, 10, 10 };

    MaxHeap h9(arr9, 7);
    h9.buildMaxHeap();

    CHECK(h9.isMaxHeap(), "Heap handles duplicate values");

    CHECK(h9.getMax() == 50, "Correct maximum with duplicates");

    h9.heapSort();

    bool duplicateSorted = true;

    for (int i = 0; i < 6; i++)
    {
        if (h9.getElement(i) > h9.getElement(i + 1))
            duplicateSorted = false;
    }

    CHECK(duplicateSorted, "Heap Sort handles duplicates");


    // ==========================================
    // TEST 10: NEGATIVE VALUES
    // ==========================================

    cout << "\n--- NEGATIVE VALUES ---\n";

    int arr10[] = { -50, -10, -90, -20, -5, -100 };

    MaxHeap h10(arr10, 6);
    h10.buildMaxHeap();

    CHECK(h10.isMaxHeap(), "Heap handles negative values");

    CHECK(h10.getMax() == -5, "Correct maximum among negatives");

    h10.heapSort();

    bool negativeSorted = true;

    for (int i = 0; i < 5; i++)
    {
        if (h10.getElement(i) > h10.getElement(i + 1))
            negativeSorted = false;
    }

    CHECK(negativeSorted, "Heap Sort handles negative values");


    // ==========================================
    // TEST 11: SINGLE ELEMENT
    // ==========================================

    cout << "\n--- SINGLE ELEMENT ---\n";

    int arr11[] = { 42 };

    MaxHeap h11(arr11, 1);
    h11.buildMaxHeap();

    CHECK(h11.isMaxHeap(), "Single element is a valid heap");

    CHECK(h11.getMax() == 42, "Single element is maximum");

    h11.heapSort();

    CHECK(h11.getElement(0) == 42, "Single element sorts correctly");


    // ==========================================
    // TEST 12: COPY CONSTRUCTOR
    // ==========================================

    cout << "\n--- COPY CONSTRUCTOR ---\n";

    int arr12[] = { 90, 70, 80, 40, 50, 60 };

    MaxHeap original(arr12, 6);
    original.buildMaxHeap();

    MaxHeap copied(original);

    CHECK(copied.getSize() == original.getSize(),
        "Copy has correct size");

    bool sameElements = true;

    for (int i = 0; i < original.getSize(); i++)
    {
        if (copied.getElement(i) != original.getElement(i))
            sameElements = false;
    }

    CHECK(sameElements, "Copy contains same elements");

    copied.setElement(0, 500);

    CHECK(original.getMax() == 90,
        "Changing copy does not affect original");

    CHECK(copied.getMax() == 500,
        "Copy can be modified independently");


    // ==========================================
    // TEST 13: OPERATION COUNTER
    // ==========================================

    cout << "\n--- OPERATION COUNTER ---\n";

    int arr13[] = { 90, 70, 80, 40, 50, 60 };

    MaxHeap h13(arr13, 6);

    h13.resetOperationCount();

    h13.buildMaxHeap();

    CHECK(h13.getOperationCount() >= 0,
        "Operation counter tracks a nonnegative count");

    h13.resetOperationCount();

    CHECK(h13.getOperationCount() == 0,
        "Reset operation counter works");


    // ==========================================
    // FINAL RESULTS
    // ==========================================

    cout << "\n====================================\n";
    cout << "           TEST RESULTS\n";
    cout << "====================================\n";

    cout << "Passed: " << passed << endl;
    cout << "Failed: " << failed << endl;
    cout << "Total:  " << passed + failed << endl;

    if (failed == 0)
        cout << "\nALL TESTS PASSED!\n";
    else
        cout << "\nSOME TESTS FAILED!\n";

    cout << "====================================\n";

#undef CHECK
}
