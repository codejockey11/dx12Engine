#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CHttpFields.h"
#include "CLinkList.h"
#include "CString.h"

class CHttpRequest
{
public:

	CString* m_buffer;

	CLinkList<CString>* m_buffers;
	

	CHttpRequest();

	CHttpRequest(CGlobalObjects* globalObjects);

	~CHttpRequest();


	void UrlRequest(const char* url);

	void FormRequest(const char* url, CLinkList<CHttpFields>* fields);

	void DumpBuffers();

private:

	CGlobalObjects* m_globalObjects;

	CURL* m_curl;
	CURLcode m_res;
};


/*
CHttpRequest* h = new CHttpRequest(globalObjects);

CLinkList<CHttpFields>* fl = new CLinkList<CHttpFields>();

CHttpFields* f = new CHttpFields("userId", "skunkle1105");

fl->Add(f, nullptr);

f = new CHttpFields("password", "skunkle1105");

fl->Add(f, nullptr);

h->FormRequest("http://127.0.0.1:26105/xmlFormatters/getLogin.php", fl);

h->DumpBuffers();

delete fl;

delete h;
*/