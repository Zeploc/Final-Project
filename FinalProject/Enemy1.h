//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Enemy.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//
#pragma once
// Local Includes //
#include "Engine\Entity.h"
#include "AI.h"

class Enemy1 :	public Entity
{
public:
	Enemy1(Utils::Transform _Transform, Utils::EANCHOR _Anchor, glm::vec3 InitialVelocity);
	~Enemy1();
	//std::shared_ptr<Entity> Target;

	void AddPathPoints();

	void Update();

	glm::vec3 GetVelocity() { return m_v3CurrentVelocity; };

private:

	float m_fSpeed = 4;
	Path CurrentPath;
	glm::vec3 m_v3CurrentVelocity = { 0, 0, 0 };	
};

