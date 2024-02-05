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
		E_TIME_LENGTH = 24,
		E_LINE_LENGTH = 1024
	};

	CErrorLog();
	CErrorLog(const char* filename);
	~CErrorLog();

	CDXGIErrorItem* GetDXGIErrorMessage(UINT error);
	CWinsockErrorItem* GetWinsockErrorMessage(UINT error);
	
	char* GetComErrorMessage(HRESULT hr);
	
	void WriteBytes(const char* bytes);
	void WriteError(const char* format, ...);

private:

	CComError* m_comError;
	CDXGIError* m_dxgiError;
	CWinsockError* m_wsError;
	
	FILE* m_file;
	
	char m_text[CErrorLog::E_LINE_LENGTH];
};