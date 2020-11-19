#pragma once
#include <string>
#include <vector>


class Renderer;

struct Texture
{
	void* id;
	std::string path;
};

struct vec3
{
	float x = 0, y = 0, z = 0;
};

struct Transform
{
	vec3 translate;
	vec3 rotate;
	vec3 scale;
};

struct vec2
{
	float x = 0, y = 0;
	void normalize();
	float GetMagnitude() const;
};

struct Vertex {
	float position[3];
	float normal[3];
	float uv[2];
};

struct Mesh {
	void* indexBuffer;
	void* vertexBuffer;
	Texture texture;
	size_t numIndices;
	void Draw(Renderer* renderer, Transform t) const;
};

Mesh CreateMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, Renderer* renderer);
	