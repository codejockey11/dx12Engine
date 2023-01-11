#pragma once

#include "standard.h"

#include "CString.h"

class CHttpFields
{
public:

	CString* m_name;
	CString* m_data;

	CHttpFields();

	CHttpFields(const char* name, const char* data);

	~CHttpFields();
};

