#pragma once

#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Buffer.h"

class  Rectangle {
public:
	Rectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLboolean normalize = GL_FALSE, GLboolean isTexture = GL_FALSE);
	void AddVertex(glm::vec3 vertex);
	void AddTexCoord(glm::vec2 texCoord);
	void SetTexture(const std::string& path);
	void SetColor(glm::vec4 color);
	void SetShader(Shader* shader);
	void Draw();

	static GLfloat NormalizeX(GLfloat x);
	static GLfloat NormalizeY(GLfloat y);
	static void NormalizaVec(glm::vec3& vec);

	static GLfloat windowWidth;
	static GLfloat windowHeight;
private:
	VertexArray* m_vertexArray;
	Buffer* m_vertexBuffer;
	Shader* m_shader;
	std::vector<GLfloat>m_vertices;
	Texture* m_texture;
	glm::vec4 m_color;
	GLboolean m_isTexture;
	glm::vec3 leftTop;
	glm::vec3 leftBottom;
	glm::vec3 rightTop;
	glm::vec3 rightBottom;
};