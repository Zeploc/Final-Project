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
// Engine Includes //
#include "Engine\CollisionBounds.h"
#include "Engine\Time.h"


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
	Entity::Update();
	std::shared_ptr<Level> LevelRef = LevelManager::GetInstance()->GetCurrentActiveLevel();

	m_v3CurrentVelocity += AI::PursueForce(this->shared_from_this(), Target, PreviousPosition, 10, 30, m_v3CurrentVelocity, m_fSpeed);

	m_v3CurrentVelocity += AI::Seperation(this->shared_from_this(), 1.5f, LevelRef->CurrentEnemies, 5);
	transform.Position += m_v3CurrentVelocity * (float)Time::dTimeDelta;
	PreviousPosition = Target->transform.Position;


	if (!EntityMesh) // No mesh added
		return;
	if (!EntityMesh->GetCollisionBounds()) // No collision Bounds added
		return;


	if (EntityMesh->GetCollisionBounds()->isColliding(LevelRef->EPlayer))
	{
		LevelRef->EPlayer->HurtPlayer(15);
	}
}

