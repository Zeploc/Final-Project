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


// Static Variables //
std::shared_ptr<NetworkManager> NetworkManager::m_pNetworkManager;

NetworkManager::NetworkManager()
{
}


NetworkManager::~NetworkManager()
{
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
