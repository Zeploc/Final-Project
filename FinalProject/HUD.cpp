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

// Local Includes //
#include "NetworkManager.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
HUD::HUD()
{
	m_pWaveTime = std::make_shared<UIText>(UIText({ Camera::GetInstance()->SCR_WIDTH / 2, 10 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "Time Remaining: 0", "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::TOP_CENTER));;
	m_pBackImage = std::make_shared<UIImage>(UIImage({ 0, Camera::GetInstance()->SCR_HEIGHT }, Utils::BOTTOM_LEFT, 0, { 0.5f, 0.5f, 0.5f, 1.0f }, 360, 60));
	m_pScore = std::make_shared<UIText>(UIText({ 10, Camera::GetInstance()->SCR_HEIGHT - 10 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "Score: 0", "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::BOTTOM_LEFT));
	m_pHealth = std::make_shared<UIText>(UIText({ 190, Camera::GetInstance()->SCR_HEIGHT - 10 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "Health: 0", "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::BOTTOM_LEFT));
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
	m_pWaveTime->Update();
	m_pBackImage->Update();
	m_pScore->Update();
	m_pHealth->Update();

	for (int i = 0; i < m_vpPlayerScore.size(); i++)
	{
		m_vpPlayerScore[i]->sText = "S: " + std::to_string(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity->PlayerEntities[m_vpPlayerName[i]->sText]->GetScore());
		m_vpPlayerHealth[i]->sText = "H: " + std::to_string((int)NetworkManager::GetInstance()->m_Network.m_pNetworkEntity->PlayerEntities[m_vpPlayerName[i]->sText]->m_fHealth);
		m_vpPlayerScore[i]->Update();
		m_vpPlayerHealth[i]->Update();
	}
}

void HUD::Render()
{
	m_pWaveTime->DrawUIElement();
	m_pBackImage->DrawUIElement();
	m_pScore->DrawUIElement();
	m_pHealth->DrawUIElement();
	for (int i = 0; i < m_vpPlayerScore.size(); i++)
	{
		m_vpPlayerName[i]->DrawUIElement();
		m_vpPlayerScore[i]->DrawUIElement();
		m_vpPlayerHealth[i]->DrawUIElement();
	}
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

void HUD::SetWaveTimer(float _fCurrentTime)
{
	int iTime = static_cast<int>(round(_fCurrentTime));
	m_pWaveTime->sText = "Time remaining: " + std::to_string(iTime);
}

void HUD::AddPlayer(std::string UserName)
{
	m_vpPlayerName.push_back(std::make_shared<UIText>(UIText({ -180 + (m_vpPlayerName.size() + 1) * 300, 10 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, UserName, "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::TOP_LEFT)));
	m_vpPlayerScore.push_back(std::make_shared<UIText>(UIText({ -20 + (m_vpPlayerScore.size() + 1) * 300, 10 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "Score: 0", "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::TOP_LEFT)));
	m_vpPlayerHealth.push_back(std::make_shared<UIText>(UIText({ 50 + (m_vpPlayerHealth.size() + 1) * 300, 10 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "Health: 0", "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::TOP_LEFT)));
}
