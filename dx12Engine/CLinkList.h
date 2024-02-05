#pragma once

#include "standard.h"

#include "CLinkListNode.h"
#include "CString.h"

template <class T>

class CLinkList
{
public:

	CLinkListNode<T>* m_list;

	int m_count;

	bool m_isBeingTraversed;
	bool m_isBeingUpdated;

	/*
	*/
	CLinkList()
	{
		CLinkList::Constructor();
	}

	/*
	*/
	~CLinkList()
	{
		m_isBeingUpdated = true;

		CLinkListNode<T>* lln = m_list;

		while (lln)
		{
			CLinkListNode<T>* n = lln->m_next;

			delete lln;

			m_count--;

			lln = n;
		}

		m_list = nullptr;

		m_isBeingUpdated = false;
	}

	/*
	*/
	void Constructor()
	{
		memset(this, 0x00, sizeof(CLinkList<T>));

		m_list = new CLinkListNode<T>();
	}
	/*
	*/
	void Add(T* element, const char* key)
	{
		while (m_isBeingTraversed)
		{
		}

		m_isBeingUpdated = true;

		if (m_list->m_object == 0)
		{
			m_list->m_object = element;
			m_list->m_key = new CString(key);
			m_list->m_next = new CLinkListNode<T>();

			m_count++;

			m_isBeingUpdated = false;

			return;
		}

		CLinkListNode<T>* previous = 0;
		CLinkListNode<T>* traverser = m_list;

		while (traverser->m_object)
		{
			previous = traverser;
			traverser = traverser->m_next;
		}

		traverser->m_previous = previous;
		traverser->m_object = element;
		traverser->m_key = new CString(key);
		traverser->m_next = new CLinkListNode<T>();

		m_count++;

		m_isBeingUpdated = false;
	}

	/*
	*/
	void Add(T* element, int key)
	{
		while (m_isBeingTraversed)
		{
		}

		m_isBeingUpdated = true;

		if (m_list->m_object == 0)
		{
			m_list->m_object = element;
			m_list->m_ikey = key;
			m_list->m_next = new CLinkListNode<T>();

			m_count++;

			m_isBeingUpdated = false;

			return;
		}

		CLinkListNode<T>* previous = 0;
		CLinkListNode<T>* traverser = m_list;

		while (traverser->m_object)
		{
			previous = traverser;
			traverser = traverser->m_next;
		}

		traverser->m_previous = previous;
		traverser->m_object = element;
		traverser->m_ikey = key;
		traverser->m_next = new CLinkListNode<T>();

		m_count++;

		m_isBeingUpdated = false;
	}

	/*
	*/
	void Clear()
	{
		while (m_isBeingTraversed)
		{
		}

		m_isBeingUpdated = true;

		CLinkListNode<T>* p = m_list;

		while (p->m_object)
		{
			p = CLinkList::Delete(p);
		}

		m_isBeingUpdated = false;
	}

	/*
	*/
	CLinkListNode<T>* Delete(CLinkListNode<T>* element)
	{
		while (m_isBeingTraversed)
		{
		}

		m_isBeingUpdated = true;

		if (m_list == element)
		{
			m_list->m_next->m_previous = 0;

			CLinkListNode<T>* next = m_list->m_next;

			delete m_list;

			m_list = next;

			m_count--;

			//if (m_count == 0)
			//{
				//m_list->m_next = new CLinkListNode<T>();
			//}

			m_isBeingUpdated = false;

			return m_list;
		}

		CLinkListNode<T>* traverser = m_list;

		while (traverser->m_object)
		{
			if (traverser == element)
			{
				CLinkListNode<T>* next = traverser->m_next;

				traverser->m_previous->m_next = traverser->m_next;
				traverser->m_next->m_previous = traverser->m_previous;

				delete traverser;

				m_count--;

				m_isBeingUpdated = false;

				return next;
			}

			traverser = traverser->m_next;
		}

		m_isBeingUpdated = false;

		return nullptr;
	}

	/*
	*/
	CLinkListNode<T>* Search(const char* key)
	{
		while (m_isBeingTraversed)
		{
		}

		m_isBeingUpdated = true;

		CLinkListNode<T>* traverser = m_list;

		while (traverser->m_object)
		{
			if (strcmp(traverser->m_key->GetText(), key) == 0)
			{
				m_isBeingUpdated = false;

				return traverser;
			}

			traverser = traverser->m_next;
		}

		m_isBeingUpdated = false;

		return nullptr;
	}

	/*
	*/
	CLinkListNode<T>* Search(int key)
	{
		while (m_isBeingTraversed)
		{
		}

		m_isBeingUpdated = true;

		CLinkListNode<T>* traverser = m_list;

		while (traverser->m_object)
		{
			if (traverser->m_ikey == key)
			{
				m_isBeingUpdated = false;

				return traverser;
			}

			traverser = traverser->m_next;
		}

		m_isBeingUpdated = false;

		return nullptr;
	}
};