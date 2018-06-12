//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    AI.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// OpenGL Library //
#include <glm\common.hpp>

// Engine Includes //
#include "Engine\Entity.h"

// Library Includes //
#include <memory>
#include <vector>

struct Path
{
	float fRadius = 0;
	std::vector<glm::vec3> v3Points;
};

#pragma once
class AI
{
public:
	AI();
	~AI();

	static void CleanUp();

	static glm::vec3 SeekForce(glm::vec3 Source, glm::vec3 Target, float fMass, glm::vec3 CurrentVelocity, float MaxSpeed);
	static glm::vec3 FleeForce(glm::vec3 Source, glm::vec3 Target, float fMass, glm::vec3 CurrentVelocity, float MaxSpeed);
	static glm::vec3 PursueForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target, glm::vec3 PreviousPosition, float ScaleFactor, float fMass, glm::vec3 CurrentVelocity, float MaxSpeed);
	static glm::vec3 EvadeForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target, glm::vec3 PreviousPosition, float ScaleFactor, float fMass, glm::vec3 CurrentVelocity, float MaxSpeed);
	static glm::vec3 SeekWithArrival(glm::vec3 Source, glm::vec3 Target, float fMass, glm::vec3 CurrentVelocity, float _fSlowingRange, float _fMaxSpeed);
	static glm::vec3 WanderForce(std::shared_ptr<Entity> Source, glm::vec3& TargetRef, glm::vec2 XRange, glm::vec2 ZRange, float& _fNextDecisionTime, float fMass, glm::vec3 CurrentVelocity, float MaxSpeed);
	static glm::vec3 pathFollowingForce(glm::vec3 Source, Path Currentpath, glm::vec3 CurrentVelocity, float fMass, float MaxSpeed);
	static glm::vec3 Seperation(std::shared_ptr<Entity> Source, float fCloseness, std::vector<std::shared_ptr<Entity>> Avoidables, float MaxSpeed);
	static glm::vec3 Align(std::shared_ptr<Entity> Source, float fRadius, std::vector<std::shared_ptr<Entity>> Avoidables, float MaxSpeed);
	static glm::vec3 Cohesion(std::shared_ptr<Entity> Source, float fRadius, std::vector<std::shared_ptr<Entity>> Avoidables, float MaxSpeed);
	static glm::vec3 ObstacleAvoidance(std::shared_ptr<Entity> Source, float MAX_SEE_AHEAD, glm::vec3 CurrentVelocity);
private:
	static glm::vec3 FindNormal(glm::vec3 Point, glm::vec3 LineStart, glm::vec3 LineEnd);

	static std::shared_ptr<Entity> TestPosition1;
	static std::shared_ptr<Entity> TestPosition2;
};

