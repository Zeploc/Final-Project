
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

// OpenGL Library Includes //

// Engine Includes //
#include "Engine\Entity.h"

// Local Includes //
#include "GameSettings.h"

// This Includes //

// Static Variables //

#pragma once
class Player : public Entity
{
public:
	Player(Utils::Transform _Transform, float _fWidth, float _fHeight, float _fDepth, Utils::EANCHOR _Anchor, glm::vec4 _Colour);
	Player(Utils::Transform _Transform, float _fWidth, float _fHeight, float _fDepth, Utils::EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, glm::vec4 UVCoords);
	~Player();

	void Update();
	glm::vec2 GetVelocity() { return glm::vec2(fHSpeed, fVSpeed); };
	void ChangeMoveSpeed(float _fSpeed) { GameSettings::fMoveSpeed = _fSpeed; };
	void Reset();

private:

	void MoveHorizontally(bool bLeft);
	
	float fHSpeed = 0;
	float fVSpeed = 0;

	bool bJump = false;
};

