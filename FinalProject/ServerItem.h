//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    ServerItem.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <string>
#include <WinSock2.h>

// OpenGL Library //
#include <glm\common.hpp>

// Engine Includes //
#include "Engine\UIButton.h"

struct ServerInfo
{
	std::string _ServerName;
	int _iPlayers;
	sockaddr_in _IPAddress;
};

#pragma once
class ServerItem
{
public:
	ServerItem(ServerInfo _ServerInfo, glm::vec2 _ItemPosition);
	~ServerItem();
	
	ServerInfo CurrentServerInfo;

	std::shared_ptr<UIButton> ServerButton;
};

