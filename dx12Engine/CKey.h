#ifndef _CKey
#define _CKey

#include "standard.h"

class CKey
{
public:

	XMFLOAT3 coordinates;
	XMFLOAT3 position;

	UINT number;

	CKey();
	~CKey();
};
#endif