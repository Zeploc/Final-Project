//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    PickUpBase.cpp
// Description    	:    Base Pickup class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "PickUpBase.h"
#include "GameSettings.h"
// Library Includes //
#include <iostream>


// Engine Includes //
#include "Engine\Time.h"
#include "Engine\CollisionBounds.h"
#include "Engine\LogManager.h"
#include "Engine/SoundManager.h"

// Local Includes //
#include "LevelManager.h"
#include "NetworkManager.h"
#include "Level.h"
#include "NetworkManager.h"
#include "Server.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
PickUpBase::PickUpBase(Utils::Transform _Transform, Utils::EANCHOR _Anchor, std::shared_ptr<Entity> _CollidingEntity, float _fRespawnTime)
	: Entity(_Transform, _Anchor), m_fRespawnTime(_fRespawnTime), m_fRespawnTimer(_fRespawnTime)
{	
	m_fOscillatingHeight = _Transform.Position.y;

}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
PickUpBase::~PickUpBase()
{
}

void PickUpBase::Init()
{
	CollidingEntities.clear();
	if (NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)
	{
		if (NetworkManager::GetInstance()->m_Network.IsServer())
		{
			for (auto& PlayerIt : NetworkManager::GetInstance()->m_Network.m_pNetworkEntity->PlayerEntities)
			{
				CollidingEntities.push_back(PlayerIt.second);
			}
		}
	}
	else
	{
		std::shared_ptr<Level> levelRef = LevelManager::GetInstance()->GetCurrentActiveLevel();
		CollidingEntities.push_back(levelRef->EPlayer);
	}
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void PickUpBase::Update()
{
	Entity::Update();

	if (!EntityMesh) // No mesh added
		return;
	if (!EntityMesh->GetCollisionBounds()) // No collision Bounds added
		return;
		
	Rotate({ 0, 10.0f * Time::dTimeDelta, 0 });
	transform.Position.y = (float)(m_fOscillatingHeight + sin(Time::dCurrentTime / 300) * 5.0f * Time::dTimeDelta);

	if (m_fRespawnTimer > 0) // Is respawning
	{
		m_fRespawnTimer -= (float)Time::dTimeDelta;
		if (m_fRespawnTimer <= 0)
		{
			m_fRespawnTimer = 0;
			RespawnPickup();
		}
	}
	else
	{
		if (!bVisible) // Not visible when checking for collisons
			SetVisible(true); // Turns visible to show it is collectable

		// If is network but not a server, don't check collisions
		if (NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)
		{
			if (!NetworkManager::GetInstance()->m_Network.IsServer())
			{
				return;
			}
		}

		// Check if colliding
		for (auto& CurrentCollidingPlayer : CollidingEntities)
		{
			if (EntityMesh->GetCollisionBounds()->isColliding(CurrentCollidingPlayer))
			{
				m_fRespawnTimer = m_fRespawnTime;
				OnPickUp(CurrentCollidingPlayer);
				break;
			}
		}
		
	}
}

void PickUpBase::ApplyPowerUpToPlayer(std::shared_ptr<Entity> CollidingEntity, POWERUPS PowerUpType, float fDuration)
{
	std::shared_ptr<Player> IsPlayer = std::dynamic_pointer_cast<Player>(CollidingEntity);
	if (IsPlayer)
	{
		// If is network but not a server, don't check collisions
		if (!NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)
		{
			IsPlayer->ApplyPowerUp(PowerUpType, fDuration);

		}
		else if (IsPlayer->m_UserName == NetworkManager::GetInstance()->m_Network.m_pNetworkEntity->GetUsername())
		{
			// Is still server, since not single player (Check done in base), and also the player is the server, so apply
			IsPlayer->ApplyPowerUp(PowerUpType, fDuration);
			std::shared_ptr<Server> ServerRef = std::dynamic_pointer_cast<Server>(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity);
			for (auto& NetworkEnt : ServerRef->NetworkEntities)
			{
				if (NetworkEnt.second == this->shared_from_this())
				{
					std::string SetInvisibleText = std::to_string(NetworkEnt.first) + " " + std::to_string(false);
					ServerRef->SendToAllClients(SetInvisibleText, SETENTITYVISIBLE);
					break;
				}
			}
			return;
		}
		else
		{
			std::shared_ptr<Server> ServerRef = std::dynamic_pointer_cast<Server>(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity);
			// Tell the client to apply powerup
			for (auto& PlayerIt : *ServerRef->GetConnectedClients())
			{
				if (PlayerIt.second.m_strName == IsPlayer->m_UserName)
				{
					std::string PowerupString = std::to_string(PowerUpType) + " " + std::to_string(fDuration); // also pass network id
					ServerRef->SetSender(PlayerIt.second.m_ClientAddress);
					TPacket _packetToSend;
					_packetToSend.Serialize(APPLYPOWERUP, const_cast<char*>(PowerupString.c_str()));
					ServerRef->SendData(_packetToSend.PacketData);
					for (auto& NetworkEnt : ServerRef->NetworkEntities)
					{
						if (NetworkEnt.second == this->shared_from_this())
						{
							std::string SetInvisibleText = std::to_string(NetworkEnt.first) + " " + std::to_string(false);
							ServerRef->SendToAllClients(SetInvisibleText, SETENTITYVISIBLE);
							break;
						}
					}
					break;
				}
			}
		}
	}
}

void PickUpBase::OnPickUp(std::shared_ptr<Entity> CollidingEntity)
{
	SoundManager::GetInstance()->AddChannel("PickupChannel");
	SoundManager::GetInstance()->AddAudio("Resources/Sound/162476__kastenfrosch__gotitem.mp3", false, "Pickup");
	SoundManager::GetInstance()->PlayAudio("Pickup","PickupChannel");
	SoundManager::GetInstance()->SetChannelVolume("PickupChannel", GameSettings::fVolumeLevel / 10);

	LogManager::GetInstance()->DisplayLogMessage("Pick up Collectd | Function \"OnPickUp\" base called!|");
	SetVisible(false);
}

void PickUpBase::RespawnPickup()
{
	if (!NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)
	{
		SetVisible(true);
	}
	else if (NetworkManager::GetInstance()->m_Network.IsServer())
	{
		SetVisible(true);
		std::shared_ptr<Server> ServerRef = std::dynamic_pointer_cast<Server>(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity);
		for (auto& NetworkEnt : ServerRef->NetworkEntities)
		{
			if (NetworkEnt.second == this->shared_from_this())
			{
				std::string SetInvisibleText = std::to_string(NetworkEnt.first) + " " + std::to_string(true);
				ServerRef->SendToAllClients(SetInvisibleText, SETENTITYVISIBLE);
				break;
			}
		}
	}
}
