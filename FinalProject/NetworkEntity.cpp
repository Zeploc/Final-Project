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
#include "Engine\Model.h"

// Local Includes //
#include "LevelManager.h"

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
		NetworkEntities.insert(std::pair<int, std::shared_ptr<Entity>>(NetworkID, NewEntity));
		/*if (NetworkEntities.size() < NetworkID - 1)
		{
			NetworkEntities.push_back(nullptr);
		}
		NetworkEntities[NetworkID] = NewEntity;*/
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
		bool bIsLit, bIsReflecting;
		std::string TexturePath = "";
		ss >> NetworkID >> PosX >> PosY >> PosZ >> RotX >> RotY >> RotZ >> ScaleX >> ScaleY >> ScaleZ >> fWidth >> fHeight >> fDepth >> ColourR >> ColourG >> ColourB >> ColourA >> bIsLit >> bIsReflecting >> TexturePath;
		glm::vec3 Pos = { PosX, PosY, PosZ };
		glm::vec3 Rot = { RotX, RotY, RotZ };
		glm::vec3 Scale = { ScaleX, ScaleY, ScaleZ };
		glm::vec4 Colour = { ColourR , ColourG, ColourB, ColourA };
		std::shared_ptr<Entity> NewEntity = std::make_shared<Entity>(Entity({ Pos, Rot, Scale }, Utils::CENTER));
		std::shared_ptr<Cube> CubeMesh;
		if(TexturePath != "")
			CubeMesh = std::make_shared<Cube>(Cube(fWidth, fHeight, fDepth, Colour, TexturePath.c_str()));
		else
			CubeMesh = std::make_shared<Cube>(Cube(fWidth, fHeight, fDepth, Colour));
		CubeMesh->SetLit(bIsLit);
		if (bIsReflecting) CubeMesh->SetReflection();
		NewEntity->AddMesh(CubeMesh);
		NetworkEntities.insert(std::pair<int, std::shared_ptr<Entity>>(NetworkID, NewEntity));
		return NewEntity;
		break;
	}
	case Utils::SPHERE:
		break;
	case Utils::MODEL:
	{
		std::stringstream ss(EntityInfo);
		float fWidth, fHeight, fDepth, ColourR, ColourG, ColourB, ColourA;
		bool bIsLit;
		std::string TexturePath = "";
		ss >> NetworkID >> PosX >> PosY >> PosZ >> RotX >> RotY >> RotZ >> ScaleX >> ScaleY >> ScaleZ >> ColourR >> ColourG >> ColourB >> ColourA >> bIsLit >> TexturePath;
		glm::vec3 Pos = { PosX, PosY, PosZ };
		glm::vec3 Rot = { RotX, RotY, RotZ };
		glm::vec3 Scale = { ScaleX, ScaleY, ScaleZ };
		glm::vec4 Colour = { ColourR , ColourG, ColourB, ColourA };
		std::shared_ptr<Entity> NewEntity = std::make_shared<Entity>(Entity({ Pos, Rot, Scale }, Utils::CENTER));
		std::shared_ptr<Model> ModelMesh = std::make_shared<Model>(Model(Colour, TexturePath.c_str()));
		ModelMesh->SetLit(bIsLit);
		NewEntity->AddMesh(ModelMesh);
		NetworkEntities.insert(std::pair<int, std::shared_ptr<Entity>>(NetworkID, NewEntity));
		return NewEntity;
		break;
	}
		
	default:
		break;
	}
	return nullptr;
}

int NetworkEntity::CreateNetworkEntity(std::shared_ptr<Entity> _Entity, int iNetworkIdentity, bool bAddToScene, std::shared_ptr<Scene> SceneToAddTo)
{
	std::shared_ptr<Level> levelRef = LevelManager::GetInstance()->GetCurrentActiveLevel();
	int iNetworkID = iNetworkIdentity;
	if (iNetworkIdentity == -1)
	{
		iNetworkID = iLastID;
		iLastID++;
	}

	NetworkEntities.insert(std::pair<int, std::shared_ptr<Entity>>(iNetworkID, _Entity));
	if (bAddToScene)
	{
		if (!SceneToAddTo)
			levelRef->AddEntity(_Entity);
		else
			SceneToAddTo->AddEntity(_Entity);
	}
	return iNetworkID;
}

void NetworkEntity::CreateNetworkPlayer(std::string UserName)
{
	std::shared_ptr<Level> levelRef = LevelManager::GetInstance()->GetCurrentActiveLevel();
	glm::vec3 SpawnPlayerPos = levelRef ? levelRef->SpawnPos : glm::vec3(10, -1, 10);
	std::shared_ptr<Player> NewPlayer = std::make_shared<Player>(Player({ SpawnPlayerPos, {0, 0, 0}, {0.01f, 0.01f, 0.01f} }, 0.5f, 1.0f, 0.5f, Utils::CENTER, glm::vec4(0.1, 1.0, 0.1, 1.0)));
	NewPlayer->EntityMesh->AddCollisionBounds(0.6f, 1.0f, 0.6f, NewPlayer);
	NewPlayer->m_UserName = UserName;
	PlayerEntities.insert(std::pair<std::string, std::shared_ptr<Player>>(UserName, NewPlayer));
	//NetworkEntities.insert(std::pair<int, std::shared_ptr<Entity>>(iNetworkID, NewPlayer));
	levelRef->AddEntity(NewPlayer);
	if (m_cUserName == UserName) // If the player being added is the same as the current player/client/server, set level Eplayer to this
		levelRef->EPlayer = NewPlayer;

	return;
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
	
	if (NetworkEntities[NetworkID]) NetworkEntities[NetworkID]->transform = { Pos , Rot , Scale };
	return;
}

std::string NetworkEntity::GetNetworkEntityString(std::shared_ptr<Entity> NetworkEntity, bool bIsUpdate, int iNetworkIdentity)
{
	int iNetworkID = iNetworkIdentity;
	if (iNetworkIdentity == -1)
	{
		iNetworkID = iLastID;
		iLastID++;
		NetworkEntities.insert(std::pair<int, std::shared_ptr<Entity>>(iNetworkID, NetworkEntity));
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
			+ " " + Vec4ToSendString(NetworkEntity->EntityMesh->Colour) + " " + std::to_string(NetworkEntity->EntityMesh->IsLit()) + " " + std::to_string(NetworkEntity->EntityMesh->IsReflecting());
		if (NetworkEntity->EntityMesh->bHasTexture)
			EntityMessage += " " + std::string(NetworkEntity->EntityMesh->TextureSource);
		return EntityMessage;
		break;
	}
	case Utils::SPHERE:
		break;
	case Utils::MODEL:
	{
		std::string EntityMessage = std::to_string(Utils::MODEL) + " " + std::to_string(iNetworkID) + " " +
			Vec3ToSendString(NetworkEntity->transform.Position) + " " + Vec3ToSendString(NetworkEntity->transform.Rotation) + " " + Vec3ToSendString(NetworkEntity->transform.Scale)
			+ " " + Vec4ToSendString(NetworkEntity->EntityMesh->Colour) + " " + std::to_string(NetworkEntity->EntityMesh->IsLit()) + " " + std::string(NetworkEntity->EntityMesh->TextureSource);
		return EntityMessage;
		break;
	}
	default:
		break;
	}
	return std::string();
}
