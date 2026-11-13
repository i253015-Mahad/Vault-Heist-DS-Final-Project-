#include <iostream>
#include "../../containers/Queue.h"

using namespace std;

void testQueue()
{
    Queue<int> q;

    cout << "Initial state:\n";
    cout << "Size: " << q.size() << endl;
    cout << "Empty: " << q.isEmpty() << endl;

    cout << "\nEnqueuing 10, 20, 30, 40, 50...\n";
    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    q.enqueue(40);
    q.enqueue(50);

    cout << "Size: " << q.size() << endl;
    cout << "Front: " << q.Front() << endl;

    cout << "\nDequeuing...\n";
    q.dequeue();
    cout << "Front: " << q.Front() << endl;

    q.dequeue();
    cout << "Front: " << q.Front() << endl;

    cout << "\nEnqueuing 60 and 70...\n";
    q.enqueue(60);
    q.enqueue(70);

    cout << "Size: " << q.size() << endl;
    cout << "Front: " << q.Front() << endl;

    cout << "\nRemoving all elements...\n";
    while (!q.isEmpty())
    {
        cout << "Removed: " << q.Front() << endl;
        q.dequeue();
    }

    cout << "Size: " << q.size() << endl;
    cout << "Empty: " << q.isEmpty() << endl;

    cout << "\nTrying to dequeue from empty queue:\n";
    cout << "Dequeue successful: " << q.dequeue() << endl;

    cout << "\nEnqueuing after emptying...\n";
    q.enqueue(100);
    cout << "Front: " << q.Front() << endl;
    cout << "Size: " << q.size() << endl;
}