#include "Input.h"

#include <windows.h>


static POINT point;

void Input::Update() {
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(0, &state);
}

XINPUT_STATE Input::state;

vec2 Input::GetLeftThumb() 
{
	vec2 left {
		state.Gamepad.sThumbLX,
		state.Gamepad.sThumbLY
	};

	left.normalize();
	if (left.magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return left;
	}
	left = {0,0};
	return left;
}

vec2 Input::get_right_thumb()
{
	vec2 right {
		state.Gamepad.sThumbRX,
		state.Gamepad.sThumbRY
	};
	
	right.normalize();
	if (right.magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		return right;
	}
	right = {0,0};
	return right;
}

LRESULT CALLBACK
Win32MainWindowCallback(HWND Window,
	UINT Message,
	WPARAM WParam,
	LPARAM LParam)
{
	RECT rect;
	GetWindowRect(Window, &rect);
	GetCursorPos(&point);
	point.x -= rect.left;
	point.y -= rect.top;
	LRESULT Result = 0;
	switch (Message)
	{

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
		switch (WParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		default:
			break;
		}
	}
	break;
	}
	return DefWindowProc(Window, Message, WParam, LParam);
}