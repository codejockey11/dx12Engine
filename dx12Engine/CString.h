#pragma once

#include "standard.h"

class CString
{
public:

	CString();
	CString(UINT l);
	CString(const char* c);
	CString(wchar_t* wc);
	~CString();

	void Clear();
	int Compare(const char* c);
	void Concat(const char* c);
	void Format(const char* format, ...);
	UINT GetLength();
	char* GetText();
	wchar_t* GetWText();
	char* MBToWide(const wchar_t* wc);
	bool Search(const char* c);

private:

	UINT m_length;
	UINT m_wlength;
	
	char* m_tempText;
	char* m_text;
	
	size_t	gfsize;
	
	wchar_t* m_wtext;
};