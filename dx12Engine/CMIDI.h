#ifndef _CMIDI
#define _CMIDI

#include "standard.h"

#include "CGlobalObjects.h"
#include "CString.h"

class CMidi
{
public:

	CString* filename;

	CMidi();

	CMidi(CGlobalObjects* g, const CHAR* fn);

	~CMidi();

	VOID PlayMidi();

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

	UINT deviceID;

	DWORD result;

	MCI_OPEN_PARMS mciOpenParms;
	
	MCI_PLAY_PARMS mciPlayParms;
	
	MCI_STATUS_PARMS mciStatusParms;
	
	MCI_SEQ_SET_PARMS mciSeqSetParms;
};
#endif