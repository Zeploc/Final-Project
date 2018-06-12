//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    EnemySeek.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

#pragma once
// Local Includes
#include "Engine\Entity.h"

// Library Includes //

class EnemySeek : public Entity
{
public:
	EnemySeek(Utils::Transform _Transform, Utils::EANCHOR _Anchor);
	~EnemySeek();
	
	void Update();

	std::shared_ptr<Entity> Target;

	glm::vec3 GetVelocity() { return m_v3CurrentVelocity; };
	
private:
	float m_fSpeed = 6;

	glm::vec3 m_v3CurrentVelocity = { 0, 0, 0 };

};

