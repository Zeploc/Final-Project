//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    FireRatePickup.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

#pragma once
// Local Includes //
#include "PickUpBase.h"

class HeatSeekerPickUp : public PickUpBase
{
public:
	HeatSeekerPickUp(Utils::Transform _Transform, Utils::EANCHOR _Anchor, std::shared_ptr<Entity> _CollidingEntity, float _fRespawnTime = 3.0f);
	~HeatSeekerPickUp();

	void OnPickUp(std::shared_ptr<Entity> CollidingEntity);
};

