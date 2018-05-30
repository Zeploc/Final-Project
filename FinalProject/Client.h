//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Client.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//
#pragma once
// Local Includes //
#include "NetworkEntity.h"
#include "socket.h"
#include "WorkQueue.h"
#include "JoinGameMenu.h"

// Library Includes //
#include <vector>

class Client :	public NetworkEntity
{
public:
	Client();
	~Client();

	void Initialise();
	bool SendData(char* _pcDataToSend);
	void ReceiveData();
	void ProcessData(std::string _DataReceived);
	void Update();

	std::string GetClientUserName() { return std::string(m_cUserName); };
	void SetClientUserName(std::string _NewName) { strncpy_s(m_cUserName, _NewName.c_str(), sizeof(_NewName) - 1); };

	bool BroadcastForServers();
private:
	void ReceiveBroadcastMessages(char* _pcBufferToReceiveData);

private:
	//A buffer to contain all packet data for the client
	//char* m_pcPacketData;
	//A client has a socket object to create the UDP socket at its end.
	CSocket * m_pClientSocket;
	// A Sockaddress structure which will have the details of the server 
	sockaddr_in m_ServerSocketAddress;
	//A username to associate with a client
	char m_cUserName[50];
	//A workQueue to distribute messages between the main thread and Receive thread.
	CWorkQueue<std::string>* m_pWorkQueue;

	//A vector to hold all the servers found after broadcasting
	std::vector<ServerInfo> m_vecServerAddr;
	bool m_bDoBroadcast;
};

