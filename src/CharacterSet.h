#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <codecvt>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    //  Horizontal offset to advance to next glyph
};

enum CodeType {
	UTF8, ANSI
};

class CharacterSet {
public:



	CodeType m_codeType;
	std::map<GLshort, Character>m_characters;

	CharacterSet(const std::string& path, CodeType type);

	static std::wstring ANSIToUnicode(const std::string& s);
	static std::wstring UTF8ToUnicode(const std::string& s);
private:
	FT_Face m_face;
	FT_Library m_library;
	void ReadAndConvert(const std::string& path);
	void LoadCharacter(const std::wstring& ws);
};