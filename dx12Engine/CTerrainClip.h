#pragma once

#include "standard.h"

#include "CVertex.h"

class CTerrainClip
{
public:

	CVertex	m_a;
	CVertex	m_b;
	CVertex	m_c;
	CVertex	m_n1;
	
	CVertex	m_d;
	CVertex	m_e;
	CVertex	m_f;
	CVertex	m_n2;

	CTerrainClip();
	~CTerrainClip();
};