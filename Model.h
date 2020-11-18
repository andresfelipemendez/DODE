#pragma once
#include <string>
#include <vector>


#include "Types.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>           
#include <assimp/postprocess.h>

#include "Renderer.h"


class Model
{
public:
	explicit Model(const char* path, Renderer& r);
	void Draw();
	
	std::vector<Mesh> meshes;
private:
	
	std::string directory;
	Renderer* renderer;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

