#pragma once
#include "Engine\Entity.h"
class Boss : public Entity
{
public:
	Boss(Utils::Transform _Transform, Utils::EANCHOR _Anchor);
	~Boss();
	void Update();
	void OnBulletCollision();
	float BossHealth = 10.0f;
private:
	float m_fOscillatingHeight;
	float OccillationSpeed = 300;
	float BobbingSpeed = 5;
	
	glm::vec3 TargetRef;
	float fNextDecisionTime = 0.0f;

	glm::vec3 CurrentVelocity;
	float fMaxSpeed;
};

