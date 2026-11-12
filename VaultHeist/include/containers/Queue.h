template <typename Q>

class Queue {
private:
	Q* arr;
	int front;
	int rear;
	int cap;

public:
	Queue();
	~Queue();
	void enqueue(Q a);
	bool dequeue();
	Q Front();
	bool isEmpty();
	int size();
};

//implementation

//constructor and destructor
template <typename Q>
Queue<Q>::Queue()
{
	front = rear = -1;
	cap = 4;
	arr = new Q[cap];
}

template <typename Q>
Queue<Q>::~Queue()
{
	if (arr == nullptr)
		return;
	delete[] arr;
	arr = nullptr;
}

//enqueue and dequeue

template <typename Q>
void Queue<Q>::enqueue(Q a)
{
	if (front == -1 && rear == -1)
		front++;

	if (rear == cap)
	{
		cap *= 2;
		Q* t = new Q[cap];
		for (int i = front; i <= rear; i++)
			t[i] = arr[i];
		delete[] arr;
		arr = t;
		t = nullptr;
	}

	rear++;
	arr[rear] = a;
}

template <typename Q>
bool Queue<Q>::dequeue()
{
	if (front == -1 && rear == -1 || arr == nullptr)
		return false;
	if (front == rear)
	{
		front = rear = -1;
		return true;
	}

	front++;
	return true;
}

//getters

template <typename Q>
Q Queue<Q>::Front()
{
	if (front != -1 && arr != nullptr)
		return arr[front];

	Q x;
	return x;
}

template <typename Q>
bool Queue<Q>::isEmpty()
{
	if (front == -1 && rear == -1 || arr == nullptr)
		return true;
	return false;
}

template <typename Q>
int Queue<Q>::size()
{
	if (front == -1 && rear == -1 || arr == nullptr)
		return 0;
	return rear - front;
}