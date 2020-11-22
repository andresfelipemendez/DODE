#pragma once
#include "Types.h"

#undef min
#undef max
#include <Windows.h>
#include "Xinput.h"

LRESULT CALLBACK
Win32MainWindowCallback(HWND window,
	UINT message,
	WPARAM w_param,
	LPARAM l_param);

class Input
{
public:
	static void Update();
	
	static Vec2 left_thumb;
	static Vec2 right_thumb;
private :
	static XINPUT_STATE m_State;
};

