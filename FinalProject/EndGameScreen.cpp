//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    EndGameScreen.cpp
// Description    	:    The end game ui screen
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "EndGameScreen.h"

// Engine Includes //
#include "Engine\Camera.h"
#include "Engine\SceneManager.h"

// Local Includes //
#include "Level.h"
#include "LevelManager.h"
#include "GameManager.h"
#include "NetworkManager.h"
#include "Server.h"
#include "UIManager.h"
#include "Client.h"

void BackToMenu();
void RestartGame();

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
EndScreen::EndScreen()
{
	m_pBackImage = std::make_shared<UIImage>(UIImage({ Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 }, Utils::CENTER, 0, { 0.5f, 0.5f, 0.5f, 1.0f }, 1000, 500));
	m_pMessage = std::make_shared<UIText>(UIText({ Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 - 120 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "You lost", "Resources/Fonts/Roboto-Black.ttf", 45, Utils::CENTER));

	m_pScore = std::make_shared<UIText>(UIText({ Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2}, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "You scored: ", "Resources/Fonts/Roboto-Thin.ttf", 45, Utils::CENTER));
	m_pQuit = std::make_shared<UIButton>(UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2 + 200, Camera::GetInstance()->SCR_HEIGHT / 2 + 150), Utils::CENTER, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 300, 60, BackToMenu));
	m_pQuit->AddText("Back to Menu", "Resources/Fonts/Roboto-Thin.ttf", 30, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	m_pRestart = std::make_shared<UIButton>(UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2 - 200, Camera::GetInstance()->SCR_HEIGHT / 2 + 150), Utils::CENTER, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 300, 60, RestartGame));
	m_pRestart->AddText("Replay", "Resources/Fonts/Roboto-Thin.ttf", 30, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
EndScreen::~EndScreen()
{
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void EndScreen::Update()
{
	m_pBackImage->Update();
	m_pMessage->Update();
	m_pScore->Update();
	m_pQuit->Update();
	m_pRestart->Update();
}

/************************************************************
#--Description--#:	Render Scene to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void EndScreen::Render()
{
	m_pBackImage->DrawUIElement();
	m_pMessage->DrawUIElement();
	m_pScore->DrawUIElement();
	m_pQuit->DrawUIElement();
	m_pRestart->DrawUIElement();
}

void EndScreen::SetScore(int iScore)
{
	m_pScore->sText = "Total score: " + std::to_string(iScore);
}

void EndScreen::SetEndText(std::string _EndText)
{
	m_pMessage->sText = _EndText;
}

void BackToMenu()
{
	if (NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)
	{
		if (NetworkManager::GetInstance()->m_Network.IsServer())
		{
			std::shared_ptr<Server> ServerRef = std::dynamic_pointer_cast<Server>(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity);
			ServerRef->CloseServer();
		}
		else
		{
			std::shared_ptr<Client> ClientRef = std::dynamic_pointer_cast<Client>(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity);
			ClientRef->SendMessageNE(ClientRef->GetUsername(), CLIENTDISCONNECT);
		}
		NetworkManager::GetInstance()->m_Network.ShutDown();
		UIManager::GetInstance()->m_HUDInstance.ClearPlayersHUD();
		std::shared_ptr<Level> CurrentLevel = std::dynamic_pointer_cast<Level>(LevelManager::GetInstance()->GetCurrentActiveLevel());
		CurrentLevel->DestroyAllEnemies();
	}	
	GameManager::GetInstance()->HideEndScreen();
	SceneManager::GetInstance()->SwitchScene("MainMenu");
}

void RestartGame()
{
	if (NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)
	{
		if (!NetworkManager::GetInstance()->m_Network.IsServer()) return;
		std::string LevelName = LevelManager::GetInstance()->GetCurrentLevelName();
		std::shared_ptr<Server> ServerPointer = std::dynamic_pointer_cast<Server>(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity);
		if (ServerPointer) ServerPointer->SendToAllClients(LevelName, LOADLEVEL); // Tell All Clients to Load Level
		LevelManager::GetInstance()->SwitchToFirstLevel();
		if (ServerPointer) ServerPointer->ServerPlayerRespondToMessage(LevelName, LOADLEVEL, "");
		return;
	}
	LevelManager::GetInstance()->SwitchToFirstLevel();
	//LevelManager::GetInstance()->SwitchToCurrentLevel();
	//LevelManager::GetInstance()->GetCurrentActiveLevel()->RestartLevel();
}
