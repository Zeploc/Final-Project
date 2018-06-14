//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Enemy2.cpp
// Description    	:    Enemy AI using AI Systems
// Author       	:    Jonty Hoekstra
// Mail         	:    Jonty.hoe7674@mediadesign.school.nz
//

// This Includes //
#include "Enemy2.h"

// Local Includes //
#include "AI.h"
#include "Level.h"
#include "Player.h"

// Engine Includes //
#include "Engine\SceneManager.h"
#include "Engine\Scene.h"
#include "Engine\Entity.h"
#include "Engine\Cube.h"
#include "Engine\Time.h"
#include "Engine\CollisionBounds.h"


Enemy2::Enemy2(Utils::Transform _Transform, Utils::EANCHOR _Anchor)
	: Entity(_Transform, _Anchor)
{
}

Enemy2::~Enemy2()
{
}

void Enemy2::Update()
{
	if (!bActive) return;
	Entity::Update();
	std::shared_ptr<Level> LevelRef = std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->GetCurrentScene());
	if (Target)
	{
		std::vector<std::shared_ptr<Entity>> Avoidables = std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->GetCurrentScene())->CurrentEnemies;
		m_v3CurrentVelocity += AI::SeekForce(transform.Position, Target->transform.Position, 10.0f, m_v3CurrentVelocity, m_fSpeed);
		m_v3CurrentVelocity += AI::Seperation(this->shared_from_this(), 1.0f, Avoidables, m_fSpeed) * 7.0f;
		m_v3CurrentVelocity += AI::Align(this->shared_from_this(), 5.0f, Avoidables, m_fSpeed) * 1.0f;
		m_v3CurrentVelocity += AI::Cohesion(this->shared_from_this(), 5.0f, Avoidables, m_fSpeed)*0.1f;
	}

	if (glm::length(m_v3CurrentVelocity) > m_fSpeed)
	{
		m_v3CurrentVelocity = glm::normalize(m_v3CurrentVelocity) * m_fSpeed;
	}
	transform.Position += m_v3CurrentVelocity * (float)Time::dTimeDelta;

	if (!EntityMesh) // No mesh added
		return;
	if (!EntityMesh->GetCollisionBounds()) // No collision Bounds added
		return;


	float fDistance = abs(glm::length(transform.Position - LevelRef->EPlayer->transform.Position));
	if (EntityMesh->GetCollisionBounds()->isColliding(LevelRef->EPlayer) || fDistance <= 1.0f)
	{
		LevelRef->EPlayer->HurtPlayer(15);
	}
}

void Enemy2::SetTarget(std::shared_ptr<Entity> _Target)
{
	Target = _Target;

	PreviousPosition = Target->transform.Position;
}

