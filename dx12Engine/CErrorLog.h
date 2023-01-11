#pragma once

#include "standard.h"

#include "CComError.h"
#include "CDXGIError.h"
#include "CWinsockError.h"

class CErrorLog
{
public:

	enum
	{
		TIME_LENGTH = 24,
		ERROR_LENGTH = 1024
	};


	CErrorLog();

	CErrorLog(const char* filename);

	~CErrorLog();


	void WriteError(const char* format, ...);

	void WriteBytes(const char* bytes);

	char* GetComErrorMessage(HRESULT hr);

	CWinsockErrorItem* GetWinsockErrorMessage(UINT error);

	CDXGIErrorItem* GetDXGIErrorMessage(UINT error);

private:

	CComError* m_comError;

	CWinsockError* m_wsError;

	CDXGIError* m_dxgiError;

	FILE* m_file;

	char m_text[CErrorLog::ERROR_LENGTH];
};