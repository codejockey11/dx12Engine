#pragma once

#include "standard.h"

#include "CString.h"

class CDXGIErrorItem
{
public:

	UINT m_nbr;

	CString* m_name;

	CString* m_message;


	CDXGIErrorItem();

	CDXGIErrorItem(UINT nbr, const char* name, const char* message);

	~CDXGIErrorItem();
};