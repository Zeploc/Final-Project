//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    HUD.cpp
// Description    	:    The Chat UI System for the game
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "HUD.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
HUD::HUD()
{
	m_pBackImage = std::make_shared<UIImage>(UIImage({ 0, Camera::GetInstance()->SCR_HEIGHT }, Utils::BOTTOM_LEFT, 0, { 0.5f, 0.5f, 0.5f, 1.0f }, 360, 60));
	m_pScore = std::make_shared<UIText>(UIText({ 10, Camera::GetInstance()->SCR_HEIGHT - 10 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "Score: 0", "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::BOTTOM_LEFT));;
	m_pHealth = std::make_shared<UIText>(UIText({ 190, Camera::GetInstance()->SCR_HEIGHT - 10 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "Health: 0", "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::BOTTOM_LEFT));;
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
HUD::~HUD()
{

}

void HUD::Update()
{
	m_pBackImage->Update();
	m_pScore->Update();
	m_pHealth->Update();
}

void HUD::Render()
{
	m_pBackImage->DrawUIElement();
	m_pScore->DrawUIElement();
	m_pHealth->DrawUIElement();
}

void HUD::SetScore(int _iScore)
{
	m_pScore->sText = "Score: " + std::to_string(_iScore);
}

void HUD::SetHealth(float _fHealth)
{
	int iHealth = static_cast<int>(round(_fHealth));
	m_pHealth->sText = "Health: " + std::to_string(iHealth);
}
