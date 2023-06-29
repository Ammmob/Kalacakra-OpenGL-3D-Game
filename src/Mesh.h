#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	// position
	glm::vec3 m_Position;
	// normal
	glm::vec3 m_Normal;
	// texCoords
	glm::vec2 m_TexCoords;
	// tangent
	glm::vec3 m_Tangent;
	// bitangent
	glm::vec3 m_Bitangent;
	//bone indexes which will influence this vertex
	GLint m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	GLfloat m_Weights[MAX_BONE_INFLUENCE];
};


class Mesh : public Renderer
{
private:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	void Initialization();
public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture*> textures);
	~Mesh();
	void Draw(Shader& shader);
	void AddVertex(Vertex vertex);
	void AddIndex(GLint index);

};

