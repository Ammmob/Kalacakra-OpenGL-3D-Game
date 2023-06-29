#include "Shader.h"

Shader::Shader(const std::string vertexPath, const std::string fragmentPath) {
	std::string vertexSource, fragmentSource;
	vertexSource = ParseShader(vertexPath);
	fragmentSource = ParseShader(fragmentPath);

	GLuint vertexShader, fragmentShader;
	vertexShader = CreateShader(GL_VERTEX_SHADER, vertexSource);
	fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentSource);

	CompileShader(vertexShader);
	CompileShader(fragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);
	glValidateProgram(m_programID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader() {
	glDeleteProgram(m_programID);
}

const std::string Shader::ParseShader(const std::string shaderPath) {
	std::ifstream shaderFile(shaderPath);
	std::stringstream shaderStream;

	if (shaderFile) {
		std::string line;
		while (getline(shaderFile, line)) {
			shaderStream << line << "\n";
		}
	}
	else {
		std::cout << "ERROR::" << shaderPath << " not found" << std::endl;
	}

	return shaderStream.str();
}

GLuint Shader::CreateShader(GLenum type, const std::string& shaderSource) {
	GLuint shader = glCreateShader(type);
	const GLchar* source = shaderSource.c_str();
	glShaderSource(shader, 1, &source, NULL);
	return shader;
}

GLboolean Shader::CompileShader(GLuint shader) {
	glCompileShader(shader);
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::" << infoLog << std::endl;
		return GL_FALSE;
	}
	return GL_TRUE;
}

void Shader::Bind() {
	glUseProgram(m_programID);
}

void Shader::UnBind() {
	glUseProgram(0);
}

GLint Shader::GetUniformLocation(const std::string& name) {
	return glGetUniformLocation(m_programID, name.c_str());
}


void Shader::SetUniform1i(const std::string& name, GLint x) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform1i(location, x);
}

void Shader::SetUniform1f(const std::string& name, GLfloat x) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform1f(location, x);
}

void Shader::SetUniform1d(const std::string& name, GLdouble x) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform1d(location, x);
}

void Shader::SetUniform2i(const std::string& name, GLint x, GLint y) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform2i(location, x, y);
}

void Shader::SetUniform2f(const std::string& name, GLfloat x, GLfloat y) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform2f(location, x, y);
}

void Shader::SetUniform2d(const std::string& name, GLdouble x, GLdouble y) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform2d(location, x, y);
}

void Shader::SetUniform3i(const std::string& name, GLint x, GLint y, GLint z) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform3i(location, x, y, z);
}

void Shader::SetUniform3f(const std::string& name, GLfloat x, GLfloat y, GLfloat z) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform3f(location, x, y, z);
}

void Shader::SetUniform3d(const std::string& name, GLdouble x, GLdouble y, GLdouble z) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform3d(location, x, y, z);
}

void Shader::SetUniform4i(const std::string& name, GLint x, GLint y, GLint z, GLint w) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform4i(location, x, y, z, w);
}

void Shader::SetUniform4f(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform4f(location, x, y, z, w);
}

void Shader::SetUniform4d(const std::string& name, GLdouble x, GLdouble y, GLdouble z, GLdouble w) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform4d(location, x, y, z, w);
}

void Shader::SetUniform1f(const std::string& name, glm::vec1 var) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform1f(location, var.x);
}

void Shader::SetUniform2f(const std::string& name, glm::vec2 var) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform2f(location, var.x, var.y);
}

void Shader::SetUniform3f(const std::string& name, glm::vec3 var) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform3f(location, var.x, var.y, var.z);
}

void Shader::SetUniform4f(const std::string& name, glm::vec4 var) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniform4f(location, var.x, var.y, var.z, var.w);
}

void Shader::SetUniformMatrix4fv(const std::string& name, GLsizei count, GLboolean transpos, const GLfloat* value) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniformMatrix4fv(location, count, transpos, value);
}

void Shader::SetUniformMatrix4fv(const std::string& name, const GLfloat* value) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void Shader::SetUniformMatrix4fv(const std::string& name, glm::mat4 mat) {
	GLint location = GetUniformLocation(name);
	if (location == -1) {
		std::cout << "WARMING:: Can not find uniform variable:" << name << std::endl;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}
