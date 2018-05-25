//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Chat.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Engine Includes //
#include "Engine\UIImage.h"
#include "Engine\UIButton.h"

// Library Includes //
#include <memory>

#pragma once­­
class Chat
{
public:
	Chat();
	~Chat();
	
	void Update();
	void Render();

private:
	std::shared_ptr<UIImage> m_pBackImage;
	std::shared_ptr<UIImage> m_pChatBackImage;
	std::shared_ptr<UIImage> m_pTypingField;
	std::shared_ptr<UIButton> m_pEnterButton;
};

