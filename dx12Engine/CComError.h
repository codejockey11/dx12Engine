#pragma once

#include "standard.h"

class CComError
{
public:

	CComError();
	~CComError();

	char* GetComErrorMessage(HRESULT hr)
	{
		_com_error err(hr);

		return (char*)err.ErrorMessage();
	}
};