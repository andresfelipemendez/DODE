#include "Input.h"

#include <windows.h>
#include <math.h>  

static POINT point;

void Input::Update() {
	/*ZeroMemory(&state, sizeof(XINPUT_STATE));*/
}

vec2 Input::GetLeftThumb() 
{
	vec2 left;
	//float LX = state.Gamepad.sThumbLX;
	//float LY = state.Gamepad.sThumbLY;

	////determine how far the controller is pushed
	//float magnitude = sqrt(LX * LX + LY * LY);

	////determine the direction the controller is pushed
	//float normalizedLX = LX / magnitude;
	//float normalizedLY = LY / magnitude;

	//float normalizedMagnitude = 0;

	////check if the controller is outside a circular dead zone
	//if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	//{
	//	//clip the magnitude at its expected maximum value
	//	if (magnitude > 32767) magnitude = 32767;

	//	//adjust magnitude relative to the end of the dead zone
	//	magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	//	//optionally normalize the magnitude with respect to its expected range
	//	//giving a magnitude value of 0.0 to 1.0
	//	normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	//}
	//else //if the controller is in the deadzone zero out the magnitude
	//{
	//	magnitude = 0.0;
	//	normalizedMagnitude = 0.0;
	//}
	//left.x = LX;
	//left.y = LY;
	return left;
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