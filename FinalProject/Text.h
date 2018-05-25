
#include <glew.h>
#include <freeglut.h>
#include <glm\common.hpp>

#include <string>
#include <map>
#include <vector>

#include "Engine\ShaderLoader.h"

#pragma once
class Text
{
public:
	Text(std::string newText, std::string newFont, glm::vec2 pos);
	~Text();


	struct Character
	{
		GLuint TextureID;	//Texture ID
		glm::ivec2 Size;	// Size of glyp
		glm::ivec2 Bearing;	// Position of glypj
		GLuint Advance;		// How far to move for the next character
	};	struct cFont	{		std::map<GLchar, Character> Characters;		std::string sPathName;		GLuint VAO;	};	static std::vector<cFont> Fonts;

	void Render();
	void SetText(std::string newText) { text = newText; };
	void SetColor(glm::vec3 newColor) { color = newColor; };
	void SetScale(GLfloat newScale) { scale = newScale; };
	void SetPosition(glm::vec2 newPosition) { position = newPosition; };
private:
	std::string text;
	GLfloat scale;
	glm::vec3 color;
	glm::vec2 position;
	GLuint VAO, VBO;
	static ShaderLoader shaderLoader;
};

