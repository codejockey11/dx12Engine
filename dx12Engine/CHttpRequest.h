#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CHttpFields.h"
#include "CLinkList.h"
#include "CString.h"

class CHttpRequest
{
public:

	CLinkList<CString>* m_buffers;
	CString* m_buffer;

	CHttpRequest();
	CHttpRequest(CGlobalObjects* globalObjects);
	~CHttpRequest();

	void DumpBuffers();
	void FormRequest(const char* url, CLinkList<CHttpFields>* fields);
	void UrlRequest(const char* url);

private:

	CGlobalObjects* m_globalObjects;

	CURL* m_curl;
	CURLcode m_res;
};