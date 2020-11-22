#include "Loader.h"

#include <fstream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Utils.h"

void* Loader::LoadImage(const std::string& path, Renderer& r) 
{
	int x, y, n;
	auto* const data = stbi_load(path.c_str(), &x, &y, &n, 0);
	if(data == nullptr) {
		const auto* const msg = stbi_failure_reason();
		Log(msg);
		return nullptr;
	}

	return r.CreateTextureBuffer(data,x,y,n);
}
