#include "Texture.h"
#include <stb_image.h>

GLuint Texture::bindedID = 0;

Texture::Texture(GLenum target, const std::string& type)
	: m_target(target), m_type(type) {
	glGenTextures(1, &m_textureID);
	SetFilpVertically(GL_TRUE);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_textureID);
}

void Texture::Bind(GLuint slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(m_target, m_textureID);
	bindedID = m_textureID;
}

void Texture::UnBind(GLuint slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(m_target, 0);
	bindedID = 0;
}

void Texture::Parameteri(GLenum pname, GLint param) {
	assert(bindedID = m_textureID);
	glTexParameteri(m_target, pname, param);
}

void Texture::SetWrap(GLint sParam, GLint tParam) {
	assert(bindedID = m_textureID);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, sParam);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, tParam);
}

void Texture::SetWrap(GLint sParam, GLint tParam, GLint rParam) {
	assert(bindedID = m_textureID);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, sParam);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, tParam);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_R, rParam);
}

void Texture::SetFilter(GLint minParam, GLint magParam) {
	assert(bindedID = m_textureID);
	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, minParam);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, magParam);
}

std::string Texture::GetType() {
	return m_type;
}

void Texture::SetFilpVertically(GLboolean state) {
	stbi_set_flip_vertically_on_load(state);
}

void Texture::LoadTexture(const std::string& path, GLenum target, const aiTexture* aitexture, GLint level, GLint border, GLenum type) {
	int width, height, channels;

	GLubyte* data;
	if (aitexture == nullptr) {
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	}
	else {
		if (aitexture->mHeight == 0) {
			data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(aitexture->pcData), aitexture->mWidth, &width, &height, &channels, 0);
		}
		else {
			data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(aitexture->pcData), aitexture->mWidth * aitexture->mHeight, &width, &height, &channels, 0);
		}
	}
	if (data) {
		GLenum format = GL_RGB;
		if (channels == 1)
			format = GL_RED;
		else if (channels == 3)
			format = GL_RGB;
		else if (channels == 4)
			format = GL_RGBA;
		glTexImage2D(target, level, format, width, height, border, format, type, data);
		stbi_image_free(data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR:: Texture loaded failed: " + path << std::endl;
	}
}
