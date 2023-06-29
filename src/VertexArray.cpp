#include "VertexArray.h"
#include <iostream>
GLuint VertexArray::bindedID = 0;

VertexArray::VertexArray() :m_stride(0) {
	glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::Bind() {
	glBindVertexArray(m_ID);
	bindedID = m_ID;
}

void VertexArray::UnBind() {
	glBindVertexArray(0);
	bindedID = 0;
}

void VertexArray::AddAttribute(GLint count, GLenum type, GLboolean normalized, GLuint offset) {
	m_layout.push_back({ count, type, normalized, offset });
	m_stride = std::max((GLuint)m_stride, offset) + count * Attribute::SizeofType(type);

	//if (offset == 0) {
	//	m_stride = m_stride + count * Attribute::SizeofType(type);
	//}
	//else {
	//	m_stride = offset + count * Attribute::SizeofType(type);
	//}
}

void VertexArray::EnableAllAttribute() {
	assert(bindedID == m_ID);
	GLuint offset = 0;
	for (int i = 0; i < m_layout.size(); i++) {
		Attribute attrib = m_layout[i];
		offset = std::max(offset, attrib.offset);
		glEnableVertexAttribArray(i);
		if (attrib.type == GL_INT) {
			glVertexAttribIPointer(i, attrib.count, attrib.type, m_stride, (const void*)offset);
		}
		else {
			glVertexAttribPointer(i, attrib.count, attrib.type, attrib.normalized, m_stride, (const void*)offset);
		}
		// std::cout << i << " " << attrib.count << " " << m_stride << " " << offset << "\n";
		offset += attrib.count * Attribute::SizeofType(attrib.type);
	}
}

void VertexArray::DisableAllAttribute() {
	assert(bindedID == m_ID);
	for (int i = 0; i < m_layout.size(); i++) {
		glDisableVertexAttribArray(i);
	}
}
