#include "Rectangle.h"

GLfloat Rectangle::windowWidth = 1960;
GLfloat Rectangle::windowHeight = 1080;

Rectangle::Rectangle(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLboolean normalize, GLboolean isTexture) {
	m_isTexture = isTexture;
	leftBottom = glm::vec3(x, y, 0.0f);
	rightTop = glm::vec3(x + width, y + height, 0.0f);
	leftTop = glm::vec3(x, y + height, 0.0f);
	rightBottom = glm::vec3(x + width, y, 0.0f);

	if (normalize) {
		NormalizaVec(leftBottom);
		NormalizaVec(rightTop);
		NormalizaVec(rightBottom);
		NormalizaVec(leftTop);
	}

	AddVertex(rightTop);
	if (isTexture) {
		AddTexCoord(glm::vec2(1.0f, 1.0f));
	}
	AddVertex(leftTop);
	if (isTexture) {
		AddTexCoord(glm::vec2(0.0f, 1.0f));
	}
	AddVertex(leftBottom);
	if (isTexture) {
		AddTexCoord(glm::vec2(0.0f, 0.0f));
	}

	AddVertex(leftBottom);
	if (isTexture) {
		AddTexCoord(glm::vec2(0.0f, 0.0f));
	}
	AddVertex(rightBottom);
	if (isTexture) {
		AddTexCoord(glm::vec2(1.0f, 0.0f));
	}
	AddVertex(rightTop);
	if (isTexture) {
		AddTexCoord(glm::vec2(1.0f, 1.0f));
	}

	m_vertexArray = new VertexArray();
	m_vertexBuffer = new Buffer(GL_ARRAY_BUFFER);
	m_vertexArray->Bind();
	m_vertexBuffer->Bind();
	m_vertexBuffer->SetData(sizeof(GLfloat) * m_vertices.size(), &m_vertices[0]);
	m_vertexArray->AddAttribute(3, GL_FLOAT);
	if (isTexture) {
		m_vertexArray->AddAttribute(2, GL_FLOAT);
	}
	m_vertexArray->EnableAllAttribute();

	if (isTexture) {
		m_shader = new Shader("./res/shader/rectTexture.vert", "./res/shader/rectTexture.frag");
	}
	else {
		m_shader = new Shader("./res/shader/rectColor.vert", "./res/shader/rectColor.frag");
	}

	if (isTexture) {
		m_texture = new Texture();
	}
	else {
		m_color = glm::vec4(0.0f);
	}
}

void Rectangle::AddVertex(glm::vec3 vertex) {
	m_vertices.push_back(vertex.x);
	m_vertices.push_back(vertex.y);
	m_vertices.push_back(vertex.z);
}

void Rectangle::AddTexCoord(glm::vec2 texCoord) {
	m_vertices.push_back(texCoord.x);
	m_vertices.push_back(texCoord.y);
}

void Rectangle::SetTexture(const std::string& path) {
	m_texture->Bind();
	Texture::LoadTexture(path);
	m_texture->UnBind();
}

void Rectangle::SetColor(glm::vec4 color) {
	m_color = color;
}

void Rectangle::SetShader(Shader* shader) {
	m_shader = shader;
}

GLfloat Rectangle::NormalizeX(GLfloat x) {
	return x / windowWidth * 2 - 1;
}

GLfloat Rectangle::NormalizeY(GLfloat y) {
	return y / windowHeight * 2 - 1;
}

void Rectangle::NormalizaVec(glm::vec3& vec) {
	vec.x = NormalizeX(vec.x);
	vec.y = NormalizeY(vec.y);
}

void Rectangle::Draw() {
	m_vertexArray->Bind();
	m_vertexBuffer->Bind();
	m_shader->Bind();
	if (m_isTexture) {
		m_texture->Bind();
		m_shader->SetUniform1i("u_texture", 0);
	}
	else {
		m_shader->SetUniform4f("u_color", m_color);
	}
	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_vertexArray->UnBind();
	m_vertexBuffer->UnBind();
	m_shader->UnBind();

	if (m_isTexture) {
		m_texture->UnBind();
	}

}
