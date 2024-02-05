#pragma once

#include "standard.h"

#include "CGlobalObjects.h"

class CMySql
{
public:

	MYSQL* m_connection;
	MYSQL_RES* m_result;
	MYSQL_ROW m_rowset;
	
	int m_fieldCount;
	
	CMySql();
	CMySql(CGlobalObjects* globalObjects);
	~CMySql();
	
	bool PerformQuery(const char* query);

	int GetFieldCount();

	MYSQL_ROW FetchRow();

private:

	CGlobalObjects* m_globalObjects;
};

