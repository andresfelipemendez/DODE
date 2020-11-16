#pragma once
#include "Types.h"

//#include <xinput.h>

#include <windows.h>

LRESULT CALLBACK
Win32MainWindowCallback(HWND Window,
	UINT Message,
	WPARAM WParam,
	LPARAM LParam);

class Input
{
public:
	static void Update();
	static vec2 GetLeftThumb();
private :
	//static XINPUT_STATE state;
	//DWORD dwResult;
};

