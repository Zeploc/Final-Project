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
#include <vector>

// OpenGL Includes //
#include <glm\common.hpp>
#include <glm\gtx\string_cast.hpp>

// Engine Includes //
#include "Engine\Utils.h"

enum EMessageType : unsigned char
{
	HANDSHAKE,
	DATA,
	KEEPALIVE,
	BROADCAST,
	CHAT,
	CLIENTCONNECTED,
	LOADLEVEL,
	CREATEENTITY,
	ENTITYUPDATE
};

struct TPacket
{
	unsigned short MessageType;
	char MessageContent[250];
	char PacketData[260];
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

	static std::string Vec3ToSendString(glm::vec3 _vec3);
	static std::string Vec4ToSendString(glm::vec4 _vec4);
	static glm::vec3 StringToVec3(std::string _vec3String);
	static void ExtractTwoVec3(std::string _vec3String, glm::vec3& Vec1, glm::vec3& Vec2);
	static void ExtractTwoVec3WithNetworkID(std::string _vec3String, int& NetworkID, glm::vec3& Vec1, glm::vec3& Vec2);
	std::shared_ptr<Entity> CreateNetworkEntity(Utils::EMESHTYPE MeshType, std::string EntityInfo);
	void UpdateNetworkEntity(std::string UpdateInfo);
	std::string GetNetworkEntityString(std::shared_ptr<Entity> Entity, bool bIsUpdate, int iNetworkIdentity = -1);

	virtual void Initialise() = 0;
	//virtual void ReceiveData(char* _pcBufferToReceiveData) = 0;
	virtual void Update() = 0;

	virtual void ServerSendToAllPlayers(std::string _pcMessage, EMessageType _Message) = 0;

	void SetOffline()
	{
		m_bOnline = false;
	}

	std::thread m_ReceiveThread;
protected:
	//Additional state variable to indicate whether a network entity is online or not
	bool m_bOnline = false;

	std::vector<std::shared_ptr<Entity>> NetworkEntities;
};

