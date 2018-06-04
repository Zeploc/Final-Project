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

// Engine Includes //

#include "Engine\Time.h"


Enemy3::Enemy3(Utils::Transform _Transform, Utils::EANCHOR _Anchor)
	: Entity(_Transform, _Anchor)
{
}


Enemy3::~Enemy3()
{
}

void Enemy3::Update()
{	
	m_v3CurrentVelocity += AI::WanderForce(this->shared_from_this(), Target, { -5, 5 }, { -5, 5 }, m_fDecisonTime, 100.0f, m_v3CurrentVelocity, m_fSpeed);
	
	transform.Position += m_v3CurrentVelocity * (float)Time::dTimeDelta;
}

