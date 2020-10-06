#pragma once
class Platform
{
public:
	static int OpenWindow(int width, int height, HINSTANCE Instance, WNDPROC callback);
	static HWND WindowHandle;
};

