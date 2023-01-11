#include "CBrushSide.h"

CBrushSide::CBrushSide()
{
	memset(this, 0x00, sizeof(CBrushSide));
}

CBrushSide::CBrushSide(CGlobalObjects* g, BYTE tp, const char* t, const char* en, CVertex* p, CVertex* sz, CVertex2* suv, CVertex2* ouv)
{
	memset(this, 0x00, sizeof(CBrushSide));

	globalObjects = g;

	type = tp;

	texture = globalObjects->textureMgr->GetTexture(t);

	effect = globalObjects->effectMgr->GetEffect(en);

	clip = CBrushSide::SC_CLIP;

	draw = CBrushSide::SD_DRAW;

	//A-B
	//| |
	//D-C
	if (tp == CBrushSide::BBS_LEFT)
	{
		vertices[0].X = p->X - (sz->X / 2.0F);
		vertices[0].Y = p->Y + (sz->Y / 2.0F);
		vertices[0].Z = p->Z + (sz->Z / 2.0F);
		vertices[0].n.X = -1.0F;
		vertices[0].n.Y = 0.0F;
		vertices[0].n.Z = 0.0F;
		vertices[0].U = 0.0F + ouv->X;
		vertices[0].V = 0.0F + ouv->Y;

		vertices[1].X = p->X - (sz->X / 2.0F);
		vertices[1].Y = p->Y + (sz->Y / 2.0F);
		vertices[1].Z = p->Z - (sz->Z / 2.0F);
		vertices[1].n.X = -1.0F;
		vertices[1].n.Y = 0.0F;
		vertices[1].n.Z = 0.0F;
		vertices[1].U = (1.0F * suv->X) + ouv->X;
		vertices[1].V = 0.0F + ouv->Y;

		vertices[2].X = p->X - (sz->X / 2.0F);
		vertices[2].Y = p->Y - (sz->Y / 2.0F);
		vertices[2].Z = p->Z - (sz->Z / 2.0F);
		vertices[2].n.X = -1.0F;
		vertices[2].n.Y = 0.0F;
		vertices[2].n.Z = 0.0F;
		vertices[2].U = (1.0F * suv->X) + ouv->X;
		vertices[2].V = (1.0F * suv->Y) + ouv->Y;

		vertices[3].X = p->X - (sz->X / 2.0F);
		vertices[3].Y = p->Y - (sz->Y / 2.0F);
		vertices[3].Z = p->Z + (sz->Z / 2.0F);
		vertices[3].n.X = -1.0F;
		vertices[3].n.Y = 0.0F;
		vertices[3].n.Z = 0.0F;
		vertices[3].U = 0.0F + ouv->X;
		vertices[3].V = (1.0F * suv->Y) + ouv->Y;
	}

	if (tp == CBrushSide::BBS_TOP)
	{
		vertices[0].X = p->X - (sz->X / 2.0F);
		vertices[0].Y = p->Y + (sz->Y / 2.0F);
		vertices[0].Z = p->Z + (sz->Z / 2.0F);
		vertices[0].n.X = 0.0F;
		vertices[0].n.Y = 1.0F;
		vertices[0].n.Z = 0.0F;
		vertices[0].U = 0.0F + ouv->X;
		vertices[0].V = 0.0F + ouv->Y;

		vertices[1].X = p->X + (sz->X / 2.0F);
		vertices[1].Y = p->Y + (sz->Y / 2.0F);
		vertices[1].Z = p->Z + (sz->Z / 2.0F);
		vertices[1].n.X = 0.0F;
		vertices[1].n.Y = 1.0F;
		vertices[1].n.Z = 0.0F;
		vertices[1].U = (1.0F * suv->X) + ouv->X;
		vertices[1].V = 0.0F + ouv->Y;

		vertices[2].X = p->X + (sz->X / 2.0F);
		vertices[2].Y = p->Y + (sz->Y / 2.0F);
		vertices[2].Z = p->Z - (sz->Z / 2.0F);
		vertices[2].n.X = 0.0F;
		vertices[2].n.Y = 1.0F;
		vertices[2].n.Z = 0.0F;
		vertices[2].U = (1.0F * suv->X) + ouv->X;
		vertices[2].V = (1.0F * suv->Y) + ouv->Y;

		vertices[3].X = p->X - (sz->X / 2.0F);
		vertices[3].Y = p->Y + (sz->Y / 2.0F);
		vertices[3].Z = p->Z - (sz->Z / 2.0F);
		vertices[3].n.X = 0.0F;
		vertices[3].n.Y = 1.0F;
		vertices[3].n.Z = 0.0F;
		vertices[3].U = 0.0F + ouv->X;
		vertices[3].V = (1.0F * suv->Y) + ouv->Y;
	}

	if (tp == CBrushSide::BBS_RIGHT)
	{
		vertices[0].X = p->X + (sz->X / 2.0F);
		vertices[0].Y = p->Y + (sz->Y / 2.0F);
		vertices[0].Z = p->Z - (sz->Z / 2.0F);
		vertices[0].n.X = 1.0F;
		vertices[0].n.Y = 0.0F;
		vertices[0].n.Z = 0.0F;
		vertices[0].U = 0.0F + ouv->X;
		vertices[0].V = 0.0F + ouv->Y;

		vertices[1].X = p->X + (sz->X / 2.0F);
		vertices[1].Y = p->Y + (sz->Y / 2.0F);
		vertices[1].Z = p->Z + (sz->Z / 2.0F);
		vertices[1].n.X = 1.0F;
		vertices[1].n.Y = 0.0F;
		vertices[1].n.Z = 0.0F;
		vertices[1].U = (1.0F * suv->X) + ouv->X;
		vertices[1].V = 0.0F + ouv->Y;

		vertices[2].X = p->X + (sz->X / 2.0F);
		vertices[2].Y = p->Y - (sz->Y / 2.0F);
		vertices[2].Z = p->Z + (sz->Z / 2.0F);
		vertices[2].n.X = 1.0F;
		vertices[2].n.Y = 0.0F;
		vertices[2].n.Z = 0.0F;
		vertices[2].U = (1.0F * suv->X) + ouv->X;
		vertices[2].V = (1.0F * suv->Y) + ouv->Y;

		vertices[3].X = p->X + (sz->X / 2.0F);
		vertices[3].Y = p->Y - (sz->Y / 2.0F);
		vertices[3].Z = p->Z - (sz->Z / 2.0F);
		vertices[3].n.X = 1.0F;
		vertices[3].n.Y = 0.0F;
		vertices[3].n.Z = 0.0F;
		vertices[3].U = 0.0F + ouv->X;
		vertices[3].V = (1.0F * suv->Y) + ouv->Y;
	}

	if (tp == CBrushSide::BBS_BOTTOM)
	{
		vertices[0].X = p->X - (sz->X / 2.0F);
		vertices[0].Y = p->Y - (sz->Y / 2.0F);
		vertices[0].Z = p->Z - (sz->Z / 2.0F);
		vertices[0].n.X = 0.0F;
		vertices[0].n.Y = -1.0F;
		vertices[0].n.Z = 0.0F;
		vertices[0].U = 0.0F + ouv->X;
		vertices[0].V = 0.0F + ouv->Y;

		vertices[1].X = p->X + (sz->X / 2.0F);
		vertices[1].Y = p->Y - (sz->Y / 2.0F);
		vertices[1].Z = p->Z - (sz->Z / 2.0F);
		vertices[1].n.X = 0.0F;
		vertices[1].n.Y = -1.0F;
		vertices[1].n.Z = 0.0F;
		vertices[1].U = (1.0F * suv->X) + ouv->X;
		vertices[1].V = 0.0F + ouv->Y;

		vertices[2].X = p->X + (sz->X / 2.0F);
		vertices[2].Y = p->Y - (sz->Y / 2.0F);
		vertices[2].Z = p->Z + (sz->Z / 2.0F);
		vertices[2].n.X = 0.0F;
		vertices[2].n.Y = -1.0F;
		vertices[2].n.Z = 0.0F;
		vertices[2].U = (1.0F * suv->X) + ouv->X;
		vertices[2].V = (1.0F * suv->Y) + ouv->Y;

		vertices[3].X = p->X - (sz->X / 2.0F);
		vertices[3].Y = p->Y - (sz->Y / 2.0F);
		vertices[3].Z = p->Z + (sz->Z / 2.0F);
		vertices[3].n.X = 0.0F;
		vertices[3].n.Y = -1.0F;
		vertices[3].n.Z = 0.0F;
		vertices[3].U = 0.0F + ouv->X;
		vertices[3].V = (1.0F * suv->Y) + ouv->Y;
	}

	if (tp == CBrushSide::BBS_FRONT)
	{
		vertices[0].X = p->X - (sz->X / 2.0F);
		vertices[0].Y = p->Y + (sz->Y / 2.0F);
		vertices[0].Z = p->Z - (sz->Z / 2.0F);
		vertices[0].n.X = 0.0F;
		vertices[0].n.Y = 0.0F;
		vertices[0].n.Z = -1.0F;
		vertices[0].U = 0.0F + ouv->X;
		vertices[0].V = 0.0F + ouv->Y;

		vertices[1].X = p->X + (sz->X / 2.0F);
		vertices[1].Y = p->Y + (sz->Y / 2.0F);
		vertices[1].Z = p->Z - (sz->Z / 2.0F);
		vertices[1].n.X = 0.0F;
		vertices[1].n.Y = 0.0F;
		vertices[1].n.Z = -1.0F;
		vertices[1].U = (1.0F * suv->X) + ouv->X;
		vertices[1].V = 0.0F + ouv->Y;

		vertices[2].X = p->X + (sz->X / 2.0F);
		vertices[2].Y = p->Y - (sz->Y / 2.0F);
		vertices[2].Z = p->Z - (sz->Z / 2.0F);
		vertices[2].n.X = 0.0F;
		vertices[2].n.Y = 0.0F;
		vertices[2].n.Z = -1.0F;
		vertices[2].U = (1.0F * suv->X) + ouv->X;
		vertices[2].V = (1.0F * suv->Y) + ouv->Y;

		vertices[3].X = p->X - (sz->X / 2.0F);
		vertices[3].Y = p->Y - (sz->Y / 2.0F);
		vertices[3].Z = p->Z - (sz->Z / 2.0F);
		vertices[3].n.X = 0.0F;
		vertices[3].n.Y = 0.0F;
		vertices[3].n.Z = -1.0F;
		vertices[3].U = 0.0F + ouv->X;
		vertices[3].V = (1.0F * suv->Y) + ouv->Y;
	}

	if (tp == CBrushSide::BBS_BACK)
	{
		vertices[0].X = p->X + (sz->X / 2.0F);
		vertices[0].Y = p->Y + (sz->Y / 2.0F);
		vertices[0].Z = p->Z + (sz->Z / 2.0F);
		vertices[0].n.X = 0.0F;
		vertices[0].n.Y = 0.0F;
		vertices[0].n.Z = 1.0F;
		vertices[0].U = 0.0F + ouv->X;
		vertices[0].V = 0.0F + ouv->Y;

		vertices[1].X = p->X - (sz->X / 2.0F);
		vertices[1].Y = p->Y + (sz->Y / 2.0F);
		vertices[1].Z = p->Z + (sz->Z / 2.0F);
		vertices[1].n.X = 0.0F;
		vertices[1].n.Y = 0.0F;
		vertices[1].n.Z = 1.0F;
		vertices[1].U = (1.0F * suv->X) + ouv->X;
		vertices[1].V = 0.0F + ouv->Y;

		vertices[2].X = p->X - (sz->X / 2.0F);
		vertices[2].Y = p->Y - (sz->Y / 2.0F);
		vertices[2].Z = p->Z + (sz->Z / 2.0F);
		vertices[2].n.X = 0.0F;
		vertices[2].n.Y = 0.0F;
		vertices[2].n.Z = 1.0F;
		vertices[2].U = (1.0F * suv->X) + ouv->X;
		vertices[2].V = (1.0F * suv->Y) + ouv->Y;

		vertices[3].X = p->X + (sz->X / 2.0F);
		vertices[3].Y = p->Y - (sz->Y / 2.0F);
		vertices[3].Z = p->Z + (sz->Z / 2.0F);
		vertices[3].n.X = 0.0F;
		vertices[3].n.Y = 0.0F;
		vertices[3].n.Z = 1.0F;
		vertices[3].U = 0.0F + ouv->X;
		vertices[3].V = (1.0F * suv->Y) + ouv->Y;
	}

	vertexBuffer = new CVertexBuffer(globalObjects->errorLog, globalObjects->videoDevice, CVertex::VT_VERTEXNT, 4, (void*)vertices);

	index[0] = 0;
	index[1] = 2;
	index[2] = 3;
	index[3] = 0;
	index[4] = 1;
	index[5] = 2;

	indexBuffer = new CIndexBuffer(globalObjects->errorLog, globalObjects->videoDevice, 6, 2, index);

#ifdef _DEBUG
	lineEffect = globalObjects->effectMgr->GetEffect("line.fx");

	for (UINT nl = 0; nl < 4; nl++)
	{
		normalLine[nl] = new CLine(globalObjects);
	}

	CBrushSide::UpdateDebug();
#endif

	CBrushSide::SetEffectValues();
}

CBrushSide::CBrushSide(CBrushSide* bs)
{
	memset(this, 0x00, sizeof(CBrushSide));

	globalObjects = bs->globalObjects;

	texture = bs->texture;

	effect = bs->effect;

	type = bs->type;
	clip = bs->clip;
	draw = bs->draw;

	for (UINT i = 0; i < 4; i++)
	{
		vertices[i] = bs->vertices[i];
	}

	vertexBuffer = new CVertexBuffer(globalObjects->errorLog, globalObjects->videoDevice, CVertex::VT_VERTEXNT, 4, (void*)vertices);

	for (UINT i = 0; i < 6; i++)
	{
		index[i] = bs->index[i];
	}

	indexBuffer = new CIndexBuffer(globalObjects->errorLog, globalObjects->videoDevice, 6, 2, index);

#ifdef _DEBUG
	lineEffect = bs->lineEffect;

	for (UINT nl = 0; nl < 4; nl++)
	{
		normalLine[nl] = new CLine(globalObjects);
	}

	CBrushSide::UpdateDebug();
#endif

	CBrushSide::SetEffectValues();
}

CBrushSide::~CBrushSide()
{
	delete vertexBuffer;
	delete indexBuffer;

#ifdef _DEBUG
	for (UINT nl = 0; nl < 4; nl++)
	{
		delete normalLine[nl];
	}
#endif
}

void CBrushSide::Draw()
{
	if (draw != CBrushSide::SD_DRAW)
	{
		return;
	}

	if (effect)
	{
		hr = effect->effect->Begin(&effect->cPasses, 0);

		for (UINT i = 0; i < effect->cPasses; i++)
		{
			hr = effect->effect->BeginPass(i);

			hr = effect->effect->SetTexture(effect->htexture00, texture->image);

			hr = effect->effect->CommitChanges();

			vertexBuffer->DrawIndexed(indexBuffer);

			hr = effect->effect->EndPass();
		}

		hr = effect->effect->End();
	}

#ifdef _DEBUG
	if (lineEffect)
	{
		hr = lineEffect->effect->Begin(&lineEffect->cPasses, 0);

		for (UINT i = 0; i < lineEffect->cPasses; i++)
		{
			hr = lineEffect->effect->BeginPass(i);

			hr = lineEffect->effect->CommitChanges();

			for (UINT nl = 0; nl < 4; nl++)
			{
				normalLine[nl]->Draw();
			}

			hr = lineEffect->effect->EndPass();
		}

		hr = lineEffect->effect->End();
	}
#endif
}

void CBrushSide::Copy(CBrushSide* bs)
{
	globalObjects = bs->globalObjects;

	texture = bs->texture;
	effect = bs->effect;
	type = bs->type;
	clip = bs->clip;
	draw = bs->draw;

	for (UINT i = 0; i < 4; i++)
	{
		vertices[i] = bs->vertices[i];
	}

	vertexBuffer = new CVertexBuffer(globalObjects->errorLog, globalObjects->videoDevice, CVertex::VT_VERTEXNT, 4, (void*)vertices);

	for (UINT i = 0; i < 6; i++)
	{
		index[i] = bs->index[i];
	}

	indexBuffer = new CIndexBuffer(globalObjects->errorLog, globalObjects->videoDevice, 6, 2, index);

#ifdef _DEBUG
	for (UINT nl = 0; nl < 4; nl++)
	{
		normalLine[nl] = new CLine(bs->normalLine[nl]);
	}
#endif

	CBrushSide::SetEffectValues();
}

void CBrushSide::SetEffectValues()
{
	if (effect)
	{
		hr = effect->effect->SetTexture(effect->htexture00, texture->image);
		hr = effect->effect->SetTexture(effect->htexture01, globalObjects->textureMgr->GetTexture("stoneWallao.tga")->image);

		hr = effect->effect->SetFloatArray(effect->hlightAmbient, White.GetFloatArray(), 4);
		hr = effect->effect->SetFloatArray(effect->hmaterial, White.GetFloatArray(), 4);
	}
}

#ifdef _DEBUG
void CBrushSide::UpdateDebug()
{
	CLine l;

	l.line[0].X = vertices[0].X;
	l.line[0].Y = vertices[0].Y;
	l.line[0].Z = vertices[0].Z;

	l.line[1].X = l.line[0].X + (vertices[0].n.X * 8.0F);
	l.line[1].Y = l.line[0].Y + (vertices[0].n.Y * 8.0F);
	l.line[1].Z = l.line[0].Z + (vertices[0].n.Z * 8.0F);

	normalLine[0]->Load(&l.line[0], &l.line[1]);


	l.line[0].X = vertices[1].X;
	l.line[0].Y = vertices[1].Y;
	l.line[0].Z = vertices[1].Z;

	l.line[1].X = l.line[0].X + (vertices[0].n.X * 8.0F);
	l.line[1].Y = l.line[0].Y + (vertices[0].n.Y * 8.0F);
	l.line[1].Z = l.line[0].Z + (vertices[0].n.Z * 8.0F);

	normalLine[1]->Load(&l.line[0], &l.line[1]);


	l.line[0].X = vertices[2].X;
	l.line[0].Y = vertices[2].Y;
	l.line[0].Z = vertices[2].Z;

	l.line[1].X = l.line[0].X + (vertices[0].n.X * 8.0F);
	l.line[1].Y = l.line[0].Y + (vertices[0].n.Y * 8.0F);
	l.line[1].Z = l.line[0].Z + (vertices[0].n.Z * 8.0F);

	normalLine[2]->Load(&l.line[0], &l.line[1]);


	l.line[0].X = vertices[3].X;
	l.line[0].Y = vertices[3].Y;
	l.line[0].Z = vertices[3].Z;

	l.line[1].X = l.line[0].X + (vertices[0].n.X * 8.0F);
	l.line[1].Y = l.line[0].Y + (vertices[0].n.Y * 8.0F);
	l.line[1].Z = l.line[0].Z + (vertices[0].n.Z * 8.0F);

	normalLine[3]->Load(&l.line[0], &l.line[1]);
}
#endif