#pragma once
#include <string>
#include <vector>


#include "Types.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>           
#include <assimp/postprocess.h>


class Model
{
public:
	explicit Model(const char* path);
	void Draw();
private:
	std::vector<Mesh> meshes;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

