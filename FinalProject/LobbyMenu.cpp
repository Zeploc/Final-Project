//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    LobbyMenu.cpp
// Description    	:    Menu system for Lobby Menu
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "LobbyMenu.h"

// Engine Includes //
#include "Engine\SceneManager.h"
#include "Engine\UIButton.h"

void StartServer();

LobbyMenu::LobbyMenu()
{
}


LobbyMenu::~LobbyMenu()
{
}

void LobbyMenu::Init(std::shared_ptr<Scene> _Scene)
{
	// Add Lobby Elements
	std::shared_ptr<UIText> LobbyText(new UIText(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, 100.0f), 0, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), "Game Lobby:", "Resources/Fonts/Roboto-Bold.ttf", 80, Utils::CENTER));
	LobbyText->SetActive(false);
	PlayerName = std::make_shared<UIText>(UIText({ 100.0f, 500.0f }, 0, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), "Player", "Resources/Fonts/Roboto-Bold.ttf", 80, Utils::CENTER_LEFT));
	PlayerName->SetActive(false);
	std::shared_ptr<UIButton> StartServerBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH - 80, Camera::GetInstance()->SCR_HEIGHT - 50), Utils::BOTTOM_RIGHT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 480, 70, StartServer));
	StartServerBtn->AddText("Start Server", "Resources/Fonts/Roboto-Thin.ttf", 34, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	StartServerBtn->SetActive(false);

	// Add elements to scene UI elements
	_Scene->AddUITextElement(LobbyText);
	_Scene->AddUITextElement(PlayerName);
	_Scene->AddUIElement(StartServerBtn);

	// Add elements to vector list
	v_ScreenElements.push_back(LobbyText);
	v_ScreenElements.push_back(PlayerName);
	v_ScreenElements.push_back(StartServerBtn);
}

void LobbyMenu::HideElements()
{
	for (auto it : v_ScreenElements)
		it->SetActive(false);
}

void LobbyMenu::ShowElements()
{
	for (auto it : v_ScreenElements)
		it->SetActive(true);
}

void LobbyMenu::ClientConnected(std::string _UserName, std::string Address)
{
	float fYPos = v_PlayersConnected.size() * 50 + 300;
	std::shared_ptr<UIText> NewClient(new UIText(glm::vec2(100, fYPos), 0, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), _UserName + "      " + Address, "Resources/Fonts/Roboto-Bold.ttf", 30, Utils::CENTER_LEFT));
	SceneManager::GetInstance()->GetCurrentScene()->AddUITextElement(NewClient);
	v_ScreenElements.push_back(NewClient);
	v_PlayersConnected.push_back(NewClient);
}

void StartServer()
{

}
