#pragma once
#include "Engine\Entity.h"

enum BossStates
{
	WANDERSTATE,
	RUSHSTATE,

};
class Boss : public Entity

{
public:
	Boss(Utils::Transform _Transform, Utils::EANCHOR _Anchor);
	~Boss();
	void Update();
	void OnBulletCollision();
	void ResetHealth() { BossHealth = 10.0f; };

	float BossHealth = 100.0f;
private:
	float m_fOscillatingHeight;
	float OccillationSpeed = 300;
	float BobbingSpeed = 5;
	float NextRushTime;
	float RushLength;
	float RushTimer = 0.7f;
	float HitCooldown = 1.3f;
	float fNextDecisionTime = 0.0f;
	float fMaxSpeed;
	bool HasHit = false;
	BossStates BossState;

	std::shared_ptr<Entity> CollidingEntity;
	glm::vec3 RushTarget;
	glm::vec3 TargetRef;
	glm::vec3 CurrentVelocity;

	
};

