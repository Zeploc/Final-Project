//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    HUD.h
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
class HUD
{
public:
	HUD();
	~HUD();

	void Update();
	void Render();

	void SetScore(int _iScore);
	void SetHealth(float _fHealth);
	void SetWaveTimer(float _fCurrentTime);

private:
	std::shared_ptr<UIText> m_pWaveTime;
	std::shared_ptr<UIImage> m_pBackImage;
	std::shared_ptr<UIText> m_pScore;
	std::shared_ptr<UIText> m_pHealth;
};

