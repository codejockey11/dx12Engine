#pragma once

#include "standard.h"

#include "CString.h"

class CWinsockErrorItem
{
public:

	CString* m_msg;
	CString* m_name;
	
	UINT m_nbr;

	CWinsockErrorItem();
	CWinsockErrorItem(UINT nbr, const char* name, const char* msg);
	~CWinsockErrorItem();
};