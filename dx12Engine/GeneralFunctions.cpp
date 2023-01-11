#include "standard.h"

size_t	gfsize;

WCHAR	gfwcs[1024] = {};
char	gfstr[1024] = {};

__declspec(dllexport) WCHAR* MBSToWide(const char* s)
{
	mbstowcs_s(&gfsize, gfwcs, 1024, s, strlen(s));

	return gfwcs;
}

__declspec(dllexport) char* WideToMBS(const WCHAR* s)
{
	wcstombs_s(&gfsize, gfstr, 1024, s, wcslen(s));

	return gfstr;
}