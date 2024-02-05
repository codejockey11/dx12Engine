#include "CNamePlate.h"


CNamePlate::CNamePlate()
{
	memset(this, 0x00, sizeof(CNamePlate));

}


CNamePlate::~CNamePlate()
{
	SAFE_DELETE(name);
	SAFE_RELEASE(texture);
	SAFE_DELETE(billboard);

}


CNamePlate::CNamePlate(CErrorLog* el, CVideoDevice* d, char* f, char* text, COLORREF color)
{
	memset(this, 0x00, sizeof(CNamePlate));

	errorLog = el;
	videoDevice = d;

	name = new CString(text);

	hFont = CreateFont(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_charSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, VARIABLE_PITCH, f);

	hr = videoDevice->device->CreateOffscreenPlainSurface(4, 4, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);
	if (FAILED(hr))
	{
		errorLog->WriteError("CNamePlate::CNamePlate::CreateOffscreenPlainSurface:%s:%s\n", text, DXGetErrorString(hr));
		return;
	}

	hr = surface->GetDC(&hdc);
	if (FAILED(hr))
	{
		errorLog->WriteError("CNamePlate::CNamePlate::GetDC:%s\n", DXGetErrorString(hr));
		return;
	}

	SelectObject(hdc, hFont);

	GetTextExtentPoint32(hdc, name->text, name->length, &size);

	hr = surface->ReleaseDC(hdc);
	if (FAILED(hr))
	{
		errorLog->WriteError("CNamePlate::CNamePlate::ReleaseDC:%s\n", DXGetErrorString(hr));
		return;
	}

	SAFE_RELEASE(surface);


	hr = videoDevice->device->CreateOffscreenPlainSurface(size.cx, size.cy, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);
	if (FAILED(hr))
	{
		errorLog->WriteError("CNamePlate::CNamePlate::CreateOffscreenPlainSurface:%s\n", DXGetErrorString(hr));
		return;
	}

	sr.left = 0;
	sr.top = 0;
	sr.right = size.cx;
	sr.bottom = size.cy;

	hr = videoDevice->device->ColorFill(surface, &sr, D3DCOLOR_XRGB(0, 0, 0));
	if (FAILED(hr))
	{
		errorLog->WriteError("CNamePlate::CNamePlate::ColorFill:%s\n", DXGetErrorString(hr));
		return;
	}

	hr = surface->GetDC(&hdc);
	if (FAILED(hr))
	{
		errorLog->WriteError("CNamePlate::CNamePlate::GetDC:%s\n", DXGetErrorString(hr));
		return;
	}

	SelectObject(hdc, hFont);

	SetTextColor(hdc, color);
	SetBkMode(hdc, TRANSPARENT);

	TextOut(hdc, 0, 0, name->text, name->length);

	hr = surface->ReleaseDC(hdc);
	if (FAILED(hr))
	{
		errorLog->WriteError("CNamePlate::CNamePlate::ReleaseDC:%s\n", DXGetErrorString(hr));
		return;
	}

	hr = D3DXCreateTexture(videoDevice->device, size.cx, size.cy, 0, D3DX_DEFAULT, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture);
	if (FAILED(hr))
	{
		errorLog->WriteError("CNamePlate::CNamePlate::D3DXCreateTexture:%s\n", DXGetErrorString(hr));
		return;
	}

	hr = texture->GetSurfaceLevel(0, &tempSurf);
	if (FAILED(hr))
	{
		errorLog->WriteError("CNamePlate::CNamePlate::GetSurfaceLevel:%s\n", DXGetErrorString(hr));
		return;
	}

	hr = D3DXLoadSurfaceFromSurface(tempSurf, NULL, NULL, surface, NULL, NULL, D3DX_FILTER_NONE, D3DCOLOR_XRGB(0,0,0));
	if (FAILED(hr))
	{
		errorLog->WriteError("CNamePlate::CNamePlate::D3DXLoadSurfaceFromSurface:%s\n", DXGetErrorString(hr));
		return;
	}

	DeleteObject(hFont);

	SAFE_RELEASE(surface);
	
	billboard = new CBillboard(errorLog, videoDevice, nullptr, nullptr, 2, 1, &CVec3(100, 2, 100));

}


void CNamePlate::Draw()
{
	hr = videoDevice->device->SetTexture(0, texture);
	if (FAILED(hr))
	{
		errorLog->WriteError("void CNamePlate::Draw::SetTexture:%s\n", DXGetErrorString(hr));
		return;
	}

	billboard->Draw();

}