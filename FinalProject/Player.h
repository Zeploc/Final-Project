
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name		:    Camera.h
// Description		:    Header file outlining the Class
// Author			:    Alex Coultas
// Mail				:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <vector>
// OpenGL Library Includes //

// Engine Includes //
#include "Engine\Entity.h"

// Local Includes //
#include "GameSettings.h"

// This Includes //


// Static Variables //

enum POWERUPS
{
	NONE,
	SPEEDBOOST,
	FIRERATE,
	HEATSEEK
};

#pragma once
class Player : public Entity
{
public:
	struct Bullet
	{
		std::shared_ptr<Entity> BulletEntity;
		std::shared_ptr<Entity> TrackingEntity;
		glm::vec3 CurrentVelocity;
		float Timer = 8.0f;
		bool bTracking = false;
	};

	std::vector<Bullet> Bullets;

	Player(Utils::Transform _Transform, float _fWidth, float _fHeight, float _fDepth, Utils::EANCHOR _Anchor, glm::vec4 _Colour);
	Player(Utils::Transform _Transform, float _fWidth, float _fHeight, float _fDepth, Utils::EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, glm::vec4 UVCoords);
	~Player();

	void Update();
	glm::vec3 GetVelocity() { return v3Speed; };
	void ChangeMoveSpeed(float _fSpeed) { m_fCurrentPlayerSpeed = _fSpeed; };

	void HurtPlayer(float Damage);
	void Reset();

	void SetHealth(float _fNewHealth);
	void ApplyHealth(float _fmodify);
	void AddScore(int _iAddScore);
	void SetScore(int _iNewScore);
	int GetScore() { return m_iScore; };

	void ApplyPowerUp(POWERUPS _PowerUp, float _fPowerUpTime);
	bool FireRatePickup = false;
	bool bSeeking = false;

	// Score and Health
	float m_fHealth = 100;
	int m_iScore = 0;
	float m_fLastHurt = 0.0f;

	std::string m_UserName;

private:
	// Bullets	
	float BulletSpeed = 30.0f;
	void HandleBullets();
	void SetTrackingClosetEnemy(Bullet& _Bullet);

	// Movement
	void MoveHorizontally(bool bLeft);
	void MoveVertical(bool bUp);
	float RollTimer = 0.8f;
	float BulletTimer = 0.12f;
	float DodgeCooldown;
	glm::vec3 v3Speed = { 0, 0, 0 };
	float m_fCurrentPlayerSpeed = GameSettings::fMoveSpeed;
	bool bHasDodged = false;
	bool bJump = false;

	

	//Power Ups
	void PowerUpComplete();
	POWERUPS CurrentPowerUp = NONE;
	float m_fPowerUpTimer = 0.0f;
};

