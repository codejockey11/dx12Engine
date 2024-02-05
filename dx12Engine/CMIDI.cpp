#include "CMidi.h"

CMidi::CMidi()
{
	memset(this, 0x00, sizeof(CMidi));
}

CMidi::CMidi(CGlobalObjects* g, const CHAR* fn)
{
	memset(this, 0x00, sizeof(CMidi));

	globalObjects = g;

	filename = new CString(fn);

	/*
	docs.microsoft.com/en-us/windows/win32/multimedia/device-names
	cdaudio=mcicda.drv
	sequencer=mciseq.drv
	waveaudio=mciwave.drv
	avivideo=mciavi.drv
	*/
	mciOpenParms.lpstrDeviceType = "sequencer";
	mciOpenParms.lpstrElementName = filename->GetText();
}

CMidi::~CMidi()
{
	delete filename;
}

VOID CMidi::PlayMidi()
{
	// Open the device by specifying the device and filename.
	// MCI will attempt to choose the MIDI mapper as the output port.
	if (result = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)& mciOpenParms))
	{
		globalObjects->errorLog->WriteError("CMidi::PlayMidi::mciSendCommand::MCI_OPEN Failed:%d\n", result);

		return;
	}

	// The device opened successfully; get the device ID.
	deviceID = mciOpenParms.wDeviceID;

	// Check if the output port is the MIDI mapper.
	mciStatusParms.dwItem = MCI_SEQ_STATUS_PORT;

	if (result = mciSendCommand(deviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)& mciStatusParms))
	{
		globalObjects->errorLog->WriteError("CMidi::PlayMidi::mciSendCommand::MCI_STATUS Failed:%d\n", result);

		mciSendCommand(deviceID, MCI_CLOSE, 0, NULL);

		return;
	}

	// Just stating that the output port is not the MIDI mapper.
	if (LOWORD(mciStatusParms.dwReturn) != MIDI_MAPPER)
	{
		globalObjects->errorLog->WriteError("CMidi::PlayMidi::mciSendCommand::The MIDI mapper is not available\n");
	}

	// Begin playback. The window procedure function for the parent
	// window will be notified with an MM_MCINOTIFY message when
	// playback is complete. At this time, the window procedure closes
	// the device.
	mciPlayParms.dwCallback = (DWORD_PTR)globalObjects->window->hWnd;

	if (result = mciSendCommand(deviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)& mciPlayParms))
	{
		globalObjects->errorLog->WriteError("CMidi::PlayMidi::mciSendCommand::MCI_PLAY Failed:%d\n", result);

		mciSendCommand(deviceID, MCI_CLOSE, 0, NULL);
	}
}