#include "CBrushMap.h"

CBrushMap::CBrushMap()
{
	memset(this, 0x00, sizeof(CBrushMap));
}

CBrushMap::CBrushMap(CGlobalObjects* g, const char* mapName)
{
	memset(this, 0x00, sizeof(CBrushMap));

	globalObjects = g;

	wc = 0;
	hc = 0;

	cubeSize.X = 256.0F;
	cubeSize.Y = 256.0F;
	cubeSize.Z = 256.0F;

	textureScale.X = 1.00F;
	textureScale.Y = 1.00F;

	textureOffset.X = 0.0F;
	textureOffset.Y = 0.0F;

	err = fopen_s(&fp, mapName, "r");

	br = fread_s(&sizeBuffer, 3 * sizeof(char), sizeof(char), 3, fp);

	width = atoi(sizeBuffer);

	br = fread_s(&sizeBuffer, 3 * sizeof(char), sizeof(char), 3, fp);

	height = atoi(sizeBuffer);

	br = fread_s(&readBuffer, sizeof(BYTE), sizeof(BYTE), 1, fp);


	position.X = (FLOAT)(-(cubeSize.X * width)) / 2;
	position.Z = (FLOAT)((cubeSize.Z * height)) / 2;

	brushes = new CHeapArray(sizeof(CBrush), TRUE, TRUE, 2, width, height);


	br = fread_s(&readBuffer, sizeof(BYTE), sizeof(BYTE), 1, fp);

	while (!feof(fp))
	{
		switch (readBuffer)
		{
			case ' ':
			{
				position.Y -= cubeSize.Y;

				brush = new CBrush(globalObjects, &position, &cubeSize, &textureScale, &textureOffset);

				position.Y += cubeSize.Y;

				for (UINT i = 0; i < 6; i++)
				{
					if (brush->side[i]->type != CBrushSide::BBS_TOP)
					{
						brush->side[i]->draw = CBrushSide::SD_NODRAW;
						brush->side[i]->clip = CBrushSide::SC_NOCLIP;
					}
				}

				break;
			}

			case 'A':
			{
				brush = new CBrush(globalObjects, &position, &cubeSize, &textureScale, &textureOffset);

				for (UINT i = 0; i < 6; i++)
				{
					if ((brush->side[i]->type == CBrushSide::BBS_TOP) || (brush->side[i]->type == CBrushSide::BBS_BOTTOM))
					{
						brush->side[i]->draw = CBrushSide::SD_NODRAW;
						brush->side[i]->clip = CBrushSide::SC_NOCLIP;
					}
				}

				break;
			}

			case 'R':
			{
				position.Y -= cubeSize.Y - 1.0F;
				brush = new CBrush(globalObjects, &position, &cubeSize, &textureScale, &textureOffset);
				position.Y += cubeSize.Y - 1.0F;

				for (UINT i = 0; i < 6; i++)
				{
					if (brush->side[i]->type == CBrushSide::BBS_TOP)
					{
						brush->side[CBrushSide::BBS_TOP]->texture = globalObjects->textureMgr->GetTexture("red.png");
					}
					else if (brush->side[i]->type == CBrushSide::BBS_BOTTOM)
					{
						brush->side[i]->draw = CBrushSide::SD_NODRAW;
						brush->side[i]->clip = CBrushSide::SC_NOCLIP;
					}
				}

				break;
			}

			case 'B':
			{
				position.Y -= cubeSize.Y - 1.0F;
				brush = new CBrush(globalObjects, &position, &cubeSize, &textureScale, &textureOffset);
				position.Y += cubeSize.Y - 1.0F;

				for (UINT i = 0; i < 6; i++)
				{
					if (brush->side[i]->type == CBrushSide::BBS_TOP)
					{
						brush->side[CBrushSide::BBS_TOP]->texture = globalObjects->textureMgr->GetTexture("blue.png");
					}
					else if (brush->side[i]->type == CBrushSide::BBS_BOTTOM)
					{
						brush->side[i]->draw = CBrushSide::SD_NODRAW;
						brush->side[i]->clip = CBrushSide::SC_NOCLIP;
					}
				}

				break;
			}

			case 'L':
			{
				position.Y -= cubeSize.Y - 1.0F;
				brush = new CBrush(globalObjects, &position, &cubeSize, &textureScale, &textureOffset);
				position.Y += cubeSize.Y - 1.0F;

				for (UINT i = 0; i < 6; i++)
				{
					if (brush->side[i]->type == CBrushSide::BBS_TOP)
					{
						brush->side[CBrushSide::BBS_TOP]->texture = globalObjects->textureMgr->GetTexture("lock.png");
					}
					else if (brush->side[i]->type == CBrushSide::BBS_BOTTOM)
					{
						brush->side[i]->draw = CBrushSide::SD_NODRAW;
						brush->side[i]->clip = CBrushSide::SC_NOCLIP;
					}
				}

				break;
			}

			case 'K':
			{
				position.Y -= cubeSize.Y - 1.0F;
				brush = new CBrush(globalObjects, &position, &cubeSize, &textureScale, &textureOffset);
				position.Y += cubeSize.Y - 1.0F;

				for (UINT i = 0; i < 6; i++)
				{
					if (brush->side[i]->type == CBrushSide::BBS_TOP)
					{
						brush->side[CBrushSide::BBS_TOP]->texture = globalObjects->textureMgr->GetTexture("key.png");
					}
					else if (brush->side[i]->type == CBrushSide::BBS_BOTTOM)
					{
						brush->side[i]->draw = CBrushSide::SD_NODRAW;
						brush->side[i]->clip = CBrushSide::SC_NOCLIP;
					}
				}

				break;
			}

			case '\n':
			{
				brush = NULL;

				break;
			}

			default:
			{
				brush = NULL;

				break;
			}
		}

		if (brush)
		{
			CBrush* b = (CBrush*)brushes->GetElement(2, wc, hc);
			
			b->Copy(brush);

			delete brush;

			wc++;

			if (wc == width)
			{
				position.X = (FLOAT)(-(cubeSize.X * width)) / 2;

				position.Z -= cubeSize.Z;

				wc = 0;

				hc++;
			}
			else
			{
				position.X += cubeSize.X;
			}
		}

		br = fread_s(&readBuffer, sizeof(BYTE), sizeof(BYTE), 1, fp);
	}

	fclose(fp);

	// Compiler

	CBrush* corner = (CBrush*)brushes->GetElement(2, 0, 0);

	corner->side[CBrushSide::BBS_LEFT]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_LEFT]->clip = CBrushSide::SC_NOCLIP;

	corner->side[CBrushSide::BBS_RIGHT]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_RIGHT]->clip = CBrushSide::SC_NOCLIP;

	corner->side[CBrushSide::BBS_FRONT]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_FRONT]->clip = CBrushSide::SC_NOCLIP;

	corner->side[CBrushSide::BBS_BACK]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_BACK]->clip = CBrushSide::SC_NOCLIP;

	corner = (CBrush*)brushes->GetElement(2, width - 1, 0);

	corner->side[CBrushSide::BBS_LEFT]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_LEFT]->clip = CBrushSide::SC_NOCLIP;

	corner->side[CBrushSide::BBS_RIGHT]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_RIGHT]->clip = CBrushSide::SC_NOCLIP;

	corner->side[CBrushSide::BBS_FRONT]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_FRONT]->clip = CBrushSide::SC_NOCLIP;

	corner->side[CBrushSide::BBS_BACK]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_BACK]->clip = CBrushSide::SC_NOCLIP;

	corner = (CBrush*)brushes->GetElement(2, 0, height - 1);

	corner->side[CBrushSide::BBS_LEFT]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_LEFT]->clip = CBrushSide::SC_NOCLIP;

	corner->side[CBrushSide::BBS_RIGHT]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_RIGHT]->clip = CBrushSide::SC_NOCLIP;

	corner->side[CBrushSide::BBS_FRONT]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_FRONT]->clip = CBrushSide::SC_NOCLIP;

	corner->side[CBrushSide::BBS_BACK]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_BACK]->clip = CBrushSide::SC_NOCLIP;

	corner = (CBrush*)brushes->GetElement(2, width - 1, height - 1);

	corner->side[CBrushSide::BBS_LEFT]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_LEFT]->clip = CBrushSide::SC_NOCLIP;

	corner->side[CBrushSide::BBS_RIGHT]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_RIGHT]->clip = CBrushSide::SC_NOCLIP;

	corner->side[CBrushSide::BBS_FRONT]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_FRONT]->clip = CBrushSide::SC_NOCLIP;

	corner->side[CBrushSide::BBS_BACK]->draw = CBrushSide::SD_NODRAW;
	corner->side[CBrushSide::BBS_BACK]->clip = CBrushSide::SC_NOCLIP;

	for (UINT h = 1; h < height - 1; h++)
	{
		for (UINT w = 1; w < width - 1; w++)
		{
			UINT ww = w - 1;
			UINT ew = w + 1;
			UINT nh = h - 1;
			UINT sh = h + 1;

			CBrush* b = (CBrush*)brushes->GetElement(2, w, h);

			CBrush* wwb = (CBrush*)brushes->GetElement(2, ww, h);

			CBrush* ewb = (CBrush*)brushes->GetElement(2, ew, h);

			CBrush* shb = (CBrush*)brushes->GetElement(2, w, sh);

			CBrush* nhb = (CBrush*)brushes->GetElement(2, w, nh);

			if (ww == 0)
			{
				wwb->side[CBrushSide::BBS_LEFT]->draw = CBrushSide::SD_NODRAW;
				wwb->side[CBrushSide::BBS_LEFT]->clip = CBrushSide::SC_NOCLIP;

				wwb->side[CBrushSide::BBS_FRONT]->draw = CBrushSide::SD_NODRAW;
				wwb->side[CBrushSide::BBS_FRONT]->clip = CBrushSide::SC_NOCLIP;

				wwb->side[CBrushSide::BBS_BACK]->draw = CBrushSide::SD_NODRAW;
				wwb->side[CBrushSide::BBS_BACK]->clip = CBrushSide::SC_NOCLIP;
			}

			if (wwb->side[CBrushSide::BBS_RIGHT]->vertices[0].Y == b->side[CBrushSide::BBS_LEFT]->vertices[0].Y)
			{
				wwb->side[CBrushSide::BBS_RIGHT]->draw = CBrushSide::SD_NODRAW;
				wwb->side[CBrushSide::BBS_RIGHT]->clip = CBrushSide::SC_NOCLIP;

				b->side[CBrushSide::BBS_LEFT]->draw = CBrushSide::SD_NODRAW;
				b->side[CBrushSide::BBS_LEFT]->clip = CBrushSide::SC_NOCLIP;
			}

			if (ewb->side[CBrushSide::BBS_LEFT]->vertices[0].Y == b->side[CBrushSide::BBS_RIGHT]->vertices[0].Y)
			{
				ewb->side[CBrushSide::BBS_LEFT]->draw = CBrushSide::SD_NODRAW;
				ewb->side[CBrushSide::BBS_LEFT]->clip = CBrushSide::SC_NOCLIP;

				b->side[CBrushSide::BBS_RIGHT]->draw = CBrushSide::SD_NODRAW;
				b->side[CBrushSide::BBS_RIGHT]->clip = CBrushSide::SC_NOCLIP;
			}

			if (ew == (width - 1))
			{
				ewb->side[CBrushSide::BBS_RIGHT]->draw = CBrushSide::SD_NODRAW;
				ewb->side[CBrushSide::BBS_RIGHT]->clip = CBrushSide::SC_NOCLIP;

				ewb->side[CBrushSide::BBS_FRONT]->draw = CBrushSide::SD_NODRAW;
				ewb->side[CBrushSide::BBS_FRONT]->clip = CBrushSide::SC_NOCLIP;

				ewb->side[CBrushSide::BBS_BACK]->draw = CBrushSide::SD_NODRAW;
				ewb->side[CBrushSide::BBS_BACK]->clip = CBrushSide::SC_NOCLIP;
			}

			if (shb->side[CBrushSide::BBS_BACK]->vertices[0].Y == b->side[CBrushSide::BBS_FRONT]->vertices[0].Y)
			{
				shb->side[CBrushSide::BBS_BACK]->draw = CBrushSide::SD_NODRAW;
				shb->side[CBrushSide::BBS_BACK]->clip = CBrushSide::SC_NOCLIP;

				b->side[CBrushSide::BBS_FRONT]->draw = CBrushSide::SD_NODRAW;
				b->side[CBrushSide::BBS_FRONT]->clip = CBrushSide::SC_NOCLIP;
			}

			if (sh == (height - 1))
			{
				shb->side[CBrushSide::BBS_FRONT]->draw = CBrushSide::SD_NODRAW;
				shb->side[CBrushSide::BBS_FRONT]->clip = CBrushSide::SC_NOCLIP;

				shb->side[CBrushSide::BBS_LEFT]->draw = CBrushSide::SD_NODRAW;
				shb->side[CBrushSide::BBS_LEFT]->clip = CBrushSide::SC_NOCLIP;

				shb->side[CBrushSide::BBS_RIGHT]->draw = CBrushSide::SD_NODRAW;
				shb->side[CBrushSide::BBS_RIGHT]->clip = CBrushSide::SC_NOCLIP;
			}

			if (nhb->side[CBrushSide::BBS_FRONT]->vertices[0].Y == b->side[CBrushSide::BBS_BACK]->vertices[0].Y)
			{
				nhb->side[CBrushSide::BBS_FRONT]->draw = CBrushSide::SD_NODRAW;
				nhb->side[CBrushSide::BBS_FRONT]->clip = CBrushSide::SC_NOCLIP;

				b->side[CBrushSide::BBS_BACK]->draw = CBrushSide::SD_NODRAW;
				b->side[CBrushSide::BBS_BACK]->clip = CBrushSide::SC_NOCLIP;
			}

			if (nh == 0)
			{
				nhb->side[CBrushSide::BBS_BACK]->draw = CBrushSide::SD_NODRAW;
				nhb->side[CBrushSide::BBS_BACK]->clip = CBrushSide::SC_NOCLIP;

				nhb->side[CBrushSide::BBS_LEFT]->draw = CBrushSide::SD_NODRAW;
				nhb->side[CBrushSide::BBS_LEFT]->clip = CBrushSide::SC_NOCLIP;

				nhb->side[CBrushSide::BBS_RIGHT]->draw = CBrushSide::SD_NODRAW;
				nhb->side[CBrushSide::BBS_RIGHT]->clip = CBrushSide::SC_NOCLIP;
			}
		}
	}
}

CBrushMap::~CBrushMap()
{
	delete brushes;
}

void CBrushMap::Draw()
{
	for (UINT h = 0; h < height; h++)
	{
		for (UINT w = 0; w < width; w++)
		{
			CBrush* b = (CBrush*)brushes->GetElement(2, w, h);

			b->Draw();
		}
	}
}