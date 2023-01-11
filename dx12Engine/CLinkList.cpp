#include "CLinkList.h"

template <class T>

CLinkList<T>::CLinkList()
{
	memset(this, 0x00, sizeof(CLinkList<T>));

	list = new CLinkListNode<T>();
}

template <class T>

CLinkList<T>::~CLinkList()
{
	CLinkListNode* lln = list;

	while (lln)
	{
		CLinkListNode* n = lln->next;

		delete lln;

		count--;

		lln = n;
	}

	list = nullptr;
}

template <class T>

void CLinkList<T>::Clear()
{
	CLinkListNode<T>* p = list;

	while (p->element)
	{
		p = CLinkList::Delete(p);
	}
}

template <class T>

void CLinkList<T>::Add(T* n, const char* k)
{
	if (list->element == 0)
	{
		list->element = n;
		list->key = new CString(k);
		list->next = new CLinkListNode<T>();

		count++;

		return;
	}

	CLinkListNode<T>* previous = 0;
	CLinkListNode<T>* traverser = list;

	while (traverser->element)
	{
		previous = traverser;
		traverser = traverser->next;
	}

	traverser->previous = previous;
	traverser->element = n;
	traverser->key = new CString(k);
	traverser->next = new CLinkListNode<T>();

	count++;
}

template <class T>

void CLinkList<T>::Add(T* n, int k)
{
	if (list->element == 0)
	{
		list->element = n;
		list->ikey = k;
		list->next = new CLinkListNode<T>();

		count++;

		return;
	}

	CLinkListNode<T>* previous = 0;
	CLinkListNode<T>* traverser = list;

	while (traverser->element)
	{
		previous = traverser;
		traverser = traverser->next;
	}

	traverser->previous = previous;
	traverser->element = n;
	traverser->ikey = k;
	traverser->next = new CLinkListNode<T>();

	count++;
}

template <class T>

CLinkListNode<T>* CLinkList<T>::Delete(CLinkListNode<T>* n)
{
	if (list == n)
	{
		list->next->previous = 0;

		CLinkListNode<T>* next = list->next;

		delete list;

		list = next;

		count--;

		if (count == 0)
		{
			list->next = new CLinkListNode<T>();
		}

		return list;
	}

	CLinkListNode<T>* traverser = list;

	while (traverser->element)
	{
		if (traverser == n)
		{
			CLinkListNode<T>* next = traverser->next;

			traverser->previous->next = traverser->next;
			traverser->next->previous = traverser->previous;

			delete traverser;

			count--;

			return next;
		}

		traverser = traverser->next;
	}

	return nullptr;
}

template <class T>

CLinkListNode<T>* CLinkList<T>::Search(const char* k)
{
	CLinkListNode<T>* traverser = list;

	while (traverser->element)
	{
		if (strcmp(traverser->key->GetText(), k) == 0)
		{
			return traverser;
		}

		traverser = traverser->next;
	}

	return nullptr;
}

template <class T>

CLinkListNode<T>* CLinkList<T>::Search(int k)
{
	CLinkListNode<T>* traverser = list;

	while (traverser->element)
	{
		if (traverser->ikey == k)
		{
			return traverser;
		}

		traverser = traverser->next;
	}

	return nullptr;
}