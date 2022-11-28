#pragma once

template <class T>
class tNode {
private:
	T m_data;
public:
	tNode<T>* m_previous;
	tNode<T>* m_next;

	tNode(T data);
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