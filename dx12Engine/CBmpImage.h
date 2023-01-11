#pragma once

#include "standard.h"

class CBmpImage
{
public:
		
	BITMAPFILEHEADER m_bmapHeader;
	BITMAPINFO m_bmapInfo;
	PALETTEENTRY m_paletteentry[256];

	int m_bytesPerPixel;

	BYTE* m_pixels8;
	BYTE* m_pixels32;

	bool m_isInitialized;

	
	CBmpImage();
	
	CBmpImage(const char* name);

	~CBmpImage();

	
	void WriteBitmapHeader(UINT width, UINT height, FILE* file);
	
	void WriteGreyscalePalette(FILE* file);

private:

	FILE* m_file;

	errno_t m_err;

	size_t m_size8;
	size_t m_size32;

	BYTE* m_p8;
	BYTE* m_p32;
	
	size_t m_bytesRead;
	size_t m_count;
};