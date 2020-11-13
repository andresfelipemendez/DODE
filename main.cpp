#include <windows.h>
#include "Platform.h"
#include "Renderer.h"

#include <fstream>
#include <iostream>
#include "Loader.h"

Renderer r;
Platform p;
Loader l;

LRESULT CALLBACK
Win32MainWindowCallback(HWND Window,
	UINT Message,
	WPARAM WParam,
	LPARAM LParam)
{
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

int CALLBACK
WinMain(HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR CommandLine,
	int ShowCode)
{
	int width = 800;
	int height = 600;

	p.OpenWindow(width, height, Instance, ShowCode, &Win32MainWindowCallback);
	r.Initialize(p.WindowHandle,800,600);
	
	//Mesh cube = l.LoadOBJ("cube.obj", r);
	Mesh cube = l.LoadOBJ("Assets/Island/Island.obj", r);

	while (true)
	{
		r.Clear();
		vec3 pos = { 0,0,-10.0f };
		r.SetBuffers(pos, cube.numIndices, cube.indexBuffer, cube.vertexBuffer);
		r.Render();
	}
}