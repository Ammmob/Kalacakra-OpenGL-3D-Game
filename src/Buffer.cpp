#include "Buffer.h"

GLuint Buffer::bindedID = 0;

Buffer::Buffer(GLenum target) :m_target(target) {
	glGenBuffers(1, &m_bufferID);
}

Buffer::~Buffer() {
	glDeleteBuffers(1, &m_bufferID);
}

void Buffer::Bind() {
	glBindBuffer(m_target, m_bufferID);
	bindedID = m_bufferID;
}

void Buffer::UnBind() {
	glBindBuffer(m_target, 0);
	bindedID = 0;
}

void Buffer::SetData(GLsizeiptr size, const void* data, GLenum usage) {
	assert(bindedID = m_bufferID);
	glBufferData(m_target, size, data, usage);
}

void Buffer::SetSubData(GLintptr offset, GLsizeiptr size, const void* data) {
	assert(bindedID = m_bufferID);
	glBufferSubData(m_target, offset, size, data);
}
