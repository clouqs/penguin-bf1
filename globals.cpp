#include "includes.h"

namespace globals
{
	HWND hGame;
	HMODULE hmModule;
	
	bool unload_dll;
	bool killed_focus = true;

	int screenshot_counter;
	int fake_screenshot_counter;
	int screen_bypass_active;
}