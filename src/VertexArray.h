#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

struct Attribute {
	GLint count;
	GLenum type;
	GLboolean normalized;
	GLuint offset = 0;
	static GLsizeiptr SizeofType(GLenum type) {
		switch (type) {
		case GL_BYTE:			return sizeof(GLbyte);
		case GL_UNSIGNED_BYTE:	return sizeof(GLubyte);
		case GL_SHORT:			return sizeof(GLshort);
		case GL_UNSIGNED_SHORT: return sizeof(GLushort);
		case GL_INT:			return sizeof(GLint);
		case GL_UNSIGNED_INT:	return sizeof(GLuint);
		case GL_FLOAT:			return sizeof(GLfloat);
		case GL_DOUBLE:			return sizeof(GLdouble);
		default:				return 0;
		}
	}
};

class VertexArray {
private:
	GLuint m_ID;
	GLsizei m_stride;
	std::vector<Attribute>m_layout;
	static GLuint bindedID;
public:
	VertexArray();
	~VertexArray();
	void Bind();
	void UnBind();
	void AddAttribute(GLint count, GLenum type, GLboolean normalized = GL_FALSE, GLuint offset = 0);
	void EnableAllAttribute();
	void DisableAllAttribute();
};