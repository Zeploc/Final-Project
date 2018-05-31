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

// Engine Includes //

#include "Engine\Time.h"


Enemy2::Enemy2(Utils::Transform _Transform, Utils::EANCHOR _Anchor)
	: Entity(_Transform, _Anchor)
{
}


Enemy2::~Enemy2()
{
}

void Enemy2::Update()
{
	if (Target) 
		transform.Position += AI::PursueForce(this->shared_from_this(), Target, PreviousPosition,30) *  m_fSpeed * (float)Time::dTimeDelta;

	PreviousPosition = Target->transform.Position;
}

void Enemy2::SetTarget(std::shared_ptr<Entity> _Target)
{
	Target = _Target;

	PreviousPosition = Target->transform.Position;
}
