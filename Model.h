#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <string>
#include <vector>

#include "Types.h"

class Model
{
public:
	explicit Model(const char* path, Renderer& r);
	void Draw(Transform t);
	
	std::vector<Mesh> meshes;
private:
	
	std::string m_Directory;
	Renderer* m_Renderer;

	void LoadModel(const std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type) const;
};

