//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    HostGameMenu.cpp
// Description    	:    Screen system for hosting a game
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "HostGameMenu.h"

// Local Includes //
#include "Menu.h"
#include "NetworkManager.h"
#include "Server.h"
#include "UIManager.h"

// Engine Includes //
#include "Engine\SceneManager.h"

// Local Functions //
void MenuScreenBtn();
void StartHostBtn();


HostGameMenu::HostGameMenu()
{
}


HostGameMenu::~HostGameMenu()
{
}

void HostGameMenu::Init(std::shared_ptr<Scene> _Scene)
{
	// Add Host Elements
	std::shared_ptr<UIText> HostText(new UIText(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, 100.0f), 0, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), "Host Game:", "Resources/Fonts/Roboto-Bold.ttf", 80, Utils::CENTER));
	HostText->SetActive(false);
	std::shared_ptr<UIButton> HostGameBackBtn(new UIButton(glm::vec2(0, Camera::GetInstance()->SCR_HEIGHT), Utils::BOTTOM_LEFT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 480, 80, MenuScreenBtn));
	HostGameBackBtn->AddText("Back", "Resources/Fonts/Roboto-Thin.ttf", 34, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	HostGameBackBtn->SetActive(false);
	PlayersCountSlider = std::make_shared<UISlider>(UISlider({ Camera::GetInstance()->SCR_WIDTH / 2, 300.0f }, 0, { 0.2f, 0.2f, 0.2f, 1.0f }, { 0.6f, 0.6f, 0.6f, 1.0f }, 300, 20, 40, 10, Utils::CENTER, "Players Count:"));
	PlayersCountSlider->SetMinimumPosition(2.0f);
	PlayersCountSlider->SetMaximumPosition(4.0f);
	PlayersCountSlider->SetLockSize(1.0f);
	PlayersCountSlider->SetStartPosition(2.0f);
	PlayersCountSlider->SetActive(false);
	std::shared_ptr<UIButton> StartHostBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT - 120), Utils::CENTER, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 420, 60, StartHostBtn));
	StartHostBtn->AddText("Start Host", "Resources/Fonts/Roboto-Thin.ttf", 34, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	StartHostBtn->SetActive(false);
	ServerName = std::make_shared<UIText>(UIText(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, 500.0f), 0, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), "Server", "Resources/Fonts/Roboto-Bold.ttf", 80, Utils::CENTER));
	ServerName->SetActive(false);
	PlayerName = std::make_shared<UITextField>(UITextField({ 100.0f, 200.0f }, Utils::CENTER_LEFT, 0, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), 400, 50, { 0.8f, 0.8f, 0.8f, 1.0f }, "PlayerName", "Resources/Fonts/Roboto-Bold.ttf", 40, Utils::CENTER_LEFT));
	PlayerName->SetActive(false);

	// Add elements to scene UI elements
	_Scene->AddUITextElement(HostText);
	_Scene->AddUIElement(HostGameBackBtn);
	_Scene->AddUIElement(PlayersCountSlider);
	_Scene->AddUIElement(StartHostBtn);
	_Scene->AddUITextElement(ServerName);
	_Scene->AddUIElement(PlayerName);

	// Add elements to vector list
	v_ScreenElements.push_back(HostText);
	v_ScreenElements.push_back(HostGameBackBtn);
	v_ScreenElements.push_back(PlayersCountSlider);
	v_ScreenElements.push_back(StartHostBtn);
	v_ScreenElements.push_back(ServerName);
	v_ScreenElements.push_back(PlayerName);

}

void HostGameMenu::HideElements()
{
	for (auto it : v_ScreenElements)
		it->SetActive(false);
}

void HostGameMenu::ShowElements()
{
	for (auto it : v_ScreenElements)
		it->SetActive(true);

	UIManager::GetInstance()->m_bDisplayChat = false;
}

///************************************************************
//#--Description--#:  Local instructions game button
//#--Author--#: 		Alex Coultas
//#--Parameters--#:	NA
//#--Return--#: 		NA
//************************************************************/
//void MenuScreenBtn()
//{
//	std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());
//	MenuRef->ToggleMenuSection(MAIN);
//}

void StartHostBtn()
{
	std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());
	ServerInfo NewProperties;
	MenuRef->HostGameScreen.ShowElements();
	//MenuRef->HostGameScreen.SetValues();
	NewProperties._iPlayers = MenuRef->HostGameScreen.GetCurrentPlayersCounter();// (int)MenuRef->PlayersCountSlider->GetValue();
	NewProperties._ServerName = MenuRef->HostGameScreen.GetServerName();// ->ServerName->sText;
	NetworkManager::GetInstance()->m_Network.InitServer(NewProperties);
	std::shared_ptr<Server> ServerRef = std::dynamic_pointer_cast<Server>(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity);
	ServerRef->SetServerUserName(MenuRef->HostGameScreen.GetPlayerName());
	//MenuRef->PlayerName->sText = MenuRef->ServerPlayerName->sText;
	MenuRef->LobbyScreen.ClientConnected(MenuRef->HostGameScreen.GetPlayerName(), ServerRef->CurrentServerAddress());
	MenuRef->LobbyScreen.SetPlayerNameText(MenuRef->HostGameScreen.GetPlayerName());
	MenuRef->ToggleMenuSection(LOBBY);
}