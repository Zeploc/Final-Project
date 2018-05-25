//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    ScoreTab.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Engine Includes //
#include "Engine\UIImage.h"
#include "Engine\UIText.h"

// Library Includes //
#include <memory>

#pragma once
class ScoreTab
{
public:
	ScoreTab();
	~ScoreTab();

	void Update();
	void Render();

private:
	std::shared_ptr<UIImage> m_pBackImage;
	std::shared_ptr<UIText> m_pScoresTitle;
};

