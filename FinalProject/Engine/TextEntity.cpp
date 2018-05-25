#include "TextEntity.h"

#include "Camera.h"

TextEntity::TextEntity(Utils::Transform _Transform, float _fWidth, float _fHeight, glm::vec4 _Colour, std::string _sText)
	: Entity(_Transform, _fWidth, _fHeight, _Colour), sText(_sText)
{
}


TextEntity::~TextEntity()
{
}

void TextEntity::DrawEntity()
{
	Camera::GetInstance()->SetMVP(transform);
	const unsigned char* Text = reinterpret_cast<const unsigned char*>(sText.c_str());

	glColor4f(Colour.r, Colour.g, Colour.b, Colour.a);

	glm::vec2 DrawPos = transform.Position;

	glRasterPos2f(DrawPos.x, DrawPos.y);

	glutBitmapString(GLUT_BITMAP_HELVETICA_18, Text);
}