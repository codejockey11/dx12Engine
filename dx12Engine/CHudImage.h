#ifndef _CHUDIMAGE
#define _CHUDIMAGE

#include "standard.h"

#include "CErrorLog.h"
#include "CFrametime.h"
#include "CIndexBuffer.h"
#include "CTextureManager.h"
#include "CVertexBuffer.h"
#include "CVideoDevice.h"

class CHudImage
{
public:

	CTexture* texture;

	CIndexBuffer* indexBuffer;

	CVertexBuffer* vertexBuffer;

	CVertex2d vertices[4];
	
	CVertex2d saveVertices[4];

	CVertex2 position;
	
	CVertex2 size;

	CVertex rotation;

	FLOAT rotX;
	FLOAT rotY;
	FLOAT rotZ;

	FLOAT pitch;
	FLOAT roll;
	FLOAT yaw;

	CHudImage();

	CHudImage(CErrorLog* el, CVideoDevice* d, CTextureManager* tm, CFrametime* f, const CHAR* i, CVertex2* s, CVertex2* p, CVertex* r);

	~CHudImage();

	VOID Draw();

	VOID Rotation();

	VOID UpdateRotation();

	VOID UpdatePosition();

private:

	HRESULT hr;

	CErrorLog* errorLog;
	CVideoDevice* videoDevice;
	CTextureManager* textureMgr;
	CFrametime* frametime;

	DWORD index[6];
};
#endif