//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    UITextField.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Local Includes //
#include "UIElement.h"

// Engine Includes //
#include "UIText.h"
#include "UIImage.h"

#pragma once
class UITextField : public UIElement
{
public:
	UITextField(glm::vec2 _Position, Utils::EANCHOR _anchor, float _fRotation, glm::vec4 _Colour, int iWidth, int iHeight, glm::vec4 _TextColour, std::string _DefaultValue, std::string _FontPath, int _iPSize, Utils::EANCHOR TextAnchor);
	UITextField(glm::vec2 _Position, Utils::EANCHOR _anchor, float _fRotation, const char* _ImagePath, int iWidth, int iHeight, glm::vec4 _TextColour, std::string _DefaultValue, std::string _FontPath, int _iPSize, Utils::EANCHOR TextAnchor);
	~UITextField();

	void SetActive(bool _bIsActive);
	
	void DrawUIElement();
	void Update();

	UIText FieldText;
	UIImage BackImage;

private:
	bool bIsFocussed = false;

	double dSpamDelay = 0.0f;
	float fSpamTime = 0.1f;
};

