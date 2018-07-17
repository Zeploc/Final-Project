// Library Includes //
#include <iostream>

// Engine Includes //
#include "Engine/Utils.h"
#include "Engine/SceneManager.h"
#include "Engine/Scene.h"
#include "Engine\Entity.h"
#include "Engine\Input.h"
#include "Engine\Time.h"
#include "Engine\Sphere.h"
#include "Engine\Model.h"
#include "Engine\Cube.h"
#include "Engine\CollisionBounds.h"

// Local Includes //
#include "Level.h"
#include "UIManager.h"
#include "Level.h"
#include "Boss.h"
#include "GameManager.h"
#include "LevelManager.h"
#include "AI.h"
#include "NetworkManager.h"

// This Includes //
#include "Bullet.h"

PlayerBullet::PlayerBullet(Utils::Transform _Transform, Utils::EANCHOR _Anchor, glm::vec3 Direction)
	: Entity(_Transform, _Anchor)
{
	CurrentVelocity = Direction * BulletSpeed;
	CurrentVelocity.y = 0;
	transform.Position.y = -2.0f;
}

PlayerBullet::~PlayerBullet()
{

}

void PlayerBullet::SetTrackingClosetEnemy()
{

	std::shared_ptr<Entity> CurrentClosestEnt;
	float fPreviousClosestDistance = 10000000.0f;
	for (auto& Enemiesit : LevelManager::GetInstance()->GetCurrentActiveLevel()->CurrentEnemies)
	{
		glm::vec3 LocationDifference = transform.Position - Enemiesit->transform.Position;
		float fDistance = abs(glm::length(LocationDifference)); // Distance from current avoidable
		if (fDistance < fPreviousClosestDistance)
		{
			CurrentClosestEnt = Enemiesit;
			fPreviousClosestDistance = fDistance;
		}
	}
	TrackingEntity = CurrentClosestEnt;
	bTracking = true;
}

void PlayerBullet::Update()
{
	// If tracking
	if (bTracking)
	{
		if (!TrackingEntity) // If tracking entity not set
		{
			SetTrackingClosetEnemy(); // Find new tracking enemy
		}
		if (TrackingEntity) // If tracking enemy is now set
		{
			// Seek tracking enemy
			CurrentVelocity += AI::SeekWithArrival(transform.Position, TrackingEntity->transform.Position, 20, CurrentVelocity, 1, BulletSpeed);
		}
	}

	// Add current celovity
	transform.Position += CurrentVelocity * (float)Time::dTimeDelta;

	// Count down run timer
	Timer -= (float)Time::dTimeDelta;

	// If timer has finished
	if (Timer <= 0)
	{
		// Destroy entity
		SceneManager::GetInstance()->GetCurrentScene()->DestroyEntity(this->shared_from_this());
	}

	// Loop through enemies
	bool bBackToStart = false;
	std::shared_ptr<Level> LevelRef = dynamic_pointer_cast<Level>(SceneManager::GetInstance()->GetCurrentScene());
	bool bEnemyKilled = true;
	while (bEnemyKilled)
	{
		// While loop to reset through for loop
		bEnemyKilled = false;
		if (LevelRef->CurrentEnemies.size() == 0) break;
		auto EnemyEnd = LevelRef->CurrentEnemies.end();
		for (auto Enemyit = LevelRef->CurrentEnemies.begin(); Enemyit != EnemyEnd; ++Enemyit)
		{
			// If colliding with enemy
			if (EntityMesh->GetCollisionBounds()->isColliding(*Enemyit))
			{
				std::shared_ptr<Boss> IsBoss = std::dynamic_pointer_cast<Boss>(*Enemyit);
				if (IsBoss)
				{
					IsBoss->OnBulletCollision();
					if (IsBoss->BossHealth <= 0 && IsBoss->IsActive())
					{
						bEnemyKilled = true;
					}
					LevelRef->DestroyCollidable(this->shared_from_this());
					TrackingEntity = nullptr;
					if (bEnemyKilled || LevelRef->CurrentEnemies.size() == 0) break;
				}
				else if ((*Enemyit)->IsActive())
				{					
					if (NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)
					{
						for (auto& NetworkEnt : NetworkManager::GetInstance()->m_Network.m_pNetworkEntity->NetworkEntities)
						{
							if (NetworkEnt.second == this->shared_from_this() || NetworkEnt.second == *Enemyit)
							{
								NetworkManager::GetInstance()->m_Network.m_pNetworkEntity->SendMessageNE(std::to_string(NetworkEnt.first), DESTROYENTITY);
							}
						}
					}
					
					LevelRef->DestroyCollidable(this->shared_from_this());
					// Send Destroy Bullet entity message to all clients
					LevelRef->DestroyEnemy((*Enemyit));
					// Send Destroy enemy message to all clients
					bEnemyKilled = true;
					TrackingEntity = nullptr;
					if (NetworkManager::GetInstance()->m_Network.m_pNetworkEntity) NetworkManager::GetInstance()->m_Network.m_pNetworkEntity->PlayerEntities[UserOwner]->AddScore(10);
					else LevelRef->EPlayer->AddScore(10);
					if (bEnemyKilled) break;
				}
			}
		}
	}

}

