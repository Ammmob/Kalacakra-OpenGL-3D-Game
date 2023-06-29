#include "Bar.h"

Bar::Bar(GLfloat x, GLfloat y, GLfloat width, GLfloat height, glm::vec3 barColor) {
	m_box = new Rectangle(x, y, width, height, GL_TRUE, GL_TRUE);
	m_box->SetTexture("./res/texture/bar.png");

	m_shader = new Shader("./res/shader/bar.vert", "./res/shader/bar.frag");
	m_shader->Bind();
	m_shader->SetUniform1f("u_posx", Rectangle::NormalizeX(x + 0.04 * width));
	m_shader->UnBind();

	m_bar = new Rectangle(x + 0.042 * width, y + 0.3 * height, 0.916 * width, 0.4 * height, GL_TRUE);
	m_bar->SetColor(glm::vec4(barColor, 1.0f));
	m_bar->SetShader(m_shader);
}

void Bar::Draw(GLfloat rate) {
	m_box->Draw();
	m_shader->Bind();
	m_shader->SetUniform1f("u_rate", rate);
	m_bar->Draw();
}
