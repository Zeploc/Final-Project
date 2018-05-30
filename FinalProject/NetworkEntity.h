//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    NetworkEntity.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <thread>
#include <string>
#include <sstream>

enum EMessageType : unsigned char
{
	HANDSHAKE,
	DATA,
	KEEPALIVE,
	BROADCAST,
	CLIENTCONNECTED
};

struct TPacket
{
	unsigned short MessageType;
	char MessageContent[50];
	char PacketData[60];
	unsigned short PacketSize;

	//void set_packet(short _x, short _y, WORD _object_type, short _object_index, WORD _param)
	void Serialize(EMessageType _MessageType, char* _message)
	{

		MessageType = _MessageType;
		strcpy_s(MessageContent, strlen(_message) + 1, _message);

		std::ostringstream oss;
		oss << MessageType;
		oss << " ";
		oss << MessageContent;

		std::string _strToSend = oss.str();
		const char* _pcToSend = _strToSend.c_str();

		strcpy_s(PacketData, strlen(_pcToSend) + 1, _pcToSend);

		PacketSize = static_cast<unsigned short>(_strToSend.size());
	}

	TPacket Deserialize(std::string _PacketData)
	{
		std::istringstream iss(_PacketData);

		iss >> this->MessageType;

		//iss >> this->MessageContent;
		std::string _tempMessageBuffer;
		getline(iss, _tempMessageBuffer);
		_tempMessageBuffer = _tempMessageBuffer.substr(1, _tempMessageBuffer.size());
		strcpy_s(MessageContent, _tempMessageBuffer.length() + 1, _tempMessageBuffer.c_str());


		return *this;
	}

};

#pragma once
class NetworkEntity
{
public:
	NetworkEntity();
	~NetworkEntity();
	NetworkEntity(const NetworkEntity& New) {  };

	virtual void Initialise() = 0;
	//virtual void ReceiveData(char* _pcBufferToReceiveData) = 0;
	virtual void Update() = 0;


	void SetOffline()
	{
		m_bOnline = false;
	}

	std::thread m_ReceiveThread;
protected:
	//Additional state variable to indicate whether a network entity is online or not
	bool m_bOnline = false;
};

