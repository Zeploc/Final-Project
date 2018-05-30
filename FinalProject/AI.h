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

#pragma once
class AI
{
public:
	AI();
	~AI();

	static glm::vec3 SeekDirection(std::shared_ptr<Entity> Source, glm::vec3 Target);
	static glm::vec3 FleeForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target);
	static glm::vec3 PursueForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target, glm::vec3 PreviousPosition, float ScaleFactor);
	static glm::vec3 EvadeForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target);
	static glm::vec3 ArrivalForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target);

private:
	static glm::vec3 FindFutureLocation(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target, float _fScaleFactor, float _fVelTarget);
};

