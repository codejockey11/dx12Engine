#pragma once

#include "standard.h"

#include "CString.h"

class CWinsockErrorItem
{
public:

	UINT m_nbr;

	CString* m_name;

	CString* m_msg;


	CWinsockErrorItem();

	CWinsockErrorItem(UINT nbr, const char* name, const char* msg);

	~CWinsockErrorItem();
};