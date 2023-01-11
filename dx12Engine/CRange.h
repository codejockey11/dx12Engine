#pragma once

#include "standard.h"

class CRange
{
public:

	enum
	{
		MAX_RANGE = 2
	};

	UINT m_index;

	D3D12_DESCRIPTOR_RANGE1 m_range[CRange::MAX_RANGE];


	CRange();

	~CRange();


	void SetRange(UINT index, D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT count);
};