#include "TextBox.h"

TextBox::TextBox(Text* text, Shader* textShader, GLfloat x, GLfloat y, GLfloat width, GLfloat height)
	:m_text(text), m_textShader(textShader) {

	m_openState = GL_FALSE;
	m_talker = "";
	m_context = "";
	m_fontColor = glm::vec3(1.0f, 1.0f, 1.0f);
	m_backgroundColor = glm::vec4(0.2f, 0.2f, 0.2f, 0.8f);
	m_talkerParam = glm::vec3(50.0f, 220.0f, 1.0f);
	m_contextParam = glm::vec3(50.0f, 100.0f, 1.0f);
	m_box = new Rectangle(x, y, width, height);
	m_box->SetColor(m_backgroundColor);

}

void TextBox::SetFontColor(glm::vec3 color) {
	m_fontColor = color;
}

void TextBox::SetBackgroundColor(glm::vec4 color) {
	m_backgroundColor = color;
}


void TextBox::SetText(Text* text) {
	m_text = text;
}

void TextBox::SetTalker(const std::string& talker) {
	m_talker = talker;
}

void TextBox::SetTalkerParam(glm::vec3 param) {
	m_talkerParam = param;
}

void TextBox::SetContext(const std::string& context) {
	m_context = context;
}

void TextBox::SetContextParam(glm::vec3 param) {
	m_contextParam = param;
}

void TextBox::SetOpenState(GLboolean state) {
	m_openState = state;
}

GLboolean TextBox::GetOpenState()
{
	return m_openState;
}

void TextBox::Draw() {
	m_box->Draw();

	glDepthFunc(GL_LEQUAL);
	if (m_talker != "") {
		m_text->Draw(*m_textShader, m_talker, m_talkerParam.x, m_talkerParam.y, m_talkerParam.z, m_fontColor);
	}
	if (m_context != "") {
		m_text->Draw(*m_textShader, m_context, m_contextParam.x, m_contextParam.y, m_contextParam.z, m_fontColor);
	}
	glDepthFunc(GL_LESS);




}
