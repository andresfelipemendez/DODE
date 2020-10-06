#include <windows.h>
#include "Platform.h"


int Platform::OpenWindow(int width, int height, HINSTANCE Instance, int ShowCode, WNDPROC callback)
{
	WNDCLASSEX wnd;
	ZeroMemory(&wnd, sizeof(WNDCLASSEX));

	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = callback;
	wnd.hInstance = Instance;
	wnd.hCursor = LoadCursor(0, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wnd.lpszClassName = L"Window Class";
	RegisterClassEx(&wnd);

	RECT wr = { 0, 0, 1920, 1080 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	WindowHandle = CreateWindowEx(
		0,
		L"Window Class",
		L"Windows Programming",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		width,
		height,
		NULL,
		NULL,
		Instance,
		NULL);

	if (WindowHandle == NULL)
	{
		system("PAUSE");
		return -1;
	}

	ShowWindow(WindowHandle, ShowCode);

	return 0;
}
