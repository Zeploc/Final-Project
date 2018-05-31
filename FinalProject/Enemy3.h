//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Enemy3.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

#pragma once
// Local Includes
#include "Engine\Entity.h"

class Enemy3 : public Entity
{
public:
	Enemy3(Utils::Transform _Transform, Utils::EANCHOR _Anchor);
	~Enemy3();
	
	
	void Update();
private:
	float m_fDecisonTime = 0;
	glm::vec3 Target;
	float m_fSpeed = 4;

	glm::vec3 m_v3CurrentVelocity = { 0, 0, 0 };


};

