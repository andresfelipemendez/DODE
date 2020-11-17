#pragma once

#include "Types.h"
#include <string>
#include <vector>

#include "Renderer.h"

class Loader
{
public:
	std::vector<Mesh> LoadOBJ(std::string path, Renderer& r) const;
	
	static void* load_image(std::string path, Renderer& r);
	
};

