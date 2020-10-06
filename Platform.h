#pragma once
class Platform
{
public:
	int OpenWindow(int width, int height, HINSTANCE Instance, int ShowCode, WNDPROC callback);
	HWND WindowHandle;
};

