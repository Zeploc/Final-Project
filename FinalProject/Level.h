//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name		:    Level.h
// Description		:    Header file outlining the Class
// Author			:    Alex Coultas
// Mail				:    alex.cou7417@mediadesign.school.nz
//

#pragma once

// Library Includes //
#include <memory>

// OpenGL Library Includes //

// Engine Includes //
#include "Engine\Scene.h"
#include "Engine\Entity.h"

// Local Includes //

// This Includes //

// Static Variables //

class Player;
class ParticleSystem;

class Level : public Scene
{
public:
	Level(std::string sSceneName);
	~Level();
	
	void Update();

	void AddCollidable(std::shared_ptr<Entity>);
	void AddEnemy(std::shared_ptr<Entity>);
	std::shared_ptr<Entity> AddCollidable(Utils::Transform _Transform, float _fWidth, float _fHeight, Utils::EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, int iCount, bool bHorizontal);
	void DestroyCollidable(std::shared_ptr<Entity>);

	void RespawnEnemies();
	void PlayRandomTrack();

	void OnLoadScene();

	std::shared_ptr<Player> EPlayer;

	std::shared_ptr<Entity> TempTarget;

	std::vector<std::shared_ptr<Entity>> Collidables;
	std::vector<std::shared_ptr<Entity>> Enemies;

	glm::vec2 SpawnPos = glm::vec2(0, -0.5f);

	std::shared_ptr<UIText> ScoreText;
	std::shared_ptr<UIText> FPSCounterText;
	
	void RestartLevel();
	void SetPlayerPosition(glm::vec2 Pos);

	void AddPoints(int _Points) { iScore += _Points; };
	int GetTotalPoints() { return iScore; };

private:
	float fCameraSpeed = 5.0f;

	int iScore = 0;
	int iCoinsCollected = 0;
};

