#include <iostream>
#include "../../containers/DynArray.h"

using namespace std;

void testDynArray()
{
	cout << "\n===== DYNAMIC ARRAY TEST =====\n";

	DynArray<int> arr(2);

	// Test initial state
	cout << "Initial size: " << arr.size() << " (Expected: 0)\n";
	cout << "Initial capacity: " << arr.capacity() << " (Expected: 2)\n";
	cout << "Initially empty: " << arr.isEmpty() << " (Expected: 1)\n";

	// Test insertion
	arr.pushBack(10);
	arr.pushBack(20);
	arr.pushBack(30);

	cout << "\nAfter inserting 10, 20, 30:\n";
	cout << "Size: " << arr.size() << " (Expected: 3)\n";
	cout << "Capacity: " << arr.capacity() << " (Expected: 4)\n";

	// Test get
	cout << "Element at index 0: " << arr.get(0) << " (Expected: 10)\n";
	cout << "Element at index 1: " << arr.get(1) << " (Expected: 20)\n";
	cout << "Element at index 2: " << arr.get(2) << " (Expected: 30)\n";

	// Test set
	arr.set(1, 50);
	cout << "\nAfter set(1, 50): " << arr.get(1) << " (Expected: 50)\n";

	// Test popBack
	arr.popBack();
	cout << "After popBack, size: " << arr.size() << " (Expected: 2)\n";
	cout << "Last element: " << arr.get(1) << " (Expected: 50)\n";

	// Test copy constructor
	DynArray<int> copy(arr);

	cout << "\nCopy constructor:\n";
	cout << "Copy size: " << copy.size() << " (Expected: 2)\n";
	cout << "Copy element 0: " << copy.get(0) << " (Expected: 10)\n";
	cout << "Copy element 1: " << copy.get(1) << " (Expected: 50)\n";

	// Test independent copies
	copy.set(0, 999);
	cout << "Original element 0: " << arr.get(0) << " (Expected: 10)\n";

	// Test assignment operator
	DynArray<int> assigned;
	assigned = arr;

	cout << "\nAssignment operator:\n";
	cout << "Assigned size: " << assigned.size() << " (Expected: 2)\n";
	cout << "Assigned element 0: " << assigned.get(0) << " (Expected: 10)\n";
	cout << "Assigned element 1: " << assigned.get(1) << " (Expected: 50)\n";

	// Test clear
	arr.clear();

	cout << "\nAfter clear:\n";
	cout << "Size: " << arr.size() << " (Expected: 0)\n";
	cout << "Is empty: " << arr.isEmpty() << " (Expected: 1)\n";
	cout << "Capacity: " << arr.capacity() << " (Expected: 4)\n";

	// Test pop on empty array
	cout << "Pop empty array: " << arr.popBack() << " (Expected: 0)\n";

	cout << "\n===== DYNAMIC ARRAY TEST COMPLETE =====\n";
}