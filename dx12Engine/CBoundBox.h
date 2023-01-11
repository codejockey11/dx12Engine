#pragma once

#include "standard.h"

#include "CVertex.h"

class CBoundBox
{
public:

	CVertex m_mins;
	CVertex m_maxs;


	CBoundBox();

	CBoundBox(CBoundBox* boundBox);

	CBoundBox(CVertex* mins, CVertex* maxs);

	~CBoundBox();


	void Copy(CBoundBox* boundBox);

	void Update(CVertex* mins, CVertex* maxs);

	bool CheckPointInBox(CVertex* point);

	bool CheckPointInBox(CVertex2* point);
};