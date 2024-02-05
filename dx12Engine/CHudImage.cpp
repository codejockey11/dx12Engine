#include "CHudImage.h"

CHudImage::CHudImage()
{
	memset(this, 0x00, sizeof(CHudImage));
}

CHudImage::CHudImage(CErrorLog* el, CVideoDevice*d, CTextureManager* tm, CFrametime* f, const CHAR* i, CVertex2* s, CVertex2* p, CVertex* r)
{
	memset(this, 0x00, sizeof(CHudImage));

	errorLog = el;
	videoDevice = d;
	textureMgr = tm;
	frametime = f;

	size = *s;
	position = *p;

	if (r)
	{
		rotX = r->X;
		rotY = r->Y;
		rotZ = r->Z;
	}

	texture = textureMgr->GetTexture(i);

	vertices[0].X = position.X - (size.X / 2.0F);
	vertices[0].Y = position.Y - (size.Y / 2.0F);
	vertices[0].Z = 0.0F;
	vertices[0].W = 1.0F;

	vertices[0].U = 0.0F;
	vertices[0].V = 0.0F;

	vertices[1].X = position.X + (size.X / 2.0F);
	vertices[1].Y = position.Y - (size.Y / 2.0F);
	vertices[1].Z = 0.0F;
	vertices[1].W = 1.0F;

	vertices[1].U = 1.0F;
	vertices[1].V = 0.0F;

	vertices[2].X = position.X + (size.X / 2.0F);
	vertices[2].Y = position.Y + (size.Y / 2.0F);
	vertices[2].Z = 0.0F;
	vertices[2].W = 1.0F;

	vertices[2].U = 1.0F;
	vertices[2].V = 1.0F;

	vertices[3].X = position.X - (size.X / 2.0F);
	vertices[3].Y = position.Y + (size.Y / 2.0F);
	vertices[3].Z = 0.0F;
	vertices[3].W = 1.0F;

	vertices[3].U = 0.0F;
	vertices[3].V = 1.0F;

	for (UINT v = 0; v < 4; v++)
	{
		saveVertices[v].X = vertices[v].X - position.X;
		saveVertices[v].Y = vertices[v].Y - position.Y;
		saveVertices[v].Z = vertices[v].Z;
		saveVertices[v].W = vertices[v].W;

		saveVertices[v].U = vertices[v].U;
		saveVertices[v].V = vertices[v].V;
	}

	vertexBuffer = new CVertexBuffer(errorLog, videoDevice, CVertex::VT_VERTEX2D, 4, (VOID*)vertices);

	index[0] = 0;
	index[1] = 2;
	index[2] = 3;
	index[3] = 0;
	index[4] = 1;
	index[5] = 2;

	indexBuffer = new CIndexBuffer(errorLog, videoDevice, 6, 2, index);
}

CHudImage::~CHudImage()
{
	delete vertexBuffer;
	delete indexBuffer;
}

VOID CHudImage::Draw()
{
	if (!texture)
	{
		return;
	}

	if (rotX || rotY || rotZ)
	{
		CHudImage::UpdateRotation();
		CHudImage::Rotation();
	}

	hr = videoDevice->device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	if (hr < 0)
	{
		errorLog->WriteError("CHudImage::Draw::SetRenderState:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	hr = videoDevice->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	if (hr < 0)
	{
		errorLog->WriteError("CHudImage::Draw::SetRenderState:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	hr = videoDevice->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (hr < 0)
	{
		errorLog->WriteError("CHudImage::Draw::SetRenderState:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	hr = videoDevice->device->SetTexture(0, texture->image);

	if (hr < 0)
	{
		errorLog->WriteError("CHudImage::Draw::SetTexture:%s\n", errorLog->GetComErrorMessage(hr));

		return;
	}

	hr = videoDevice->device->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_NONE);

	vertexBuffer->DrawIndexed(indexBuffer);

	hr = videoDevice->device->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_CCW);
}

VOID CHudImage::UpdateRotation()
{
	if (!frametime)
	{
		return;
	}

	rotation.X += rotX * frametime->frametime;

	if (rotation.X < 0.0F)
	{
		rotation.X = 359.0F;
	}
	else if (rotation.X > 359.0F)
	{
		rotation.X = 0.0F;
	}

	rotation.Y += rotY * frametime->frametime;

	if (rotation.Y < 0.0F)
	{
		rotation.Y = 359.0F;
	}
	else if (rotation.Y > 359.0F)
	{
		rotation.Y = 0.0F;
	}

	rotation.Z += rotZ * frametime->frametime;

	if (rotation.Z < 0.0F)
	{
		rotation.Z = 359.0F;
	}
	else if (rotation.Z > 359.0F)
	{
		rotation.Z = 0.0F;
	}
}

VOID CHudImage::Rotation()
{
	yaw = rotation.Y * PiDiv180;
	pitch = rotation.X * PiDiv180;
	roll = rotation.Z * PiDiv180;

	XMMATRIX xmMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	for (UINT v = 0; v < 4; v++)
	{
		XMFLOAT3 vv;

		vv.x = saveVertices[v].X;
		vv.y = saveVertices[v].Y;
		vv.z = saveVertices[v].Z;

		FXMVECTOR vvv = XMLoadFloat3(&vv);

		XMVECTOR rr = XMVector3TransformCoord(vvv, xmMatrix);

		XMFLOAT3 rrr = XMFLOAT3(XMVectorGetX(rr), XMVectorGetY(rr), XMVectorGetZ(rr));

		vertices[v].X = rrr.x + position.X;
		vertices[v].Y = rrr.y + position.Y;
		vertices[v].Z = rrr.z;
	}

	vertexBuffer->LoadBuffer(vertices);
}

VOID CHudImage::UpdatePosition()
{
	for (UINT v = 0; v < 4; v++)
	{
		vertices[v].X = saveVertices[v].X + position.X;
		vertices[v].Y = saveVertices[v].Y + position.Y;
		vertices[v].Z = saveVertices[v].Z;
	}

	vertexBuffer->LoadBuffer(vertices);
}