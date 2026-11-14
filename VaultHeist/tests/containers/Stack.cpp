#include <iostream>
#include "../../containers/Stack.h"

using namespace std;

void testStack()
{
    cout << "\n===== STACK TEST =====\n";

    Stack<int> s;

    // Test initial state
    cout << "Initially empty: " << s.isEmpty() << " (Expected: 1)\n";
    cout << "Initial size: " << s.size() << " (Expected: 0)\n";

    // Test push
    s.push(10);
    s.push(20);
    s.push(30);

    cout << "\nAfter pushing 10, 20, 30:\n";
    cout << "Size: " << s.size() << " (Expected: 3)\n";
    cout << "Top: " << s.top() << " (Expected: 30)\n";

    // Test pop
    s.pop();

    cout << "\nAfter one pop:\n";
    cout << "Top: " << s.top() << " (Expected: 20)\n";
    cout << "Size: " << s.size() << " (Expected: 2)\n";

    // Test remaining elements
    s.pop();
    cout << "\nAfter second pop:\n";
    cout << "Top: " << s.top() << " (Expected: 10)\n";

    s.pop();

    cout << "\nAfter removing all elements:\n";
    cout << "Is empty: " << s.isEmpty() << " (Expected: 1)\n";
    cout << "Size: " << s.size() << " (Expected: 0)\n";

    // Test pop on empty stack
    cout << "Pop empty stack: " << s.pop() << " (Expected: 0)\n";

    // Test reuse after emptying
    s.push(100);

    cout << "\nAfter pushing 100 into empty stack:\n";
    cout << "Top: " << s.top() << " (Expected: 100)\n";
    cout << "Size: " << s.size() << " (Expected: 1)\n";

    cout << "\n===== STACK TEST COMPLETE =====\n";
}