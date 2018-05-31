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
	transform.Position += AI::WanderDirection(this->shared_from_this(), Target, {-5, 5}, {-5, 5}, m_fDecisonTime) *  m_fSpeed * (float)Time::dTimeDelta;
}

