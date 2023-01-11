#pragma once

#include "standard.h"

class CViewport
{
public:

	D3D12_VIEWPORT m_viewport;

	D3D12_RECT m_scissorRect;


	CViewport();

	CViewport(float x, float y, float width, float height);

	~CViewport();
};