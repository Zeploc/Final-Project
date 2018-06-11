
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
	SPEEDBOOST
};

#pragma once
class Player : public Entity
{
public:
	struct Bullet
	{
		std::shared_ptr<Entity> BulletEntity;
		glm::vec3 CurrentVelocity;
		float Timer = 8.0f;
	};

	std::vector<Bullet> Bullets;

	Player(Utils::Transform _Transform, float _fWidth, float _fHeight, float _fDepth, Utils::EANCHOR _Anchor, glm::vec4 _Colour);
	Player(Utils::Transform _Transform, float _fWidth, float _fHeight, float _fDepth, Utils::EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, glm::vec4 UVCoords);
	~Player();

	void Update();
	glm::vec2 GetVelocity() { return glm::vec2(fHSpeed, fVSpeed); };
	void ChangeMoveSpeed(float _fSpeed) { m_fCurrentPlayerSpeed = _fSpeed; };
	void Reset();

	void SetHealth(float _fNewHealth);
	void ApplyHealth(float _fmodify);
	void AddScore(int _iAddScore);

	void ApplyPowerUp(POWERUPS _PowerUp, float _fPowerUpTime);

private:
	// Bullets
	
	float BulletSpeed = 30.0f;
	

	// Movement
	void MoveHorizontally(bool bLeft);
	void MoveVertical(bool bUp);
	float RollTimer = 0.8f;
	float BulletTimer = 0.12f;
	float DodgeCooldown;
	float fHSpeed = 0;
	float fVSpeed = 0;
	float m_fCurrentPlayerSpeed = GameSettings::fMoveSpeed;
	bool bHasDodged = false;
	bool bJump = false;

	// Score and Health
	float m_fHealth = 100;
	int m_iScore = 0;

	//Power Ups
	void PowerUpComplete();
	POWERUPS CurrentPowerUp = NONE;
	float m_fPowerUpTimer = 0.0f;
};

