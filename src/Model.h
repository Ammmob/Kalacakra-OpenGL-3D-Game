#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "AssimpGLMHelper.hpp"

struct BoneInfo {
	int id;
	glm::mat4 offset;
};

class Model {
public:
	Model(std::string const& path, GLboolean gamma = false);
	void Draw(Shader& shader);
	auto& GetBoneInfoMap() { return m_BoneInfoMap; }
	int& GetBoneCount() { return m_BoneCounter; }

private:
	GLboolean m_gammaCorrection;
	std::vector<Mesh> m_meshes;
	std::string m_directory;
	std::unordered_map<std::string, Texture*>m_loadedTextures;

	std::map<std::string, BoneInfo> m_BoneInfoMap;
	int m_BoneCounter = 0;
	void LoadModel(std::string const& path);
	void LoadMaterialTextures(std::vector<Texture*>& textures, aiMaterial* material, aiTextureType type, std::string typeName, const aiScene* scene);
	void CreateNode(aiNode* node, const aiScene* scene);
	Mesh CreateMesh(aiMesh* mesh, const aiScene* scene);
	void SetVertexBoneDataToDefault(Vertex& vertex);
	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
};