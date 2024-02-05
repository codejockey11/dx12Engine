#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CHttpRequest.h"
#include "CServerInfo.h"
#include "CString.h"

class CAccountInfo
{
public:

	CGlobalObjects* m_globalObjects;
	CHttpRequest* m_httpRequest;
	CServerInfo* m_serverInfo;
	CString* m_url;
	
	void* m_server;
	
	CAccountInfo();
	CAccountInfo(CGlobalObjects* globalObjects, void* server, CServerInfo* serverInfo, const char* url);
	~CAccountInfo();

	void RequestAccountInfo();
};