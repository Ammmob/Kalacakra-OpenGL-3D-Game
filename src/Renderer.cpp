#include "Renderer.h"


Renderer::Renderer()
	: m_vertexBuffer(nullptr), m_elementBuffer(nullptr), m_vertexArray(nullptr), m_shader(nullptr) {
}

Renderer::Renderer(Buffer* vertexBuffer, VertexArray* vertexArray)
	:m_vertexBuffer(vertexBuffer), m_elementBuffer(nullptr), m_vertexArray(vertexArray), m_shader(nullptr) {
}

Renderer::Renderer(Buffer* vertexBuffer, VertexArray* vertexArray, Shader* shader)
	:m_vertexBuffer(vertexBuffer), m_elementBuffer(nullptr), m_vertexArray(vertexArray), m_shader(shader) {
}

Renderer::Renderer(Buffer* vertexBuffer, Buffer* elementBuffer, VertexArray* vertexArray, Shader* shader)
	:m_vertexBuffer(vertexBuffer), m_elementBuffer(elementBuffer), m_vertexArray(vertexArray), m_shader(shader) {
}

Renderer::~Renderer() {
}

void Renderer::Bind() {
	m_vertexArray->Bind();
	m_vertexBuffer->Bind();
	m_shader->Bind();
	if (m_elementBuffer) {
		m_elementBuffer->Bind();
	}
	for (int i = 0; i < m_textures.size(); i++) {
		m_textures[i]->Bind(i);
	}
}

void Renderer::UnBind() {
	m_vertexArray->UnBind();
	m_vertexBuffer->UnBind();
	m_shader->UnBind();
	if (m_elementBuffer) {
		m_elementBuffer->UnBind();
	}
	for (int i = 0; i < m_textures.size(); i++) {
		m_textures[i]->UnBind(i);
	}
}

void Renderer::CreateBufferArray(GLboolean vertexBuffer, GLboolean elementBuffer, GLboolean vertexArray) {
	if (vertexBuffer) {
		m_vertexBuffer = new Buffer(GL_ARRAY_BUFFER);
	}
	if (elementBuffer) {
		m_elementBuffer = new Buffer(GL_ELEMENT_ARRAY_BUFFER);
	}
	if (vertexArray) {
		m_vertexArray = new VertexArray();
	}
}

void Renderer::SetVertexBuffer(Buffer* vbo) {
	m_vertexBuffer = vbo;
}

void Renderer::SetElementBuffer(Buffer* ebo) {
	m_elementBuffer = ebo;
}

void Renderer::SetBuffer(Buffer* vbo, Buffer* ebo) {
	m_vertexBuffer = vbo;
	m_elementBuffer = ebo;
}

void Renderer::SetVertexArray(VertexArray* vao) {
	m_vertexArray = vao;
}

void Renderer::SetShader(Shader* shader) {
	assert(this != nullptr);
	m_shader = shader;
}

void Renderer::SetShader(const std::string vertexPath, const std::string fragmentPath) {
	m_shader = new Shader(vertexPath, fragmentPath);
}

void Renderer::AddTexture(Texture* texture) {
	m_textures.push_back(texture);
}

void Renderer::SetTextures(std::vector<Texture*> textures) {
	m_textures = textures;
}

Shader* Renderer::GetShader() {
	return m_shader;
}

Texture* Renderer::GetTexture(GLuint slot) {
	return m_textures[slot];
}

void Renderer::DrawArrays(GLenum mode, GLint first, GLsizei count) {
	Bind();
	glDrawArrays(mode, first, count);
	UnBind();
}

void Renderer::DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) {
	Bind();
	glDrawElements(mode, count, type, indices);
	UnBind();
}
