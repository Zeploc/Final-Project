//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    PickUpBase.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Engine Includes //
#include "Engine\Entity.h"

// Library Includes //
#include <memory>

// Library Includes //
#include <vector>
#pragma once
class PickUpBase : public Entity
{
public:
	PickUpBase(Utils::Transform _Transform, Utils::EANCHOR _Anchor, std::shared_ptr<Entity> _CollidingEntity, float _fRespawnTime = 3.0f);
	~PickUpBase();
	
	void Update();
	void Init();

	virtual void OnPickUp(std::shared_ptr<Entity> CollidingEntity);
	void RespawnPickup();

	float m_fRespawnTime;

protected:
	float m_fRespawnTimer;
	float m_fOscillatingHeight;

	std::vector<std::shared_ptr<Entity>> CollidingEntities;
	
};

