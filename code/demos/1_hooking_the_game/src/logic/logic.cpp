//
// Created by César B. on 4/14/2024.
//

#include "logic.hpp"
#include <windows.h>
#include <cassert>
#include "globals/globals.hpp"

void logic::ensure_initialized()
{	static bool initialized = false;
	if(!initialized)
	{	// call application logic initialization functions here
		load_real_xinput();

		// initialization done
		MessageBoxA(NULL, "Sucessfully initialized the hook !", "Game Process Hooked", MB_OK); \
		initialized = true;
	}
}

void logic::load_real_xinput()
{	// load microsoft's xinput dll from System32
	HMODULE loaded_dll = LoadLibraryA("C:\\Windows\\System32\\XInput9_1_0.dll");
	assert(loaded_dll);

	// load function pointers from loaded_dll into globals::real_xinput_pointers
#define load_symbol(name) \
	do \
	{	globals::real_xinput_pointers.name = (void*)GetProcAddress(loaded_dll, #name); \
		assert(globals::real_xinput_pointers.name); \
	} while(false)

	load_symbol(XInputSetState);
	load_symbol(XInputGetState);
	load_symbol(XInputGetCapabilities);
	load_symbol(XInputGetDSoundAudioDeviceGuids);

#undef load_symbol
}
