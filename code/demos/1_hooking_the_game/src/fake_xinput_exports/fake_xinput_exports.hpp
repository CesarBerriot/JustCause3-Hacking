//
// Created by César B. on 4/14/2024.
//

#pragma once

#include <windows.h>

extern "C"
{	typedef struct _XINPUT_VIBRATION XINPUT_VIBRATION;
	typedef struct _XINPUT_STATE XINPUT_STATE;
	typedef struct _XINPUT_CAPABILITIES XINPUT_CAPABILITIES;
	DWORD __stdcall __declspec(dllexport) XInputSetState(DWORD, XINPUT_VIBRATION *);
	DWORD __stdcall __declspec(dllexport) XInputGetState(DWORD, XINPUT_STATE *);
	DWORD __stdcall __declspec(dllexport) XInputGetCapabilities(DWORD, DWORD, XINPUT_CAPABILITIES *);
	DWORD __stdcall __declspec(dllexport) XInputGetDSoundAudioDeviceGuids(DWORD, GUID *, GUID *);
}
