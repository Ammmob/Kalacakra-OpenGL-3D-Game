#pragma once

#include "Rectangle.h"
class Bar {
public:
	Bar(GLfloat x, GLfloat y, GLfloat width, GLfloat height, glm::vec3 barColor);
	void Draw(GLfloat rate);
private:
	Rectangle* m_box;
	Rectangle* m_bar;
	Shader* m_shader;
};