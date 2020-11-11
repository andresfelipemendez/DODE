#pragma once
struct vec3
{
	float x, y, z;
};

struct vec2
{
	float x, y;
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
