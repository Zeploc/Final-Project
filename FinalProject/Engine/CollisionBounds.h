//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    CollisionBounds.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <memory>

// Engine Library //
#include "Entity.h"

// OpenGL Library //
#include <glm\common.hpp>

#pragma once
class CollisionBounds
{
public:
	CollisionBounds();
	~CollisionBounds();

	float fHeight;
	float fWidth;
	float fDepth;
	glm::vec3 v2Offset = glm::vec3();

	bool isColliding(std::shared_ptr<Entity> Entity1, std::shared_ptr<Entity> Entity2);
	bool CheckCollision(std::shared_ptr<Entity> Entity1, std::shared_ptr<Entity> Entity2, glm::vec3 Movement);

};

