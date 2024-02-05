#include "CAccountInfo.h"

/*
*/
CAccountInfo::CAccountInfo()
{
	memset(this, 0x00, sizeof(CAccountInfo));
}

/*
*/
CAccountInfo::CAccountInfo(CGlobalObjects* globalObjects, void* server, CServerInfo* serverInfo, const char* url)
{
	memset(this, 0x00, sizeof(CAccountInfo));

	m_globalObjects = globalObjects;

	m_server = server;

	m_serverInfo = serverInfo;

	m_httpRequest = new CHttpRequest(globalObjects);

	//m_url = new CString("https://www.aviationweather.gov/adds/dataserver_current/httpparam?dataSource=stations&requestType=retrieve&format=xml&stationString=KJFK");
	m_url = new CString("https://www.aviationweather.gov/adds/dataserver_current/httpparam?dataSource=metars&requestType=retrieve&format=xml&hoursBeforeNow=1&stationString=KJFK");
	//m_url = new CString("https://www.aviationweather.gov/adds/dataserver_current/httpparam?dataSource=tafs&requestType=retrieve&format=xml&hoursBeforeNow=1&stationString=KJFK");
}

/*
*/
CAccountInfo::~CAccountInfo()
{
	delete m_url;

	delete m_httpRequest;
}

/*
*/
void CAccountInfo::RequestAccountInfo()
{
	m_httpRequest->UrlRequest(m_url->GetText());
}