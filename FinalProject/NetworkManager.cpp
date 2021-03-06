//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    NetworkManager.cpp
// Description    	:    The main system for the network
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "NetworkManager.h"

// Local Includes //
#include "JoinGameMenu.h"
#include "Client.h"
#include "Server.h"
#include "Player.h"
#include "LevelManager.h"
#include "Level.h"

// Engine Includes //
#include "Engine\Time.h"

// Static Variables //
std::shared_ptr<NetworkManager> NetworkManager::m_pNetworkManager;

NetworkManager::NetworkManager()
{
	fTimeRateInterval = 1 / iTickRate;
}


NetworkManager::~NetworkManager()
{
}

void NetworkManager::JoinServer(ServerInfoProperties _ServerProperties)
{	
	std::shared_ptr<Client> ClientRef = std::dynamic_pointer_cast<Client>(m_Network.m_pNetworkEntity);
	ClientRef->ChangeServer(_ServerProperties._IPAddress);
	TPacket _packet;
	_packet.Serialize(HANDSHAKE, const_cast<char *>(std::dynamic_pointer_cast<Client>(m_Network.m_pNetworkEntity)->GetClientUserName().c_str()));
	ClientRef->SendData(_packet.PacketData);
}

void NetworkManager::Update()
{
	if (fCurrentTime >= fNextTime)
	{
		fNextTime += fTimeRateInterval;
		if (m_Network.m_pNetworkEntity)
		{
			UpdateClientEntities();
			UpdatePlayers();
		}
	}

	fCurrentTime += Time::dTimeDelta;


}

void NetworkManager::DestroyNetworkEntity(std::shared_ptr<Entity> EntityToDestroy)
{
	if (m_Network.IsServer())
	{
		int iDestroyID = -1;
		std::shared_ptr<Server> ServerPointer = std::dynamic_pointer_cast<Server>(m_Network.m_pNetworkEntity);
		for (auto& Ent : m_Network.m_pNetworkEntity->NetworkEntities)
		{
			if (Ent.second == EntityToDestroy)
			{
				iDestroyID = Ent.first;
				break;
			}
		}
		if (iDestroyID == -1) return; // Entity Not found, not network entity
		ServerPointer->DestroyNetworkEntity(iDestroyID);
	}

}

void NetworkManager::UpdateClientEntities()
{
	if (m_Network.IsServer())
	{
		std::shared_ptr<Server> ServerPointer = std::dynamic_pointer_cast<Server>(m_Network.m_pNetworkEntity);
		for (auto& Ent : m_Network.m_pNetworkEntity->NetworkEntities)
		{
			ServerPointer->UpdateNetworkEntity(Ent.second, Ent.first);
		}
	}
}

void NetworkManager::UpdatePlayers()
{
	if (m_Network.IsServer())
	{
		std::shared_ptr<Server> ServerPointer = std::dynamic_pointer_cast<Server>(m_Network.m_pNetworkEntity);
		for (auto& Ent : m_Network.m_pNetworkEntity->PlayerEntities)
		{
			ServerPointer->UpdatePlayer(Ent.second);
		}	
	}
	else
	{
		if (!LevelManager::GetInstance()->GetCurrentActiveLevel()) return;
		std::shared_ptr<Player> PlayerRef = LevelManager::GetInstance()->GetCurrentActiveLevel()->EPlayer;
		if (!PlayerRef) return;
		std::string PlayerUpdate = PlayerRef->m_UserName + " " + NetworkEntity::Vec3ToSendString(PlayerRef->transform.Position) + " " + NetworkEntity::Vec3ToSendString(PlayerRef->transform.Rotation);
		TPacket _packet;
		_packet.Serialize(PLAYERUPDATE, const_cast<char *>(PlayerUpdate.c_str()));
		std::dynamic_pointer_cast<Client>(m_Network.m_pNetworkEntity)->SendData(_packet.PacketData);
	}

}
/************************************************************
#--Description--#:  Retrieves static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		Returns static pointer to self
************************************************************/
std::shared_ptr<NetworkManager> NetworkManager::GetInstance()
{
	if (!m_pNetworkManager) // null or doesn't exist
	{
		m_pNetworkManager = std::shared_ptr<NetworkManager>(new NetworkManager());
	}
	return m_pNetworkManager;
}

/************************************************************
#--Description--#:  Destroys static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void NetworkManager::DestoryInstance()
{
	m_pNetworkManager = nullptr;
}
