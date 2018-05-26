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

Enemy1::Enemy1(Utils::Transform _Transform, Utils::EANCHOR _Anchor)
	: Entity(_Transform, _Anchor)
{
}


Enemy1::~Enemy1()
{
}

void Enemy1::Update()
{
	if (Target) m_v3CurrentVelocity += AI::SeekForce(this->shared_from_this(), Target);
}
