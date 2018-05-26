//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Enem.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//
#pragma once
// Local Includes //
#include "Engine\Entity.h"

class Enemy1 :	public Entity
{
public:
	Enemy1(Utils::Transform _Transform, Utils::EANCHOR _Anchor);
	~Enemy1();

	void Update();

private:
	glm::vec3 m_v3CurrentVelocity = { 0, 0, 0 };

	std::shared_ptr<Entity> Target;
};

