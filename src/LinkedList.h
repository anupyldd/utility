/*
 * Templated linked list data structure implemented for practice.
 * Some of the methods are still missing, but it already supports
 * search by index or element, accessing first/last element, insertion
*/



#pragma once
#include <memory>
#include <iostream>
#include <exception>
#include <stdexcept>

namespace util
{
namespace data_structures
{

template<typename Type>
struct Node
{
	Node(Type inData) : data(inData) { };
	Node(const Node<Type>& src) { data = src.data; next = new Node<Type>(src.next); }
	~Node() { delete next; }

	Node& operator =(const Node<Type>& rhs) { data = rhs.data; next = new Node<Type>(rhs.next); }

	Type  data;
	Node* next = nullptr;
};

template<typename Type>
class LinkedList
{
public:
	 LinkedList() = default;
	~LinkedList();

	LinkedList<Type>&	Add(Type inData);					// add element at the end of the list
	void				Pop();								// remove last element
	void				Insert(const Type& data, unsigned int index);
	void				Remove(unsigned int index);
	void				Clear();

	size_t		Size();

	bool		Contains(Type inData);
	Node<Type>* First();			
	Node<Type>* Last();		
	Node<Type>* At(unsigned int index);			// get element by index
	Node<Type>* Elem(Type inData);				// get element by data
	Node<Type>* operator[](unsigned int index);

	void Sort(bool ascending = true);	
	void Reverse();
	//void Apply();								// apply function to each element

	// for debug
	void OutputElems();
	
private:
	Node<Type>* first = nullptr;
	Node<Type>* last  = nullptr;
	size_t size = 0;
};

template<typename Type>
LinkedList<Type>::~LinkedList()
{
	delete first;
	delete last;
}

template<typename Type>
LinkedList<Type>& LinkedList<Type>::Add(Type inData)
{
	Node<Type>* newNode = new Node<Type>(inData);
	if (size == 0) 
	{
		first = newNode;
		last  = newNode;
	}
	else
	{
		last->next = newNode;
		last = newNode;
	}
	size++;

	return *this;
}

template<typename Type>
void LinkedList<Type>::Pop()
{
	if (size == 0) return;
	if (size == 1)
	{
		delete first;
		first = nullptr;
		last  = nullptr;
		size--;
		return;
	}

	Node<Type>* iter = first;
	while (iter->next->next != nullptr)
	{
		iter = iter->next;
	}
	delete last;
	iter->next = nullptr;
	last = iter;
	size--;
}

template<typename Type>
void LinkedList<Type>::Insert(const Type& data, unsigned int index)
{
	if (size > 0 && index > size) throw std::runtime_error("Error: index out of range\n");

	if (size == 0 || size == index)
	{
		Add(data);
		return;
	}

	Node<Type>* newNode = new Node(data);

	Node<Type>* placeToInsert = first;
	for(int i = 0; i < index-1; i++)
	{
		placeToInsert = placeToInsert->next;
	}

	Node<Type>* nextNode = placeToInsert->next;
	newNode->next = nextNode;
	placeToInsert->next = newNode;
	size++;
}

template<typename Type>
void LinkedList<Type>::Remove(unsigned int index)
{
	if (size == 0) return;
	if (size > 0 && index >= size) throw std::runtime_error("Error: index out of range\n");

	size--;

	if (index == 0)
	{
		Node<Type>* second = nullptr;
		second = first->next;
		delete first;			// BREAKS SECOND AS WELL, NEEDS FIX
		first = second;
		return;
	}

	Node<Type>* nodeBefore = first;
	for (int i = 0; i < index; i++)
	{
		nodeBefore = nodeBefore->next;
	}
	Node<Type>* nodeAfter = nodeBefore->next->next;
	delete nodeBefore->next;
	nodeBefore->next = nodeAfter;
}

template<typename Type>
void LinkedList<Type>::Clear()
{
	while (first != nullptr)
	{
		Pop();
	}
}

template<typename Type>
size_t LinkedList<Type>::Size()
{
	return size;
}

template<typename Type>
bool LinkedList<Type>::Contains(Type inData)
{
	if (size == 0) return false;

	Node<Type>* iter = first;
	while (iter != nullptr)
	{
		if (iter->data == inData) return true;
		iter = iter->next;
	}

	return false;
}

template<typename Type>
Node<Type>* LinkedList<Type>::At(unsigned int index)
{
	if (size  == 0)		throw std::runtime_error("Error: linked list is empty\n");
	if (index > size-1)	throw std::runtime_error("Error: index out of range\n");
	if (index == 0)		return first;

	int i = 0;
	Node<Type>* iter = first;
	while (iter != nullptr)
	{
		if (i == index) return iter;
		iter = iter->next;
		i++;
	}
	throw std::runtime_error("Error: element with this index not found\n");
}

template<typename Type>
Node<Type>* LinkedList<Type>::Elem(Type inData)
{
	if (size == 0) throw std::runtime_error("Error: linked list is empty\n");

	Node<Type>* iter = first;
	while (iter != nullptr)
	{
		if (iter->data == inData) return iter;
		iter = iter->next;
	}

	throw std::runtime_error("Error: element with this data not found\n");
}

template<typename Type>
Node<Type>* LinkedList<Type>::operator[](unsigned int index)
{
	return At(index);
}

template<typename Type>
void LinkedList<Type>::Sort(bool ascending)
{

}

template<typename Type>
void LinkedList<Type>::Reverse()
{

}

template<typename Type>
Node<Type>* LinkedList<Type>::First()
{
	if (size > 0) return first;
	else throw std::runtime_error("Error: linked list is empty\n");
}

template<typename Type>
Node<Type>* LinkedList<Type>::Last()
{
	if (size > 0) return last;
	else throw std::runtime_error("Error: linked list is empty\n");
}

template<typename Type>
void LinkedList<Type>::OutputElems()
{
	if (size == 0)
	{
		std::cout << "Nothing to output\n";
		return;
	}
	Node<Type>* iter = first;
	while (iter != nullptr)
	{
		std::cout << " ( " << iter->data << " ) ";
		iter = iter->next;
	} 
	std::cout << '\n';
}

}
}