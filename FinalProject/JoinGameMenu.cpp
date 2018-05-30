//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    JoinGameMenu.cpp
// Description    	:    Menu system for the joining a server screen
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "JoinGameMenu.h"

// Local Includes //
#include "utils.h"
#include "NetworkManager.h"
#include "Client.h"

// Engine Includes //
#include "Engine\SceneManager.h"
#include "Engine\Scene.h"
#include "Menu.h"

// Local Functions
void JoinSerer();
void MenuScreenBtn();
void SearchForServersBtn();

// Local Variables //
ServerInfo LocalVariable;


JoinGameMenu::JoinGameMenu()
{
	
}


JoinGameMenu::~JoinGameMenu()
{
}

void JoinGameMenu::Init(std::shared_ptr<Scene> _Scene)
{
	// Add Join Elements
	std::shared_ptr<UIText> JoinText(new UIText(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, 100.0f), 0, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), "Join Game:", "Resources/Fonts/Roboto-Bold.ttf", 80, Utils::CENTER));
	JoinText->SetActive(false);
	std::shared_ptr<UIButton> JoinGameBackBtn(new UIButton(glm::vec2(0, Camera::GetInstance()->SCR_HEIGHT), Utils::BOTTOM_LEFT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 480, 80, MenuScreenBtn));
	JoinGameBackBtn->AddText("Back", "Resources/Fonts/Roboto-Thin.ttf", 34, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	JoinGameBackBtn->SetActive(false);
	std::shared_ptr<UIButton> SendBroadcastBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT - 120), Utils::CENTER, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 420, 60, SearchForServersBtn));
	SendBroadcastBtn->AddText("Refresh", "Resources/Fonts/Roboto-Thin.ttf", 34, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	SendBroadcastBtn->SetActive(false);
	PlayerName = std::make_shared<UIText>(UIText({ 100.0f, 500.0f }, 0, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), "Player", "Resources/Fonts/Roboto-Bold.ttf", 80, Utils::CENTER_LEFT));
	PlayerName->SetActive(false);

	// Add elements to scene UI elements
	_Scene->AddUITextElement(JoinText);
	_Scene->AddUIElement(JoinGameBackBtn);
	_Scene->AddUIElement(SendBroadcastBtn);
	_Scene->AddUITextElement(PlayerName);

	// Add elements to vector list
	v_ScreenElements.push_back(JoinText);
	v_ScreenElements.push_back(JoinGameBackBtn);
	v_ScreenElements.push_back(SendBroadcastBtn);
	v_ScreenElements.push_back(PlayerName);

	ServerListPos = { Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT - 400 };
}

void JoinGameMenu::HideElements()
{
	for (auto it : v_ScreenElements)
		it->SetActive(false);
}

void JoinGameMenu::ShowElements()
{
	for (auto it : v_ScreenElements)
		it->SetActive(true);
}

void JoinGameMenu::AddServers(std::vector<ServerInfo> Servers)
{
	for (int i = 0; i < Servers.size(); i++)
	{
		glm::vec2 NewPos = ServerListPos;
		NewPos.y += i * 100;
		ServerItem NewServerItem(Servers[i], NewPos);
		v_ServerList.push_back(NewServerItem);
		v_ScreenElements.push_back(NewServerItem.ServerButton);
	}
}

ServerItem::ServerItem(ServerInfo _ServerInfo, glm::vec2 _ItemPosition)
{
	CurrentServerInfo = _ServerInfo;
	LocalVariable = _ServerInfo;
	ServerButton = std::make_shared<UIButton>(UIButton(_ItemPosition, Utils::TOP_CENTER, 0, { 0.3f, 0.3f, 0.3f, 1.0f }, { 0.7f, 0.7f, 0.7f, 1.0f }, 1000, 60, JoinSerer));
	std::string ServerText = "SERVER: \"" + CurrentServerInfo._ServerName + "\" with " + std::to_string(CurrentServerInfo._iPlayers) + " Players                     IP: " + ToString(CurrentServerInfo._IPAddress);
	ServerButton->AddText(ServerText, "Resources/Fonts/Roboto-Light.ttf", 30, { 1.0f, 1.0f, 1.0f, 1.0f }, Utils::CENTER);
	SceneManager::GetInstance()->GetCurrentScene()->AddUIElement(ServerButton);
}

ServerItem::~ServerItem()
{
}

void JoinSerer()
{
	std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());
	std::dynamic_pointer_cast<Client>(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)->SetClientUserName(MenuRef->JoinGameScreen.GetPlayerName());
	NetworkManager::GetInstance()->JoinServer(LocalVariable);
	MenuRef->LobbyScreen.SetPlayerNameText(MenuRef->JoinGameScreen.GetPlayerName());
}

void SearchForServersBtn()
{
	std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());
	MenuRef->JoinGameScreen.ClearServerList();
	std::dynamic_pointer_cast<Client>(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)->BroadcastForServers();
}