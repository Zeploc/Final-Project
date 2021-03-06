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
#include <memory>
#include <string>

// OpenGL Library Includes //

// Engine Includes //

// Local Includes //
#include "Level.h"
#include "Player.h"

// Static Variables //


#pragma once
class GameManager
{
public:
	void SwitchToCurrentLevel();
	void ShowEndScreen(bool _bLost);
	void HideEndScreen();

	void LevelWon();
	void PlayerDeath(std::shared_ptr<Player> PlayerDied);
	void RespawnPlayer();

	bool IsPlayerAlive() {
		return !(GetPlayer() ? GetPlayer()->bPlayerDead : true);
	}

	std::shared_ptr<Player> GetPlayer();

private:

	

	// Singleton
public:
	static std::shared_ptr<GameManager> GetInstance();
	static void DestoryInstance();
	~GameManager(); // Shared pointer has to be able to call destructor so can't be private

private:
	static std::shared_ptr<GameManager> m_pGameManager;
	GameManager();
	GameManager(GameManager const&);              // Don't Implement
	void operator=(GameManager const&); // Don't implement
};

