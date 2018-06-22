//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Client.cpp
// Description    	:    The Client entity for the network
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "Client.h"

// Engine Includes //
#include "Engine\SceneManager.h"
#include "Engine\Cube.h"

// Library Includes //
#include <Windows.h>
#include <iostream>

// Local Includes //
#include "NetworkSystem.h"
#include "utils.h"
#include "UIManager.h"
#include "Bullet.h"
#include "LevelManager.h"
#include "Menu.h"
#include "NetworkManager.h"

// OpenGL Include //
#include <glm\gtx\rotate_vector.hpp>

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Client::Client()
{
}


/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Client::~Client()
{
	if (m_bOnline)
	{
		m_bOnline = false;
		m_ReceiveThread.join();
		/*delete[] m_pcPacketData;
		m_pcPacketData = 0;*/

		delete m_pClientSocket;
		m_pClientSocket = 0;

		delete m_pWorkQueue;
		m_pWorkQueue = 0;
	}
}

/************************************************************
#--Description--#:  Initialises the class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Client::Initialise()
{
	//Local Variables to hold Server's IP address and Port NUmber as entered by the user
	char _cServerIPAddress[MAX_ADDRESS_LENGTH];
	ZeroMemory(&_cServerIPAddress, MAX_ADDRESS_LENGTH);
	char _cServerPort[MAX_PORT_LENGTH];
	ZeroMemory(&_cServerPort, MAX_PORT_LENGTH);
	//unsigned short _usServerPort;

	//Local variable to hold the index of the server chosen to connect to
	char _cServerChosen[5];
	ZeroMemory(_cServerChosen, 5);
	/*unsigned int _uiServerIndex;*/

	//Local variable to hold client's name
	/*char _cUserName[MAX_USERNAME_LENGTH];*/
	ZeroMemory(&m_cUserName, MAX_USERNAME_LENGTH);

	//Zero out the memory for all the member variables.
	ZeroMemory(&m_cUserName, strlen(m_cUserName));

	//Create a work queue to distribute messages between the main  thread and the receive thread.
	m_pWorkQueue = new CWorkQueue<std::string>();

	//Create a socket object
	m_pClientSocket = new CSocket();

	//Get the port number to bind the socket to
	unsigned short _usClientPort = DEFAULT_CLIENT_PORT;// QueryPortNumber(DEFAULT_CLIENT_PORT);
	//Initialise the socket to the port number
	if (!m_pClientSocket->Initialise(_usClientPort))
	{
		return;
	}

	//Set the client's online status to true
	m_bOnline = true;

	FullSearchForServers();
	m_ReceiveThread = std::thread(&Client::ReceiveData, this);
}

bool Client::SendData(char * _pcDataToSend)
{
	int _iBytesToSend = (int)strlen(_pcDataToSend) + 1;

	char _RemoteIP[MAX_ADDRESS_LENGTH];
	inet_ntop(AF_INET, &m_ServerSocketAddress.sin_addr, _RemoteIP, sizeof(_RemoteIP));
	//std::cout << "Trying to send " << _pcDataToSend << " to " << _RemoteIP << ":" << ntohs(m_ServerSocketAddress.sin_port) << std::endl;
	char _message[MAX_MESSAGE_LENGTH];
	strcpy_s(_message, strlen(_pcDataToSend) + 1, _pcDataToSend);

	int iNumBytes = sendto(
		m_pClientSocket->GetSocketHandle(),				// socket to send through.
		_pcDataToSend,									// data to send
		_iBytesToSend,									// number of bytes to send
		0,												// flags
		reinterpret_cast<sockaddr*>(&m_ServerSocketAddress),	// address to be filled with packet target
		sizeof(m_ServerSocketAddress)							// size of the above address struct.
	);
	//iNumBytes;
	if (_iBytesToSend != iNumBytes)
	{
		if (_iBytesToSend == WSAECONNRESET) // Doesn't work?
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
			std::cout << "Could not send Message, Lost Connection to Server: Host Closed" << std::endl;
			//ExitConnection();
			return false;
		}
		std::cout << "There was an error in sending data from client to server" << std::endl;
		return false;
	}
	return true;
}

void Client::ReceiveData()
{
	sockaddr_in _FromAddress; // Make a local variable to extract the IP and port number of the sender from whom we are receiving
							  //In this case; it should be the details of the server; since the client only ever receives from the server
	int iSizeOfAdd = sizeof(_FromAddress);
	int _iNumOfBytesReceived;

	//Receive data into a local buffer
	char _buffer[MAX_MESSAGE_LENGTH];
	//For debugging purpose only, convert the Address structure to a string.
	char _pcAddress[50];
	ZeroMemory(&_pcAddress, 50);
	while (m_bOnline)
	{
		// pull off the packet(s) using recvfrom()
		_iNumOfBytesReceived = recvfrom(			// pulls a packet from a single source...
			this->m_pClientSocket->GetSocketHandle(),						// client-end socket being used to read from
			_buffer,							// incoming packet to be filled
			MAX_MESSAGE_LENGTH,					   // length of incoming packet to be filled
			0,										// flags
			reinterpret_cast<sockaddr*>(&_FromAddress),	// address to be filled with packet source
			&iSizeOfAdd								// size of the above address struct.
		);
		inet_ntop(AF_INET, &_FromAddress, _pcAddress, sizeof(_pcAddress));

		if (_iNumOfBytesReceived < 0)
		{
			//Error in receiving data 
			//std::cout << "recvfrom failed with error " << WSAGetLastError();
			//_pcBufferToReceiveData = 0;
		}
		else if (_iNumOfBytesReceived == 0)
		{
			//The remote end has shutdown the connection
			//_pcBufferToReceiveData = 0;
		}
		else
		{
			//There is valid data received.

			std::string NewData(_buffer);
			//strcpy_s(m_pcPacketData, strlen(_buffer) + 1, _buffer);
			//Put this packet data in the workQ
			/*m_ServerSocketAddress = _FromAddress;
			if (int(m_pcPacketData[0] - '0') == KEEPALIVE)
			{
				ProcessData(m_pcPacketData);
			}
			else
			{*/
				m_pWorkQueue->push(NewData);
			//}
		}
		//std::this_thread::yield(); //Yield the processor; giving the main a chance to run.
	}
}

void Client::ProcessData(std::string _DataReceived)
{
	TPacket _packetRecvd;
	_packetRecvd = _packetRecvd.Deserialize(_DataReceived);
	switch (_packetRecvd.MessageType)
	{
		case HANDSHAKE:
		{
			std::string Result = _packetRecvd.MessageContent;
			if (Result == "Invalid")
			{
				UIManager::GetInstance()->ShowMessageBox("Name Taken! Pick a different one");
				// User name taken Display
				break;
			}
			else if (Result == "Started")
			{
				// Game already in session
				UIManager::GetInstance()->ShowMessageBox("The Server selected is already in session!");
				break;
			}

			std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());
			MenuRef->LobbyScreen.SetServerNameText(Result);
			MenuRef->ToggleMenuSection(LOBBY);
			break;
		}
		case CLIENTCONNECTED:
		{
			std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());

			std::string Result = _packetRecvd.MessageContent;
			std::string Username;
			std::string Address;

			for (unsigned int i = 0; i < Result.size(); i++)
			{
				if (Result[i] != ' ')
				{
					Username += Result[i];
					continue;
				}
				Address = Result.substr(i + 1);
				break;
			}			
			MenuRef->LobbyScreen.ClientConnected(Username, Address);
			break;
		}
		case CLIENTDISCONNECT:
		{
			std::string Username = _packetRecvd.MessageContent;
			// Destroy player instance
			SceneManager::GetInstance()->GetCurrentScene()->DestroyEntity(PlayerEntities[Username]);
			// Remove from players map
			PlayerEntities.erase(Username);
			// Remove player from UI Hud
			UIManager::GetInstance()->m_HUDInstance.RemovePlayer(Username);

			break;
		}
		case SERVERCLOSED:
		{
			NetworkManager::GetInstance()->m_Network.ShutDown();
			UIManager::GetInstance()->m_HUDInstance.ClearPlayersHUD();
			SceneManager::GetInstance()->SwitchScene("MainMenu");
			UIManager::GetInstance()->ShowMessageBox("Server Closed");
			break;
		}
		case LOADLEVEL:
		{
			LevelManager::GetInstance()->NextLevel();
			for (auto& player : PlayerEntities)
			{
				player.second->SetVisible(true);
			}
			//SceneManager::GetInstance()->SwitchScene(_packetRecvd.MessageContent);
			break;
		}
		case CHAT:
		{
		std::string Result = _packetRecvd.MessageContent;
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
			break;
		}
		case ENTITYUPDATE:
		{
			std::string Result = _packetRecvd.MessageContent;
			// Create entity from mesh type, and miss out the first number and space
			UpdateNetworkEntity(Result);
			break;
		}
		case CREATEENTITY:
		{
			std::string Result = _packetRecvd.MessageContent;
			std::istringstream iss(Result);
			int MeshType;
			iss >> MeshType;
			if (Result.size() < 3) break;
			// Create entity from mesh type, and miss out the first number and space
			std::shared_ptr<Entity> NewEntity = CreateNetworkEntity(Utils::EMESHTYPE(MeshType), Result.substr(2));

			SceneManager::GetInstance()->GetCurrentScene()->AddEntity(NewEntity);
			break;
		}
		case DESTROYENTITY:
		{
			std::string Result = _packetRecvd.MessageContent;
			// Destroy Entity from ID
			DestroyNetworkEntity(std::stoi(Result));
			break;
		}
		case PLAYERUPDATE:
		{
			std::string Result = _packetRecvd.MessageContent;
			std::stringstream ss(Result);
			std::string Username;
			int Score;
			float Health, PosX, PosY, PosZ, RotX, RotY, RotZ;

			ss >> Username >> Health >> Score >> PosX >> PosY >> PosZ >> RotX >> RotY >> RotZ;

			glm::vec3 position = { PosX, PosY, PosZ };
			glm::vec3 rotation = { RotX, RotY, RotZ };

			PlayerEntities[Username]->SetScore(Score);
			PlayerEntities[Username]->SetHealth(Health);
			// Don't update current players position, only other players
			if (Username != m_cUserName)
			{
				PlayerEntities[Username]->transform.Position = position;
				PlayerEntities[Username]->transform.Rotation = rotation;
			}

			break;
		}
		case CREATEPLAYER:
		{
			std::string Result = _packetRecvd.MessageContent;
			std::stringstream ss(Result);
			std::string Username;

			ss >> Username;
			CreateNetworkPlayer(Username);
			if (Username != m_cUserName) UIManager::GetInstance()->m_HUDInstance.AddPlayer(Username);
			break;
		}
		case PLAYERDEATH:
		{
			std::string Username = _packetRecvd.MessageContent;
			PlayerEntities[Username]->SetVisible(false);
			break;
		}
		case CREATEBULLET:
		{
			std::string Result = _packetRecvd.MessageContent;
			std::stringstream ss(Result);
			std::string Username;
			float PosX, PosY, PosZ, RotX, RotY, RotZ;
			int iNetworkID;

			ss >> Username >> PosX >> PosY >> PosZ >> RotX >> RotY >> RotZ >> iNetworkID;

			glm::vec3 position = { PosX, PosY, PosZ };
			glm::vec3 rotation = { RotX, RotY, RotZ };

			glm::vec3 Direction = glm::rotateY(glm::vec3( 0, 0, 1 ), glm::radians(RotY));

			std::shared_ptr<PlayerBullet> NewBullet = std::make_shared<PlayerBullet>(PlayerBullet({ position, rotation, glm::vec3(0.1f, 0.1f, 0.1f) }, Utils::CENTER, Direction));
			std::shared_ptr<Cube> BulletCube = std::make_shared<Cube>(Cube(1, 1, 1, { 1,0,0,1 }));
			BulletCube->AddCollisionBounds(0.3f, 10.0f, 0.3f, NewBullet);
			NewBullet->AddMesh(BulletCube);
			NewBullet->UserOwner = Username;
			CreateNetworkEntity(NewBullet, iNetworkID);
			break;
		}
		case APPLYPOWERUP:
		{
			std::string Result = _packetRecvd.MessageContent;
			std::stringstream ss(Result);

			int PowerupType;
			float Duration;
			ss >> PowerupType >> Duration;
			
			PlayerEntities[m_cUserName]->ApplyPowerUp((POWERUPS)PowerupType, Duration);
			break;
		}
		case SETENTITYVISIBLE:
		{
			std::string Result = _packetRecvd.MessageContent;
			std::stringstream ss(Result);

			int NetworkID;
			bool Visible;
			ss >> NetworkID >> Visible;

			NetworkEntities[NetworkID]->SetVisible(Visible);
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
void Client::Update()
{
	//If the message queue is empty 
	while (!m_pWorkQueue->empty())
	{
		//Retrieve off a message from the queue and process it
		std::string temp;
		m_pWorkQueue->pop(temp);
		ProcessData(temp);
	}
}

void Client::DestroyNetworkEntity(int iNetworkID)
{
	SceneManager::GetInstance()->GetCurrentScene()->DestroyEntity(NetworkEntities[iNetworkID]);
	NetworkEntities.erase(iNetworkID);
}

void Client::SendMessageNE(std::string _pcMessage, EMessageType _Message)
{
	std::string MessageToSend = _pcMessage;
	if (_Message == CHAT)
	{
		std::string UserAndMessage(m_cUserName);
		UserAndMessage += " " + _pcMessage;
		MessageToSend = UserAndMessage;
	}
	TPacket _packet;
	_packet.Serialize(_Message, const_cast<char *>(MessageToSend.c_str()));
	SendData(_packet.PacketData);
}

bool Client::BroadcastForServers()
{
	//Make a broadcast packet
	TPacket _packet;
	_packet.Serialize(BROADCAST, "Broadcast to Detect Server");

	char _pcTempBuffer[MAX_MESSAGE_LENGTH];
	//Send out a broadcast message using the broadcast address
	m_pClientSocket->SetRemoteAddress(INADDR_BROADCAST);
	m_pClientSocket->SetRemotePort(DEFAULT_SERVER_PORT);

	m_ServerSocketAddress.sin_family = AF_INET;
	m_ServerSocketAddress.sin_addr.S_un.S_addr = INADDR_BROADCAST;

	for (int i = 0; i < 10; i++) //Just try  a series of 10 ports to detect a runmning server; this is needed since we are testing multiple servers on the same local machine
	{
		m_ServerSocketAddress.sin_port = htons(DEFAULT_SERVER_PORT + i);
		SendData(_packet.PacketData);
	}
	ReceiveBroadcastMessages(_pcTempBuffer);

	std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());
	MenuRef->JoinGameScreen.AddServers(m_vecServerAddr);
	return true;
}

void Client::FullSearchForServers()
{
	m_bDoBroadcast = true;
	m_pClientSocket->EnableBroadcast();
	BroadcastForServers();
	if (m_vecServerAddr.size() == 0)
	{
		std::cout << "No Servers Found " << std::endl;
	}
	else {
		//Give a list of servers for the user to choose from :
		for (unsigned int i = 0; i < m_vecServerAddr.size(); i++)
		{
			std::cout << std::endl << "[" << i << "]" << " SERVER : found at " << ToString(m_vecServerAddr[i]._IPAddress) << std::endl;
		}
	}
	m_bDoBroadcast = false;
	m_pClientSocket->DisableBroadcast();
}

void Client::ReceiveBroadcastMessages(char * _pcBufferToReceiveData)
{
	m_vecServerAddr.clear();
	//set a timer on the socket for one second
	struct timeval timeValue;
	timeValue.tv_sec = 100;
	timeValue.tv_usec = 0;
	setsockopt(m_pClientSocket->GetSocketHandle(), SOL_SOCKET, SO_RCVTIMEO,
		(char*)&timeValue, sizeof(timeValue));

	//Receive data into a local buffer
	char _buffer[MAX_MESSAGE_LENGTH];
	sockaddr_in _FromAddress;
	int iSizeOfAdd = sizeof(sockaddr_in);
	//char _pcAddress[50];

	while (m_bDoBroadcast)
	{
		// pull off the packet(s) using recvfrom()
		int _iNumOfBytesReceived = recvfrom(				// pulls a packet from a single source...
			m_pClientSocket->GetSocketHandle(),	// client-end socket being used to read from
			_buffer,									// incoming packet to be filled
			MAX_MESSAGE_LENGTH,							// length of incoming packet to be filled
			0,											// flags
			reinterpret_cast<sockaddr*>(&_FromAddress),	// address to be filled with packet source
			&iSizeOfAdd								// size of the above address struct.
		);

		if (_iNumOfBytesReceived < 0)
		{
			//Error in receiving data 
			int _iError = WSAGetLastError();
			//std::cout << "recvfrom failed with error " << _iError;
			if (_iError == WSAETIMEDOUT) // Socket timed out on Receive
			{
				m_bDoBroadcast = false; //Do not broadcast any more
				break;
			}
			_pcBufferToReceiveData = 0;
		}
		else if (_iNumOfBytesReceived == 0)
		{
			//The remote end has shutdown the connection
			_pcBufferToReceiveData = 0;
		}
		else
		{
			//There is valid data received.
			strcpy_s(_pcBufferToReceiveData, strlen(_buffer) + 1, _buffer);
			m_ServerSocketAddress = _FromAddress;
			std::string Message(_pcBufferToReceiveData);
			Message = Message.substr(2);
			ServerInfoProperties NewServer;
			NewServer._IPAddress = m_ServerSocketAddress;
			NewServer._iPlayers = Message[0] - '0';
			Message = Message.substr(2);
			NewServer._ServerName = Message;
			m_vecServerAddr.push_back(NewServer);
		}
	}//End of while loop
}
