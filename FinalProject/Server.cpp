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
}

/************************************************************
#--Description--#:  Initialises the class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Server::Initialise()
{
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
	//m_pConnectedClients = new std::map < std::string, TClientDetails >();
	m_ReceiveThread = std::thread(&Server::ReceiveData, this, std::ref(m_pcPacketData));

}

void Server::ReceiveData(char * _pcBufferToReceiveData)
{
	int iSizeOfAdd = sizeof(m_ClientAddress);
	int _iNumOfBytesReceived;
	int _iPacketSize;

	//Receive data into a local buffer
	char _buffer[MAX_MESSAGE_LENGTH];

	while (true)
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
			//ErrorRoutines::PrintWSAErrorInfo(_iError);
			//return false;
		}
		else
		{
			_iPacketSize = static_cast<int>(strlen(_buffer)) + 1;
			strcpy_s(_pcBufferToReceiveData, _iPacketSize, _buffer);
			char _IPAddress[100];
			//inet_ntop(AF_INET, &m_ClientAddress.sin_addr, _IPAddress, sizeof(_IPAddress));

			std::cout << "Server Received \"" << _pcBufferToReceiveData << "\" from " <<
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
				//m_pWorkQueue->push(_pcBufferToReceiveData);
			//}
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

}
