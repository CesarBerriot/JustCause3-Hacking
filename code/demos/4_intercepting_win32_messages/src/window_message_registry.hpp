//
// Created by Administrator on 4/17/2024.
//

#pragma once

#include <map>
#include <windows.h>

struct hooked_window_message_info
{	std::string label;
	enum { intercept, redirect_to_win32, block } action;
};

#define make_entry(message, action) { message, { #message, hooked_window_message_info::action } }
#define make_unknown_entry(message_value, action) { message_value, { "UNKNOWN", hooked_window_message_info::action } }

std::map<int, hooked_window_message_info> window_message_registry =
{	make_entry(WM_NULL, intercept),
	make_entry(WM_DESTROY, intercept),
	make_entry(WM_MOVE, intercept),
	make_entry(WM_SIZE, intercept),
	make_entry(WM_ACTIVATE, intercept),
	make_entry(WM_SETFOCUS, intercept),
	make_entry(WM_KILLFOCUS, intercept),
	make_entry(WM_PAINT, intercept),
	make_entry(WM_CLOSE, intercept),
	make_entry(WM_QUERYOPEN, intercept),
	make_entry(WM_ERASEBKGND, intercept),
	make_entry(WM_ACTIVATEAPP, intercept),
	make_entry(WM_CANCELMODE, intercept),
	make_entry(WM_SETCURSOR, intercept),
	make_entry(WM_MOUSEACTIVATE, intercept),
	make_entry(WM_GETMINMAXINFO, intercept),
	make_entry(WM_WINDOWPOSCHANGING, intercept),
	make_entry(WM_WINDOWPOSCHANGED, intercept),
	make_entry(WM_CONTEXTMENU, intercept),
	make_entry(WM_STYLECHANGING, intercept),
	make_entry(WM_STYLECHANGED, intercept),
	make_entry(WM_GETICON, intercept),
	make_entry(WM_NCDESTROY, intercept),
	make_entry(WM_NCCALCSIZE, intercept),
	make_entry(WM_NCHITTEST, intercept),
	make_entry(WM_NCPAINT, intercept),
	make_entry(WM_NCACTIVATE, intercept),
	make_unknown_entry(144, intercept),
	make_entry(WM_NCMOUSEMOVE, intercept),
	make_entry(WM_NCLBUTTONDOWN, intercept),
	make_entry(WM_NCLBUTTONUP, intercept),
	make_unknown_entry(174, intercept),
	make_entry(WM_KEYFIRST, intercept),
	make_entry(WM_KEYUP, intercept),
	make_entry(WM_CHAR, intercept),
	make_entry(WM_SYSKEYDOWN, intercept),
	make_entry(WM_SYSKEYUP, intercept),
	make_entry(WM_SYSCHAR, intercept),
	make_entry(WM_SYSCOMMAND, intercept),
	make_entry(WM_MOUSEMOVE, block),
	make_entry(WM_LBUTTONDOWN, redirect_to_win32),
	make_entry(WM_LBUTTONUP, redirect_to_win32),
	make_entry(WM_RBUTTONDOWN, intercept),
	make_entry(WM_RBUTTONUP, intercept),
	make_entry(WM_MBUTTONDOWN, intercept),
	make_entry(WM_MBUTTONUP, intercept),
	make_entry(WM_MOUSEWHEEL, intercept),
	make_entry(WM_SIZING, intercept),
	make_entry(WM_CAPTURECHANGED, intercept),
	make_entry(WM_MOVING, intercept),
	make_entry(WM_ENTERSIZEMOVE, intercept),
	make_entry(WM_EXITSIZEMOVE, intercept),
	make_entry(WM_IME_SETCONTEXT, intercept),
	make_entry(WM_IME_NOTIFY, intercept),
	make_unknown_entry(5372, intercept),
	make_unknown_entry(5374, intercept),
};

#undef make_entry
