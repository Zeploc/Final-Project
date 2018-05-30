//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    NetworkSystem.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <memory>

// Local Includes //
#include "NetworkEntity.h"
#include "JoinGameMenu.h"

//Types
enum EEntityType
{
	CLIENT = 1,
	SERVER
};

namespace ErrorRoutines
{
	void PrintWSAErrorInfo(int iError);
}

//constants
namespace
{
	unsigned const DEFAULT_SERVER_PORT = 50030;
	unsigned const DEFAULT_CLIENT_PORT = 60013;
	unsigned const MAX_MESSAGE_LENGTH = 256;
	unsigned const MAX_ADDRESS_LENGTH = 32;
	unsigned const MAX_PORT_LENGTH = 16;
	unsigned const MAX_USERNAME_LENGTH = 50;
}

#pragma once
class NetworkSystem
{
public:
	NetworkSystem();
	~NetworkSystem();

	void InitClient();
	void InitServer(ServerInfo NewServerProperties);
	void Update();
	void ShutDown();

	std::shared_ptr<NetworkEntity> m_pNetworkEntity;

private:
	bool m_bOnline;
	char* m_pcPacketData = 0; //A local buffer to receive packet data info
	
};

