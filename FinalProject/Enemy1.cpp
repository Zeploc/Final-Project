//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Enemy1.cpp
// Description    	:    Enemy AI using AI Systems
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "Enemy1.h"

// Local Includes //
#include "Level.h"
#include "Player.h"
#include "NetworkManager.h"

// OpenGL Includes //
#include <glm/gtc/matrix_transform.hpp>

// Engine Includes //
#include "Engine\SceneManager.h"
#include "Engine\Scene.h"
#include "Engine\Entity.h"
#include "Engine\Cube.h"
#include "Engine\Time.h"
#include "Engine\CollisionBounds.h"


Enemy1::Enemy1(Utils::Transform _Transform, Utils::EANCHOR _Anchor, glm::vec3 InitialVelocity)
	: Entity(_Transform, _Anchor)
{
	m_v3CurrentVelocity = InitialVelocity;
	m_fSpeed = glm::length(m_v3CurrentVelocity);
	if (m_fSpeed == 0) m_fSpeed = 1;
	CurrentPath.v3Points.push_back({-5, -2.5, 0});
	CurrentPath.v3Points.push_back({ 30, -2.5, 5 });
	CurrentPath.v3Points.push_back({ 30, -2.5, 30 });
	CurrentPath.v3Points.push_back({ 5, -2.5, 30 });
	CurrentPath.fRadius = 1.0f;
}


Enemy1::~Enemy1()
{
}

void Enemy1::AddPathPoints()
{
	for (unsigned int i = 0; i < CurrentPath.v3Points.size(); i++)
	{
		std::shared_ptr<Entity> NewPoint = std::make_shared<Entity>(Entity({ CurrentPath.v3Points[i] , glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f) }, Utils::CENTER));
		std::shared_ptr<Cube> CubeMesh = std::make_shared<Cube>(Cube(0.5f, 1.5f, 0.5f, glm::vec4(0.1f, 0.8f, 0.1f, 1.0f)));
		CubeMesh->SetLit(true);
		NewPoint->AddMesh(CubeMesh);
		SceneManager::GetInstance()->GetCurrentScene()->AddEntity(NewPoint);
		std::shared_ptr<Entity> Width = std::make_shared<Entity>(Entity({ CurrentPath.v3Points[i] , glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f) }, Utils::CENTER));
		std::shared_ptr<Cube> RadiusWidth = std::make_shared<Cube>(Cube(CurrentPath.fRadius, 0.5f, CurrentPath.fRadius, glm::vec4(0.6f, 0.2f, 0.1f, 1.0f)));
		RadiusWidth->SetLit(true);
		Width->AddMesh(RadiusWidth);
		SceneManager::GetInstance()->GetCurrentScene()->AddEntity(Width);
	}
}

void Enemy1::Update()
{
	if (!bActive) return;
	Entity::Update();
	std::shared_ptr<Level> LevelRef = std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->GetCurrentScene());

	std::vector<std::shared_ptr<Entity>> Avoidables = std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->GetCurrentScene())->CurrentEnemies;
	m_v3CurrentVelocity += AI::Align(this->shared_from_this(), 20.0f, Avoidables, m_fSpeed) * 0.2f;
	m_v3CurrentVelocity += AI::Cohesion(this->shared_from_this(), 10.0f, Avoidables, m_fSpeed) * 0.1f;
	m_v3CurrentVelocity += AI::Seperation(this->shared_from_this(), 1.0f, Avoidables, m_fSpeed) * 10.0f;
	m_v3CurrentVelocity += AI::pathFollowingForce(transform.Position, CurrentPath, m_v3CurrentVelocity, 10.0f, m_fSpeed) * 7.0f;
	
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
				break;
			}
		}
	}
	else if (EntityMesh->GetCollisionBounds()->isColliding(LevelRef->EPlayer) || fDistance <= 1.0f)
	{
		LevelRef->EPlayer->HurtPlayer(15);
	}

	if (glm::length(m_v3CurrentVelocity) > m_fSpeed)
	{
		m_v3CurrentVelocity = glm::normalize(m_v3CurrentVelocity) * m_fSpeed;
	}
	transform.Position += m_v3CurrentVelocity * (float)Time::dTimeDelta;
}
