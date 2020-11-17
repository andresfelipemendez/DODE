#include "Utils.h"

void Log(vec2 v) {
	std::string s;
	s += std::to_string(v.x) + " ";
	s += std::to_string(v.y) + "\n";
	OutputDebugStringA(s.c_str());
}

void Log(const char* v)
{
	OutputDebugStringA(v);
}
