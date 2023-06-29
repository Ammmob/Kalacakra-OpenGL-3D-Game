#pragma once

#include <vector>
#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

class Renderer {
protected:
	Buffer* m_vertexBuffer;
	Buffer* m_elementBuffer;
	VertexArray* m_vertexArray;
	std::vector<Texture*> m_textures;
public:
	Shader* m_shader;

	Renderer();
	Renderer(Buffer* vertexBuffer, VertexArray* vertexArray);
	Renderer(Buffer* vertexBuffer, VertexArray* vertexArray, Shader* shader);
	Renderer(Buffer* vertexBuffer, Buffer* elementBuffer, VertexArray* vertexArray, Shader* shader);
	~Renderer();
	void Bind();
	void UnBind();
	void CreateBufferArray(GLboolean vertexBuffer = 1, GLboolean elementBuffer = 1, GLboolean vertexArray = 1);
	void SetVertexBuffer(Buffer* vbo);
	void SetElementBuffer(Buffer* ebo);
	void SetBuffer(Buffer* vbo, Buffer* ebo);
	void SetVertexArray(VertexArray* vao);
	void SetShader(Shader* shader);
	void SetShader(const std::string vertexPath, const std::string fragmentPath);
	void AddTexture(Texture* texture);
	void SetTextures(std::vector<Texture*> textures);
	Shader* GetShader();
	Texture* GetTexture(GLuint slot = 0);
	void DrawArrays(GLenum mode, GLint first, GLsizei count);
	void DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices);
};