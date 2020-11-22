#pragma once
#include <combaseapi.h>


struct Platform
{
	int OpenWindow(int width, int height, HINSTANCE instance, int show_code, WNDPROC callback);
	HWND window_handle;
};

