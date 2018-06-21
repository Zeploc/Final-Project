//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    SpeedBoostPickUp.cpp
// Description    	:    Pick Up to speed boost player
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "FireRatePickup.h"

// Local Includes //
#include "Player.h"
#include "NetworkManager.h"
#include "Server.h"
#include "Client.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
FireRatePickup::FireRatePickup(Utils::Transform _Transform, Utils::EANCHOR _Anchor, std::shared_ptr<Entity> _CollidingEntity, float _fRespawnTime)
: PickUpBase(_Transform, _Anchor, _CollidingEntity, _fRespawnTime)
{
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
FireRatePickup::~FireRatePickup()
{
}

void FireRatePickup::OnPickUp(std::shared_ptr<Entity> CollidingEntity)
{
	PickUpBase::OnPickUp(CollidingEntity);

	std::shared_ptr<Player> IsPlayer = std::dynamic_pointer_cast<Player>(CollidingEntity);
	if (IsPlayer)
	{
		// If is network but not a server, don't check collisions
		if (!NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)
		{
			IsPlayer->ApplyPowerUp(FIRERATE, 4.0f);
			
		}
		else if (IsPlayer->m_UserName == NetworkManager::GetInstance()->m_Network.m_pNetworkEntity->GetUsername())
		{
			// Is still server, since not single player (Check done in base), and also the player is the server, so apply
			IsPlayer->ApplyPowerUp(FIRERATE, 4.0f);
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
					std::string PowerupString = std::to_string(FIRERATE) + " " + std::to_string(4.0f);
					std::shared_ptr<Client> ClientRef = std::dynamic_pointer_cast<Client>(NetworkManager::GetInstance()->m_Network.m_pNetworkEntity);
					TPacket _packetToSend;
					_packetToSend.Serialize(APPLYPOWERUP, const_cast<char*>(PowerupString.c_str()));
					ClientRef->SendData(_packetToSend.PacketData);

					// create message, format PowerUP type, Duration
					// send data to that client


					break;
				}
			}
		}
	}
}
