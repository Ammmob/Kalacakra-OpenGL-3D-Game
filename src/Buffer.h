#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Buffer {
private:
	GLuint m_bufferID;
	GLenum m_target;
	static GLuint bindedID;
public:
	Buffer(GLenum target);
	~Buffer();
	void Bind();
	void UnBind();
	void SetData(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW);
	void SetSubData(GLintptr offset, GLsizeiptr size, const void* data);
};