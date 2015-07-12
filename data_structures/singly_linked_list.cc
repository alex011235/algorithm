/** -------------------------------------------------------
* Singly Linked List implementation
* Alexander Karlsson, 2015-07-10
* --------------------------------------------------------- */
#include "singly_linked_list.h"
using namespace std;

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() {
	first = NULL;
}

template <typename T>
void SinglyLinkedList<T>::init() {
	tail = new Node<T>;
	first = tail;
	tail->next = NULL;
}

template <typename T>
void SinglyLinkedList<T>::push_back(T t) {
	if(size == 0) {
		init(); // Same as callaing constructor
		first->value = t;
	} else {
		Node<T> *temp = new Node<T>;
		temp->value = t;
		tail->next = temp;
		tail = temp;
	}
	size++;
}

template <typename T>
void SinglyLinkedList<T>::push_front(T t) {
	if (size == 0) {
		init();
		first->value = t;
	} else {
		Node<T> *node = new Node<T>;
		node->value = t;
		node->next = first;
		first = node;	
	}
	
	size++;
}

template <typename T>
void SinglyLinkedList<T>::insertAt(int index, T t)  {
	if (index == 0) { 
		push_front(t); // case already implmented
	} else if (index >= size) {
		push_back(t); // This case also

	} else {

		Node<T> *node = new Node<T>, *temp1 = first, *temp2 = first;
		node->value = t;
		int k = 0;
		while (k != index) {
			k++;
			temp1 = temp2;
			temp2 = temp2->next;
		}

		temp1->next = node;
		node->next = temp2;

	}
	++size;
}

template <typename T>
void SinglyLinkedList<T>::pop_front() {
	if (size == 0) {
		first = NULL;
		tail = NULL;
		return;
	}

	if (size == 1) {
		first = NULL;
		tail = NULL;
		--size;
		return;
	} 

	first = first->next;
	--size;
}

template <typename T>
void SinglyLinkedList<T>::pop_back() {
	if (size == 0) {
		return; // Trying to remove from an empty list
	}

	/* Special case when size == 1 */
	if (size == 1) {
		first = NULL;
		--size;
		return;
	}

	Node<T> *temp = first;
	while (temp->next) {
		tail = temp;
		temp = temp->next;
	}
	
	--size;

	tail->next = NULL;	
}

template <typename T>
void SinglyLinkedList<T>::removeAt(int index) {
		if (size == 0) {
		first = NULL;
		tail = NULL;
		return;
	}

	if (size == 1 && index == size-1) {
		first = NULL;
		tail = NULL;
		--size;
		return;
	}

	if (index > size-1) {
		return; // Should not remove something	
	}

	if (index == 0) {
		pop_front(); //case already implemented
		return;
	}

	if(index == size-1) {
		pop_back(); //case already implemented
		return;
	}

	int k = 0;
	Node<T> *temp = first;
	while (k != index-1) {
		k++;
		temp = temp->next;
	}

	temp->next = temp->next->next;
	--size;
}

template <typename T>
void SinglyLinkedList<T>::clear() {
	Node<T> *temp = first, *old;
	while (temp) {
		old = temp;
		temp = temp->next;
		delete old;
		old = NULL;
	}

	first = NULL;
	size = 0;
}

template <typename T>
T SinglyLinkedList<T>::front() {
	return first->value;
}

template <typename T>
T SinglyLinkedList<T>::back() {
	return tail->value;
}


template <typename T>
T SinglyLinkedList<T>::at(int index) {
	/* Let the return value for these two cases be 0'*/
	if ((size == 0) || (index >= size)) return 0;

	int k = 0;
	Node<T> *temp = first;
	while (k != index) {
		k++;
		temp = temp->next;
	} 

	return temp->value;
}

template <typename T>
int SinglyLinkedList<T>::length() {
	return size;
}

template <typename T>
void SinglyLinkedList<T>::print() {
	Node<T> *temp = first;
	while (temp) {
		cout << temp->value << endl;
		temp = temp->next;
	}
}

