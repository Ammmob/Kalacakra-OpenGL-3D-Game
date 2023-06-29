#include "Mesh.h"

void Mesh::Initialization() {
	CreateBufferArray(1, 1, 1);

	m_vertexArray->Bind();
	m_vertexBuffer->Bind();
	m_elementBuffer->Bind();

	m_vertexBuffer->SetData(m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
	m_elementBuffer->SetData(m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

	m_vertexArray->AddAttribute(3, GL_FLOAT, GL_FALSE, 0);
	m_vertexArray->AddAttribute(3, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_Normal));
	m_vertexArray->AddAttribute(2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_TexCoords));
	m_vertexArray->AddAttribute(3, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_Tangent));
	m_vertexArray->AddAttribute(3, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_Bitangent));
	m_vertexArray->AddAttribute(4, GL_INT, GL_FALSE, offsetof(Vertex, m_BoneIDs));
	m_vertexArray->AddAttribute(4, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_Weights));
	m_vertexArray->EnableAllAttribute();

	m_vertexArray->UnBind();
	m_vertexBuffer->UnBind();
	m_elementBuffer->UnBind();
}


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture*> textures)
	: m_vertices(vertices), m_indices(indices), Renderer() {
	m_textures = textures;
	Initialization();
}

Mesh::~Mesh() {
}

void Mesh::Draw(Shader& shader) {
	m_shader = &shader;
	m_shader->Bind();

	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;
	unsigned int normalNr = 0;
	unsigned int heightNr = 0;
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		std::string number;
		std::string name = m_textures[i]->GetType();
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);
		m_shader->SetUniform1i((name + number).c_str(), i);
		m_textures[i]->Bind(i);
	}
	DrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::AddVertex(Vertex vertex) {
	m_vertices.push_back(vertex);
}

void Mesh::AddIndex(GLint index) {
	m_indices.push_back(index);
}
