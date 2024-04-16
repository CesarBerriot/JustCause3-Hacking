//
// Created by César B. on 4/15/2024.
//

#include <windows.h>
#include <thread>
#include <shared/fake_xinput_exports/fake_xinput_exports.hpp>
#include <shared/utils/utils.hpp>
#include <shared/constants/constants.hpp>
#include <iostream>

void on_jc3_tick_callback();
void create_main_menu();
void create_window_manipulation_menu();
void create_fullscreen_mode_menu();

BOOL WINAPI DllMain(HMODULE loaded_dll_module, DWORD call_reason, LPVOID additional_info)
{	switch(call_reason)
	{	case DLL_PROCESS_ATTACH:
		{	utils::create_win32_console();
			fake_xinput_exports::injected_code_callbacks::XInputGetState = [](...) { on_jc3_tick_callback(); return 0; };
			bool was_statically_loaded = additional_info;
			MessageBoxA(NULL, "Successfully hooked the game.", "Code injected !", MB_OK);
			MessageBoxA(NULL, "Press G once loaded to open the actions menu.", "Info", MB_OK);
			puts("Successfully loaded.");
			std::cout << "Load Type : " << (was_statically_loaded ? "Static" : "Dynamic") << '\n';
			break;
		}
		case DLL_THREAD_ATTACH:
			std::cout << constants::TERMESC_GREEN << "Thread creation intercepted. Thread ID : " << std::this_thread::get_id() << constants::TERMESC_WHITE << '\n';
			break;
		case DLL_THREAD_DETACH:
			std::cout << constants::TERMESC_RED << "\033[0;32Thread destruction intercepted. Thread ID : " << std::this_thread::get_id() << constants::TERMESC_WHITE << '\n';
			break;
		case DLL_PROCESS_DETACH:
			MessageBoxA(NULL, "", additional_info ? "Terminated" : "Exited normally", MB_OK);
			break;
	}
	// dll entry success
	return TRUE;
}

void on_jc3_tick_callback()
{	enum { VK_A = 0x41 };
	if(GetAsyncKeyState(VK_A + 'g' - 'a') & (1 << 15))
		create_main_menu();
}

void create_main_menu()
{	utils::create_console_menu("basic mod menu",
	{	{ "window manipulation", create_window_manipulation_menu },
		{ "quit game", []{ exit(EXIT_SUCCESS); } },
		{ "cancel", nullptr }
	});
}

void create_window_manipulation_menu()
{	utils::create_console_menu(
		"window manipulation",
	{	{ "fullscreen mode", create_fullscreen_mode_menu },
		{ "maximize", []{ ShowWindow(utils::get_game_window_win32_handle(), SW_MAXIMIZE); } },
		{ "minimize", []{ ShowWindow(utils::get_game_window_win32_handle(), SW_MINIMIZE); } },
		{ "cancel", nullptr }
	});
}

void create_fullscreen_mode_menu()
{	utils::create_console_menu("fullscreen mode",
	{	{ "windowed", nullptr },
		{ "fullscreen", nullptr },
		{ "borderless fullscreen", nullptr }
	});
}
