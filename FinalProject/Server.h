//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Server.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//
#pragma once
// Local Includes //
#include "NetworkEntity.h"
#include "socket.h"
#include "WorkQueue.h"
#include "ServerItem.h"

// Library Includes
#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <Windows.h>
#include <WS2tcpip.h>
#include <map>

//Structure to hold the details of all connected clients
struct TClientDetails
{
	sockaddr_in m_ClientAddress;
	//bool m_bIsActive;
	std::string m_strName;
	//time_t m_timeOfLastMessage;
};

class Server : public NetworkEntity
{
public:
	Server();
	~Server();

	void Initialise();
	void Initialise(ServerInfo NewServerProperties);
	void ReceiveData();
	bool SendData(char* _pcDataToSend);
	void ProcessData(std::string _DataReceived);
	void Update();

	std::string CurrentServerAddress() { return m_pServerSocket->GetSocketAddress(); };
	void SetServerUserName(std::string _NewName) { strncpy_s(m_cUserName, _NewName.c_str(), sizeof(_NewName) - 1); };

	void SendToAllClients(std::string _pcMessage, EMessageType _Message, std::string ExcludeAddress);

private:
	bool AddClient(std::string _strClientName);

private:
	//A Buffer to contain all packet data for the server
	char* m_pcPacketData;
	//A server has a socket object to create the UDP socket at its end.
	CSocket* m_pServerSocket;
	// Make a member variable to extract the IP and port number of the sender from whom we are receiving
	//Since it is a UDP socket capable of receiving from multiple clients; these details will change depending on who has sent the packet we are currently processing.
	sockaddr_in m_ClientAddress;
	//A username to associate with the server
	char m_cUserName[50];

	ServerInfo CurrentServerProperties;

	//The structure maps client addresses to client details
	std::map<std::string, TClientDetails>* m_pConnectedClients;

	//A workQueue to distribute messages between the main thread and Receive thread.
	CWorkQueue<std::string>* m_pWorkQueue;
};

