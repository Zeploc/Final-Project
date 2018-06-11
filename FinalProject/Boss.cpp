
// Library Includes //
#include <memory.h>

// This Includes //
#include "Boss.h"


// Local Includes //
#include "AI.h"
#include "Level.h"


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
	m_fOscillatingHeight = _Transform.Position.y;
	CurrentVelocity = { 0, 0, 10 };
	fMaxSpeed = glm::length(CurrentVelocity);
	TargetRef = { 0, 0, 0 };
}


Boss::~Boss()
{
}

void Boss::Update()
{
	Entity::Update();

	if (!EntityMesh) // No mesh added
		return;
	if (!EntityMesh->GetCollisionBounds()) // No collision Bounds added
		return;

	//Rotate({ 0, 10.0f * Time::dTimeDelta, 0 });
	//transform.Position.y = m_fOscillatingHeight + sin(Time::dCurrentTime / 200) * 20.0f * Time::dTimeDelta;

	CurrentVelocity += AI::WanderForce(this->shared_from_this(), TargetRef, { 0, 30 }, { 0, 10 }, fNextDecisionTime, 100, CurrentVelocity, fMaxSpeed);
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
