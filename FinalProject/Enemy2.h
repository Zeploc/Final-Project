#pragma once
#include "d:\Users\Jonty.hoe7674\Desktop\Final-Project\FinalProject\Engine\Entity.h"
class Enemy2 : public Entity
{
public:
	Enemy2(Utils::Transform _Transform, Utils::EANCHOR _Anchor);
	~Enemy2();
	
	
	void Update();
	void SetTarget(std::shared_ptr<Entity> _Target);
private:

	std::shared_ptr<Entity> Target;
	glm::vec3 PreviousPosition;
	float m_fSpeed = 4;

	glm::vec3 m_v3CurrentVelocity = { 0, 0, 0 };


};

