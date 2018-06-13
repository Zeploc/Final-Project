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
//#include "Model.h"

// This Includes //

// Static Variables //

class Player;
class Spectator;
class ParticleSystem;

class Level : public Scene
{
public:
	Level(std::string sSceneName);
	~Level();
	
	void Update();
	void RenderScene();

	void AddCollidable(std::shared_ptr<Entity>);
	std::shared_ptr<Entity> AddCollidable(Utils::Transform _Transform, float _fWidth, float _fHeight, Utils::EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, int iCount, bool bHorizontal);
	void DestroyCollidable(std::shared_ptr<Entity>);
	void AddHexPlatform(std::string _ModelPath, glm::vec3 _v3Postion, glm::vec3 Rotation);
	
	void AddEnemy(std::shared_ptr<Entity>);
	void AddTempEnemy(std::shared_ptr<Entity> NewEnemy);

	void DestroyAllEnemies();
	void RespawnEnemies();
	void PlayRandomTrack();

	void OnLoadScene();

	std::shared_ptr<Entity> CurrentController;
	std::shared_ptr<Player> EPlayer;
	std::shared_ptr<Spectator> ESpectator;

	std::shared_ptr<Entity> TempTarget;
	std::shared_ptr<Entity> TargetRef;
	std::shared_ptr<Entity> Enemy1Ref;
	std::shared_ptr<Entity> MouseAimTarget;
	std::shared_ptr<Entity> BossRef;


	std::vector<std::shared_ptr<Entity>> Collidables;
	std::vector<std::shared_ptr<Entity>> CurrentEnemies;

	glm::vec3 SpawnPos;

	std::shared_ptr<UIText> ScoreText;
	std::shared_ptr<UIText> FPSCounterText;
	
	void RestartLevel();
	void SetPlayerPosition(glm::vec3 Pos);

	void AddPoints(int _Points) { iScore += _Points; };
	int GetTotalPoints() { return iScore; };

	//Model* TestModel;

private:

	int iScore = 0;
	int iCoinsCollected = 0;

	glm::vec3 camVel = glm::vec3();

	std::vector<std::shared_ptr<Entity>> EnemiesTemplate;

	void CameraMovement();
};

