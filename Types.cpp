#include "Types.h"

#include <cmath>

#include "Renderer.h"

void vec2::normalize()
{
	magnitude = static_cast<float>(sqrt(
		static_cast<double>(x) * static_cast<double>(x) +
		static_cast<double>(y) * static_cast<double>(y)));
	x = x / magnitude;
	y = y / magnitude;
}

void Mesh::Draw(Renderer* renderer) const
{
	vec3 pos = { 0.0f, 0.0f, 0.0f };
	renderer->SetBuffers(pos, numIndices, indexBuffer, vertexBuffer, texture.id);
}

Mesh CreateMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices,
				const std::vector<Texture>& textures, Renderer* renderer)
{
	Mesh m {};
	m.vertexBuffer = renderer->CreateVertexBuffer(const_cast<Vertex*>(&vertices[0]), vertices.size());
	m.indexBuffer = renderer->CreateIndexBuffer(const_cast<unsigned*>(&indices[0]), indices.size());
	m.numIndices = indices.size();
	if(!textures.empty()) {
		m.texture = textures[0];
	}
	return m;
}
