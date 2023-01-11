#pragma once

#include "standard.h"

#include "CImage.h"

class COrthoKey
{
public:

	CVertex2 m_coordinates;
	
	CVertex2 m_position;

	int m_number;

	CImage* m_image;

	
	COrthoKey();

	~COrthoKey();
};