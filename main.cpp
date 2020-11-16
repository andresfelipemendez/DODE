#include <windows.h>
#include "Platform.h"
#include "Renderer.h"

#include "Loader.h"
#include "Time.h"
#include "Input.h"



int CALLBACK
WinMain(HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR CommandLine,
	int ShowCode)
{
	Renderer r;
	Platform p;
	Loader l;
	int width = 800;
	int height = 600;

	p.OpenWindow(width, height, Instance, ShowCode, &Win32MainWindowCallback);
	r.Initialize(p.WindowHandle, width, height);
	
	Mesh cube = l.LoadOBJ("cube.obj", r);
	
	Time::Init();

	MSG Message = { 0 };
	while (true)
	{
		Time::Tick();
		
		//r.CameraRotation(dir);

		if (PeekMessage(&Message, 0, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
			if (Message.message == WM_QUIT)
			{
				break;
			}
		}

		r.Clear();

		vec3 pos = { 0, 0, -20.0f };
		r.SetBuffers(pos, cube.numIndices, cube.indexBuffer, cube.vertexBuffer);

		r.Render();
	}
}
