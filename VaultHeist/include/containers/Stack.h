#include "DynArray.h"

template <typename S>

class Stack {
private:
	DynArray<S> arr;
	
public:
	Stack();
	~Stack();
	void push(S a);
	bool pop();
	S top();
	bool isEmpty();
	int size();
};

//implementation

//constructor and destructor

template <typename S>
Stack<S>::Stack() { arr = DynArray<S>(2); }

template <typename S>
Stack<S>::~Stack() {}

//push pop functions

template <typename S>
void Stack<S>::push(S a) { arr.pushBack(a); }

template <typename S>
bool Stack<S>::pop() { return arr.popBack(); }

//Getters

template <typename S>
S Stack<S>::top() { return arr.get(arr.size() - 1); }

template <typename S>
bool Stack<S>::isEmpty() { return arr.isEmpty(); }

template <typename S>
int Stack<S>::size() { return arr.size(); }