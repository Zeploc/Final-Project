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
#include "UIManager.h"
#include "LevelManager.h"
#include "Level.h"
#include "Bullet.h"

// Engine Includes //
#include "Engine\SceneManager.h"
#include "Engine\Entity.h"
#include "Engine\Cube.h"

// OpenGL Include //
#include <glm\gtx\rotate_vector.hpp>


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
	ServerInfoProperties DefaultServer;
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
void Server::Initialise(ServerInfoProperties NewServerProperties)
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
	/*int _iPacketSize;*/

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
				std::string SenderAddress = ToString(m_ClientAddress);

				// Welcomes connected client by sending handshake message
				_packetToSend.Serialize(HANDSHAKE, const_cast<char*>(CurrentServerProperties._ServerName.c_str()));
				SendData(_packetToSend.PacketData);
								
				// Tell sender Servers name and ip
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
				
				// Add new client to server's lobby
				ServerPlayerRespondToMessage(_packetRecvd.MessageContent, CLIENTCONNECTED, SenderAddress);
			}
			else
			{
				_packetToSend.Serialize(HANDSHAKE, "Invalid");
				SendData(_packetToSend.PacketData);
			}
			break;
		}
		case CHAT:
		{
			std::string SenderAddress = ToString(m_ClientAddress);
			std::cout << _packetRecvd.MessageContent << std::endl;
			SendToAllClients(_packetRecvd.MessageContent, CHAT);
			ServerPlayerRespondToMessage(_packetRecvd.MessageContent, CHAT, SenderAddress);
			break;
		}
		case PLAYERUPDATE:
		{
			std::string Result = _packetRecvd.MessageContent;

			std::stringstream ss(Result);
			std::string Username;
			float PosX, PosY, PosZ, RotX, RotY, RotZ;

			ss >> Username >> PosX >> PosY >> PosZ >> RotX >> RotY >> RotZ;

			glm::vec3 position = { PosX, PosY, PosZ };
			glm::vec3 rotation = { RotX, RotY, RotZ };

			PlayerEntities[Username]->transform.Position = position;
			PlayerEntities[Username]->transform.Rotation = rotation;

			break;
		}
		case CREATEBULLET:
		{
			std::string Result = _packetRecvd.MessageContent;
			std::stringstream ss(Result);
			std::string Username;
			float PosX, PosY, PosZ, RotX, RotY, RotZ;

			ss >> Username >> PosX >> PosY >> PosZ >> RotX >> RotY >> RotZ;

			glm::vec3 position = { PosX, PosY, PosZ };
			glm::vec3 rotation = { RotX, RotY, RotZ };

			glm::vec3 Direction = glm::rotateY(glm::vec3( 0, 0, 1 ), glm::radians(RotY));

			std::shared_ptr<PlayerBullet> NewBullet = std::make_shared<PlayerBullet>(PlayerBullet({ position, rotation, glm::vec3(0.1f, 0.1f, 0.1f) }, Utils::CENTER, Direction));
			std::shared_ptr<Cube> BulletCube = std::make_shared<Cube>(Cube(1, 1, 1, { 1,0,0,1 }));
			BulletCube->AddCollisionBounds(0.3f, 10.0f, 0.3f, NewBullet);
			NewBullet->AddMesh(BulletCube);
			NewBullet->UserOwner = Username;
			
			Result += " " + std::to_string(CreateNetworkEntity(NewBullet));
			SendToAllClients(Result, CREATEBULLET);
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
	while (!m_pWorkQueue->empty())
	{
		std::string temp;
		//Retrieve off a message from the queue and process it
		m_pWorkQueue->pop(temp);
		ProcessData(temp);
	}
}

void Server::ServerSendToAllPlayers(std::string _pcMessage, EMessageType _Message)
{
	std::string MessageToSend = _pcMessage;
	if (_Message == CHAT)
	{
		std::string UserAndMessage(m_cUserName);
		UserAndMessage += " " + _pcMessage;
		MessageToSend = UserAndMessage;
	}
	//std::string SenderAddress = ToString(m_ClientAddress);
	SendToAllClients(MessageToSend, CHAT);
	ServerPlayerRespondToMessage(MessageToSend, CHAT, "");
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

void Server::UpdateNetworkEntity(std::shared_ptr<Entity> NewEntity, int iNetworkID)
{
	SendToAllClients(GetNetworkEntityString(NewEntity, true, iNetworkID), ENTITYUPDATE);
}
void Server::UpdatePlayer(std::shared_ptr<Player> PlayerEnt)
{
	std::string PlayerInfoMessage = PlayerEnt->m_UserName + " " + std::to_string(PlayerEnt->m_fHealth) + " " + std::to_string(PlayerEnt->GetScore()) + " " + Vec3ToSendString(PlayerEnt->transform.Position) + " " + Vec3ToSendString(PlayerEnt->transform.Rotation);

	std::string PlayerAddress;
	for (auto& itClient : *m_pConnectedClients)
	{
		if (itClient.second.m_strName == PlayerEnt->m_UserName)
			PlayerAddress = itClient.first;
	}
	SendToAllClients(PlayerInfoMessage, PLAYERUPDATE, PlayerAddress);
}


void Server::DestroyNetworkEntity(int iNetworkID)
{
	SendToAllClients(std::to_string(iNetworkID), DESTROYENTITY);
	NetworkEntities.erase(iNetworkID);
}

void Server::CreatePlayers()
{
	for (auto& Client : *m_pConnectedClients)
	{
		CreateNetworkPlayer(Client.second.m_strName);
		CreatePlayerOnClients(Client.second.m_strName);
	}
	CreateNetworkPlayer(m_cUserName);
	CreatePlayerOnClients(m_cUserName);

	// Create and pass Server's player
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
	// Check if name is same as server
	if (_strClientName == m_cUserName)
	{
		return false;
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

void Server::CreatePlayerOnClients(std::string PlayerName)
{
	std::string PlayerMessage = PlayerName;
	SendToAllClients(PlayerMessage, CREATEPLAYER);
}

void Server::ServerPlayerRespondToMessage(std::string _pcMessage, EMessageType _Message, std::string SenderAddress)
{
	switch (_Message)
	{
	case HANDSHAKE:
		break;
	case DATA:
		break;
	case KEEPALIVE:
		break;
	case BROADCAST:
		break;
	case CHAT:
	{
		std::string Result = _pcMessage;
		std::string Username;
		std::string Message;

		for (unsigned int i = 0; i < Result.size(); i++)
		{
			if (Result[i] != ' ')
			{
				Username += Result[i];
				continue;
			}
			Message = Result.substr(i + 1);
			break;
		}
		UIManager::GetInstance()->m_ChatInstance.AddChatMessage({ Username, Message });
		std::cout << "Chat Recieved: [" + Username + "]: " + Message << std::endl;
	}
		break;
	case CLIENTCONNECTED:
	{
		std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());
		MenuRef->LobbyScreen.ClientConnected(m_pConnectedClients->find(SenderAddress)->second.m_strName, SenderAddress);
	}
	break;
	case LOADLEVEL:
	{
		TPacket _packetToSend;

		std::shared_ptr<Entity> NewEntity = std::make_shared<Entity>(Entity({ { 12.56f, -1.6f, 6.47f },{ 0.0f, 90.0f, 0.0f },{ 1.0f, 1.0f, 1.0f } }, Utils::CENTER));
		std::shared_ptr<Cube> CubeMesh = std::make_shared<Cube>(Cube(1, 1, 1, { 0.0f, 1.0f, 0.0f, 1.0f }));
		NewEntity->AddMesh(CubeMesh);
		SceneManager::GetInstance()->GetCurrentScene()->AddEntity(NewEntity);
		LevelManager::GetInstance()->GetCurrentActiveLevel()->NetworkEntity = NewEntity;		
		SendToAllClients(GetNetworkEntityString(NewEntity, false), CREATEENTITY);
		CreatePlayers();
		break;
	}
	case CREATEBULLET:
	{
		std::string Result = _pcMessage;
		std::stringstream ss(Result);
		std::string Username;
		float PosX, PosY, PosZ, RotX, RotY, RotZ;

		ss >> Username >> PosX >> PosY >> PosZ >> RotX >> RotY >> RotZ;

		glm::vec3 position = { PosX, PosY, PosZ };
		glm::vec3 rotation = { RotX, RotY, RotZ };

		glm::vec3 Direction = glm::rotateY(glm::vec3(0, 0, 1), glm::radians(RotY));

		std::shared_ptr<PlayerBullet> NewBullet = std::make_shared<PlayerBullet>(PlayerBullet({ position, rotation, glm::vec3(0.1f, 0.1f, 0.1f) }, Utils::CENTER, Direction));
		std::shared_ptr<Cube> BulletCube = std::make_shared<Cube>(Cube(1, 1, 1, { 1,0,0,1 }));
		BulletCube->AddCollisionBounds(0.3f, 10.0f, 0.3f, NewBullet);
		NewBullet->AddMesh(BulletCube);
		NewBullet->UserOwner = Username;

		Result += " " + std::to_string(CreateNetworkEntity(NewBullet));
		SendToAllClients(Result, CREATEBULLET);
		break;
	}
	default:
		break;
	}
}
