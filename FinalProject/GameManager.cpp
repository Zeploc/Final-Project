//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    GameManager.cpp
// Description    	:    main implementation for GameManager
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //

// OpenGL Library Includes //

// Engine Includes //
#include "Engine\SceneManager.h"
#include "Engine\LogManager.h"

// Local Includes //
#include "LevelManager.h"
#include "UIManager.h"
#include "NetworkManager.h"

// This Includes //
#include "GameManager.h"
#include "Player.h"

// Static Variables //
std::shared_ptr<GameManager> GameManager::m_pGameManager;

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
GameManager::GameManager()
{
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
GameManager::~GameManager()
{
}

/************************************************************
#--Description--#:  Switch to current level
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void GameManager::SwitchToCurrentLevel()
{
	SceneManager::GetInstance()->SwitchScene("Level " + std::to_string(LevelManager::GetInstance()->CurrentLevel()));
	HideEndScreen();
	std::shared_ptr<Level> CurrentLevel = std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->GetCurrentScene());
	CurrentLevel->RestartLevel();
}

/************************************************************
#--Description--#:  Display end screen
#--Author--#: 		Alex Coultas
#--Parameters--#:	Boolean whether its lost or win screen
#--Return--#: 		NA
************************************************************/
void GameManager::ShowEndScreen(bool _bLost)
{
	UIManager::GetInstance()->SwitchUIMode(true);
	UIManager::GetInstance()->m_bEndScreen = true;
	UIManager::GetInstance()->m_EndScreen.SetScore(LevelManager::GetInstance()->GetCurrentActiveLevel()->EPlayer->GetScore());
	//std::shared_ptr<Level> CurrentLevel = std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->GetCurrentScene());
	//if (!CurrentEndScreen) CurrentEndScreen = std::make_shared<EndScreen>(_bLost, CurrentLevel->GetTotalPoints());
	//else
	//	CurrentEndScreen->SetConditions(_bLost, CurrentLevel->GetTotalPoints());//->ConditionText->sText = sEndMessage + "\nwith score " + std::to_string(CurrentLevel->GetTotalPoints());
	//	
	//SceneManager::GetInstance()->AddScene(CurrentEndScreen);
	//SceneManager::GetInstance()->SwitchScene("EndScreen");
}

/************************************************************
#--Description--#:  On player death display end screen
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void GameManager::PlayerDeath(std::shared_ptr<Player> PlayerDied)
{
	PlayerDied->bPlayerDead = true;
	//LogManager::GetInstance()->DisplayLogMessage("Player is Dead!");
	std::shared_ptr<Level> CurrentLevel = std::dynamic_pointer_cast<Level>(LevelManager::GetInstance()->GetCurrentActiveLevel());
	if (!NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)
	{
		ShowEndScreen(true);
		CurrentLevel->DestroyAllEnemies();
	}
	else if (NetworkManager::GetInstance()->m_Network.IsServer())
	{
		PlayerDied->SetVisible(false);
		std::shared_ptr<NetworkEntity> NetworkEntityRef = NetworkManager::GetInstance()->m_Network.m_pNetworkEntity;
		NetworkEntityRef->SendMessageNE(PlayerDied->m_UserName, PLAYERDEATH);
		bool bPlayerAlive = false;
		for (auto& PlayerEnt : NetworkEntityRef->PlayerEntities)
		{
			if (!PlayerEnt.second->bPlayerDead)
				bPlayerAlive = true;
		}
		if (!bPlayerAlive)
		{
			NetworkEntityRef->SendMessageNE(std::to_string(true), SHOWENDSCREEN);
			ShowEndScreen(true);
			CurrentLevel->DestroyAllEnemies();
		}
	}
}

void GameManager::RespawnPlayer()
{
	std::shared_ptr<Level> CurrentLevel = std::dynamic_pointer_cast<Level>(LevelManager::GetInstance()->GetCurrentActiveLevel());
	//bPlayerDead = false;
	if (!CurrentLevel) return;
	CurrentLevel->EPlayer->Restart();
}

std::shared_ptr<Player> GameManager::GetPlayer()
{	
	return LevelManager::GetInstance()->GetCurrentActiveLevel()->EPlayer;
}

/************************************************************
#--Description--#:  On level won display end screen
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void GameManager::LevelWon()
{
	LevelManager::GetInstance()->GetCurrentActiveLevel()->EPlayer->bPlayerDead = true;
	//bPlayerDead = true;
	//LevelManager::GetInstance()->CheckHighscore();
	ShowEndScreen(false);
}

/************************************************************
#--Description--#:  Hide end screen
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void GameManager::HideEndScreen()
{
	UIManager::GetInstance()->SwitchUIMode(false);
	UIManager::GetInstance()->m_bEndScreen = false;
}

/************************************************************
#--Description--#:  Retrieves static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		Returns static pointer to self
************************************************************/
std::shared_ptr<GameManager> GameManager::GetInstance()
{
	if (!m_pGameManager) // null or doesn't exist
	{
		m_pGameManager = std::shared_ptr<GameManager>(new GameManager());
	}
	return m_pGameManager;
}

/************************************************************
#--Description--#:  Destroys static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void GameManager::DestoryInstance()
{
	m_pGameManager = nullptr;
}