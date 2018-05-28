//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    NetworkSystem.cpp
// Description    	:    The main system for the network
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "NetworkSystem.h"

// Static Variables //
std::shared_ptr<NetworkSystem> NetworkSystem::m_pNetworkSystem;

// Local Includes //
#include "Client.h"
#include "Server.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
NetworkSystem::NetworkSystem()
{
}


/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
NetworkSystem::~NetworkSystem()
{
}

/************************************************************
#--Description--#:  Initialises the class based on entity type
#--Author--#: 		Alex Coultas
#--Parameters--#:	Entity Type for network
#--Return--#: 		NA
************************************************************/
void NetworkSystem::Init(EEntityType EntityType)
{
	m_pcPacketData = new char[MAX_MESSAGE_LENGTH];
	strcpy_s(m_pcPacketData, strlen("") + 1, "");
	m_bOnline = true;
	switch (EntityType)
	{
	case CLIENT:
	{
		m_pNetworkEntity = std::make_shared<Client>(Client());
		m_pNetworkEntity->Initialise();

		std::shared_ptr<Client> NewClientVer = std::dynamic_pointer_cast<Client>(m_pNetworkEntity);
		m_pNetworkEntity->m_ReceiveThread = std::thread(&Client::ReceiveData, NewClientVer, std::ref(m_pcPacketData));
		break;
	}
	case SERVER:
	{
		m_pNetworkEntity = std::make_shared<Server>(Server());
		m_pNetworkEntity->Initialise();

		std::shared_ptr<Server> NewServerVer = std::dynamic_pointer_cast<Server>(m_pNetworkEntity);
		std::thread(&Server::ReceiveData, NewServerVer, std::ref(m_pcPacketData));
		break;
	}
	default:
	{
		//Add some error handling in here
		break;
	}
	}
}

/************************************************************
#--Description--#: 	Updates every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void NetworkSystem::Update()
{
	if (m_bOnline)
	{
		m_pNetworkEntity->Update();
		// Main Network Step
	}
}

/************************************************************
#--Description--#: 	Shuts down the network
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void NetworkSystem::ShutDown()
{
	m_bOnline = false;
}

/************************************************************
#--Description--#:  Retrieves static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		Returns static pointer to self
************************************************************/
std::shared_ptr<NetworkSystem> NetworkSystem::GetInstance()
{
	if (!m_pNetworkSystem) // null or doesn't exist
	{
		m_pNetworkSystem = std::shared_ptr<NetworkSystem>(new NetworkSystem());
	}
	return m_pNetworkSystem;
}

/************************************************************
#--Description--#:  Destroys static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void NetworkSystem::DestoryInstance()
{
	m_pNetworkSystem = nullptr;
}