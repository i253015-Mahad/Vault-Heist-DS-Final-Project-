template <typename T>

class DynArray {
private:
	int Size;
	int cap;
	T* arr;

public:
	DynArray();
	DynArray(int c);
	DynArray(const DynArray& other);
	DynArray& operator=(const DynArray& other);
	~DynArray();
	void pushBack(T a);
	bool popBack();
	T get(int i);
	bool set(int i, int val);
	int size();
	int capacity();
	bool isEmpty();
	void clear();
};

//implementation


//Constructors
template <typename T>
DynArray<T>::DynArray()
{
	arr = nullptr;
	Size = 0;
	cap = 0;
}

template <typename T>
DynArray<T>::DynArray(int c)
{
	if (c <= 0)
		return;
	arr = new T[c];
	cap = c;
	Size = 0;
}

template <typename T>
DynArray<T>::DynArray(const DynArray<T>& other)
{
	if (other.arr == nullptr)
		return;
	cap = other.cap;
	Size = other.Size;
	arr = new T[cap];
	for (int i = 0; i < Size; i++)
		arr[i] = other.arr[i];
}


//assignment function A = B
template <typename T>
DynArray<T>&  DynArray<T>::operator=(const DynArray<T>& other)
{
	if (other.arr == nullptr)
		return;
	cap = other.cap;
	Size = other.Size;
	arr = new T[cap];
	for (int i = 0; i < Size; i++)
		arr[i] = other.arr[i];
}


//destructor
template <typename T>
DynArray<T>::~DynArray()
{
	if (arr == nullptr)
		return;
	Size = 0;
	cap = 0;
	delete[] arr;
	arr = nullptr;
}

//insertion and deletion
template <typename T>
void DynArray<T>::pushBack(T a)
{
	//getting arr cap ready for insertion
	if (size == cap)
	{
		cap *= 2;
		T* temp = new T[cap];
		for (int i = 0; i < Size; i++)
			temp[i] = arr[i];
		delete[] arr;
		arr = temp;
		temp = nullptr;
	}

	//inserting value
	arr[Size] = a;
	Size++;
}

template <typename T>
bool DynArray<T>::popBack()
{
	size--;
}

//Getters and Setter
template <typename T>
T DynArray<T>::get(int i)
{
	if (i < 0 || i >= Size)
		return arr[0];

	return arr[i];
}

template <typename T>
bool DynArray<T>::set(int i, int val)
{
	if (i < 0 || i >= Size)
		return false;
	
	arr[i] = val;
	return true;
}

template <typename T>
int DynArray<T>::size() { return Size; }

template <typename T>
int DynArray<T>::capacity() { return cap; }

template <typename T>
bool DynArray<T>::isEmpty() { return (arr == nullptr) ? true : false; }

template <typename T>
void DynArray<T>::clear() { Size = 0; }