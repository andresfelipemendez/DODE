#pragma once

#include "Types.h"
#include <string>
#include "Renderer.h"

class Loader
{
public:
	Mesh LoadOBJ(std::string path, Renderer& r);
};
