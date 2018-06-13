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
#include <memory>

// OpenGL Library Includes //

// Engine Includes //

// Local Includes //
#include "Level.h"

// This Includes //

// Static Variables //

enum RoundTimes
{
	WAVE1 = 35,
	WAVE2 = 0
};

#pragma once
class LevelManager
{
public:

	void NextLevel();
	void SwitchToCurrentLevel();
	int CurrentLevel() { return iCurrentLevelID; };
	std::shared_ptr<Level> GetCurrentActiveLevel();
	std::string GetCurrentLevelName() { return "Level " + std::to_string(iCurrentLevelID); };
	int GetHighscore(int _iLevel);
	void CheckHighscore();
	void EnemySpawner();

	void Init();

private:
	int iCurrentLevelID;
	int iNumberOfLevels = 0;

	RoundTimes CurrentRoundTime = WAVE1;
	float fCurrentRoundElapsed;

	std::vector<int> LevelHighScores;

	bool AddLevel(std::shared_ptr<Level> _Scene);
	bool PopulateLevel(std::shared_ptr<Level> _Scene, int _iLevel);

	// Singleton
public:
	static std::shared_ptr<LevelManager> GetInstance();
	static void DestoryInstance();
	void Update();
	~LevelManager(); // Shared pointer has to be able to call destructor so can't be private

private:
	static std::shared_ptr<LevelManager> m_pLevelManager;
	LevelManager();
	LevelManager(LevelManager const&);              // Don't Implement
	float SpawnTimer = 0.5f;
	void operator=(LevelManager const&); // Don't implement
};

