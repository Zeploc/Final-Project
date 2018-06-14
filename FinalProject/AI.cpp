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
#include <glm\gtx\string_cast.hpp>

// Engine Includes //
#include "Engine\Time.h"
#include "Engine\Sphere.h"
#include "Engine\SceneManager.h"
#include "Engine\Scene.h"
// Library Includes //
#include <iostream>

// Local Includes //
#include "Enemy1.h"
#include "Enemy2.h"
#include "EnemySeek.h"

std::shared_ptr<Entity> AI::TestPosition1 = nullptr;
std::shared_ptr<Entity> AI::TestPosition2 = nullptr;

AI::AI()
{
}


AI::~AI()
{
}

void AI::CleanUp()
{
	TestPosition1 = nullptr;
	TestPosition2 = nullptr;
}

glm::vec3 AI::SeekForce(glm::vec3 Source, glm::vec3 Target, float fMass, glm::vec3 CurrentVelocity, float MaxSpeed)
{
	glm::vec3 LookAtDirection =  Target - Source;
	LookAtDirection.y = 0;
	
	if (glm::length(LookAtDirection) != 0) // In case vector is zero vector (Can't normalise zero vector)
	{
		LookAtDirection = glm::vec3(glm::normalize(LookAtDirection));
	}	
	
	glm::vec3 DesiredVelocity = LookAtDirection * MaxSpeed;
	glm::vec3 Steering = DesiredVelocity - CurrentVelocity;
	Steering /= fMass;
	return (Steering);
}

glm::vec3 AI::FleeForce(glm::vec3 Source, glm::vec3 Target, float fMass, glm::vec3 CurrentVelocity, float MaxSpeed)
{
	glm::vec3 LookAtDirection = Target - Source; 
	if (glm::length(LookAtDirection) != 0) // In case vector is zero vector (Can't normalise zero vector)
	{
		LookAtDirection = glm::vec3(glm::normalize(LookAtDirection));
	}
	glm::vec3 DesiredVelocity = LookAtDirection * MaxSpeed;
	glm::vec3 Steering = CurrentVelocity - DesiredVelocity;
	Steering /= fMass;
	return (Steering);
}

glm::vec3 AI::PursueForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target, glm::vec3 PreviousPosition, float ScaleFactor, float fMass, glm::vec3 CurrentVelocity, float MaxSpeed)
{	
	if (glm::length(Source->transform.Position - Target->transform.Position) < ScaleFactor / 1.5f)
	{
		return SeekForce(Source->transform.Position, Target->transform.Position, fMass, CurrentVelocity, MaxSpeed);
	}
	//ScaleFactor = glm::length(Source->transform.Position - Target->transform.Position) * MaxSpeed;
	glm::vec3 TargetDirection = Target->transform.Position - PreviousPosition;
	if (glm::length(TargetDirection) != 0) // In case vector is zero vector (Can't normalise zero vector)
	{
		TargetDirection = glm::vec3(glm::normalize(TargetDirection));
	}
	glm::vec3 FuturePosition = TargetDirection * ScaleFactor + Target->transform.Position; //Future position

	return SeekForce(Source->transform.Position, FuturePosition, fMass, CurrentVelocity, MaxSpeed);
}

glm::vec3 AI::EvadeForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target, glm::vec3 PreviousPosition, float ScaleFactor, float fMass, glm::vec3 CurrentVelocity, float MaxSpeed)
{
	glm::vec3 TargetVelocity = SeekForce(PreviousPosition, Target->transform.Position, fMass, CurrentVelocity, MaxSpeed);

	-TargetVelocity * ScaleFactor + Target->transform.Position; //Future position inverted

	return SeekForce(Source->transform.Position, -TargetVelocity * ScaleFactor + Target->transform.Position, fMass, CurrentVelocity, MaxSpeed);
}

glm::vec3 AI::SeekWithArrival(glm::vec3 Source, glm::vec3 Target, float fMass, glm::vec3 CurrentVelocity, float _fSlowingRange, float _fMaxSpeed)
{
	glm::vec3 v3SeekDirection = SeekForce(Source, Target, fMass, CurrentVelocity, _fMaxSpeed);
	float fDistanceToTarget = abs(glm::length((Target - Source)));
	if (fDistanceToTarget < _fSlowingRange)
	{
		return v3SeekDirection * (fDistanceToTarget / _fSlowingRange);
	}
	return v3SeekDirection;
}

glm::vec3 AI::WanderForce(std::shared_ptr<Entity> Source, glm::vec3& TargetRef, glm::vec2 XRange, glm::vec2 ZRange, float& _fNextDecisionTime, float fMass, glm::vec3 CurrentVelocity, float MaxSpeed)
{
	bool bIsAtTarget = abs(glm::length(Source->transform.Position - TargetRef)) < 0.2f;
	if (Time::dCurrentTime >= _fNextDecisionTime || bIsAtTarget)
	{
		_fNextDecisionTime = (float)Time::dCurrentTime + (rand() % 3000);
		float fXRangeSize = abs(XRange.x - XRange.y);
		float fZRangeSize = abs(ZRange.x - ZRange.y);
		float fNewPosX = (rand() % (int)(fXRangeSize * 100.0f)) / 100.0f;
		float fNewPosZ = (rand() % (int)(fZRangeSize * 100.0f)) / 100.0f;
		float fSmallestXPos = std::fmin(XRange.x, XRange.y);
		float fSmallestZPos = std::fmin(ZRange.x, ZRange.y);

		glm::vec3 NewTargetPos = { fSmallestXPos + fNewPosX, 0, fSmallestZPos + fNewPosZ };
		TargetRef = NewTargetPos;
	}

	return SeekForce(Source->transform.Position, TargetRef, fMass, CurrentVelocity, MaxSpeed);
}

glm::vec3 AI::pathFollowingForce(glm::vec3 Source, Path Currentpath, glm::vec3 CurrentVelocity, float fMass, float MaxSpeed)
{
	if (Currentpath.v3Points.size() == 0) return glm::vec3();
	glm::vec3 VelocityDirection = glm::length(CurrentVelocity) == 0 ? CurrentVelocity : glm::normalize(CurrentVelocity); // If is zero vector, don't normalize
	glm::vec3 PredictPosition = VelocityDirection * Currentpath.fRadius;
	PredictPosition += Source;
	
	glm::vec3 NormalPosition = FindNormal(PredictPosition, Currentpath.v3Points[0], Currentpath.v3Points[1]);
	float Distance = 9999999999.0f;
	glm::vec3 PathDirection = glm::normalize(Currentpath.v3Points[0] - Currentpath.v3Points.back());
	for (unsigned int i = 0; i < Currentpath.v3Points.size(); i++)
	{
		int SecondPos = i + 1;
		if ((unsigned)(SecondPos) > Currentpath.v3Points.size() - 1) SecondPos = 0;
		glm::vec3 StartPos = Currentpath.v3Points[i];
		glm::vec3 EndPos = Currentpath.v3Points[SecondPos];
		glm::vec3 CurrentNormalPosition = FindNormal(PredictPosition, StartPos, EndPos);
		float fCurrentDistance = abs(glm::length(PredictPosition - CurrentNormalPosition));
		float fDistanceFromPoint = abs(glm::length(Currentpath.v3Points[SecondPos] - Source));
		if (fCurrentDistance <= Distance && fDistanceFromPoint > Currentpath.fRadius * 2)
		{
			Distance = fCurrentDistance;
			NormalPosition = CurrentNormalPosition;
			if (abs(glm::length(Currentpath.v3Points[0] - Source)) < Currentpath.fRadius * 2 && SecondPos != 0)
				PathDirection = glm::normalize(Currentpath.v3Points[1] - Currentpath.v3Points[0]);
			else
				PathDirection = glm::normalize(EndPos - StartPos);
		}
	}
	
	return SeekForce(Source, NormalPosition + PathDirection * (Currentpath.fRadius * 2), fMass, CurrentVelocity, MaxSpeed);	
}

glm::vec3 AI::Seperation(std::shared_ptr<Entity> Source, float fCloseness, std::vector<std::shared_ptr<Entity>> Avoidables, float MaxSpeed)
{
	glm::vec3 AverageDirection = glm::vec3();
	int iCount = 0;
	for (auto& it : Avoidables)
	{
		if (it == Source) continue;
		glm::vec3 LocationDifference = Source->transform.Position - it->transform.Position;
		float fDistance = abs(glm::length(LocationDifference)); // Distance from current avoidable

		if (fDistance > 0 && fDistance < fCloseness) // If distance is too close
		{
			glm::vec3 DirectionAwayFromCurrent = glm::normalize(LocationDifference);
			AverageDirection += DirectionAwayFromCurrent;
			iCount++;

		}
		else if (fDistance == 0)
		{
			// Random Direction
			glm::vec2 RandomDirection = { rand() % 2000, rand() % 2000 };
			RandomDirection *= glm::vec2(0.001, 0.001);
			RandomDirection += glm::vec2(-1, -1);
			return glm::vec3(RandomDirection.x, 0, RandomDirection.y);
		}
	}

	if (iCount > 0)
	{
		float fDividend = 1.0f / (float)iCount;
		AverageDirection = glm::normalize(AverageDirection * fDividend);
		AverageDirection.y = 0;
	}
	else // None in range
		return AverageDirection;

	glm::vec3 NewVelocity = AverageDirection * MaxSpeed;
	
	return NewVelocity;
}

glm::vec3 AI::Align(std::shared_ptr<Entity> Source, float fRadius, std::vector<std::shared_ptr<Entity>> Avoidables, float MaxSpeed)
{
	glm::vec3 AverageDirection = glm::vec3();
	int iCount = 0;
	bool bIsEnemy1 = std::dynamic_pointer_cast<Enemy1>(Source) != nullptr;
	bool bIsEnemy2 = std::dynamic_pointer_cast<Enemy2>(Source) != nullptr;
	bool bIsEnemySeek = std::dynamic_pointer_cast<EnemySeek>(Source) != nullptr;
	for (auto& it : Avoidables)
	{
		if (it == Source) continue;
		glm::vec3 LocationDifference = it->transform.Position - Source->transform.Position;
		float fDistance = abs(glm::length(LocationDifference)); // Distance from current avoidable

		if (fDistance < fRadius) // If within allign radius
		{
			glm::vec3 DirectionAwayFromCurrent = glm::normalize(LocationDifference);
			std::shared_ptr<Enemy1> IsEnemy1 = std::dynamic_pointer_cast<Enemy1>(it);
			std::shared_ptr<Enemy2> IsEnemy2 = std::dynamic_pointer_cast<Enemy2>(it);
			std::shared_ptr<EnemySeek> IsEnemySeek = std::dynamic_pointer_cast<EnemySeek>(it);
			if (IsEnemy1 && bIsEnemy1)
			{
				if (glm::length(IsEnemy1->GetVelocity()) == 0) continue;
				AverageDirection += glm::normalize(IsEnemy1->GetVelocity());
			}
			else if (IsEnemy2 && bIsEnemy2)
			{
				if (glm::length(IsEnemy2->GetVelocity()) == 0) continue;
				AverageDirection += glm::normalize(IsEnemy2->GetVelocity());
			}
			else if (IsEnemySeek && bIsEnemySeek)
			{
				if (glm::length(IsEnemySeek->GetVelocity()) == 0) continue;
				AverageDirection += glm::normalize(IsEnemySeek->GetVelocity());
			}
			else
			{
				continue;
			}
			iCount++;
		}
	}

	if (iCount > 0)
	{
		float fDividend = 1.0f / (float)iCount;
		AverageDirection = glm::length(AverageDirection) == 0 ? AverageDirection : glm::normalize(AverageDirection * fDividend);
	}
	else // None in range
		return AverageDirection;

	glm::vec3 NewVelocity = AverageDirection * MaxSpeed;

	return NewVelocity;
}

glm::vec3 AI::Cohesion(std::shared_ptr<Entity> Source, float fRadius, std::vector<std::shared_ptr<Entity>> Avoidables, float MaxSpeed)
{
	glm::vec3 AverageTransform = glm::vec3();
	int iCount = 0;
	bool bIsEnemy1 = std::dynamic_pointer_cast<Enemy1>(Source) != nullptr;
	bool bIsEnemy2 = std::dynamic_pointer_cast<Enemy2>(Source) != nullptr;
	bool bIsEnemySeek = std::dynamic_pointer_cast<EnemySeek>(Source) != nullptr;
	for (auto& it : Avoidables)
	{
		if (it == Source) continue;
		glm::vec3 LocationDifference = it->transform.Position - Source->transform.Position;
		float fDistance = abs(glm::length(LocationDifference)); // Distance from current avoidable

		std::shared_ptr<Enemy1> IsEnemy1 = std::dynamic_pointer_cast<Enemy1>(it);
		std::shared_ptr<Enemy2> IsEnemy2 = std::dynamic_pointer_cast<Enemy2>(it);
		std::shared_ptr<EnemySeek> IsEnemySeek = std::dynamic_pointer_cast<EnemySeek>(it);

		if (fDistance < fRadius && ((IsEnemy1 && bIsEnemy1) || IsEnemy2 && bIsEnemy2 || IsEnemySeek && bIsEnemySeek)) // If within allign radius
		{
			glm::vec3 DirectionAwayFromCurrent = glm::normalize(LocationDifference);
			if (glm::length((it)->transform.Position) == 0) continue;
			AverageTransform += ((it)->transform.Position);
			iCount++;
		}
	}

	if (iCount > 0)
	{
		float fDividend = 1.0f / (float)iCount;
		AverageTransform = (AverageTransform * fDividend);
	}
	else // None in range
		return AverageTransform;

	std::shared_ptr<Enemy1> IsEnemy1 = std::dynamic_pointer_cast<Enemy1>(Source);
	std::shared_ptr<Enemy2> IsEnemy2 = std::dynamic_pointer_cast<Enemy2>(Source);
	std::shared_ptr<EnemySeek> IsEnemySeek = std::dynamic_pointer_cast<EnemySeek>(Source);
	glm::vec3 Velocity = glm::vec3();

	if (IsEnemy1)
	{
		Velocity = IsEnemy1->GetVelocity();
	}
	else if (IsEnemy2)
	{
		Velocity = IsEnemy2->GetVelocity();
	}
	else if (IsEnemySeek)
	{
		Velocity = IsEnemySeek->GetVelocity();
	}
	
	return SeekForce(Source->transform.Position, AverageTransform, 1, Velocity,MaxSpeed);
}

glm::vec3 AI::ObstacleAvoidance(std::shared_ptr<Entity> Source, float MAX_SEE_AHEAD, glm::vec3 CurrentVelocity)
{
	//glm::vec3 Ahead = Source->transform.Position + glm::normalize(CurrentVelocity) * MAX_SEE_AHEAD;
	glm::vec3 Ahead = Source->transform.Position + glm::normalize(CurrentVelocity) * MAX_SEE_AHEAD * 0.5f;

	return Ahead;
}

glm::vec3 AI::FindNormal(glm::vec3 Point, glm::vec3 LineStart, glm::vec3 LineEnd)
{
	glm::vec3 StartToPredict = Point - LineStart;
	glm::vec3 PathDirection = glm::normalize(LineEnd - LineStart);
	glm::vec3 StartToNormal = PathDirection * glm::dot(StartToPredict, PathDirection);
	glm::vec3 NormalPosition = LineStart + StartToNormal;

	glm::vec3 TopMost = LineStart.y > LineEnd.y ? LineStart : LineEnd;
	glm::vec3 RightMost = LineStart.x > LineEnd.x ? LineStart : LineEnd;
	glm::vec3 BottomMost = LineStart == TopMost ? LineEnd : LineStart;
	glm::vec3 LeftMost = LineStart == RightMost ? LineEnd : LineStart;

	if (NormalPosition.x < LeftMost.x)
	{
		return LeftMost;
	}
	else if (NormalPosition.x > RightMost.x)
	{
		return RightMost;
	}
	else if (NormalPosition.y < BottomMost.y)
	{
		return BottomMost;
	}
	else if(NormalPosition.y > TopMost.y)
	{
		return TopMost;
	}

	return NormalPosition;
}
