#pragma once

#include "standard.h"

#include "CString.h"

class CHttpFields
{
public:

	CString* m_data;
	CString* m_name;

	CHttpFields();
	CHttpFields(const char* name, const char* data);
	~CHttpFields();
};

