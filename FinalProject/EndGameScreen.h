//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    EndGameScreen.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Engine Includes //
#include "Engine\UIImage.h"
#include "Engine\UIText.h"
#include "Engine/UIButton.h"

// Library Includes //
#include <memory>

#pragma once
class EndScreen
{
public:
	EndScreen();
	~EndScreen();

	void Update();
	void Render();

private:
	std::shared_ptr<UIImage> m_pBackImage;
	std::shared_ptr<UIText> m_pMessage;
	std::shared_ptr<UIText> m_pScore;
	std::shared_ptr<UIButton> m_pQuit;
	std::shared_ptr<UIButton> m_pRestart;

};

