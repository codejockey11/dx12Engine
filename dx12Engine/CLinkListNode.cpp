#include "CLinkListNode.h"

template <class T>

CLinkListNode<T>::CLinkListNode()
{
	memset(this, 0x00, sizeof(CLinkListNode<T>));
}

template <class T>

CLinkListNode<T>::~CLinkListNode()
{
	delete element;
}