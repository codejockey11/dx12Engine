#pragma once

#include "standard.h"

#include "CString.h"

template<class T>

class CLinkListNode
{
public:

	CLinkListNode* m_previous;
	CLinkListNode* m_next;

	CString* m_key;

	int m_ikey;

	bool m_deleteMe;

	T* m_element;


	CLinkListNode()
	{
		memset(this, 0x00, sizeof(CLinkListNode));
	}

	~CLinkListNode()
	{
		delete m_element;
	}
};