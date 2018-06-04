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
#include "AI.h"

// OpenGL Includes //
#include <glm/gtc/matrix_transform.hpp>

// Engine Includes //

#include "Engine\Time.h"

Enemy1::Enemy1(Utils::Transform _Transform, Utils::EANCHOR _Anchor, glm::vec3 InitialVelocity)
	: Entity(_Transform, _Anchor)
{
	m_v3CurrentVelocity = InitialVelocity;
	m_fSpeed = glm::length(m_v3CurrentVelocity);
}


Enemy1::~Enemy1()
{
}

void Enemy1::Update()
{
	if (Target)
		m_v3CurrentVelocity += AI::SeekForce(transform.Position, Target->transform.Position, 100.0f, m_v3CurrentVelocity, m_fSpeed);
	
	transform.Position += m_v3CurrentVelocity * (float)Time::dTimeDelta;
}
