//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    NetworkEntity.cpp
// Description    	:    The Base class for a network entity
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "NetworkEntity.h"

// Library Includes //
#include <iostream>

// Engine Includes //
#include "Engine\Entity.h"
#include "Engine\Cube.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
NetworkEntity::NetworkEntity()
{
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
NetworkEntity::~NetworkEntity()
{
}

std::string NetworkEntity::Vec3ToSendString(glm::vec3 _vec3)
{
	std::string Vec3String = std::to_string(_vec3.x);
	Vec3String += " " + std::to_string(_vec3.y);
	Vec3String += " " + std::to_string(_vec3.z);
	return Vec3String;
}

std::string NetworkEntity::Vec4ToSendString(glm::vec4 _vec4)
{
	std::string Vec4String = std::to_string(_vec4.r);
	Vec4String += " " + std::to_string(_vec4.g);
	Vec4String += " " + std::to_string(_vec4.b);
	Vec4String += " " + std::to_string(_vec4.a);
	return Vec4String;
}

glm::vec3 NetworkEntity::StringToVec3(std::string _vec3String)
{
	std::stringstream ss(_vec3String);
	float x, y, z;
	ss >> x >> y >> z;
	return { x, y, z };
}

void NetworkEntity::ExtractTwoVec3(std::string _vec3String, glm::vec3 & Vec1, glm::vec3 & Vec2)
{
	std::stringstream ss(_vec3String);
	float x1, y1, z1, x2, y2, z2;
	ss >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
	Vec1 = { x1, y1, z1 };
	Vec2 = { x2, y2, z2 };
	return;
}

void NetworkEntity::ExtractTwoVec3WithNetworkID(std::string _vec3String, int & NetworkID, glm::vec3 & Vec1, glm::vec3 & Vec2)
{
	std::stringstream ss(_vec3String);
	float x1, y1, z1, x2, y2, z2;
	ss >> NetworkID >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
	Vec1 = { x1, y1, z1 };
	Vec2 = { x2, y2, z2 };
	return;
}

std::shared_ptr<Entity> NetworkEntity::CreateNetworkEntity(Utils::EMESHTYPE MeshType, std::string EntityInfo)
{
	int NetworkID;
	float PosX, PosY, PosZ, RotX, RotY, RotZ, ScaleX, ScaleY, ScaleZ;
	switch (MeshType)
	{
	case Utils::NONE:
	{
		std::stringstream ss(EntityInfo);
		ss >> NetworkID >> PosX >> PosY >> PosZ >> RotX >> RotY >> RotZ >> ScaleX >> ScaleY >> ScaleZ;
		glm::vec3 Pos = { PosX, PosY, PosZ };
		glm::vec3 Rot = { RotX, RotY, RotZ };
		glm::vec3 Scale = { ScaleX, ScaleY, ScaleZ };
		std::shared_ptr<Entity> NewEntity = std::make_shared<Entity>(Entity({ Pos, Rot, Scale }, Utils::CENTER));
		if (NetworkEntities.size() < NetworkID - 1)
		{
			NetworkEntities.push_back(nullptr);
		}
		NetworkEntities[NetworkID] = NewEntity;
		return NewEntity;
		break;
	}
	case Utils::PLANE:
		break;
	case Utils::PYRAMID:
		break;
	case Utils::CUBE:
	{
		std::stringstream ss(EntityInfo);
		float fWidth, fHeight, fDepth, ColourR, ColourG, ColourB, ColourA;		
		ss >> NetworkID >> PosX >> PosY >> PosZ >> RotX >> RotY >> RotZ >> ScaleX >> ScaleY >> ScaleZ >> fWidth >> fHeight >> fDepth >> ColourR >> ColourG >> ColourB >> ColourA;
		glm::vec3 Pos = { PosX, PosY, PosZ };
		glm::vec3 Rot = { RotX, RotY, RotZ };
		glm::vec3 Scale = { ScaleX, ScaleY, ScaleZ };
		glm::vec4 Colour = { ColourR , ColourG, ColourB, ColourA };
		std::shared_ptr<Entity> NewEntity = std::make_shared<Entity>(Entity({ Pos, Rot, Scale }, Utils::CENTER));
		std::shared_ptr<Cube> CubeMesh = std::make_shared<Cube>(Cube(fWidth, fHeight, fDepth, Colour));
		NewEntity->AddMesh(CubeMesh);
		if (NetworkEntities.size() < NetworkID - 1)
		{
			NetworkEntities.push_back(nullptr);
		}
		NetworkEntities[NetworkID] = NewEntity;
		return NewEntity;
		break;
	}
	case Utils::SPHERE:
		break;
	case Utils::MODEL:
		break;
	default:
		break;
	}
	return nullptr;
}

void NetworkEntity::UpdateNetworkEntity(std::string UpdateInfo)
{
	int NetworkID;
	float PosX, PosY, PosZ, RotX, RotY, RotZ, ScaleX, ScaleY, ScaleZ;
	std::stringstream ss(UpdateInfo);
	ss >> NetworkID >> PosX >> PosY >> PosZ >> RotX >> RotY >> RotZ >> ScaleX >> ScaleY >> ScaleZ;
	glm::vec3 Pos = { PosX, PosY, PosZ };
	glm::vec3 Rot = { RotX, RotY, RotZ };
	glm::vec3 Scale = { ScaleX, ScaleY, ScaleZ };
	
	NetworkEntities[NetworkID]->transform = { Pos , Rot , Scale };

	return;
}

std::string NetworkEntity::GetNetworkEntityString(std::shared_ptr<Entity> NetworkEntity, bool bIsUpdate, int iNetworkIdentity)
{
	int iNetworkID = iNetworkIdentity;
	if (iNetworkIdentity == -1)
	{
		iNetworkID = NetworkEntities.size();
		NetworkEntities.push_back(nullptr);
		NetworkEntities[iNetworkID] = NetworkEntity;
	}
	// No Mesh
	if (!NetworkEntity->EntityMesh || bIsUpdate)
	{
		std::string EntityMessage;
		if (!bIsUpdate) EntityMessage += std::to_string(Utils::NONE) + " ";
		EntityMessage += std::to_string(iNetworkID) + " " + Vec3ToSendString(NetworkEntity->transform.Position) + " " + Vec3ToSendString(NetworkEntity->transform.Rotation) + " " + Vec3ToSendString(NetworkEntity->transform.Scale);
		return EntityMessage;
	}

	switch (NetworkEntity->EntityMesh->m_eShape)
	{
	case Utils::PLANE:
		break;
	case Utils::PYRAMID:
		break;
	case Utils::CUBE:
	{
		std::string EntityMessage = std::to_string(Utils::CUBE) + " " + std::to_string(iNetworkID) + " " +
			Vec3ToSendString(NetworkEntity->transform.Position) + " " + Vec3ToSendString(NetworkEntity->transform.Rotation) + " " + Vec3ToSendString(NetworkEntity->transform.Scale)
			+ " " + std::to_string(NetworkEntity->EntityMesh->m_fWidth) + " " + std::to_string(NetworkEntity->EntityMesh->m_fHeight) + " " + std::to_string(NetworkEntity->EntityMesh->m_fDepth)
			+ " " + Vec4ToSendString(NetworkEntity->EntityMesh->Colour);
		return EntityMessage;
		break;
	}
	case Utils::SPHERE:
		break;
	case Utils::MODEL:
		break;
	default:
		break;
	}
	return std::string();
}
