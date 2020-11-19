#pragma once
#include "Types.h"

#include <Windows.h>
#include "Xinput.h"

LRESULT CALLBACK
Win32MainWindowCallback(HWND Window,
	UINT Message,
	WPARAM WParam,
	LPARAM LParam);

class Input
{
public:
	static void Update();
	
	static vec2 leftThumb;
	static vec2 rightThumb;
private :
	static XINPUT_STATE state;
};

