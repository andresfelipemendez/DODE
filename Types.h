#pragma once
struct vec3
{
	float x = 0, y = 0, z = 0;
};

struct vec2
{
	float x = 0, y = 0, magnitude = 0;
	void normalize();
};

struct Vertex {
	float position[3];
	float normal[3];
	float uv[2];
};
struct Mesh {
	void* indexBuffer;
	void* vertexBuffer;
	size_t numIndices;
};
