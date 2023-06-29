#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {
private:
	GLuint m_programID;
	const std::string ParseShader(const std::string shaderPath);
	GLuint CreateShader(GLenum type, const std::string& shaderSource);
	GLboolean CompileShader(GLuint shader);

public:
	Shader(const std::string vertexPath, const std::string fragmentPath);
	~Shader();
	void Bind();
	void UnBind();
	GLint GetUniformLocation(const std::string& name);
	void SetUniform1i(const std::string& name, GLint x);
	void SetUniform1f(const std::string& name, GLfloat x);
	void SetUniform1d(const std::string& name, GLdouble x);
	void SetUniform2i(const std::string& name, GLint x, GLint y);
	void SetUniform2f(const std::string& name, GLfloat x, GLfloat y);
	void SetUniform2d(const std::string& name, GLdouble x, GLdouble y);
	void SetUniform3i(const std::string& name, GLint x, GLint y, GLint z);
	void SetUniform3f(const std::string& name, GLfloat x, GLfloat y, GLfloat z);
	void SetUniform3d(const std::string& name, GLdouble x, GLdouble y, GLdouble z);
	void SetUniform4i(const std::string& name, GLint x, GLint y, GLint z, GLint w);
	void SetUniform4f(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void SetUniform4d(const std::string& name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);

	void SetUniform1f(const std::string& name, glm::vec1 var);
	void SetUniform2f(const std::string& name, glm::vec2 var);
	void SetUniform3f(const std::string& name, glm::vec3 var);
	void SetUniform4f(const std::string& name, glm::vec4 var);

	void SetUniformMatrix4fv(const std::string& name, GLsizei count, GLboolean transpos, const GLfloat* value);
	void SetUniformMatrix4fv(const std::string& name, const GLfloat* value);
	void SetUniformMatrix4fv(const std::string& name, glm::mat4 mat);
};
