//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    MessageBox.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <memory>

// Engine Includes //
#include "Engine\Scene.h"
#include "Engine/UIButton.h"

#pragma once
class UIMessageBox
{
public:
	UIMessageBox();
	~UIMessageBox();
	
	void SetMessageText(std::string _Message);
	
	void Update();
	void Render();

private:
	std::shared_ptr<UIImage> m_pBackImage;
	std::shared_ptr<UIText> m_pMessage;
	std::shared_ptr<UIButton> m_pAcceptButton;
};

