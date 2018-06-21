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
#include "Menu.h"
#include "UIManager.h"

// Engine Includes //
#include "Engine\SceneManager.h"
#include "Engine\Scene.h"

// Local Functions
void JoinSerer();
void MenuScreenBtn();
void SearchForServersBtn();

// Local Variables //

JoinGameMenu::JoinGameMenu()
{
	
}


JoinGameMenu::~JoinGameMenu()
{
	//LocalVariable._ServerName = "";
}

void JoinGameMenu::Init(std::shared_ptr<Scene> _Scene)
{
	if (bInitialised)
	{
		HideElements();
		return;
	}
	// Add Join Elements
	std::shared_ptr<UIText> JoinText(new UIText(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, 100.0f), 0, glm::vec4(0.9, 0.9, 0.9, 1.0f), "Join Game:", "Resources/Fonts/Roboto-Bold.ttf", 80, Utils::CENTER));
	JoinText->SetActive(false);
	std::shared_ptr<UIButton> JoinGameBackBtn(new UIButton(glm::vec2(0, Camera::GetInstance()->SCR_HEIGHT), Utils::BOTTOM_LEFT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 480, 80, MenuScreenBtn));
	JoinGameBackBtn->AddText("Back", "Resources/Fonts/Roboto-Thin.ttf", 34, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	JoinGameBackBtn->SetActive(false);
	std::shared_ptr<UIButton> SendBroadcastBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT - 120), Utils::CENTER, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 420, 60, SearchForServersBtn));
	SendBroadcastBtn->AddText("Refresh", "Resources/Fonts/Roboto-Thin.ttf", 34, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	SendBroadcastBtn->SetActive(false);
	PlayerName = std::make_shared<UITextField>(UITextField({ 100.0f, 200.0f }, Utils::CENTER_LEFT, 0, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), 400, 50, { 0.8f, 0.8f, 0.8f, 1.0f }, "PlayerName", "Resources/Fonts/Roboto-Bold.ttf", 40, Utils::CENTER_LEFT));
	PlayerName->SetActive(false);

	// Add elements to scene UI elements
	_Scene->AddUITextElement(JoinText);
	_Scene->AddUIElement(JoinGameBackBtn);
	_Scene->AddUIElement(SendBroadcastBtn);
	_Scene->AddUIElement(PlayerName);

	// Add elements to vector list
	v_ScreenElements.push_back(JoinText);
	v_ScreenElements.push_back(JoinGameBackBtn);
	v_ScreenElements.push_back(SendBroadcastBtn);
	v_ScreenElements.push_back(PlayerName);

	ServerListPos = { Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT - 400 };
	bInitialised = true;
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

	UIManager::GetInstance()->m_bDisplayChat = false;
}

void JoinGameMenu::AddServers(std::vector<ServerInfoProperties> Servers)
{
	if (Servers.size() == 0)
	{
		UIManager::GetInstance()->ShowMessageBox("No Servers Found!");
		return;
	}
	for (unsigned int i = 0; i < Servers.size(); i++)
	{
		glm::vec2 NewPos = ServerListPos;
		NewPos.y += i * 100;
		ServerItem NewServerItem(Servers[i], NewPos);
		v_ServerList.push_back(NewServerItem);
		v_ScreenElements.push_back(NewServerItem.ServerButton);
	}
}

void JoinGameMenu::JoinCurrentServer()
{
	std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());
	MenuRef->LobbyScreen.SetPlayerNameText(GetPlayerName());
	std::dynamic_pointer_cast<Client>(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)->SetClientUserName(GetPlayerName());
	for (auto it : v_ScreenElements)
	{
		std::shared_ptr<UIButton> ButtonCast = std::dynamic_pointer_cast<UIButton>(it);
		if (!ButtonCast) continue;
		if (ButtonCast->bPressed)
		{
			for (auto Serv : v_ServerList)
			{
				if (ButtonCast == Serv.ServerButton)
				{
					NetworkManager::GetInstance()->JoinServer(Serv.CurrentServerInfo);
					break;
				}
			}
			break;
		}
	}
}

ServerItem::ServerItem(ServerInfoProperties _ServerInfo, glm::vec2 _ItemPosition)
{
	CurrentServerInfo = _ServerInfo;
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
	MenuRef->JoinGameScreen.JoinCurrentServer();
}

void SearchForServersBtn()
{
	std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());
	MenuRef->JoinGameScreen.ClearServerList();
	std::dynamic_pointer_cast<Client>(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)->FullSearchForServers();
}