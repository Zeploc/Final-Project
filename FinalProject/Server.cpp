//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Server.cpp
// Description    	:    The Server entity for the network
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "Server.h"

// Library Includes //
#include <iostream>
//#include <Windows.h>
//#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */

// Local Includes //
#include "NetworkSystem.h"
#include "utils.h"
#include "Menu.h"

// Engine Includes //
#include "Engine\SceneManager.h"


/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Server::Server()
	:m_pServerSocket(0)
{

}


/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Server::~Server()
{
	if (m_bOnline)
	{
		m_bOnline = false;
		m_ReceiveThread.join();

		delete m_pConnectedClients;
		m_pConnectedClients = 0;

		delete m_pServerSocket;
		m_pServerSocket = 0;

		delete m_pWorkQueue;
		m_pWorkQueue = 0;

		delete[] m_pcPacketData;
		m_pcPacketData = 0;
	}
}

void Server::Initialise()
{
	ServerInfo DefaultServer;
	DefaultServer._ServerName = "Default Server";
	DefaultServer._iPlayers = 2;
	Initialise(DefaultServer);
}

/************************************************************
#--Description--#:  Initialises the class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Server::Initialise(ServerInfo NewServerProperties)
{
	m_bOnline = true;
	m_pcPacketData = new char[MAX_MESSAGE_LENGTH];

	//Create a work queue to distribute messages between the main  thread and the receive thread.
	m_pWorkQueue = new CWorkQueue<std::string>();
	//Create a socket object
	m_pServerSocket = new CSocket();

	//Get the port number to bind the socket to
	unsigned short _usServerPort = DEFAULT_SERVER_PORT;// QueryPortNumber(DEFAULT_SERVER_PORT);

	//Initialise the socket to the local loop back address and port number
	if (!m_pServerSocket->Initialise(_usServerPort))
	{
		return;
	}

	//Qs 2: Create the map to hold details of all connected clients
	m_pConnectedClients = new std::map < std::string, TClientDetails >();
	m_ReceiveThread = std::thread(&Server::ReceiveData, this);
	CurrentServerProperties = NewServerProperties;
}

void Server::ReceiveData()
{
	int iSizeOfAdd = sizeof(m_ClientAddress);
	int _iNumOfBytesReceived;
	int _iPacketSize;

	//Receive data into a local buffer
	char _buffer[MAX_MESSAGE_LENGTH];

	while (m_bOnline)
	{
		 //pull off the packet(s) using recvfrom()
		_iNumOfBytesReceived = recvfrom(			// pulls a packet from a single source...
			m_pServerSocket->GetSocketHandle(),						// client-end socket being used to read from
			_buffer,							// incoming packet to be filled
			MAX_MESSAGE_LENGTH,					   // length of incoming packet to be filled
			0,										// flags
			reinterpret_cast<sockaddr*>(&m_ClientAddress),	// address to be filled with packet source
			&iSizeOfAdd								// size of the above address struct.
		);
		if (_iNumOfBytesReceived < 0)
		{
			int _iError = WSAGetLastError();
			ErrorRoutines::PrintWSAErrorInfo(_iError);
			//return false;
		}
		else
		{
			std::string NewData(_buffer);
			char _IPAddress[100];
			inet_ntop(AF_INET, &m_ClientAddress.sin_addr, _IPAddress, sizeof(_IPAddress));

			std::cout << "Server Received \"" << NewData << "\" from " <<
				_IPAddress << ":" << ntohs(m_ClientAddress.sin_port) << std::endl;
			/*if (int(_pcBufferToReceiveData[0] - '0') == KEEPALIVE)
			{
				KeepAlivePacket NewPacket;
				NewPacket._sMessage = std::string(_pcBufferToReceiveData);
				NewPacket._sClientAddress = ToString(m_ClientAddress);
				m_pKeepAliveWorkQueue->push(NewPacket);
			}
			else
			{*/
				//Push this packet data into the WorkQ
				m_pWorkQueue->push(NewData);
			//}
		}
	}
}

bool Server::SendData(char * _pcDataToSend)
{
	int _iBytesToSend = (int)strlen(_pcDataToSend) + 1;

	int iNumBytes = sendto(
		m_pServerSocket->GetSocketHandle(),				// socket to send through.
		_pcDataToSend,									// data to send
		_iBytesToSend,									// number of bytes to send
		0,												// flags
		reinterpret_cast<sockaddr*>(&m_ClientAddress),	// address to be filled with packet target
		sizeof(m_ClientAddress)							// size of the above address struct.
	);
	if (_iBytesToSend != iNumBytes)
	{
		std::cout << "There was an error in sending data from server to client" << std::endl; //Changed
		return false;
	}
	return true;
}

void Server::ProcessData(std::string _DataReceived)
{
	TPacket _packetRecvd, _packetToSend;
	_packetRecvd = _packetRecvd.Deserialize(_DataReceived);
	switch (_packetRecvd.MessageType)
	{
		case BROADCAST:
		{
			std::cout << "Received a broadcast packet" << std::endl;
			std::string ServerProperties = std::to_string(CurrentServerProperties._iPlayers) + " " + CurrentServerProperties._ServerName;
			//Just send out a packet to the back to the client again which will have the server's IP and port in it's sender fields
			_packetToSend.Serialize(BROADCAST, const_cast<char*>(ServerProperties.c_str()));
			SendData(_packetToSend.PacketData);
			break;
		}
		case HANDSHAKE:
		{
			if (AddClient(_packetRecvd.MessageContent))
			{
				// Get string of all connected Users
				std::string ServerConnectedNames;
				std::string SenderAddress = ToString(m_ClientAddress);
				for (auto it = m_pConnectedClients->begin(); it != m_pConnectedClients->end(); ++it)
				{
					ServerConnectedNames += it->second.m_strName + " ";
				}
				// Welcomes connected client by sending handshake message
				_packetToSend.Serialize(HANDSHAKE, const_cast<char*>(ServerConnectedNames.c_str()));
				SendData(_packetToSend.PacketData);

				std::string _strToSend = std::string(m_cUserName) + " " + m_pServerSocket->GetSocketAddress();
				_packetToSend.Serialize(CLIENTCONNECTED, const_cast<char*>(_strToSend.c_str()));
				SendData(_packetToSend.PacketData);

				// Send client connected for each existing client to current client
				for (auto it = m_pConnectedClients->begin(); it != m_pConnectedClients->end(); ++it)
				{
					std::string _strToSend = it->second.m_strName + " " + it->first;
					_packetToSend.Serialize(CLIENTCONNECTED, const_cast<char*>(_strToSend.c_str()));
					SendData(_packetToSend.PacketData);
				}
								
				// Send the new client to all current clients
				_strToSend = m_pConnectedClients->find(SenderAddress)->second.m_strName + " " + SenderAddress;
				SendToAllClients(_strToSend, CLIENTCONNECTED, SenderAddress);

				std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());
				MenuRef->LobbyScreen.ClientConnected(m_pConnectedClients->find(SenderAddress)->second.m_strName, SenderAddress);
	
			}
			else
			{
				_packetToSend.Serialize(HANDSHAKE, "Invalid");
				SendData(_packetToSend.PacketData);
			}
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
void Server::Update()
{
	if (!m_pWorkQueue->empty())
	{
		std::string temp;
		//Retrieve off a message from the queue and process it
		m_pWorkQueue->pop(temp);
		ProcessData(temp);
	}
}

void Server::SendToAllClients(std::string _pcMessage, EMessageType _MessageType, std::string ExcludeAddress)
{
	const char * NewMessage = _pcMessage.c_str();
	TPacket _packetToSend;
	for (auto it = m_pConnectedClients->begin(); it != m_pConnectedClients->end(); ++it)
	{
		if ((*it).first == ExcludeAddress)
			continue;

		// Sets client address to current client in vector iterator
		m_ClientAddress = it->second.m_ClientAddress;
		// Sends new user message to current client
		_packetToSend.Serialize(_MessageType, const_cast<char*>(NewMessage));
		SendData(_packetToSend.PacketData);
	}
}

bool Server::AddClient(std::string _strClientName)
{
	for (auto it = m_pConnectedClients->begin(); it != m_pConnectedClients->end(); ++it)
	{
		//Check to see that the client to be added does not already exist in the map, 
		if (it->first == ToString(m_ClientAddress))
		{
			return false;
		}
		//also check for the existence of the username
		else if (it->second.m_strName == _strClientName)
		{
			return false;
		}
	}
	//Add the client to the map.
	TClientDetails _clientToAdd;
	_clientToAdd.m_strName = _strClientName;
	_clientToAdd.m_ClientAddress = this->m_ClientAddress;
	//_clientToAdd.m_bIsActive = true;
	//_clientToAdd.m_timeOfLastMessage = 0;

	std::string _strAddress = ToString(m_ClientAddress);
	m_pConnectedClients->insert(std::pair < std::string, TClientDetails >(_strAddress, _clientToAdd));
	return true;
}