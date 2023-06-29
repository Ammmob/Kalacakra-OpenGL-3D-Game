#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Texture {
private:
	GLuint m_textureID;
	static GLuint bindedID;
	GLenum m_target;
	std::string m_type;
public:
	Texture(GLenum targe = GL_TEXTURE_2D, const std::string& type = "texture_diffuse");
	~Texture();
	void Bind(GLuint slot = 0);
	void UnBind(GLuint slot = 0);
	void Parameteri(GLenum pname, GLint param);
	void SetWrap(GLint sParam, GLint tParam);
	void SetWrap(GLint sParam, GLint tParam, GLint rParam);
	void SetFilter(GLint minParam, GLint magParam);
	std::string GetType();

	static void SetFilpVertically(GLboolean state);
	static void LoadTexture(const std::string& path, GLenum target = GL_TEXTURE_2D, const aiTexture* aitexture = nullptr, GLint level = 0, GLint border = 0, GLenum type = GL_UNSIGNED_BYTE);
};