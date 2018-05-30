//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    NetworkManager.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <memory>

// Local Includes //
#include "NetworkSystem.h"



#pragma once
class NetworkManager
{
public:
	NetworkSystem m_Network;

	void JoinServer();


	// Singleton
public:
	static std::shared_ptr<NetworkManager> GetInstance();
	static void DestoryInstance();
	~NetworkManager(); // Shared pointer has to be able to call destructor so can't be private

private:
	static std::shared_ptr<NetworkManager> m_pNetworkManager;
	NetworkManager();
	NetworkManager(NetworkManager const&);              // Don't Implement
	void operator=(NetworkManager const&); // Don't implement
};

