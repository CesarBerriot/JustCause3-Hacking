//
// Created by César B. on 4/14/2024.
//

#include "fake_xinput_exports.hpp"
#include <cassert>
#include "logic/logic.hpp"
#include "globals/globals.hpp"

// helper function containing generic middle-man code between the game and the actual xinput
template<typename _func, typename... _args>
DWORD call(void ** func, _args... args)
{	assert(func);
	logic::ensure_initialized();
	return ((_func) * func)(args...);
}

DWORD __stdcall __declspec(dllexport) XInputSetState(DWORD p1, XINPUT_VIBRATION * p2)
{	return call<DWORD(*)(DWORD, XINPUT_VIBRATION *)>(&globals::real_xinput_pointers.XInputSetState, p1, p2);
}

DWORD __stdcall __declspec(dllexport) XInputGetState(DWORD p1, XINPUT_STATE * p2)
{	// XInputGetState is used for injected tick code since
	// in theory the game polls controller state every frame
	logic::on_jc3_tick_callback();
	return call<DWORD(*)(DWORD, XINPUT_STATE *)>((void**)&globals::real_xinput_pointers.XInputGetState, p1, p2);
}

DWORD __stdcall __declspec(dllexport) XInputGetCapabilities(DWORD p1, DWORD p2, XINPUT_CAPABILITIES * p3)
{	return call<DWORD(*)(DWORD, DWORD, XINPUT_CAPABILITIES *)>(&globals::real_xinput_pointers.XInputGetCapabilities, p1, p2, p3);
}

DWORD __stdcall __declspec(dllexport) XInputGetDSoundAudioDeviceGuids(DWORD p1, GUID * p2, GUID * p3)
{	return call<DWORD(*)(DWORD, GUID *, GUID *)>(&globals::real_xinput_pointers.XInputGetDSoundAudioDeviceGuids, p1, p2, p3);
}
