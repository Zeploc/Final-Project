//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    JoinGameMenu.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//
// Library Includes //
#include <string>
#include <WinSock2.h>
#include <memory>

// OpenGL Library //
#include <glm\common.hpp>

// Engine Includes //
#include "Engine\Scene.h"
#include "Engine\UIButton.h"
#include "Engine\UITextField.h"

struct ServerInfoProperties
{
	std::string _ServerName;
	int _iPlayers;
	sockaddr_in _IPAddress;
};

#pragma once
class ServerItem
{
public:
	ServerItem(ServerInfoProperties _ServerInfo, glm::vec2 _ItemPosition);
	~ServerItem();

	ServerInfoProperties CurrentServerInfo;

	std::shared_ptr<UIButton> ServerButton;
};


class JoinGameMenu
{
public:
	JoinGameMenu();
	~JoinGameMenu();

	void Init(std::shared_ptr<Scene> _Scene);

	void HideElements();
	void ShowElements();

	void ClearServerList() { v_ServerList.clear(); };
	void AddServers(std::vector<ServerInfoProperties> Servers);

	void JoinCurrentServer();

	std::string GetPlayerName() { return PlayerName->FieldText.sText; };

	std::vector<std::shared_ptr<UIElement>> v_ScreenElements;
private:
	std::shared_ptr<UITextField> PlayerName;
	glm::vec2 ServerListPos;

	std::vector<ServerItem> v_ServerList;

	bool bInitialised = false;
};

