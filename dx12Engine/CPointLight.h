#ifndef _CPOINTLIGHT
#define _CPOINTLIGHT

#include "standard.h"

#include "CColor.h"
#include "CVertex.h"

class CPointLight
{
public:

	enum
	{
		MAXLIGHTS = 8,

		MAXLIGHTCOUNT = 0xFF
	};

	UINT  number;

	FLOAT position[CPointLight::MAXLIGHTS][4];
	FLOAT color[CPointLight::MAXLIGHTS][4];
	FLOAT radius[CPointLight::MAXLIGHTS];
	FLOAT linear[CPointLight::MAXLIGHTS];
	FLOAT quadratic[CPointLight::MAXLIGHTS];
	FLOAT intensity[CPointLight::MAXLIGHTS];

	CPointLight();

	CPointLight(UINT n, CVertex* p, CColor* c, FLOAT r, FLOAT l, FLOAT q, FLOAT i);

	~CPointLight();

	VOID SetLight(UINT n, CVertex* p, CColor* c, FLOAT r, FLOAT l, FLOAT q, FLOAT i);
};
#endif