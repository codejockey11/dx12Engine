#pragma once

#include "standard.h"

#include "CErrorLog.h"
#include "CLinkList.h"
#include "CTexture.h"
#include "CVideoDevice.h"

class CTextureManager
{
public:

	CTextureManager();

	CTextureManager(CErrorLog* errorLog, CVideoDevice* videoDevice);

	~CTextureManager();


	CTexture* Create(const char* filename);

	CTexture* Get(const char* filename);

	void Delete(const char* filename);

private:

	CErrorLog* m_errorLog;

	CVideoDevice* m_videoDevice;

	CLinkList<CTexture>* m_textures;
};