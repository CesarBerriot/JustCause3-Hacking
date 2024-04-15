//
// Created by César B. on 4/14/2024.
//

#pragma once

namespace globals
{
	/**
	 * @brief
	 * real xinput function pointers, loaded from the xinput dll
	 * within C:/Windows/System32 (XInput9_1_0.dll)
	 * @note
	 * after some research, due to either some truly obscure
	 * c++ specification I couldn't get my hands on or
	 * a straight-up compiler bug, inline anonymous
	 * data structures instances end up being copied
	 * in every translation unit, thus ignoring the 'inline'
	 * specifier. no matter whether I use inline or __forceinline
	 * nothing works out. my personal hypothesis is
	 * there's some code in the linker that uses
	 * data structure tags for linking other translation units
	 * to the one containing the actual data structure
	 * instance symbol, with fallback code on error
	 * that just makes a unit-local symbol instead
	 * without telling the user.
	 */
	struct __real_xinput_pointers
	{	void * XInputSetState;
		void * XInputGetState;
		void * XInputGetCapabilities;
		void * XInputGetDSoundAudioDeviceGuids;
	} inline real_xinput_pointers;
}
