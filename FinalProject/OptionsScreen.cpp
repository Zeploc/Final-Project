//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    OptionScreen.cpp
// Description    	:    The In Game Options Screen
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "OptionsScreen.h"

// Engine Includes //
#include "Engine\SceneManager.h"

// Local Includes //
#include "UIManager.h"
#include "NetworkManager.h"
#include "Client.h"

// Local Functions //
void QuitButton();
void ResumeButton();

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
OptionsScreen::OptionsScreen()
{
	m_pBackImage = std::make_shared<UIImage>(UIImage({ Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 }, Utils::CENTER, 0, { 0.7f, 0.7f, 0.7f, 0.5f }, Camera::GetInstance()->SCR_WIDTH, Camera::GetInstance()->SCR_HEIGHT));
	m_pOptionsTitle = std::make_shared<UIText>(UIText({ Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 - 250 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "Options:", "Resources/Fonts/Roboto-Black.ttf", 50, Utils::CENTER));;
	m_pQuitButton = std::make_shared<UIButton>(UIButton({ Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 + 50}, Utils::CENTER, 0, { 0.4f, 0.4f, 0.4f, 1.0f }, { 0.8f, 0.8f, 0.8f, 1.0f }, 300, 60, QuitButton));
	m_pQuitButton->AddText("Quit", "Resources/Fonts/Roboto-Thin.ttf", 40, { 1.0f, 1.0f, 1.0f, 1.0f }, Utils::CENTER);
	m_pResumeButton = std::make_shared<UIButton>(UIButton({ Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 - 50 }, Utils::CENTER, 0, { 0.4f, 0.4f, 0.4f, 1.0f }, { 0.8f, 0.8f, 0.8f, 1.0f }, 300, 60, ResumeButton));
	m_pResumeButton->AddText("Resume", "Resources/Fonts/Roboto-Thin.ttf", 40, { 1.0f, 1.0f, 1.0f, 1.0f }, Utils::CENTER);
}


/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
OptionsScreen::~OptionsScreen()
{
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void OptionsScreen::Update()
{
	if (NetworkManager::GetInstance()->m_Network.IsServer() && m_pQuitButton->TextComponent.sText != "Disconnect")
		m_pQuitButton->TextComponent.sText = "Disconnect";

	m_pBackImage->Update();
	m_pOptionsTitle->Update();
	m_pQuitButton->Update();
	m_pResumeButton->Update();
}

/************************************************************
#--Description--#:	Render Scene to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void OptionsScreen::Render()
{
	m_pBackImage->DrawUIElement();
	m_pOptionsTitle->DrawUIElement();
	m_pQuitButton->DrawUIElement();
	m_pResumeButton->DrawUIElement();
}

void QuitButton()
{
	if (NetworkManager::GetInstance()->m_Network.IsServer())
	{
		// Disconnect all clients
		// clear connected clients
		// clear players map
	}
	else
	{
		std::shared_ptr<Client> ClientRef = std::dynamic_pointer_cast<Client>(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity);
		ClientRef->SendMessageNE(ClientRef->GetUsername(), CLIENTDISCONNECT);
		NetworkManager::GetInstance()->m_Network.ShutDown();
	}
	UIManager::GetInstance()->m_HUDInstance.ClearPlayersHUD();
	SceneManager::GetInstance()->SwitchScene("MainMenu");
}

void ResumeButton()
{
	UIManager::GetInstance()->m_bDisplayPauseOptions = false;
	UIManager::GetInstance()->SwitchUIMode(false);
}
