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

// OpenGL Includes //
#include <glm/gtc/matrix_transform.hpp>

// Engine Includes //
#include "Engine\SceneManager.h"
#include "Engine\Scene.h"
#include "Engine\Entity.h"
#include "Engine\Cube.h"
#include "Engine\Time.h"

Enemy1::Enemy1(Utils::Transform _Transform, Utils::EANCHOR _Anchor, glm::vec3 InitialVelocity)
	: Entity(_Transform, _Anchor)
{
	m_v3CurrentVelocity = InitialVelocity;
	m_fSpeed = glm::length(m_v3CurrentVelocity);
	CurrentPath.v3Points.push_back({0, -2.5, 0});
	CurrentPath.v3Points.push_back({ 30, -2.5, 10 });
	CurrentPath.v3Points.push_back({ 10, -2.5, 20 });
	CurrentPath.fRadius = 1.0f;
}


Enemy1::~Enemy1()
{
}

void Enemy1::AddPathPoints()
{
	for (int i = 0; i < CurrentPath.v3Points.size(); i++)
	{
		std::shared_ptr<Entity> NewPoint = std::make_shared<Entity>(Entity({ CurrentPath.v3Points[i] , glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f) }, Utils::CENTER));
		std::shared_ptr<Cube> CubeMesh = std::make_shared<Cube>(Cube(0.5f, 1.5f, 0.5f, glm::vec4(0.1f, 0.8f, 0.1f, 1.0f)));
		NewPoint->AddMesh(CubeMesh);
		SceneManager::GetInstance()->GetCurrentScene()->AddEntity(NewPoint);
		std::shared_ptr<Entity> Width = std::make_shared<Entity>(Entity({ CurrentPath.v3Points[i] , glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f) }, Utils::CENTER));
		std::shared_ptr<Cube> RadiusWidth = std::make_shared<Cube>(Cube(CurrentPath.fRadius, 0.5f, CurrentPath.fRadius, glm::vec4(0.6f, 0.2f, 0.1f, 1.0f)));
		Width->AddMesh(RadiusWidth);
		SceneManager::GetInstance()->GetCurrentScene()->AddEntity(Width);
	}
}

void Enemy1::Update()
{
	if (Target)
		m_v3CurrentVelocity += AI::pathFollowingForce(transform.Position, CurrentPath, m_v3CurrentVelocity, 50.0f, m_fSpeed);
	
	transform.Position += m_v3CurrentVelocity * (float)Time::dTimeDelta;
}
