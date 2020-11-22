#pragma once
#include <d3d11.h>
#include <string>
#include <vector>

class Renderer;

struct Texture
{
	void* id;
	std::string path;
};

struct Vec3
{
	float x = 0, y = 0, z = 0;
};

struct Transform
{
	Vec3 translate;
	Vec3 rotate;
	Vec3 scale;
};

struct Vec2
{
	float x = 0, y = 0;
	void Normalize();
	float GetMagnitude() const;
};

struct Vertex
{
	float position[3];
	float normal[3];
	float uv[2];
};

struct Mesh
{
	ID3D11Buffer* index_buffer;
	ID3D11Buffer* vertex_buffer;
	Texture texture;
	size_t num_indices;
	void Draw(Renderer* renderer, Transform t) const;
};

Mesh CreateMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, Renderer* renderer);
	