#include "CharacterSet.h"


CharacterSet::CharacterSet(const std::string& path, CodeType type) {
	m_codeType = type;

	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&m_library))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	if (FT_New_Face(m_library, path.c_str(), 0, &m_face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(m_face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	if (m_codeType == CodeType::UTF8) {
		ReadAndConvert("./res/fonts/library/chineseUTF8.txt");
	}
	else {
		ReadAndConvert("./res/fonts/library/chineseANSI.txt");
	}

	// Destroy FreeType once we're finished
	FT_Done_Face(m_face);
	FT_Done_FreeType(m_library);
}

std::wstring CharacterSet::ANSIToUnicode(const std::string& s) {
	std::string curLocale = setlocale(LC_ALL, NULL);  //curLocale="C"
	setlocale(LC_ALL, "chs");
	const char* source = s.c_str();
	size_t charNum = s.size() + 1;

	wchar_t* dest = new wchar_t[charNum];
	mbstowcs_s(NULL, dest, charNum, source, _TRUNCATE);
	std::wstring result = dest;
	delete[] dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

//std::wstring CharacterSet::ANSIToUnicode(const std::string& str)
//{
//	std::wstring ret;
//	std::mbstate_t state = {};
//	const char* src = str.data();
//	size_t len = std::mbsrtowcs(nullptr, &src, 0, &state);
//	if (static_cast<size_t>(-1) != len) {
//		std::unique_ptr< wchar_t[] > buff(new wchar_t[len + 1]);
//		len = std::mbsrtowcs(buff.get(), &src, len, &state);
//		if (static_cast<size_t>(-1) != len) {
//			ret.assign(buff.get(), len);
//		}
//	}
//	return ret;
//}

std::wstring CharacterSet::UTF8ToUnicode(const std::string& s) {
	std::wstring result;
	try {
		std::wstring_convert< std::codecvt_utf8<wchar_t>> wcv;
		result = wcv.from_bytes(s);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return result;
}


void CharacterSet::ReadAndConvert(const std::string& path) {
	std::ifstream library(path);
	if (!library) {
		std::cout << "ERROR:: " << path << " not found \n";
		return;
	}
	std::stringstream buffer;
	buffer << library.rdbuf();
	std::string s(buffer.str());
	std::wstring ws;
	if (m_codeType == CodeType::UTF8) {
		ws = UTF8ToUnicode(s);
	}
	else if (m_codeType == CodeType::ANSI) {
		ws = ANSIToUnicode(s);
	}
	else {
		std::cout << "ERROR:: Code type not utf-8 nor ANSI! \n";
	}

	LoadCharacter(ws);
}

void CharacterSet::LoadCharacter(const std::wstring& ws) {
	for (int i = 0; i < ws.size(); i++)
	{
		GLint c = ws[i];
		if (FT_Load_Char(m_face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		}
		assert(m_face->glyph);

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			m_face->glyph->bitmap.width,
			m_face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			m_face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
			glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
			m_face->glyph->advance.x
		};
		m_characters.insert(std::pair<GLshort, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}