//
// Created by César B. on 4/14/2024.
//

#include "fake_xinput_exports.hpp"
#include <cassert>
#include <windows.h>

void fake_xinput_exports::real_xinput::load_all()
{	// make sure xinput hasn't already been loaded
	assert(!loaded_dll_module);

	// load microsoft's xinput dll from System32
	loaded_dll_module = LoadLibraryA("C:\\Windows\\System32\\XInput9_1_0.dll");
	assert(loaded_dll_module);

	// load function pointers from loaded_dll_module
#define load_symbol(name, /*function_type*/...) \
	do \
	{	name = (__VA_ARGS__)GetProcAddress(loaded_dll_module, #name); \
		assert(name); \
	} while(false)

	load_symbol(XInputSetState, DWORD(*)(DWORD, XINPUT_VIBRATION *));
	load_symbol(XInputGetState, DWORD(*)(DWORD, XINPUT_STATE *));
	load_symbol(XInputGetCapabilities, DWORD(*)(DWORD, DWORD, XINPUT_CAPABILITIES *));
	load_symbol(XInputGetDSoundAudioDeviceGuids, DWORD(*)(DWORD, GUID *, GUID *));

#undef load_symbol
}

#define default_behaviour(name, /*parameters*/...) \
	do \
	{	using namespace fake_xinput_exports; \
		if(injected_code_callbacks::name) \
			injected_code_callbacks::name(__VA_ARGS__); \
		if(!real_xinput::loaded_dll_module) \
			real_xinput::load_all(); \
		assert(real_xinput::name); \
		return real_xinput::name(__VA_ARGS__); \
	} while(false)

DWORD __stdcall __declspec(dllexport) XInputSetState(DWORD p1, XINPUT_VIBRATION * p2)
{	default_behaviour(XInputSetState, p1, p2);
}

DWORD __stdcall __declspec(dllexport) XInputGetState(DWORD p1, XINPUT_STATE * p2)
{	default_behaviour(XInputGetState, p1, p2);
}

DWORD __stdcall __declspec(dllexport) XInputGetCapabilities(DWORD p1, DWORD p2, XINPUT_CAPABILITIES * p3)
{	default_behaviour(XInputGetCapabilities, p1, p2, p3);
}

DWORD __stdcall __declspec(dllexport) XInputGetDSoundAudioDeviceGuids(DWORD p1, GUID * p2, GUID * p3)
{	default_behaviour(XInputGetDSoundAudioDeviceGuids, p1, p2, p3);
}
