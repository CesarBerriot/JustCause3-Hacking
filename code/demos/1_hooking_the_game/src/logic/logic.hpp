//
// Created by César B. on 4/14/2024.
//

#pragma once

namespace logic
{	/**
	* @brief makes sure application logic has been initialized
	* @details might be called anywhere, anytime
	*/
	void ensure_initialized();

	/**
	 * @brief loads real xinput functions into globals::real_xinput_pointers
	 * @see globals/globals.hpp
	 */
	void load_real_xinput();

	/**
	 * your hooked code that runs every frame here.
	 * called whenever the game calls the hooked
	 * XInputGetState, before the real xinput call.
	 */
	inline void on_jc3_tick_callback() {}
}
