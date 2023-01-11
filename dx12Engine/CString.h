#pragma once

#include "standard.h"

class CString
{
public:

	CString();

	CString(const char* s);

	CString(UINT l);

	~CString();


	UINT GetLength() { return m_length; }

	char* GetText() { return m_text; }

	void Clear();

	void Format(const char* format, ...);

	void Concat(const char* c);

	bool Search(const char* c);

private:

	UINT m_length;

	char* m_text;
};