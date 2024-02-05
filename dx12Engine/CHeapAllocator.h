#pragma once

#include "standard.h"

class CHeapAllocator
{
public:

	UINT m_count;
	UINT m_entrySize;

	CHeapAllocator();
	~CHeapAllocator();
};