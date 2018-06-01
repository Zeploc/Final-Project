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
#include "Utils.h"

// OpenGL Library //
#include <glm\common.hpp>
struct Transform;
#pragma once
class CollisionBounds
{
public:
	CollisionBounds(float fHeight, float fWidth, float fDepth, std::shared_ptr<Entity> _EntityRef);
	~CollisionBounds();

	void SetOffset(glm::vec3 _NewOffset) { v3Offset = _NewOffset; };
	bool isColliding(std::shared_ptr<Entity> Entity2);
	bool CheckCollision(std::shared_ptr<Entity> Entity2, glm::vec3 Movement);

private:
	float fHeight;
	float fWidth;
	float fDepth;
	glm::vec3 v3Offset = glm::vec3();

	std::shared_ptr<Entity> EntityRef;

};

