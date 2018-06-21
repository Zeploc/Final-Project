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
#include "HeatSeekerPickUp.h"

// Local Includes //
#include "Player.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
HeatSeekerPickUp::HeatSeekerPickUp(Utils::Transform _Transform, Utils::EANCHOR _Anchor, std::shared_ptr<Entity> _CollidingEntity, float _fRespawnTime)
: PickUpBase(_Transform, _Anchor, _CollidingEntity, _fRespawnTime)
{
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
HeatSeekerPickUp::~HeatSeekerPickUp()
{
}

void HeatSeekerPickUp::OnPickUp(std::shared_ptr<Entity> CollidingEntity)
{
	PickUpBase::OnPickUp(CollidingEntity);

	ApplyPowerUpToPlayer(CollidingEntity, HEATSEEK, 4.0f);
	
}
