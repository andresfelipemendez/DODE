#include <windows.h>
#include "Platform.h"
#include "Renderer.h"

#include <fstream>
#include <iostream>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"


ID3D11Buffer* indexbuffer;

Renderer r;
Platform p;

bool loadModel(tinygltf::Model& model, const char* filename) {
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;
	bool res = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
	if (!warn.empty()) {
		std::cout << "WARN: " << warn << std::endl;
	}

	if (!err.empty()) {
		std::cout << "ERR: " << err << std::endl;
	}

	if (!res)
		std::cout << "Failed to load glTF: " << filename << std::endl;
	else
		std::cout << "Loaded glTF: " << filename << std::endl;

	return res;
}

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

void bindModelNodes(tinygltf::Model& model, tinygltf::Node& node) {
	if ((node.mesh >= 0) && (node.mesh < model.meshes.size()))
	{
		for (size_t j = 0; j < model.bufferViews.size(); ++j)
		{
			const tinygltf::BufferView& bufferView = model.bufferViews[j];
			if (bufferView.target == 0) {
				OutputDebugStringA("WARN: bufferView.target is zero\n");
				continue;
			}

			const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

			tinygltf::Mesh& mesh = model.meshes[node.mesh];

			std::string msg("bufferview.target ");
			msg += std::to_string(bufferView.target);
			msg += "\n";

			switch (bufferView.target)
			{
			case GL_ARRAY_BUFFER: 
			case GL_ELEMENT_ARRAY_BUFFER: 
				r.CreateBuffer(bufferView, buffer);

				OutputDebugStringA(msg.c_str());

				break;
			default:
				std::cout << "bufferview.target " << bufferView.target << "\n";
				break;
			}

			/*
			for (size_t j = 0; j < mesh.primitives.size(); ++j) {
				tinygltf::Primitive primitive = mesh.primitives[j];
				tinygltf::Accessor indexAccesor = model.accessors[primitive.indices];

				for (auto& attrib : primitive.attributes) {

					tinygltf::Accessor accesor = model.accessors[attrib.second];
					int byteStride = accesor.ByteStride(model.bufferViews[accesor.bufferView]);

					int vaa = -1;
					if (attrib.first.compare("POSITION") == 0) vaa = 0;
					if (attrib.first.compare("NORMAL") == 0) vaa = 1;
					if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;

					//glVertexAttribPointer(
					//	vaa, 
					//	size, 
					//	accessor.componentType,
					//	accessor.normalized ? GL_TRUE : GL_FALSE,
					//	byteStride, 
					//	BUFFER_OFFSET(accessor.byteOffset))
				}
			}
			*/
		}
	}

	for (size_t i = 0; i < node.children.size(); ++i)
	{
		assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
		bindModelNodes(model, model.nodes[node.children[i]]);
	}

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



	tinygltf::Model model;
	std::string filename = "Duck.glb";

	if (!loadModel(model, filename.c_str())) 
		return 1;

	const tinygltf::Scene& scene = model.scenes[model.defaultScene];

	for (size_t i = 0; i < scene.nodes.size(); ++i) {
		assert((scene.nodes[i] >= 0) && (scene.nodes[i] < model.nodes.size()));
		bindModelNodes(model, model.nodes[scene.nodes[i]]);
	}
	
	while (true)
	{
		r.Render();
	}
}