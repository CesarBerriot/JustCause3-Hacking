//
// Created by César B. on 4/16/2024.
//

#include <windows.h>
#include <shared/utils/utils.hpp>
#include <shared/fake_xinput_exports/fake_xinput_exports.hpp>
#include <iostream>
#include <cassert>

/// the game's real window procedure function
WNDPROC game_window_proc = nullptr;

/// called on every game tick
void on_jc3_tick_callback();
/// our proxy window procedure function
LRESULT game_window_proc_hook(HWND, UINT, WPARAM, LPARAM);
/// hooks the game's window procedure with game_window_proc_hook().
void hook_game_window_proc();

/// dll entry point
BOOL WINAPI DllMain(HMODULE loaded_dll_module, DWORD call_reason, LPVOID additional_info)
{	switch(call_reason)
	{	case DLL_PROCESS_ATTACH:
		{	utils::create_win32_console();
			MessageBoxA(NULL, "Game Hooked.", "Info", MB_OK);
			puts("Successfully loaded.");
			fake_xinput_exports::injected_code_callbacks::XInputGetState = [](...) { on_jc3_tick_callback(); return 0; };
			break;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			MessageBoxA(NULL, additional_info ? "Terminated" : "Exited normally", "Info", MB_OK);
			break;
	}
	// dll entry success
	return TRUE;
}

void on_jc3_tick_callback()
{	static bool already_hooked = false;
	if(already_hooked || !utils::get_game_window_win32_handle())
		return;
	hook_game_window_proc();
	already_hooked = true;
}

LRESULT game_window_proc_hook(HWND window, UINT message, WPARAM param1, LPARAM param2)
{	puts("Intercepted a window message.");
	return game_window_proc(window, message, param1, param2);
}

void hook_game_window_proc()
{	HWND game_window = utils::get_game_window_win32_handle();
	assert(game_window);
	game_window_proc = (WNDPROC)GetWindowLongPtrA(game_window, GWLP_WNDPROC);
	assert(game_window_proc);
	WNDPROC SetWindowLongPtr_result = (WNDPROC)SetWindowLongPtrA(game_window, GWLP_WNDPROC, (LONG_PTR)game_window_proc_hook);
	assert(SetWindowLongPtr_result == game_window_proc);
}
