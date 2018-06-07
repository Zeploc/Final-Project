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

std::shared_ptr<Entity> AI::TestPosition1 = nullptr;
std::shared_ptr<Entity> AI::TestPosition2 = nullptr;

AI::AI()
{
}


AI::~AI()
{
}

glm::vec3 AI::SeekForce(glm::vec3 Source, glm::vec3 Target, float fMass, glm::vec3 CurrentVelocity, float MaxSpeed)
{
	glm::vec3 LookAtDirection =  Target - Source;
	
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
	// Fix tho
	glm::vec3 DesiredVelocity = LookAtDirection * MaxSpeed;
	glm::vec3 Steering = CurrentVelocity - DesiredVelocity;
	Steering /= fMass;
	return (Steering);
}

glm::vec3 AI::PursueForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target, glm::vec3 PreviousPosition, float ScaleFactor, float fMass, glm::vec3 CurrentVelocity, float MaxSpeed)
{	
	//glm::vec3 TargetVelocity = SeekForce(PreviousPosition, Target->transform.Position, fMass, CurrentVelocity, MaxSpeed);
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

glm::vec3 AI::SeekWithArrival(std::shared_ptr<Entity> Source, glm::vec3 Target, float _fSlowingRange, float _fMaxSpeed)
{
	glm::vec3 v3SeekDirection = SeekForce(Source->transform.Position, Target, 1, { 1, 0 ,0 }, 5);
	float fDistanceToTarget = abs(glm::length((Target - Source->transform.Position)));
	if (fDistanceToTarget < _fSlowingRange)
	{
		return v3SeekDirection * _fMaxSpeed * (fDistanceToTarget / _fSlowingRange);
	}
	return v3SeekDirection * _fMaxSpeed;
}

glm::vec3 AI::WanderForce(std::shared_ptr<Entity> Source, glm::vec3& TargetRef, glm::vec2 XRange, glm::vec2 ZRange, float& _fNextDecisionTime, float fMass, glm::vec3 CurrentVelocity, float MaxSpeed)
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

	return SeekForce(Source->transform.Position, TargetRef, fMass, CurrentVelocity, MaxSpeed);
}

glm::vec3 AI::pathFollowingForce(glm::vec3 Source, Path Currentpath, glm::vec3 CurrentVelocity, float fMass, float MaxSpeed)
{
	if (Currentpath.v3Points.size() == 0) return glm::vec3();
	glm::vec3 VelocityDirection = glm::normalize(CurrentVelocity);
	glm::vec3 PredictPosition = VelocityDirection * Currentpath.fRadius;
	PredictPosition += Source;
	
	glm::vec3 NormalPosition = FindNormal(PredictPosition, Currentpath.v3Points[0], Currentpath.v3Points[1]);
	float Distance = 9999999999.0f;
	glm::vec3 PathDirection = glm::normalize(Currentpath.v3Points[0] - Currentpath.v3Points.back());
	for (int i = 0; i < Currentpath.v3Points.size(); i++)
	{
		int SecondPos = i + 1;
		if (SecondPos > Currentpath.v3Points.size() - 1) SecondPos = 0;
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
	
	//if (Distance > Currentpath.fRadius)
	//{	
	//	return SeekForce(Source, NormalPosition + PathDirection * 1.0f, fMass, CurrentVelocity, MaxSpeed);
	//
	//}

}

glm::vec3 AI::FindFutureLocation(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target, float _fScaleFactor, float _fVelTarget)
{
	return glm::vec3();
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
