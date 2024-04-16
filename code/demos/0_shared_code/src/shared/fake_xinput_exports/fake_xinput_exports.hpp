//
// Created by César B. on 4/14/2024.
//

#pragma once

#include <minwindef.h>
#include <functional>

typedef struct _XINPUT_VIBRATION XINPUT_VIBRATION;
typedef struct _XINPUT_STATE XINPUT_STATE;
typedef struct _XINPUT_CAPABILITIES XINPUT_CAPABILITIES;

namespace fake_xinput_exports
{	/// custom injected code
	namespace injected_code_callbacks
	{	inline std::function<void(DWORD, XINPUT_VIBRATION *)> XInputSetState;
		inline std::function<void(DWORD, XINPUT_STATE *)> XInputGetState;
		inline std::function<void(DWORD, DWORD, XINPUT_CAPABILITIES *)> XInputGetCapabilities;
		inline std::function<void(DWORD, GUID *, GUID *)> XInputGetDSoundAudioDeviceGuids;
	}
	/// real xinput pointers, loaded from the xinput dll in System32
	namespace real_xinput
	{	inline HMODULE loaded_dll_module = NULL;
		inline std::function<DWORD(DWORD, XINPUT_VIBRATION *)> XInputSetState;
		inline std::function<DWORD(DWORD, XINPUT_STATE *)> XInputGetState;
		inline std::function<DWORD(DWORD, DWORD, XINPUT_CAPABILITIES *)> XInputGetCapabilities;
		inline std::function<DWORD(DWORD, GUID *, GUID *)> XInputGetDSoundAudioDeviceGuids;
		/// load real xinput pointers from XInput9_1_0.dll in System32
		void load_all();
	}
}

extern "C"
{	DWORD __stdcall __declspec(dllexport) XInputSetState(DWORD, XINPUT_VIBRATION *);
	DWORD __stdcall __declspec(dllexport) XInputGetState(DWORD, XINPUT_STATE *);
	DWORD __stdcall __declspec(dllexport) XInputGetCapabilities(DWORD, DWORD, XINPUT_CAPABILITIES *);
	DWORD __stdcall __declspec(dllexport) XInputGetDSoundAudioDeviceGuids(DWORD, GUID *, GUID *);
}
