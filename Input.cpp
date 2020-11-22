#include "Input.h"
#include <Windows.h>
#include "Lib/imgui/imgui.h"

extern IMGUI_IMPL_API LRESULT ImGuiImplWin32WndProcHandler(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param);

static POINT point;

Vec2 Input::left_thumb;
Vec2 Input::right_thumb;
XINPUT_STATE Input::m_State;

void Input::Update()
{
	m_State = {};
	XInputGetState(0, &m_State);

	left_thumb = {
		static_cast<float>(m_State.Gamepad.sThumbLX),
		static_cast<float>(m_State.Gamepad.sThumbLY)
	};

	if (left_thumb.GetMagnitude() > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		left_thumb.Normalize();
	}
	else
	{
		left_thumb = {0, 0};	
	}
	
	right_thumb = {
		static_cast<float>(m_State.Gamepad.sThumbRX),
		static_cast<float>(m_State.Gamepad.sThumbRY)
	};

	if (right_thumb.GetMagnitude() > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		right_thumb.Normalize();
	}
	else
	{
		right_thumb = {0, 0};
	}
}

LRESULT CALLBACK Win32MainWindowCallback(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	RECT rect;
	GetWindowRect(window, &rect);
	GetCursorPos(&point);
	point.x -= rect.left;
	point.y -= rect.top;
	
	if (ImGuiImplWin32WndProcHandler(window, message, w_param, l_param))
		return true;
	switch (message)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		{
			if (w_param == VK_ESCAPE)
			{
				PostQuitMessage(0);
				return 0;
			}
		}
		break;
	default: 
		break;
	}
	return DefWindowProc(window, message, w_param, l_param);
}
