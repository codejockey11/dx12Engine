#include "CBrush.h"

CBrush::CBrush()
{
	memset(this, 0x00, sizeof(CBrush));
}

CBrush::~CBrush()
{
	for (UINT i = 0; i < 6; i++)
	{
		delete side[i];
	}
}

CBrush::CBrush(CGlobalObjects* g, CVertex* p, CVertex* sz, CVertex2* suv, CVertex2* ouv)
{
	memset(this, 0x00, sizeof(CBrush));

	globalObjects = g;

	side[0] = new CBrushSide(globalObjects, CBrushSide::BBS_TOP, "grass_03_d.tga", "brush.fx", p, sz, suv, ouv);
	side[1] = new CBrushSide(globalObjects, CBrushSide::BBS_BOTTOM, "stoneWall.tga", "normalMap.fx", p, sz, suv, ouv);
	side[2] = new CBrushSide(globalObjects, CBrushSide::BBS_LEFT, "stoneWall.tga", "normalMap.fx", p, sz, suv, ouv);
	side[3] = new CBrushSide(globalObjects, CBrushSide::BBS_RIGHT, "stoneWall.tga", "normalMap.fx", p, sz, suv, ouv);
	side[4] = new CBrushSide(globalObjects, CBrushSide::BBS_FRONT, "stoneWall.tga", "normalMap.fx", p, sz, suv, ouv);
	side[5] = new CBrushSide(globalObjects, CBrushSide::BBS_BACK, "stoneWall.tga", "normalMap.fx", p, sz, suv, ouv);
}

void CBrush::Draw()
{
	for (UINT i = 0; i < 6; i++)
	{
		if (side[i]->draw == CBrushSide::SD_DRAW)
		{
			side[i]->Draw();
		}
	}
}

void CBrush::Copy(CBrush* b)
{
	memset(this, 0x00, sizeof(CBrush));

	globalObjects = b->globalObjects;

	for (UINT i = 0; i < 6; i++)
	{
		side[i] = new CBrushSide(b->side[i]);
	}
}

CDBrush::CDBrush()
{
	memset(this, 0x00, sizeof(CDBrush));
}

CDBrush::CDBrush(CGlobalObjects* g, CVertex* p, CVertex* sz, CVertex2* suv, CVertex2* ouv, UINT ttl)
{
	memset(this, 0x00, sizeof(CDBrush));

	globalObjects = g;

	position = *p;

	timer = new CTimer(ttl);

	CVertex v(0.0F, 0.0F, 0.0F);

	side[0] = new CBrushSide(globalObjects, CBrushSide::BBS_TOP, "grass_03_d.tga", "brush.fx", &v, sz, suv, ouv);
	side[1] = new CBrushSide(globalObjects, CBrushSide::BBS_BOTTOM, "stoneWall.tga", "normalMap.fx", &v, sz, suv, ouv);
	side[2] = new CBrushSide(globalObjects, CBrushSide::BBS_LEFT, "stoneWall.tga", "normalMap.fx", &v, sz, suv, ouv);
	side[3] = new CBrushSide(globalObjects, CBrushSide::BBS_RIGHT, "stoneWall.tga", "normalMap.fx", &v, sz, suv, ouv);
	side[4] = new CBrushSide(globalObjects, CBrushSide::BBS_FRONT, "stoneWall.tga", "normalMap.fx", &v, sz, suv, ouv);
	side[5] = new CBrushSide(globalObjects, CBrushSide::BBS_BACK, "stoneWall.tga", "normalMap.fx", &v, sz, suv, ouv);

	for (UINT s = 0; s < 6; s++)
	{
		for (UINT v = 0; v < 4; v++)
		{
			saveVertices[s][v].x = side[s]->vertices[v].X;
			saveVertices[s][v].y = side[s]->vertices[v].Y;
			saveVertices[s][v].z = side[s]->vertices[v].Z;

			saveNormals[s][v].x = side[s]->vertices[v].n.X;
			saveNormals[s][v].y = side[s]->vertices[v].n.Y;
			saveNormals[s][v].z = side[s]->vertices[v].n.Z;
		}
	}
}

CDBrush::~CDBrush()
{
	delete timer;
}

void CDBrush::UpdateRotation()
{
	rotation.X += rotX * globalObjects->frametime->frametime;

	if (rotation.X < 0.0F)
	{
		rotation.X = 359.0F;
	}
	else if (rotation.X > 359.0F)
	{
		rotation.X = 0.0F;
	}

	rotation.Y += rotY * globalObjects->frametime->frametime;

	if (rotation.Y < 0.0F)
	{
		rotation.Y = 359.0F;
	}
	else if (rotation.Y > 359.0F)
	{
		rotation.Y = 0.0F;
	}

	rotation.Z += rotZ * globalObjects->frametime->frametime;

	if (rotation.Z < 0.0F)
	{
		rotation.Z = 359.0F;
	}
	else if (rotation.Z > 359.0F)
	{
		rotation.Z = 0.0F;
	}
}

void CDBrush::Rotation()
{
	yaw = rotation.Y * PiDiv180;
	pitch = rotation.X * PiDiv180;
	roll = rotation.Z * PiDiv180;
	axis = rotA * PiDiv180;

	XMFLOAT3 xmfyaw = XMFLOAT3(0.0F, 1.0F, 0.0F);

	FXMVECTOR fxmvyaw = XMLoadFloat3(&xmfyaw);

	XMVECTOR xmvyaw = XMVector3Normalize(fxmvyaw);

	XMMATRIX xmmyaw = XMMatrixRotationAxis(xmvyaw, yaw);


	XMFLOAT3 xmfpitch = XMFLOAT3(1.0F, 0.0F, 0.0F);

	FXMVECTOR fxmvpitch = XMLoadFloat3(&xmfpitch);

	XMVECTOR xmvpitch = XMVector3Normalize(fxmvpitch);

	XMMATRIX xmmpitch = XMMatrixRotationAxis(xmvpitch, pitch);


	XMFLOAT3 xmfroll = XMFLOAT3(0.0F, 0.0F, 1.0F);

	FXMVECTOR fxmvroll = XMLoadFloat3(&xmfroll);

	XMVECTOR xmvroll = XMVector3Normalize(fxmvroll);

	XMMATRIX xmmroll = XMMatrixRotationAxis(xmvroll, roll);

	XMMATRIX xmmfinal = xmmpitch * xmmyaw * xmmroll;


	if ((xmfspin.x != 0.0F) || (xmfspin.y != 0.0F) || (xmfspin.z != 0.0F))
	{
		FXMVECTOR fxmvspin = XMLoadFloat3(&xmfspin);

		XMVECTOR xmvspin = XMVector3Normalize(fxmvspin);

		XMMATRIX xmmspin = XMMatrixRotationAxis(xmvspin, axis);

		xmmfinal *= xmmspin;
	}

	for (UINT s = 0; s < 6; s++)
	{
		for (UINT v = 0; v < 4; v++)
		{
			FXMVECTOR fxmvv = XMLoadFloat3(&saveVertices[s][v]);

			XMVECTOR xmvtv = XMVector3TransformCoord(fxmvv, xmmfinal);

			XMFLOAT3 xmfvr = XMFLOAT3(XMVectorGetX(xmvtv), XMVectorGetY(xmvtv), XMVectorGetZ(xmvtv));

			side[s]->vertices[v].X = xmfvr.x + position.X;
			side[s]->vertices[v].Y = xmfvr.y + position.Y;
			side[s]->vertices[v].Z = xmfvr.z + position.Z;


			FXMVECTOR fxmvn = XMLoadFloat3(&saveNormals[s][v]);

			XMVECTOR xmvtn = XMVector3TransformCoord(fxmvn, xmmfinal);

			XMFLOAT3 xmfnr = XMFLOAT3(XMVectorGetX(xmvtn), XMVectorGetY(xmvtn), XMVectorGetZ(xmvtn));

			side[s]->vertices[v].n.X = xmfnr.x;
			side[s]->vertices[v].n.Y = xmfnr.y;
			side[s]->vertices[v].n.Z = xmfnr.z;
		}

#ifdef _DEBUG
		side[s]->UpdateDebug();
#endif

		side[s]->vertexBuffer->LoadBuffer(side[s]->vertices);
	}
}