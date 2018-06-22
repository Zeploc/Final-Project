//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    EnemySeek.cpp
// Description    	:    Enemy AI using AI Seek
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "EnemySeek.h"

// Local Includes //
#include "AI.h"
#include "Level.h"
#include "LevelManager.h"
#include "Player.h"
#include "NetworkManager.h"

// Engine Includes //
#include "Engine\Time.h"
#include "Engine\CollisionBounds.h"

// Library Includes //
#include <cassert>


EnemySeek::EnemySeek(Utils::Transform _Transform, Utils::EANCHOR _Anchor)
	: Entity(_Transform, _Anchor)
{

	
}


EnemySeek::~EnemySeek()
{
}

void EnemySeek::Update()
{
	if (!bActive) return;
	std::shared_ptr<Level> LevelRef = LevelManager::GetInstance()->GetCurrentActiveLevel();

	if (Target)
	{
		m_v3CurrentVelocity += AI::SeekWithArrival(transform.Position, Target->transform.Position, 50, m_v3CurrentVelocity, 5.0f, m_fSpeed);
		m_v3CurrentVelocity += AI::Seperation(this->shared_from_this(), 1.5f, LevelRef->CurrentEnemies, 5);
	}
		
	
	transform.Position += m_v3CurrentVelocity * (float)Time::dTimeDelta;

	float fDistance = abs(glm::length(transform.Position - LevelRef->EPlayer->transform.Position));
	if (NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)
	{
		for (auto& PlayerIt : NetworkManager::GetInstance()->m_Network.m_pNetworkEntity->PlayerEntities)
		{
			float fCurrentDistance = abs(glm::length(transform.Position - PlayerIt.second->transform.Position));
			if (EntityMesh->GetCollisionBounds()->isColliding(PlayerIt.second) || fCurrentDistance <= 1.0f)
			{
				PlayerIt.second->HurtPlayer(15);

				break;
			}
		}
	}
	else if (EntityMesh->GetCollisionBounds()->isColliding(LevelRef->EPlayer) || fDistance <= 1.0f)
	{
		LevelRef->EPlayer->HurtPlayer(15);
	}
}

