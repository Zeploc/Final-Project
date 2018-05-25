
#include <string>
#pragma once
#include "Entity.h"

class TextEntity :	public Entity
{
public:
	TextEntity(Utils::Transform _Transform, float _fWidth, float _fHeight, glm::vec4 _Colour, std::string _sText);
	~TextEntity();

	void DrawEntity();

	std::string sText;
};

