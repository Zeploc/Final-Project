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

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
PickUpBase::PickUpBase(Utils::Transform _Transform, Utils::EANCHOR _Anchor, std::shared_ptr<Entity> _CollidingEntity, float _fRespawnTime)
	: Entity(_Transform, _Anchor), m_fRespawnTime(_fRespawnTime), m_fRespawnTimer(_fRespawnTime), CollidingEntity(_CollidingEntity)
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
	transform.Position.y = m_fOscillatingHeight + sin(Time::dCurrentTime / 300) * 5.0f * Time::dTimeDelta;

	if (m_fRespawnTimer > 0) // Is respawning
	{
		m_fRespawnTimer -= Time::dTimeDelta;
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
		// Check if colliding
		if (EntityMesh->GetCollisionBounds()->isColliding(CollidingEntity))
		{
			m_fRespawnTimer = m_fRespawnTime;
			OnPickUp();
		}
	}
}

void PickUpBase::OnPickUp()
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
	SetVisible(true);
}
