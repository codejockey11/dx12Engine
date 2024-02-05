#include "CWavefront.h"

CObjModel::CObjModel()
{
	memset(this, 0x00, sizeof(CObjModel));
}

CObjModel::CObjModel(CGlobalObjects* g, CLinkList<CVertexBuffer>* vertexBuffers, CLinkList<CString>* materials, const CHAR* fn)
{
	memset(this, 0x00, sizeof(CObjModel));

	globalObjects = g;

	filename = new CString(fn);

	err = fopen_s(&file, filename->GetText(), "r");

	if (err)
	{
		globalObjects->errorLog->WriteError("CObjModel::CObjModel::fopen_s:%s\n", filename->GetText());

		return;
	}

	BYTE ft = TRUE;

	if (file)
	{
		while (fscanf_s(file, "%s", line, CObjModel::MAX_LINE_LENGTH) != EOF)
		{
			if (strcmp(line, "usemtl") == 0)
			{
				br = fscanf_s(file, "%s\n", &materialName[inbr], CObjModel::MAX_MATERIAL_LENGTH);

				CString* m = new CString(materialName[inbr]);

				materials->Add(m, m->GetText());

				inbr++;

				if (ft)
				{
					ft = FALSE;
				}
				else if (inbr > 0)
				{
					vertexBuffer = new CVertexBuffer(globalObjects, CVertex::VT_VERTEXNT, vntcount, vnt);

					vertexBuffers->Add(vertexBuffer, NULL);

					vntcount = 0;
				}
			}

			if (strcmp(line, "v") == 0)
			{
				br = fscanf_s(file, "%f %f %f\n", &vertex[vindex].X, &vertex[vindex].Y, &vertex[vindex].Z);
				
				vindex++;
			}

			if (strcmp(line, "vt") == 0)
			{
				br = fscanf_s(file, "%f %f\n", &uv[uvindex].X, &uv[uvindex].Y);
				
				uvindex++;
			}

			if (strcmp(line, "vn") == 0)
			{
				br = fscanf_s(file, "%f %f %f\n", &normal[nindex].X, &normal[nindex].Y, &normal[nindex].Z);

				nindex++;
			}

			if (strcmp(line, "f") == 0)
			{
				for (UINT i = 0; i < 8; i++)
				{
					vi[i] = uvi[i] = ni[i] = 0;
				}

				br = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vi[0], &uvi[0], &ni[0],
					&vi[1], &uvi[1], &ni[1],
					&vi[2], &uvi[2], &ni[2],
					&vi[3], &uvi[3], &ni[3],
					&vi[4], &uvi[4], &ni[4],
					&vi[5], &uvi[5], &ni[5],
					&vi[6], &uvi[6], &ni[6],
					&vi[7], &uvi[7], &ni[7]
				);

				for (UINT i = 0; i < 3; i++)
				{
					if ((vi[i] > 0) && (ni[i] > 0) && (uvi[i] > 0))
					{
						vnt[vntcount].X = vertex[vi[i] - 1].X;
						vnt[vntcount].Y = vertex[vi[i] - 1].Y;
						vnt[vntcount].Z = vertex[vi[i] - 1].Z;

						vnt[vntcount].n.X = normal[ni[i] - 1].X;
						vnt[vntcount].n.Y = normal[ni[i] - 1].Y;
						vnt[vntcount].n.Z = normal[ni[i] - 1].Z;

						vnt[vntcount].U = uv[uvi[i] - 1].X;
						vnt[vntcount].V = uv[uvi[i] - 1].Y;

						vntcount++;
					}
				}

				maxIndex = 0;

				for (UINT i = 3; i < 8; i++)
				{
					if (vi[i] != 0)
					{
						maxIndex = i - 2;
					}
				}

				for (INT i = 0; i < maxIndex; i++)
				{
					vnt[vntcount].X = vertex[vi[i] - 1].X;
					vnt[vntcount].Y = vertex[vi[i] - 1].Y;
					vnt[vntcount].Z = vertex[vi[i] - 1].Z;

					vnt[vntcount].n.X = normal[ni[i] - 1].X;
					vnt[vntcount].n.Y = normal[ni[i] - 1].Y;
					vnt[vntcount].n.Z = normal[ni[i] - 1].Z;

					vnt[vntcount].U = uv[uvi[i] - 1].X;
					vnt[vntcount].V = uv[uvi[i] - 1].Y;

					vntcount++;


					vnt[vntcount].X = vertex[vi[i + 2] - 1].X;
					vnt[vntcount].Y = vertex[vi[i + 2] - 1].Y;
					vnt[vntcount].Z = vertex[vi[i + 2] - 1].Z;

					vnt[vntcount].n.X = normal[ni[i + 2] - 1].X;
					vnt[vntcount].n.Y = normal[ni[i + 2] - 1].Y;
					vnt[vntcount].n.Z = normal[ni[i + 2] - 1].Z;

					vnt[vntcount].U = uv[uvi[i + 2] - 1].X;
					vnt[vntcount].V = uv[uvi[i + 2] - 1].Y;

					vntcount++;


					vnt[vntcount].X = vertex[vi[i + 3] - 1].X;
					vnt[vntcount].Y = vertex[vi[i + 3] - 1].Y;
					vnt[vntcount].Z = vertex[vi[i + 3] - 1].Z;

					vnt[vntcount].n.X = normal[ni[i + 3] - 1].X;
					vnt[vntcount].n.Y = normal[ni[i + 3] - 1].Y;
					vnt[vntcount].n.Z = normal[ni[i + 3] - 1].Z;

					vnt[vntcount].U = uv[uvi[i + 3] - 1].X;
					vnt[vntcount].V = uv[uvi[i + 3] - 1].Y;

					vntcount++;
				}
			}
		}

		vertexBuffer = new CVertexBuffer(globalObjects, CVertex::VT_VERTEXNT, vntcount, vnt);

		vertexBuffers->Add(vertexBuffer, NULL);

		fclose(file);
	}
}

CObjModel::~CObjModel()
{
	delete filename;
}
//
// f: 0 1 2 3
//
// 0 1 2
// 0 2 3
//
// 3--2
// | /|
// |/ |
// 0--1
//
// f: A B C D E F G
//
// A B C
// A C D
// B D E
// C E F
// D F G
//
