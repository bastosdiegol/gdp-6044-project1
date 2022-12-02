#pragma once
#include "tNode.h"

template <class T> 
class tDLList {
private:
	tNode<T>* m_head;
	tNode<T>* m_tail;
	int m_size;

	/// <summary>
	/// Finds the tNode at specified index and returns it
	/// </summary>
	/// <param name="index">Specified Index</param>
	/// <returns>The tNode pointer</returns>
	tNode<T>* getNodeAt(int index);
public:
	tDLList();
	~tDLList();

	/// <summary>
	/// Returns size member variable
	/// </summary>
	/// <returns>m_size</returns>
	int getSize();

	/// <summary>
	/// Adds a tNode containing T add the end of the linked list
	/// </summary>
	/// <param name="data">Template variable</param>
	void addAtTheEnd(T data);
	/// <summary>
	/// Adds a tNode containing T add the beginning of the linked list
	/// </summary>
	/// <param name="data">Template variable</param>
	void addAtTheBeginning(T data);
	/// <summary>
	/// Calls addAtTheEnd()
	/// </summary>
	/// <param name="data">Template variable</param>
	void pushBack(T data);
	/// <summary>
	/// Finds the tNode at specified index and removes it
	/// </summary>
	/// <param name="index">Specified index</param>
	void removeAt(int index);
	/// <summary>
	/// Finds the tNode at specified index and returns its data
	/// </summary>
	/// <param name="index">Specified Index</param>
	/// <returns>Template variable stored at that tNode</returns>
	T getAt(int index);
};

template<class T>
inline tDLList<T>::tDLList() {
	m_head = nullptr;
	m_tail = nullptr;
	m_size = 0;
}

template <class T>
inline tDLList<T>::~tDLList() {
	// While head is not null
	while (m_size > 0 && m_head) {
		// Grabs next node
		tNode<T>* next = m_head->m_next;
		// Deletes current head
		delete m_head;
		// Shrinks the size
		m_size--;
		// Head now points to second node
		m_head = next;
	}
}

template <class T>
inline int tDLList<T>::getSize() {
	return m_size;
}

template <class T>
inline void tDLList<T>::addAtTheBeginning(T data) {
	// Creates new node
	tNode<T>* newNode = new tNode<T>(data);
	// Prepares the new node pointer
	newNode->m_next = nullptr;
	newNode->m_previous = nullptr;
	// Checks if its first node
	if (m_head == nullptr) {
		m_head = newNode;
		m_tail = newNode;
		m_size++;
	} else { // Adds the new node as new head
		m_head->m_previous = newNode;
		newNode->m_next = m_head;
		m_head = newNode;
		m_size++;
	}
}

template <class T>
inline void tDLList<T>::addAtTheEnd(T data) {
	// Instantiate new node
	tNode<T>* newNode = new tNode<T>(data);
	// Prepares the new node pointer
	newNode->m_next = nullptr;
	newNode->m_previous = nullptr;
	// Checks if its first node
	if (m_head == nullptr) {
		m_head = newNode;
		m_tail = newNode;
		m_size++;
	} else { // Pushes it back as new tail
		m_tail->m_next = newNode;
		newNode->m_previous = m_tail;
		m_tail = newNode;
		m_size++;
	}
}

template <class T>
inline void tDLList<T>::pushBack(T data) {
	this->addAtTheEnd(data);
}

template<class T>
inline void tDLList<T>::removeAt(int index) {
	// Checks if the list has only 1 node
	if (m_size == 1) {
		// Deletes current head
		delete m_head;
		// Shrinks the size
		m_size--;
		// Head now points to nullptr
		m_head = nullptr;
	} else if(m_size <= 0) {
		// Does nothing if the list has no member
	} else {
		// Checks if its the head
		if (index == 0) {
			// Grabs next node
			tNode<T>* next = m_head->m_next;
			// Deletes current head
			delete m_head;
			// Shrinks the size
			m_size--;
			// Head now points to second node
			m_head = next;
			// Checks if its the tail
		} else if (index == (m_size - 1)) {
			// Grabs previous node
			tNode<T>* previous = m_tail->m_previous;
			// Deletes current tail
			delete m_tail;
			// Shrinks the size
			m_size--;
			// Tail now points to second last node
			m_tail = previous;
		} else {
			// Gets the Node
			tNode<T>* node = getNodeAt(index);
			// Links previous node with the next
			node->m_previous->m_next = node->m_next;
			// Links next node with previous
			node->m_next->m_previous = node->m_previous;
			// Deletes the node
			delete node;
			// Shrinks the size
			m_size--;
		}
	}
}

template <class T>
inline tNode<T>* tDLList<T>::getNodeAt(int index) {
	// Checks if its the head
	if (index == 0) {
		return m_head;
	} else if (index == m_size - 1) { // Checks if its the tail
		return m_tail;
	} else if (index < 0 || index >= m_size) { // Checks for invalid position
		throw("Out of Range Exception!");
	}

	// Checks the desired index
	// If its closer to the head lets start at 0
	if (index < m_size / 2) {
		tNode<T>* temp = m_head;
		int i = 0;
		while (temp) {
			if (i == index)
				return temp;
			i++;
			temp = temp->m_next;
		}
	} else { // If its closer to the tail, starts at the end		
		tNode<T>* temp = m_tail;
		int i = m_size - 1;
		while (temp) {
			if (i == index)
				return temp;
			i--;
			temp = temp->m_previous;
		}
	}
}

template <class T>
inline T tDLList<T>::getAt(int index) {
	// Checks if its the head
	if (index == 0) {
		return m_head->getData();
	} else if (index == m_size - 1) { // Checks if its the tail
		return m_tail->getData();
	} else if (index < 0 || index >= m_size) { // Checks for invalid position
		throw("Out of Range Exception!");
	}

	// Checks the desired index
	// If its closer to the head lets start at 0
	if (index < m_size / 2) {
		tNode<T>* temp = m_head;
		int i = 0;
		while (temp) {
			if (i == index)
				return temp->getData();
			i++;
			temp = temp->m_next;
		}
	} else { // If its closer to the tail, starts at the end		
		tNode<T>* temp = m_tail;
		int i = m_size - 1;
		while (temp) {
			if (i == index)
				return temp->getData();
			i--;
			temp = temp->m_previous;
		}
	}
}