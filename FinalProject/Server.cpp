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

/************************************************************
#--Description--#:  Initialises the class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Server::Initialise()
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
			//Just send out a packet to the back to the client again which will have the server's IP and port in it's sender fields
			_packetToSend.Serialize(BROADCAST, "0 Server Name!");
			SendData(_packetToSend.PacketData);
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
