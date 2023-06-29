#pragma once

#include "Text.h"
#include "Rectangle.h"

class TextBox {
public:
	TextBox(Text* text, Shader* textShader, GLfloat x = -1.0f, GLfloat y = -1.0f, GLfloat width = 2.0f, GLfloat height = 0.5f);
	void SetFontColor(glm::vec3 color);
	void SetBackgroundColor(glm::vec4 color);
	void SetText(Text* text);
	void SetTalker(const std::string& talker);
	void SetTalkerParam(glm::vec3 param);
	void SetContext(const std::string& context);
	void SetContextParam(glm::vec3 param);
	void SetOpenState(GLboolean state);
	GLboolean GetOpenState();
	void Draw();
private:
	glm::vec4 m_backgroundColor;
	glm::vec3 m_fontColor;
	std::string m_talker;
	std::string m_context;
	Text* m_text;
	Rectangle* m_box;
	glm::vec3 m_talkerParam;
	glm::vec3 m_contextParam;

	Shader* m_textShader;

	GLboolean m_openState;

};