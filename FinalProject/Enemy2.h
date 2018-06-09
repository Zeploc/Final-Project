//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Enemy2.h
// Description    	:    Header file outlining the Class
// Author       	:    Jonty Hoekstra
// Mail         	:    Jonty.hoe7674@mediadesign.school.nz
//

#pragma once
// Local Includes
#include "Engine\Entity.h"

class Enemy2 : public Entity
{
public:
	Enemy2(Utils::Transform _Transform, Utils::EANCHOR _Anchor);
	~Enemy2();
	
	
	void Update();
	void SetTarget(std::shared_ptr<Entity> _Target);
	glm::vec3 GetVelocity() { return m_v3CurrentVelocity; };
private:

	std::shared_ptr<Entity> Target;
	glm::vec3 PreviousPosition;

private:

	float m_fSpeed = 10;

	glm::vec3 m_v3CurrentVelocity = { 0, 0, 0 };


};

