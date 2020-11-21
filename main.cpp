#define NOMINMAX
#include <Windows.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(p.WindowHandle);
	ImGui_ImplDX11_Init(r.d3ddev, r.d3dctx);

	Transform t;
	t.translate=  {0,-3.0f,0};
	t.rotate = {0,3.14f,0};
	
	t.scale = {0.05f,0.05f,0.05f};
	//Model cb("Assets\\Link\\link.obj", r);
	Model cb("baldosa_relleno.glb", r);
	
	//Model cb("cube.obj", r);
	
	Time::Init();

	MSG Message = { 0 };

	 bool show_demo_window = false;
	
	while (true)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		
		Time::Tick();
		Input::Update();
		
		r.CameraPosition(Input::leftThumb, Time::GetDeltaTime());
		r.CameraRotation(Input::rightThumb, Time::GetDeltaTime());

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

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		
		ImGui::Begin("Hello, world!");
		ImGui::Text("This is some useful text.");
		ImGui::InputFloat2("Left Thumb",&Input::leftThumb.x);
		ImGui::InputFloat3("Camera pos",&r.camera_pos_.x);
		ImGui::DragFloat3("translation",&t.translate.x);
		ImGui::DragFloat3("rotation",&t.rotate.x,0.01f);
		ImGui::DragFloat3("scale",&t.scale.x,0.01f);
		
		ImGui::End();

		ImGui::Render();
		r.viewport();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		cb.Draw(t);
		
		 if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		
		r.Present();
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
