//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    AU.cpp
// Description    	:    Systems used in AI
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "AI.h"

// OpenGL includes //
#include <glm\matrix.hpp>

// Engine Includes //
#include "Engine\Time.h"


AI::AI()
{
}


AI::~AI()
{
}

glm::vec3 AI::SeekDirection(glm::vec3 Source, glm::vec3 Target)
{
	glm::vec3 DesiredVelocity =  Target - Source;
	if (0 != (DesiredVelocity.x || DesiredVelocity.y || DesiredVelocity.z)) // In case vector is zero vector (Can't normalise zero vector)
	{
		DesiredVelocity = glm::vec3(glm::normalize(DesiredVelocity));
	}	
	
	return glm::vec3(DesiredVelocity);
}

glm::vec3 AI::FleeDirection(glm::vec3 Source, glm::vec3 Target)
{
	glm::vec3 DesiredVelocity =  Source;
	if (0 != (DesiredVelocity.x || DesiredVelocity.y || DesiredVelocity.z)) // In case vector is zero vector (Can't normalise zero vector)
	{
		DesiredVelocity = glm::vec3(glm::normalize(DesiredVelocity));
	}

	return glm::vec3(DesiredVelocity);
}

glm::vec3 AI::PursueForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target, glm::vec3 PreviousPosition, float ScaleFactor)
{	
	glm::vec3 TargetVelocity = SeekDirection(PreviousPosition, Target->transform.Position);

	TargetVelocity * ScaleFactor + Target->transform.Position; //Future position

	return SeekDirection(Source->transform.Position, TargetVelocity * ScaleFactor + Target->transform.Position);

}

glm::vec3 AI::EvadeForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target, glm::vec3 PreviousPosition, float ScaleFactor)
{
	glm::vec3 TargetVelocity = SeekDirection(PreviousPosition, Target->transform.Position);

	-TargetVelocity * ScaleFactor + Target->transform.Position; //Future position inverted

	return SeekDirection(Source->transform.Position, -TargetVelocity * ScaleFactor + Target->transform.Position);
}

glm::vec3 AI::SeekWithArrival(std::shared_ptr<Entity> Source, glm::vec3 Target, float _fSlowingRange, float _fMaxSpeed)
{
	glm::vec3 v3SeekDirection = SeekDirection(Source->transform.Position, Target);
	float fDistanceToTarget = abs(glm::length((Target - Source->transform.Position)));
	if (fDistanceToTarget < _fSlowingRange)
	{
		return v3SeekDirection * _fMaxSpeed * (fDistanceToTarget / _fSlowingRange);
	}
	return v3SeekDirection * _fMaxSpeed;
}

glm::vec3 AI::WanderDirection(std::shared_ptr<Entity> Source, glm::vec3& TargetRef, glm::vec2 XRange, glm::vec2 ZRange, float& _fNextDecisionTime)
{
	bool bIsAtTarget = abs(glm::length(Source->transform.Position - TargetRef)) < 0.2f;
	if (Time::dCurrentTime >= _fNextDecisionTime || bIsAtTarget)
	{
		_fNextDecisionTime = Time::dCurrentTime + (rand() % 3000);
		float fXRangeSize = abs(XRange.x - XRange.y);
		float fZRangeSize = abs(ZRange.x - ZRange.y);
		float fNewPosX = (rand() % (int)(fXRangeSize * 100.0f)) / 100.0f;
		float fNewPosZ = (rand() % (int)(fZRangeSize * 100.0f)) / 100.0f;
		float fSmallestXPos = std::fmin(XRange.x, XRange.y);
		float fSmallestZPos = std::fmin(ZRange.x, ZRange.y);

		glm::vec3 NewTargetPos = { fSmallestXPos + fNewPosX, 0, fSmallestZPos + fNewPosZ };
		TargetRef = NewTargetPos;
	}

	return SeekDirection(Source->transform.Position, TargetRef);
}

glm::vec3 AI::FindFutureLocation(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target, float _fScaleFactor, float _fVelTarget)
{
	return glm::vec3();
}
