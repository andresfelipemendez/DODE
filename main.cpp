
#define NOMINMAX
#include <windows.h>

#include "Platform.h"
#include "Renderer.h"

#include "Loader.h"
#include "Time.h"
#include "Input.h"
#include "Model.h"

#include "Utils.h"



int CALLBACK
WinMain(
	_In_ HINSTANCE Instance,
	_In_opt_  HINSTANCE PrevInstance,
	_In_ LPSTR CommandLine,
	_In_ int ShowCode)
{
	Renderer r;
	Platform p{};
	Loader l;
	const auto width = 800;
	const auto height = 600;

	p.OpenWindow(width, height, Instance, ShowCode, &Win32MainWindowCallback);
	r.Initialize(p.WindowHandle, width, height);

	const auto texture = l.load_image("Assets\\Textures\\OutsSS04.png", r);
	auto cubes = l.LoadOBJ("cube.obj", r);

	std::string path("Assets\\Island\\Island.obj");
	Model m(path.c_str());
	Time::Init();

	MSG Message = { 0 };

	auto cube = cubes[0];
	
	while (true)
	{
		Time::Tick();
		Input::Update();
		
		auto lt = Input::GetLeftThumb();
		
		r.CameraPosition(lt, Time::GetDeltaTime());
		r.CameraRotation(Input::get_right_thumb(), Time::GetDeltaTime());

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

		vec3 pos = { 0, 5, -20.0f };
		r.SetBuffers(pos, cube.numIndices, cube.indexBuffer, cube.vertexBuffer, texture);

		r.Render();
	}
}
