#pragma once

// Engine Includes //
#include "Engine\Entity.h"

class PlayerBullet : public Entity
{
public:
	PlayerBullet(Utils::Transform _Transform, Utils::EANCHOR _Anchor, glm::vec3 Direction);
	~PlayerBullet();

	void SetTrackingClosetEnemy();

	void Update();

	std::string UserOwner;

private:
	float Timer = 8.0f;
	bool bTracking = false;
	float BulletSpeed = 30.0f;
	std::shared_ptr<Entity> TrackingEntity;
	glm::vec3 CurrentVelocity;
};

