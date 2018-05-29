//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    ServerItem.cpp
// Description    	:    A server item for the join servers screen
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "ServerItem.h"

// Local Includes //
#include "utils.h"

// Engine Includes //
#include "Engine\SceneManager.h"
#include "Engine\Scene.h"

void JoinSerer();

ServerItem::ServerItem(ServerInfo _ServerInfo, glm::vec2 _ItemPosition)
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
}
