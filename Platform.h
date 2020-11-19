#pragma once
struct Platform
{
	int OpenWindow(int width, int height, HINSTANCE Instance, int ShowCode, WNDPROC callback);
	HWND WindowHandle;
};

