#pragma once

#include "standard.h"

class CVertex
{
public:

	enum
	{
		VT_VERTEXRGBA = 0,
		VT_VERTEX,
		VT_VERTEXT,
		VT_VERTEXNT,
		VT_VERTEXLINE,
		VT_VERTEX2D
	};

	XMFLOAT3 p;


	CVertex();

	CVertex(float p1, float p2, float p3);

	~CVertex();


	void operator += (const CVertex& v);

	void operator -= (const CVertex& v);

	void operator *= (float v);

	void operator /= (float v);

	CVertex operator + (const CVertex& v);

	CVertex operator - (const CVertex& v);

	CVertex operator * (float v);

	CVertex operator / (float v);

	bool operator == (const CVertex v);

	bool operator != (const CVertex v);

	float Length();

	void Normalize();

	void RadiusNormalize();

	float Dot(CVertex v);

	CVertex Cross(CVertex v);

	CVertex CalculateNormal(CVertex* a, CVertex* b, CVertex* c);

	CVertex PointToRadian();

	CVertex PointToDegree();

	CVertex RotateAngleByAxis(const float angleInRad, const CVertex referenceAxis);
};

class CVertexRGBA : public CVertex
{
public:

	XMFLOAT4 c;


	CVertexRGBA();

	~CVertexRGBA();
};

class CVertexT : public CVertex
{
public:

	XMFLOAT2 uv;


	CVertexT();

	~CVertexT();
};

class CVertexNT : public CVertex
{
public:

	XMFLOAT3 n;

	XMFLOAT2 uv;


	CVertexNT();

	~CVertexNT();
};

class CVertex2d
{
public:

	XMFLOAT2 p;

	XMFLOAT2 uv;


	CVertex2d();

	~CVertex2d();
};

class CVertex2
{
public:

	XMFLOAT2 p;


	CVertex2();

	CVertex2(float p1, float p2);

	~CVertex2();

	void RadiusNormalize();
};