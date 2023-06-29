#pragma once 

#include "CharacterSet.h"
#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"

class Text {
public:
	Text(CharacterSet* characterSet);
	void SetProjection(glm::mat4 projection);
	void Draw(Shader& shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
private:
	glm::mat4 m_projection;
	CharacterSet* m_characterSet;
	Buffer m_vertexBuffer;
	VertexArray m_vertexArray;

};