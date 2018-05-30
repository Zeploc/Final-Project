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


AI::AI()
{
}


AI::~AI()
{
}

glm::vec3 AI::SeekDirection(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target)
{
	glm::vec3 DesiredVelocity =  Target->transform.Position - Source->transform.Position;
	DesiredVelocity = glm::normalize(DesiredVelocity);
	
	
	return glm::vec3(DesiredVelocity);
}

glm::vec3 AI::FleeForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target)
{
	glm::vec3 DesiredVelocity =  Source->transform.Position - Target->transform.Position;
	DesiredVelocity = glm::normalize(DesiredVelocity);


	return glm::vec3(DesiredVelocity);
}

glm::vec3 AI::PursueForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target, float PredictionModifier)
{
	glm::vec3 DesiredVelocity = Source->transform.Position - Target->transform.Position;
	
	glm::vec3 Tracking = Target->transform.Position;

	DesiredVelocity += glm::normalize(Tracking);

	DesiredVelocity = glm::normalize(DesiredVelocity);



	return glm::vec3();
}

glm::vec3 AI::EvadeForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target)
{
	return glm::vec3();
}

glm::vec3 AI::ArrivalForce(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target)
{
	return glm::vec3();
}

glm::vec3 AI::FindFutureLocation(std::shared_ptr<Entity> Source, std::shared_ptr<Entity> Target, float _fScaleFactor, float _fVelTarget)
{
	return glm::vec3();
}
