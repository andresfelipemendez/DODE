#include "Types.h"

#include <cmath>

#include "Renderer.h"

Vec3 operator*(const Vec3& lhs, const float& rhs)
{
	Vec3 ret;
	ret.x = lhs.x * rhs;
	ret.y = lhs.y * rhs;
	ret.z = lhs.z * rhs;
	return ret;
}

::Vec3& Vec3::operator*=(const float& radius)
{
	this->x *= radius;
	this->y *= radius;
	this->z *= radius;
	return *this;
}

void Vec2::Normalize()
{
    const auto magnitude = GetMagnitude();
    x = x / magnitude;
    y = y / magnitude;
}

float Vec2::GetMagnitude() const
{
	return static_cast<float>(sqrt(
		static_cast<double>(x) * static_cast<double>(x) +
		static_cast<double>(y) * static_cast<double>(y)));
}


void Mesh::Draw(Renderer* renderer, Transform t) const
{
	Vec3 pos = { 0.0f, 0.0f, 0.0f};
	renderer->Draw(t, num_indices, index_buffer, vertex_buffer, texture.id);
}

Mesh CreateMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices,
                const std::vector<Texture>& textures, Renderer* renderer)
{
	Mesh m {};
	m.vertex_buffer = renderer->CreateVertexBuffer(const_cast<Vertex*>(&vertices[0]), vertices.size());
	m.index_buffer = renderer->CreateIndexBuffer(const_cast<unsigned*>(&indices[0]), indices.size());
	m.num_indices = indices.size();
	if(!textures.empty()) {
		m.texture = textures[0];
	}
	return m;
}
