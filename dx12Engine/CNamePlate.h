#ifndef _CNAMEPLATE
#define _CNAMEPLATE

#include "standard.h"

#include "CBillboard.h"
#include "CErrorLog.h"
#include "CFontManager.h"
#include "CString.h"
#include "CTexture.h"
#include "CVideoDevice.h"

class CNamePlate
{
	public:
		
		CString*	name;

		CNamePlate();
		CNamePlate::CNamePlate(CErrorLog* el, CVideoDevice* d, char* f, char* text, COLORREF color);

		~CNamePlate();

		void CNamePlate::Draw();

	private:

		HRESULT			hr;
		CErrorLog*		errorLog;
		CVideoDevice*	videoDevice;

		IDirect3DSurface9*	surface;
		IDirect3DSurface9*	tempSurf;
		IDirect3DTexture9*	texture;

		HDC				hdc;

		CBillboard*		billboard;
		
		SIZE			size;
		HFONT			hFont;

		RECT			sr;

};

#endif