#include "CBillboard.h"

CBillboard::CBillboard()
{
	memset(this, 0x00, sizeof(CBillboard));
}

CBillboard::CBillboard(CGlobalObjects* g, const CHAR* fn, CVertex2* wh, CVertex* p, BYTE t)
{
	memset(this, 0x00, sizeof(CBillboard));

	globalObjects = g;

	width = (UINT)wh->X;
	height = (UINT)wh->Y;

	position = *p;

	type = t;

	texture = globalObjects->textureMgr->GetTexture(fn);

	textureEffect = globalObjects->effectMgr->GetEffect("texture.fx");

	vertexBuffer = new CVertexBuffer(globalObjects->errorLog, globalObjects->videoDevice, CVertex::VT_VERTEXNT, 4, NULL);

	CBillboard::UpdateVertices();

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 0;
	index[4] = 2;
	index[5] = 3;

	indexBuffer = new CIndexBuffer(globalObjects->errorLog, globalObjects->videoDevice, 6, 2, index);
}

CBillboard::~CBillboard()
{
	delete vertexBuffer;
	delete indexBuffer;
}

VOID CBillboard::UpdateVertices()
{
	rightVect = CVertex(view._11, view._21, view._31);

	rightVect.Normalize();

	rightVect = rightVect * (FLOAT)width * 0.5F;

	upVect = CVertex(view._12, view._22, view._32);

	upVect.Normalize();

	upVect = upVect * (FLOAT)height * 0.5F;

	upperLeft = position - rightVect + upVect;
	upperRight = position + upVect + rightVect;
	lowerRight = position - upVect + rightVect;
	lowerLeft = position - rightVect - upVect;

	normal = normal.CalculateNormal(&lowerRight, &upperLeft, &upperRight);

	vertices[0].X = upperLeft.X;
	vertices[0].Y = upperLeft.Y;
	vertices[0].Z = upperLeft.Z;
	vertices[0].n.X = normal.X;
	vertices[0].n.Y = normal.Y;
	vertices[0].n.Z = normal.Z;
	vertices[0].U = 0.0F;
	vertices[0].V = 0.0F;

	vertices[1].X = upperRight.X;
	vertices[1].Y = upperRight.Y;
	vertices[1].Z = upperRight.Z;
	vertices[1].n.X = normal.X;
	vertices[1].n.Y = normal.Y;
	vertices[1].n.Z = normal.Z;
	vertices[1].U = 1.0F;
	vertices[1].V = 0.0F;

	vertices[2].X = lowerRight.X;
	vertices[2].Y = lowerRight.Y;
	vertices[2].Z = lowerRight.Z;
	vertices[2].n.X = normal.X;
	vertices[2].n.Y = normal.Y;
	vertices[2].n.Z = normal.Z;
	vertices[2].U = 1.0F;
	vertices[2].V = 1.0F;

	vertices[3].X = lowerLeft.X;
	vertices[3].Y = lowerLeft.Y;
	vertices[3].Z = lowerLeft.Z;
	vertices[3].n.X = normal.X;
	vertices[3].n.Y = normal.Y;
	vertices[3].n.Z = normal.Z;
	vertices[3].U = 0.0F;
	vertices[3].V = 1.0F;

	vertexBuffer->LoadBuffer(vertices);
}

VOID CBillboard::Draw()
{
	CBillboard::UpdateVertices();

	switch (type)
	{
	case CBillboard::BT_NAMEPLATE:
	{
		hr = globalObjects->videoDevice->device->SetTransform(D3DTRANSFORMSTATETYPE::D3DTS_VIEW, &view);

		hr = globalObjects->videoDevice->device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		hr = globalObjects->videoDevice->device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		hr = globalObjects->videoDevice->device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		hr = globalObjects->videoDevice->device->SetTexture(0, texture->image);

		vertexBuffer->DrawIndexed(indexBuffer);

		break;
	}

	case CBillboard::BT_DRAW:
	{
		if (textureEffect)
		{
			hr = textureEffect->effect->Begin(&textureEffect->cPasses, 0);

			for (UINT i = 0; i < textureEffect->cPasses; i++)
			{
				hr = textureEffect->effect->BeginPass(i);

				hr = textureEffect->effect->SetTexture(textureEffect->htexture00, texture->image);

				hr = textureEffect->effect->SetFloatArray(textureEffect->hlightAmbient, White.GetFloatArray(), 4);
				hr = textureEffect->effect->SetFloatArray(textureEffect->hmaterial, White.GetFloatArray(), 4);

				hr = textureEffect->effect->CommitChanges();

				vertexBuffer->DrawIndexed(indexBuffer);

				hr = textureEffect->effect->EndPass();
			}

			hr = textureEffect->effect->End();
		}

		break;
	}
	}
}