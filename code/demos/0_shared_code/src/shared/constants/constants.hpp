//
// Created by César B. on 4/15/2024.
//

#pragma once

namespace constants
{
#pragma region terminal escape codes
	inline char TERMESC_RED[] = "\033[0;31m";
	inline char TERMESC_GREEN[] = "\033[0;32m";
	inline char TERMESC_YELLOW[] = "\033[0;33m";
	inline char TERMESC_BLUE[] = "\033[0;34m";
	inline char TERMESC_MAGENTA[] = "\033[0;35m";
	inline char TERMESC_CYAN[] = "\033[0;36m";
	inline char TERMESC_WHITE[] = "\033[0;37m";
	/// resets the cursor to the first element of the stream
	inline char TERMESC_HOME[] = "\033[H";
	/// erases anything subsequent to the cursor
	inline char TERMESC_ERASE_SUBSEQUENT[] = "\033[J";
	/// clears the terminal
	inline std::string TERMESC_CLEAR = make_string(TERMESC_HOME << TERMESC_ERASE_SUBSEQUENT);
#pragma endregion terminal escape codes
#pragma region getch() return values
	enum
	{	GETCH_ARROW_UP = 72,
		GETCH_ARROW_LEFT = 75,
		GETCH_ARROW_RIGHT = 77,
		GETCH_ARROW_DOWN = 80,
		GETCH_RETURN = 13,
	};
#pragma endregion getch() return values
}
