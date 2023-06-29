#include "Model.h"

void Model::LoadModel(std::string const& path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	/*
	if (scene->HasAnimations()) {
		std::cout << "This model has animations!" << std::endl;
	}
	else {
		std::cout << "This model has no animations!" << std::endl;
	}
	*/

	m_directory = path.substr(0, path.find_last_of('/'));
	CreateNode(scene->mRootNode, scene);
}

void Model::LoadMaterialTextures(std::vector<Texture*>& textures, aiMaterial* material, aiTextureType type, std::string typeName, const aiScene* scene) {
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
		aiString fileName;
		material->GetTexture(type, i, &fileName);
		std::string path = m_directory + '/' + fileName.C_Str();
		if (path.find("\\") != std::string::npos) {
			path = path.replace(path.find("\\"), 1, "/");

		}
		//std::cout << path << "\n";
		if (m_loadedTextures.find(path) == m_loadedTextures.end()) {
			const aiTexture* aitexture = scene->GetEmbeddedTexture(fileName.C_Str());
			Texture* texture = new Texture(GL_TEXTURE_2D, typeName);
			texture->Bind();
			Texture::LoadTexture(path, GL_TEXTURE_2D, aitexture);
			texture->SetWrap(GL_REPEAT, GL_REPEAT);
			texture->SetFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
			textures.push_back(texture);
			m_loadedTextures[path] = texture;
		}
		else {
			textures.push_back(m_loadedTextures[path]);
		}
	}
}

void Model::CreateNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		m_meshes.push_back(CreateMesh(scene->mMeshes[node->mMeshes[i]], scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		CreateNode(node->mChildren[i], scene);
	}
}

Mesh Model::CreateMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture*> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		SetVertexBoneDataToDefault(vertex);
		if (mesh->HasPositions())
			vertex.m_Position = AssimpGLMHelper::GetGLMVec(mesh->mVertices[i]);
		if (mesh->HasNormals())
			vertex.m_Normal = AssimpGLMHelper::GetGLMVec(mesh->mNormals[i]);

		if (mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
		{
			glm::vec2 v2;
			glm::vec3 v3;
			v2.x = mesh->mTextureCoords[0][i].x;
			v2.y = mesh->mTextureCoords[0][i].y;
			vertex.m_TexCoords = v2;
			if (mesh->HasTangentsAndBitangents()) {
				// tangent
				vertex.m_Tangent = AssimpGLMHelper::GetGLMVec(mesh->mTangents[i]);
				// bitangent
				vertex.m_Bitangent = AssimpGLMHelper::GetGLMVec(mesh->mBitangents[i]);
			}
			else {
				vertex.m_Tangent = glm::vec3(0.0f);
				vertex.m_Bitangent = glm::vec3(0.0f);
			}
		}
		else {
			vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		LoadMaterialTextures(textures, material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		LoadMaterialTextures(textures, material, aiTextureType_SPECULAR, "texture_specular", scene);
		LoadMaterialTextures(textures, material, aiTextureType_NORMALS, "texture_normal", scene);
		LoadMaterialTextures(textures, material, aiTextureType_HEIGHT, "texture_height", scene);
	}
	ExtractBoneWeightForVertices(vertices, mesh, scene);
	return Mesh(vertices, indices, textures);
}



void Model::SetVertexBoneDataToDefault(Vertex& vertex)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.m_BoneIDs[i] = -1;
		vertex.m_Weights[i] = 0.0f;
	}
}

void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight) {
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
		if (vertex.m_BoneIDs[i] < 0) {
			vertex.m_Weights[i] = weight;
			vertex.m_BoneIDs[i] = boneID;
			break;
		}
	}
}

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	auto& boneInfoMap = m_BoneInfoMap;
	int& boneCount = m_BoneCounter;

	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = boneCount;
			newBoneInfo.offset = AssimpGLMHelper::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCount;
			boneCount++;
		}
		else
		{
			boneID = boneInfoMap[boneName].id;
		}
		assert(boneID != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

Model::Model(std::string const& path, GLboolean gamma) {
	LoadModel(path);
}

void Model::Draw(Shader& shader) {
	for (Mesh& mesh : m_meshes) {
		mesh.Draw(shader);
	}
}
