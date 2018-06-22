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
	m_pWaveTime = std::make_shared<UIText>(UIText({ Camera::GetInstance()->SCR_WIDTH - 10, 10 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "Time Remaining: 0", "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::TOP_RIGHT));;
	m_pBackImage = std::make_shared<UIImage>(UIImage({ 10, Camera::GetInstance()->SCR_HEIGHT }, Utils::BOTTOM_LEFT, 0, { 0.5f, 0.5f, 0.5f, 1.0f }, 400, 60));
	m_pScore = std::make_shared<UIText>(UIText({ 20, Camera::GetInstance()->SCR_HEIGHT - 10 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "Score: 0", "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::BOTTOM_LEFT));
	m_pHealth = std::make_shared<UIText>(UIText({ 200, Camera::GetInstance()->SCR_HEIGHT - 10 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "Health: 0", "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::BOTTOM_LEFT));
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
		m_vpPlayerScore[i]->sText = "$: " + std::to_string(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity->PlayerEntities[m_vpPlayerName[i]->sText]->GetScore());
		m_vpPlayerHealth[i]->sText = "+: " + std::to_string((int)NetworkManager::GetInstance()->m_Network.m_pNetworkEntity->PlayerEntities[m_vpPlayerName[i]->sText]->m_fHealth);
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
	float fxPosition;
	if (m_vpPlayerName.size() > 0)
	{
		fxPosition = m_vpPlayerHealth.back()->GetPosition().x + 150;
	}
	else
		fxPosition = 10;

	m_vpPlayerName.push_back(std::make_shared<UIText>(UIText({ fxPosition, 10 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, UserName, "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::TOP_LEFT)));
	fxPosition += UserName.size() * 15 + 30;
	m_vpPlayerScore.push_back(std::make_shared<UIText>(UIText({ fxPosition, 10 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "Score: 0", "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::TOP_LEFT)));
	m_vpPlayerHealth.push_back(std::make_shared<UIText>(UIText({ fxPosition + 60, 10 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "Health: 0", "Resources/Fonts/Roboto-Medium.ttf", 30, Utils::TOP_LEFT)));
}

void HUD::RemovePlayer(std::string UserName)
{
	int iCount = 0;
	for (auto it = m_vpPlayerName.begin(); it != m_vpPlayerName.end(); it++)
	{
		if ((*it)->sText == UserName)
		{
			m_vpPlayerName.erase(it);
			break;
		}
		iCount++;
	}
	int Current = 0;
	for (auto it = m_vpPlayerScore.begin(); it != m_vpPlayerScore.end(); it++)
	{
		if (Current == iCount)
		{
			m_vpPlayerScore.erase(it);
			break;
		}
	}
	Current = 0;
	for (auto it = m_vpPlayerHealth.begin(); it != m_vpPlayerHealth.end(); it++)
	{
		if (Current == iCount)
		{
			m_vpPlayerHealth.erase(it);
			break;
		}
	}
	RepositionPlayersHUD();
}

void HUD::ClearPlayersHUD()
{
	m_vpPlayerName.clear();
	m_vpPlayerScore.clear();
	m_vpPlayerHealth.clear();
}

void HUD::RepositionPlayersHUD()
{
	float fxPosition = 10;

	for (int i = 0; i < m_vpPlayerScore.size(); i++)
	{
		if (i != 0) fxPosition = m_vpPlayerHealth[i - 1]->GetPosition().x + 150;
		m_vpPlayerName[i]->SetPosition({ fxPosition, 10 });
		fxPosition += m_vpPlayerName[i]->sText.size() * 15 + 30;
		m_vpPlayerScore[i]->SetPosition({ fxPosition, 10 });
		m_vpPlayerHealth[i]->SetPosition({ fxPosition + 60, 10 });
	}
}
