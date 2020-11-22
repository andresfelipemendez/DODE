#include "Utils.h"

void Log(Vec2 v) {
	char s[30];
	sprintf(s,"vec2: %f, %f",v.x,v.y);
	OutputDebugStringA(s);
}

void Log(const char* v)
{
	OutputDebugStringA(v);
}
