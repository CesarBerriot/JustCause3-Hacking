//
// Created by César B. on 4/15/2024.
//

#pragma once

#include <string>
#include <vector>
#include <functional>
#include <windows.h>
#include <tlhelp32.h>

namespace utils
{ 	void create_win32_console();
	void create_console_menu(std::string text, std::vector<std::pair<std::string, std::function<void()>>> options);
	HWND get_game_window_win32_handle();
	void activate_window(HWND);
	inline void activate_console_window() { activate_window(GetConsoleWindow()); }
	inline void activate_game_window() { activate_window(get_game_window_win32_handle()); }
	void log_info(std::string message);
	void log_warning(std::string message);
	void log_error(std::string message);
	DWORD get_process_parent(DWORD process);
	std::vector<DWORD> get_process_children(DWORD process);
	std::vector<PROCESSENTRY32> run_toolhelp_process_snapshot();
	std::string get_process_executable_path(DWORD process, bool use_asserts = true);
}
