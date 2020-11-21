#include "Model.h"

#include "Loader.h"

Model::Model(const char* path, Renderer& r)
{
	renderer = &r;
	loadModel(path);
}

void Model::Draw(Transform t)
{
	for(const auto& mesh : meshes)
	{
		mesh.Draw(renderer, t);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		//error
		return;
	}
	directory = path.substr(0, path.find_last_of('\\'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh  = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position[0] = mesh->mVertices[i].x;
		vertex.position[1] = mesh->mVertices[i].y;
		vertex.position[2] = mesh->mVertices[i].z;

		if(mesh->mTextureCoords[0])
		{
			vertex.uv[0] = mesh->mTextureCoords[0][i].x;
			vertex.uv[1] = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.uv[0] = 0;
			vertex.uv[1] = 0;
		}

		if(mesh->mNormals)
		{
			vertex.normal[0] = mesh->mNormals[i].x;
			vertex.normal[1] = mesh->mNormals[i].y;
			vertex.normal[2] = mesh->mNormals[i].z;
		}
		
		//mesh->m
		vertices.push_back(vertex);
	}

	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		for(unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			indices.push_back(mesh->mFaces[i].mIndices[j]);
	}

	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return CreateMesh(vertices,indices,textures,renderer);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type,i,&str);
		Texture texture;
		std::string file(str.C_Str());

		
		
		//std::string textureFileName = "\\"+file;//.substr(file.find_last_of('\\'), file.size() - 1);
		//textureFileName.pop_back();
		std::string texturePath = directory;// + textureFileName;
		
		texture.id = Loader::load_image(texturePath, *renderer);
		//texture.type =
		texture.path = texturePath;
		textures.push_back(texture);
	}
	return textures;
}
