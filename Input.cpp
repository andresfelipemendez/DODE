#include "Input.h"

#include <windows.h>

#include "imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static POINT point;

vec2 Input::leftThumb;
vec2 Input::rightThumb;

void Input::Update() {
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	XInputGetState(0, &state);

	leftThumb = {
		static_cast<float>(state.Gamepad.sThumbLX),
		static_cast<float>(state.Gamepad.sThumbLY)
	};

	if (leftThumb.GetMagnitude() > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		leftThumb.normalize();
	}
	else
	{
		leftThumb = {0,0};	
	}
	
	rightThumb = {
		static_cast<float>(state.Gamepad.sThumbRX),
		static_cast<float>(state.Gamepad.sThumbRY)
	};

	if (rightThumb.GetMagnitude() > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		rightThumb.normalize();
	}
	else
	{
		rightThumb = {0,0};
	}
}

XINPUT_STATE Input::state;

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

	if (ImGui_ImplWin32_WndProcHandler(Window, Message, WParam, LParam))
		return true;
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