//
// Created by César B. on 4/16/2024.
//

#include <windows.h>
#include <shared/utils/utils.hpp>
#include <shared/constants/constants.hpp>
#include <vector>
#include <mutex>
#include <thread>
#include <cassert>
#include <iostream>

/// known threads registry
std::vector<std::thread::id> thread_registry;
/// mutex for interacting with the thread registry
std::mutex registry_mutex;

/// prints info with color text into the console about thread creation/destruction
void print_thread_info_message(std::thread::id, bool creation);
/// register a thread into the registry
void register_thread(std::thread::id);
/// unregister a thread from the registry
void unregister_thread(std::thread::id);
/// thread_registry.size() with a mutex
size_t registry_size();
/**
 * returns the index of a known thread in the registry,
 * fails an assertion if the given thread can't be found.
 */
int registry_index_of(std::thread::id);

/// dll entry point
BOOL WINAPI DllMain(HMODULE loaded_dll_module, DWORD call_reason, LPVOID additional_info)
{	switch(call_reason)
	{	case DLL_PROCESS_ATTACH:
		{	utils::create_win32_console();
			MessageBoxA(NULL, "Game Hooked.", "Info", MB_OK);
			puts("Successfully loaded.");
			register_thread(std::this_thread::get_id());
			break;
		}
		case DLL_THREAD_ATTACH:
		{	std::thread::id this_thread = std::this_thread::get_id();
			register_thread(this_thread);
			print_thread_info_message(this_thread, true);
			break;
		}
		case DLL_THREAD_DETACH:
		{	std::thread::id this_thread = std::this_thread::get_id();
			print_thread_info_message(this_thread, false);
			unregister_thread(this_thread);
			break;
		}
		case DLL_PROCESS_DETACH:
			MessageBoxA(NULL, additional_info ? "Terminated" : "Exited normally", "Info", MB_OK);
			unregister_thread(std::this_thread::get_id());
			break;
	}
	// dll entry success
	return TRUE;
}

void print_thread_info_message(std::thread::id id, bool creation)
{	std::cout << (creation ? constants::TERMESC_GREEN : constants::TERMESC_RED)
			  << "Thread " << (creation ? "creation" : "destruction") << " intercepted. "
			  << "Thread : " << registry_index_of(id) << '/' << registry_size() << ". "
			  << "ID : " << id << ".\n"
			  << constants::TERMESC_WHITE;
}

void register_thread(std::thread::id id)
{	registry_mutex.lock();
	// there's no vector::contains in c++ 17
	for(auto entry : thread_registry)
		assert(entry != id);
	thread_registry.push_back(id);
	registry_mutex.unlock();
}

void unregister_thread(std::thread::id id)
{	registry_mutex.lock();
	// there's no vector::contains in c++ 17
	for(int i = 0; i < thread_registry.size(); ++i)
		if(thread_registry[i] == id)
		{	thread_registry.erase(thread_registry.begin() + i);
			goto ok;
		}
	assert(!"attempt to unregister a thread that wasn't part of the registry");
ok:
	thread_registry.push_back(id);
	registry_mutex.unlock();
}

size_t registry_size()
{	std::lock_guard guard(registry_mutex);
	return thread_registry.size();
}

int registry_index_of(std::thread::id id)
{	std::lock_guard guard(registry_mutex);
	for(int i = 0; i < thread_registry.size(); ++i)
		if(thread_registry[i] == id)
			return i;
	assert(!"element search failed");
	return -1;
}
