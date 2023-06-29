#include "Text.h"

Text::Text(CharacterSet* characterSet) :m_vertexBuffer(GL_ARRAY_BUFFER) {
	m_characterSet = characterSet;

	m_projection = glm::mat4(1.0f);
	m_vertexBuffer.Bind();
	m_vertexArray.Bind();
	m_vertexBuffer.SetData(sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	m_vertexArray.AddAttribute(4, GL_FLOAT, GL_FALSE);
	m_vertexArray.EnableAllAttribute();
	m_vertexBuffer.UnBind();
	m_vertexArray.UnBind();
}


void Text::SetProjection(glm::mat4 projection) {
	m_projection = projection;
}

void Text::Draw(Shader& shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
	shader.Bind();
	shader.SetUniform1i("u_text", 0);
	shader.SetUniform3f("u_textColor", color);
	shader.SetUniformMatrix4fv("u_projection", m_projection);
	glActiveTexture(GL_TEXTURE0);
	m_vertexArray.Bind();

	std::wstring ws;
	if (m_characterSet->m_codeType == CodeType::UTF8) {
		ws = CharacterSet::UTF8ToUnicode(text);
	}
	else {
		ws = CharacterSet::ANSIToUnicode(text);
	}

	// Iterate through all characters
	std::string::const_iterator c;
	for (int i = 0; i < ws.size(); i++)
	{
		GLint c = ws[i];
		Character ch = m_characterSet->m_characters[c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		m_vertexBuffer.Bind();
		m_vertexBuffer.SetSubData(0, sizeof(vertices), vertices);
		m_vertexBuffer.UnBind();
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	m_vertexArray.UnBind();
	glBindTexture(GL_TEXTURE_2D, 0);
}
