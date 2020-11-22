#include "Platform.h"

#undef max
#undef min
#include <Windows.h>

int Platform::OpenWindow(int width, int height, HINSTANCE instance, int show_code, WNDPROC callback)
{
	WNDCLASSEX wnd;
	ZeroMemory(&wnd, sizeof(WNDCLASSEX));

	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = callback;
	wnd.hInstance = instance;
	wnd.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wnd.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
	wnd.lpszClassName = L"Window Class";
	RegisterClassEx(&wnd);

	RECT wr = {0, 0, width, height};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	window_handle = CreateWindowEx(
		0,
		L"Window Class",
		L"Windows Programming",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		width,
		height,
		nullptr,
		nullptr,
		instance,
		nullptr);

	if (window_handle == nullptr)
	{
		return -1;
	}

	ShowWindow(window_handle, show_code);
	return 0;
}
