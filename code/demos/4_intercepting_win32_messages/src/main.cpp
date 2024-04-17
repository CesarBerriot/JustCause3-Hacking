//
// Created by César B. on 4/16/2024.
//

#include <window_message_registry.hpp>
#include <shared/utils/utils.hpp>
#include <shared/fake_xinput_exports/fake_xinput_exports.hpp>
#include <shared/constants/constants.hpp>
#include <iostream>
#include <cassert>
#include <map>

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
{	/* @note
	 * ===============================================
	 * here's what I found about Avalanche Engine's
	 * use of window messages by experimenting :
	 * ===============================================
	 * WM_MOUSEMOVE is used by all UIs to obtain
	 * the mouse position
	 * -----------------------------------------------
	 * after alt-tabbing out of the game window,
	 * AE will use WM_PAINT as a signal to know when
	 * to redraw the window.
	 * ------------------------------
	 */

	static std::map<int, int> intercepted_messages;
	static std::map<int, int> redirected_messages;
	static std::map<int, int> blocked_messages;
	static std::map<int, int> unknown_messages;

	auto redraw = [](int highlighted_message)
	{	enum : char { end_line = '\n' };

		auto make_separator = []
		{	std::stringstream result;
			for(int i = 0; i < 60; ++i)
				result << '=';
			result << end_line;
			return result.str();
		};

		auto make_list = [&highlighted_message](auto list, bool are_messages_known)
		{	std::stringstream result;
			result << "list size : " << list.size() << end_line;
			for(auto pair : list)
			{	if(pair.first == highlighted_message)
					result << constants::TERMESC_GREEN;
				result << pair.first;
				if(are_messages_known)
					result << ' ' << window_message_registry[pair.first].label;
				result << " (" << pair.second << ')' << end_line << constants::TERMESC_WHITE;
			}
			return result.str();
		};

		std::stringstream result;
		result << constants::TERMESC_CLEAR
			   << make_separator()
			   << "Demo 4 : Intercepting win32 API window messages" << end_line
			   << make_separator()
			   << "intercepted messages :" << end_line
			   << make_list(intercepted_messages, true)
			   << make_separator()
			   << "blocked messages :" << end_line
			   << make_list(blocked_messages, true)
			   << make_separator()
			   << "messages redirected to the win32 API :" << end_line
			   << make_list(redirected_messages, true)
			   << make_separator()
			   << "unknown messages :" << end_line
			   << make_list(unknown_messages, false)
			   << make_separator();

		std::cout << constants::TERMESC_WHITE << result.str() << constants::TERMESC_WHITE;
	};

	if(window_message_registry.count(message))
		switch(window_message_registry[message].action)
		{	case hooked_window_message_info::intercept:
				++intercepted_messages[message];
				redraw(message);
				return game_window_proc(window, message, param1, param2);
			case hooked_window_message_info::redirect_to_win32:
				++redirected_messages[message];
				redraw(message);
				return DefWindowProcA(window, message, param1, param2);
			case hooked_window_message_info::block:
				++blocked_messages[message];
				redraw(message);
				return 0;
			default:
				assert(!"unreacheable code executed");
				return 0;
		}
	else
	{	++unknown_messages[message];
		redraw(message);
		return game_window_proc(window, message, param1, param2);
	}
}

void hook_game_window_proc()
{	HWND game_window = utils::get_game_window_win32_handle();
	assert(game_window);
	game_window_proc = (WNDPROC)GetWindowLongPtrA(game_window, GWLP_WNDPROC);
	assert(game_window_proc);
	WNDPROC SetWindowLongPtr_result = (WNDPROC)SetWindowLongPtrA(game_window, GWLP_WNDPROC, (LONG_PTR)game_window_proc_hook);
	assert(SetWindowLongPtr_result == game_window_proc);
}
