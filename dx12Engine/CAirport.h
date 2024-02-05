#pragma once

#include "standard.h"

#include "CMySql.h"

class CAirport : public CMySql
{
public:

	// columns from rowset
	enum
	{
		ICAO = 0,
		Name
	};

	using CMySql::CMySql;
};

