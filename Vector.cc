// Implementation of the templated Vector class
// ECE4122/6122 Assignment 5
// Yushan Cai

#include <iostream> // debugging
#include "Vector.h"

using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector()
{
	elements = NULL;
	count = 0;
	reserved = 0;
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs)
{
	count = rhs.count;
	reserved = rhs.reserved;
	T * temp = (T*) malloc(reserved * sizeof(T));
	for (int i = 0; i < count; i++)
	{
		new (&temp[i]) T(rhs.elements[i]);
	}
	elements = temp; // point elements to the newly allocated and copied memory
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
	if (this != &rhs) // in case assigning to itself
	{
		if (elements != NULL) // free original
		{
			for (int i = 0; i < count; ++i)
			{
				elements[i].~T();
			}
			free(elements);
		}
		count = rhs.count;
		reserved = rhs.reserved;
		T * temp = (T*) malloc(reserved * sizeof(T));
		for (int i = 0; i < count; i++)
		{
			new (&temp[i]) T(rhs.elements[i]);
		}
		elements = temp; // assign new
	}
	return *this;
}

// new T, ~T() and malloc, free always pair together

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved)
{ // Initialize with reserved memory
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
{ // Initialize with "n" copies of "t"
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{
}
#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
	if (elements != NULL) // only free list if it exists
	{
		for (int i = 0; i < count; ++i)
		{
			elements[i].~T();
		}
		free(elements);
	}
	count = 0;
	reserved =0;
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
	if (reserved == count)
	{
		reserved++;
		T * temp = (T*) malloc(reserved * sizeof(T));
		for (int i = 0; i < count; ++i)
		{
			new (&temp[i]) T(elements[i]);
			elements[i].~T();
		}
		free(elements);
		elements = temp;
	} // copy over original to new reserved space
	new (&elements[count++]) T(rhs); // add new element to the end
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
	if (reserved == count)
	{
		reserved++;
		T * temp = (T*) malloc(reserved * sizeof(T));
		for (int i = 0; i < count; ++i)
		{
			new (&temp[i+1]) T(elements[i]);
			elements[i].~T();
		}
		free(elements);
		elements = temp;
	} // copy over original to new reserved space
	elements = new (elements) T(rhs); // add new element to the front
	count++;
}

// Remove last element
template <typename T>
void Vector<T>::Pop_Back()
{
	if (count) // if elements not empty, free the last and decrease count
	{
		elements[count-1].~T();
		count--;
	}
}

// Remove first element
template <typename T>
void Vector<T>::Pop_Front()
{
	if (count) // if elements not empty, free the first by copy over the remainder to a new list and decrease count
	{
		T * temp = (T*) malloc(reserved * sizeof(T));
		for (int i = 1; i < count; ++i)
		{
			new (&temp[i-1]) T(elements[i]);
			elements[i].~T();
		}
		elements[0].~T();
		free(elements);
		elements = temp;
		count--;
	}
}

// Element Access
template <typename T>
T& Vector<T>::Front() const
{
	return elements[0];
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
	return elements[count - 1];
}

// Element retrieval
template <typename T>
T& Vector<T>::operator[](size_t i)
{
	return const_cast<T&>(static_cast<const Vector&>(*this)[i]);
}

template <typename T>
const T& Vector<T>::operator[](size_t i) const
{
	if (i < 0 || i >= count)
	{
		cerr << "Illegal vector index: " << i << " max index = " << count - 1 << endl;
		abort();
	}
	return elements[i];
}

// return current size of list
template <typename T>
size_t Vector<T>::Size() const
{
	return count;
}

// check if list is empty
template <typename T>
bool Vector<T>::Empty() const
{
	return count == 0;
}

// Clear out any stored element
template <typename T>
void Vector<T>::Clear()
{
	for (int i = 0; i < count; ++i)
	{
		elements[i].~T();
	}
	count = 0;
}

// 13. Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
	return elements;
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
	return elements + count;
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
}
#endif

// Iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
{
	current = NULL;
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
	current = c;
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
	current = rhs.current;
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
	return *current;
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
	current++;
	return *this;
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
	VectorIterator<T> old(*this);
	current++;
	return old; // return the iterator before increment
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
	return current != rhs.current;
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
	return current == rhs.current;
}