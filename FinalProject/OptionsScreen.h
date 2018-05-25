//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    OptionsScreen.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Engine Includes //
#include "Engine\UIImage.h"
#include "Engine\UIButton.h"
#include "Engine\UIText.h"

#pragma once
class OptionsScreen
{
public:
	OptionsScreen();
	~OptionsScreen();

	void Update();
	void Render();

private:
	std::shared_ptr<UIImage> m_pBackImage;
	std::shared_ptr<UIText> m_pOptionsTitle;
	std::shared_ptr<UIButton> m_pResumeButton;
	std::shared_ptr<UIButton> m_pQuitButton;
};

