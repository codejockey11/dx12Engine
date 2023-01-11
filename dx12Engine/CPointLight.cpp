#include "CPointLight.h"

CPointLight::CPointLight()
{
	memset(this, 0x00, sizeof(CPointLight));
}

CPointLight::CPointLight(UINT n, CVertex* p, CColor* c, FLOAT r, FLOAT l, FLOAT q, FLOAT i)
{
	memset(this, 0x00, sizeof(CPointLight));

	CPointLight::SetLight(n, p, c, r, l, q, i);
}

CPointLight::~CPointLight()
{
}

VOID CPointLight::SetLight(UINT n, CVertex* p, CColor* c, FLOAT r, FLOAT l, FLOAT q, FLOAT i)
{
	if (n >= MAXLIGHTS)
	{
		return;
	}

	number = n;

	position[number][0] = p->X;
	position[number][1] = p->Y;
	position[number][2] = p->Z;
	position[number][3] = 0.0F;

	color[number][0] = c->RGBA[0];
	color[number][1] = c->RGBA[1];
	color[number][2] = c->RGBA[2];
	color[number][3] = c->RGBA[3];

	radius[number] = r;
	linear[number] = l;

	quadratic[number] = q;
	intensity[number] = i;
}