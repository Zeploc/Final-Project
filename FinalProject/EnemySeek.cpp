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

// Engine Includes //

#include "Engine\Time.h"


EnemySeek::EnemySeek(Utils::Transform _Transform, Utils::EANCHOR _Anchor)
	: Entity(_Transform, _Anchor)
{
}


EnemySeek::~EnemySeek()
{
}

void EnemySeek::Update()
{	
	if (Target)
		m_v3CurrentVelocity += AI::SeekWithArrival(transform.Position, Target->transform.Position, 50, m_v3CurrentVelocity, 3.0f, m_fSpeed);
	
	transform.Position += m_v3CurrentVelocity * (float)Time::dTimeDelta;
}

