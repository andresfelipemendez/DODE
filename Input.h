#pragma once
#include "Types.h"


#include <Windows.h>
#include "xinput.h"

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
	static vec2 get_right_thumb();
	
private :
	static XINPUT_STATE state;
	//DWORD dwResult;
};

