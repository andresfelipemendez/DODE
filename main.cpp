
#include "Input.h"
#include "Loader.h"
#include "Model.h"
#include "Platform.h"
#include "Renderer.h"
#include "Time.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#undef max
#undef min
#include <Windows.h>

int CALLBACK
WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int show_code)
{
	Renderer r{};
	Platform p{};
	
	const auto width = 800;
	const auto height = 600;

	p.OpenWindow(width, height, instance, show_code, &Win32MainWindowCallback);
	r.Initialize(p.window_handle, width, height);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(p.window_handle);
	ImGui_ImplDX11_Init(r.d3ddev, r.d3dctx);

	Transform t;
	t.translate = {0, 0.0f, 0};
	t.rotate = {0, 3.14f, 0};
	t.scale = {1.0f, 1.0f, 1.0f};

	Model cb("baldosa_relleno.glb", r);
	
	Time::Init();

	MSG message;
	auto show_demo_window = false;
	
	while (true)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		Time::Tick();
		Input::Update();
		
		r.CameraPosition(Input::left_thumb, Time::GetDeltaTime());
		r.CameraRotation(Input::right_thumb, Time::GetDeltaTime());

		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
			if (message.message == WM_QUIT)
			{
				break;
			}
		}
		
		r.Clear();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		
		ImGui::Begin("Hello, world!");
		ImGui::Text("This is some useful text.");
		ImGui::InputFloat2("Left Thumb", &Input::left_thumb.x);
		ImGui::InputFloat3("Camera pos", &r.camera_pos.x);
		ImGui::DragFloat3("translation", &t.translate.x);
		ImGui::DragFloat3("rotation", &t.rotate.x, 0.01f);
		ImGui::DragFloat3("scale", &t.scale.x, 0.01f);
		
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
