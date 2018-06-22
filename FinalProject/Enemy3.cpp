//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Enemy3.cpp
// Description    	:    Enemy AI using AI Systems
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "Enemy3.h"

// Local Includes //
#include "AI.h"
#include "Level.h"
#include "LevelManager.h"
#include "Player.h"
#include "NetworkManager.h"

// Engine Includes //
#include "Engine\CollisionBounds.h"
#include "Engine\Time.h"

#include <iostream>

Enemy3::Enemy3(Utils::Transform _Transform, Utils::EANCHOR _Anchor)
	: Entity(_Transform, _Anchor)
{
	PreviousPosition = transform.Position;
}


Enemy3::~Enemy3()
{
}

void Enemy3::Update()
{
	if (!bActive) return;
	Entity::Update();
	std::shared_ptr<Level> LevelRef = LevelManager::GetInstance()->GetCurrentActiveLevel();

	if (Target) m_v3CurrentVelocity += AI::PursueForce(this->shared_from_this(), Target, PreviousPosition, 10, 30, m_v3CurrentVelocity, m_fSpeed);

	m_v3CurrentVelocity += AI::Seperation(this->shared_from_this(), 1.5f, LevelRef->CurrentEnemies, 5);
	transform.Position += m_v3CurrentVelocity * (float)Time::dTimeDelta;
	PreviousPosition = Target->transform.Position;


	if (!EntityMesh) // No mesh added
		return;
	if (!EntityMesh->GetCollisionBounds()) // No collision Bounds added
		return;

	float fDistance = abs(glm::length(transform.Position - LevelRef->EPlayer->transform.Position));
	if (NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)
	{
		for (auto& PlayerIt : NetworkManager::GetInstance()->m_Network.m_pNetworkEntity->PlayerEntities)
		{
			float fCurrentDistance = abs(glm::length(transform.Position - PlayerIt.second->transform.Position));
			if (EntityMesh->GetCollisionBounds()->isColliding(PlayerIt.second) || fCurrentDistance <= 1.0f)
			{
				PlayerIt.second->HurtPlayer(15);
				std::cout << PlayerIt.first + " player has been hit by persuit enemy\n";
				break;
			}
		}
	}
	else if (EntityMesh->GetCollisionBounds()->isColliding(LevelRef->EPlayer) || fDistance <= 1.0f)
	{
		LevelRef->EPlayer->HurtPlayer(15);
	}
}

