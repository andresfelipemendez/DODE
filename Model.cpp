#include "Model.h"

#include "Loader.h"

#include "assimp/postprocess.h"

Model::Model(const char* path, Renderer& r)
{
	m_Renderer = &r;
	LoadModel(path);
}

void Model::Draw(Transform t)
{
	for (const auto& mesh : meshes)
	{
		mesh.Draw(m_Renderer, t);
	}
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer import;
	const auto* const scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		//error
		return;
	}
	m_Directory = path.substr(0, path.find_last_of('\\'));
	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		auto* const mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex{};
		vertex.position[0] = mesh->mVertices[i].x;
		vertex.position[1] = mesh->mVertices[i].y;
		vertex.position[2] = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.uv[0] = mesh->mTextureCoords[0][i].x;
			vertex.uv[1] = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.uv[0] = 0;
			vertex.uv[1] = 0;
		}

		if (mesh->mNormals)
		{
			vertex.normal[0] = mesh->mNormals[i].x;
			vertex.normal[1] = mesh->mNormals[i].y;
			vertex.normal[2] = mesh->mNormals[i].z;
		}
		
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			indices.push_back(mesh->mFaces[i].mIndices[j]);
	}

	if(mesh->mMaterialIndex >= 0)
	{
		auto* const material = scene->mMaterials[mesh->mMaterialIndex];

		auto diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
		auto specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
	}
	return CreateMesh(vertices, indices, textures,m_Renderer);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type) const
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture {
			Loader::LoadImage(m_Directory, *m_Renderer),
			m_Directory
		};

		/*texture.id = Loader::LoadImage(m_Directory, *m_Renderer);
		texture.path = m_Directory;*/
		textures.push_back(texture);
	}
	return textures;
}
