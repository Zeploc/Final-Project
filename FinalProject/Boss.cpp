
// Library Includes //
#include <memory.h>

// This Includes //
#include "Boss.h"


// Local Includes //
#include "AI.h"
#include "Level.h"
#include "Player.h"


// Engine Includes //
#include "Engine\SceneManager.h"
#include "Engine\Scene.h"
#include "Engine\Entity.h"
#include "Engine\Cube.h"
#include "Engine\Time.h"
#include "Engine\CollisionBounds.h"



Boss::Boss(Utils::Transform _Transform, Utils::EANCHOR _Anchor)
	: Entity(_Transform, _Anchor)
{
	BossState = WANDERSTATE;
	m_fOscillatingHeight = _Transform.Position.y;
	CurrentVelocity = { 0, 0, 10 };
	fMaxSpeed = glm::length(CurrentVelocity);
	TargetRef = { 0, 0, 0 };
	NextRushTime = Time::dCurrentTime/1000 + 15.0f;
}

Boss::~Boss()
{
}

void Boss::Update()
{
	Entity::Update();
	std::shared_ptr<Level> LevelRef = std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->GetCurrentScene());
<<<<<<< HEAD
	HitCooldown -= Time::dTimeDelta;
	if (EntityMesh->GetCollisionBounds()->isColliding(LevelRef->EPlayer))
	{
		if (HitCooldown <= 0)
		{
			LevelRef->EPlayer->HurtPlayer(25);
			HitCooldown = 1.3f;
		}
		
	}

=======
		
>>>>>>> 1dac37704edc2dc20280f3ad8dd3f762e27c8ebd
	if (bActive = false)
	{
		return;
	}

	if (!EntityMesh) // No mesh added
		return;
	if (!EntityMesh->GetCollisionBounds()) // No collision Bounds added
		return;

	if (EntityMesh->GetCollisionBounds()->isColliding(LevelRef->EPlayer))
	{
		LevelRef->EPlayer->HurtPlayer(25);
	}
	
	//Rotate({ 0, 10.0f * Time::dTimeDelta, 0 });
	//transform.Position.y = m_fOscillatingHeight + sin(Time::dCurrentTime / 200) * 20.0f * Time::dTimeDelta;
	if (Time::dCurrentTime/1000 >= NextRushTime)
	{
		NextRushTime = Time::dCurrentTime/1000 + 15.0f;
		BossState = RUSHSTATE;
		LevelRef = std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->GetCurrentScene());
		RushTarget = LevelRef->EPlayer->transform.Position;
		RushLength = 5.0f;
	}

	if (RushLength > 0)
	{
		RushLength -= Time::dTimeDelta;
	}
	else
	{
		RushLength = 0;
		BossState = WANDERSTATE;
	}
	
	float DistanceFromRushTarget = abs(glm::length(transform.Position - RushTarget));

	if (DistanceFromRushTarget <= 0.2f)
	{
		std::shared_ptr<Level> LevelRef = std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->GetCurrentScene());
		RushTarget = LevelRef->EPlayer->transform.Position;
	}

	if (BossState == RUSHSTATE)
	{
		RushTimer -= Time::dTimeDelta;
		if (RushTimer <= 0)
		{
			CurrentVelocity += AI::SeekForce(transform.Position, RushTarget, 1, CurrentVelocity, fMaxSpeed);
			RushTimer = 0.7f;
		}
		
	}

	else if (BossState == WANDERSTATE)
	{
		CurrentVelocity += AI::WanderForce(this->shared_from_this(), TargetRef, { 0, 30 }, { 0, 30 }, fNextDecisionTime, 100, CurrentVelocity, fMaxSpeed);
	}

	transform.Position += CurrentVelocity * (float)Time::dTimeDelta;

	
}

void Boss::OnBulletCollision()
{
	BossHealth -= 1;

	if (BossHealth <= 0)
	{
		/*std::shared_ptr<Level> LevelRef = dynamic_pointer_cast <Level>(SceneManager::GetInstance()->GetCurrentScene());*/
	}
}



