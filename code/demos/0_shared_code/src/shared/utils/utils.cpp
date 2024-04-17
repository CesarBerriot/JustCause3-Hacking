//
// Created by César B. on 4/15/2024.
//

#include "utils.hpp"
#include <windows.h>
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

HWND utils::get_game_window_win32_handle()
{	HWND result = NULL;
	// *INDENT-OFF*
	EnumWindows(
		[](HWND window, LPARAM result_ptr)
		{	char title[150] = { '\0' };
			GetWindowTextA(window, title, 150);
			if(!strcmp(title, "Just Cause 3"))
				if(get_process_executable_path(GetCurrentProcessId()) == get_process_executable_path(GetWindowThreadProcessId(window, nullptr)))
				{	*(HWND*)result_ptr = window;
					return FALSE;
				}
			return TRUE;
		},
		(LPARAM)&result
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

DWORD utils::get_process_parent(DWORD process)
{	for(auto snapshot_process : run_toolhelp_process_snapshot())
		if(snapshot_process.th32ProcessID == process)
			return snapshot_process.th32ParentProcessID;
	assert(!"get_process_parent search failed");
	return 0;
}

std::vector<DWORD> utils::get_process_children(DWORD process_id)
{	return std::vector<DWORD>();
}

std::vector<PROCESSENTRY32> utils::run_toolhelp_process_snapshot()
{	std::vector<PROCESSENTRY32> result;

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
	{	result.push_back(current_process_entry);
	} while(Process32Next(snapshot, &current_process_entry));

	// snapshot cleanup
	CloseHandle(snapshot);

	return result;
}

std::string utils::get_process_executable_path(DWORD process, bool use_asserts)
{	char result[MAX_PATH] = { '\0' };

	if(process = GetCurrentProcessId())
	{	GetModuleFileNameA(NULL, result, MAX_PATH);
		return result;
	}

	// open the process for reading its executable path
	HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, process);
	if(!process_handle)
	{	// this error is to be expected on any system process
		log_warning(make_string("Failed to open process with ID " << process));
		if(use_asserts)
			assert(!"get_process_executable_path : open process failed");
		return "";
	}

	// obtain the current process' executable path
	K32GetModuleFileNameExA(process_handle, NULL, result, MAX_PATH);

	// close the process handle, as its no longer required
	CloseHandle(process_handle);

	return result;
}
