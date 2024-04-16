//
// Created by César B. on 4/15/2024.
//

#include "utils.hpp"
#include <windows.h>
#include <tlhelp32.h>
#include <cassert>
#include <iostream>
#include <shared/constants/constants.hpp>
#include <conio.h>
#include <psapi.h>

void utils::create_win32_console()
{	// make sure the console doesn't get allocated twice
	assert(!GetConsoleWindow());

	// create the console
	AllocConsole();

	// redirect standard streams to the console
	freopen("CONOUT$", "w", stdout);
	freopen("CONERR$", "w", stderr);
	freopen("CONIN$", "r", stdin);

	/*
	 * enable the console's virtual mode.
	 * there's an official way to do this but
	 * as always with microsoft it takes 400+
	 * characters of code to do such a simple
	 * thing and there's some dumb hack to
	 * make it happen in 5 seconds.
	 */
	system("cls");
}

void utils::create_console_menu(std::string text, std::vector<std::pair<std::string, std::function<void()>>> options)
{	utils::activate_console_window();
	assert(options.size());
	fflush(stdin);
	int cursor = 0;
	auto render = [&text, &cursor, &options]
	{	std::cout << constants::TERMESC_CLEAR;
		std::cout << text << '\n';
		for(int i = 0; i < options.size(); ++i)
			std::cout << "--" << (i == cursor ? '>' : '-') << ' ' << options[i].first << '\n';
	};
	render();
	for(;;)
		switch(int character = getch())
		{	case constants::GETCH_ARROW_DOWN:
				++cursor;
				if(cursor >= options.size())
					cursor = 0;
				render();
				break;
			case constants::GETCH_ARROW_UP:
				--cursor;
				if(cursor < 0)
					cursor = options.size() - 1;
				render();
				break;
			case constants::GETCH_RETURN:
				utils::activate_console_window();
				if(auto callback = options[cursor].second)
					callback();
				return;
		}
}

DWORD utils::get_game_process_id()
{	/*
	 * @note
	 * I originally tried to use GetConsoleProcessList()
	 * to do this whole thing in a way simpler way, but
	 * I couldn't get it to work for some reason.
	 *
	 * The new method consists of using the ToolHelp API
	 * to take a snapshot of all running processes,
	 * and comparing the executable path of each found
	 * process with this process'. Unless the player is
	 * using a cracked version of the game, in which case
	 * don't expect me to make my code compatible to any
	 * extent, it shouldn't be possible for the game
	 * executable to have multiple instances (due to
	 * the way the SteamWorks API works) so in theory
	 * this works under any circumstance.
	 *
	 * I don't think this is the best solution, mostly
	 * performance-wise, so any recommendation on a
	 * better way to make this happen is welcome.
	 */

	DWORD result = 0;

	// store this process' executable path for comparison
	char this_process_executable_path[MAX_PATH] = { '\0' };
	GetModuleFileNameA(NULL, this_process_executable_path, MAX_PATH);
	std::cout << "This process : " << this_process_executable_path << '\n';

	// take a snapshot of all running processes
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	assert(snapshot);

	// initialize the current process info data structure
	PROCESSENTRY32 current_process_entry;
	ZeroMemory(&current_process_entry, sizeof(PROCESSENTRY32));
	current_process_entry.dwSize = sizeof(PROCESSENTRY32);

	// store the first snapshot process into the current process info data, assert the snapshot is valid
	bool snapshot_contains_processes = Process32First(snapshot, &current_process_entry);
	assert(snapshot_contains_processes);

	// iterate all captured process info structures
	do
	{	log_info(make_string("Comparing process : " << current_process_entry.szExeFile));

		// open the current process for reading its executable path
		HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, current_process_entry.th32ProcessID);
		if(!process_handle)
		{	// this error is to be expected on any system process
			log_warning(make_string("Failed to open process : " << current_process_entry.szExeFile));
			continue;
		}

		// obtain the current process' executable path
		char current_process_executable_path[MAX_PATH] = { '\0' };
		K32GetModuleFileNameExA(process_handle, NULL, current_process_executable_path, MAX_PATH);

		// close the process handle, as its no longer required
		CloseHandle(process_handle);

		// compare this process' executable path to the current process', and break out of the loop if correct
		if(!strcmp(current_process_executable_path, this_process_executable_path))
		{	log_info(make_string("Found process : " << current_process_entry.szExeFile));
			result = current_process_entry.th32ProcessID;
			break;
		}
	} while(Process32Next(snapshot, &current_process_entry));

	// snapshot cleanup
	CloseHandle(snapshot);

	assert(result);

	return result;
}

HWND utils::get_game_window_win32_handle()
{	HWND result = NULL;
	// *INDENT-OFF*
	/*EnumWindows(
		[](HWND window, LPARAM result_ptr)
		{	char title[150] = { '\0' };
			GetWindowTextA(window, title, 150);
			puts(title);
			if(GetWindowThreadProcessId(window, nullptr) == GetCurrentProcessId())
				if(window != GetConsoleWindow())
					*(HWND*)result_ptr = window;
			if(!strcmp(title, "Just Cause 3"))
			{
				std::cout << "This process : " << GetCurrentProcessId() << '\n'
						  << "Other process : " << GetWindowThreadProcessId(window, nullptr) << '\n'
					  	  << "Is console window : " << (window == GetConsoleWindow() ? "Yes" : "No") << '\n';
				assert(!"nique ta mere");
			}
			return TRUE;
		},
		(LPARAM)&result
	);
	// *INDENT-ON**/
DWORD game_process_id = get_game_process_id();
assert(game_process_id);
struct passed_window_enum_proc_data { HWND * result_ptr; DWORD game_process_id; } passed_window_enum_proc_data = { &result, game_process_id };
	// *INDENT-OFF*
	EnumWindows(
		[](HWND window, LPARAM passed_data_param)
		{	struct passed_window_enum_proc_data passed_data = *(struct passed_window_enum_proc_data*)passed_data_param;
			if(GetWindowThreadProcessId(window, nullptr) == passed_data.game_process_id)
			{	*passed_data.result_ptr = window;
					return FALSE;
			}
			//
			char title[150] = { '\0' };
			GetWindowTextA(window, title, 150);
			if(!strcmp(title, "Just Cause 3"))
			{
				std::cout << "This process : " << GetCurrentProcessId() << '\n'
						  << "JC3 process : " << GetWindowThreadProcessId(window, nullptr) << '\n'
						  << "Console Attached Process : " << passed_data.game_process_id << '\n';
				assert(!"nique ta mere");
			}
			//
			return TRUE;
		},
		(LPARAM)&passed_window_enum_proc_data
	);
	// *INDENT-ON*
	assert(result);
	return result;
}

void utils::activate_window(HWND handle)
{	bool b = ShowWindow(handle, SW_NORMAL); assert(b);
	b = SetForegroundWindow(handle); assert(b);
	HWND h = SetActiveWindow(handle); assert(h);
	assert(GetForegroundWindow() == handle);
}

void utils::log_info(std::string message)
{	std::cout << constants::TERMESC_WHITE << message << '\n';
}

void utils::log_warning(std::string message)
{	std::cout << constants::TERMESC_YELLOW << message << '\n' << constants::TERMESC_WHITE;
}

void utils::log_error(std::string message)
{	std::cout << constants::TERMESC_RED << message << '\n' << constants::TERMESC_WHITE;
}
