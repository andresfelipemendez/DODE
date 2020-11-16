#include "Input.h"

#include <windows.h>
#include <math.h>  

static POINT point;

void Input::Update() {
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(0, &state);
}

XINPUT_STATE Input::state;

vec2 Input::GetLeftThumb() 
{
	vec2 left;
	float LX = state.Gamepad.sThumbLX;
	float LY = state.Gamepad.sThumbLY;

	float magnitude = sqrt(LX * LX + LY * LY);

	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	float normalizedMagnitude = 0;

	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		if (magnitude > 32767) magnitude = 32767;

		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

		left.x = normalizedLX;
		left.y = normalizedLY;
		return left;
	}
	else
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;

		return left;
	}
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