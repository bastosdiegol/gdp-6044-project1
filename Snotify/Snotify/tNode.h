#pragma once

template <class T>
class tNode {
	// Only Doubly Linked List can instantiate this class
	template <class T> friend class tDLList;

private:
	T m_data;
	// No one should instantiate this class
	tNode(T data);
public:
	tNode<T>* m_previous;
	tNode<T>* m_next;

	~tNode();

	T getData();
};

template <class T>
inline tNode<T>::tNode(T data) {
	m_data = data;
}

template <class T>
inline tNode<T>::~tNode() {
}

template <class T>
inline T tNode<T>::getData() {
	return m_data;
}