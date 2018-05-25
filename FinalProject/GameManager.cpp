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

// Local Includes //
#include "LevelManager.h"

// This Includes //
#include "GameManager.h"

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
void GameManager::PlayerDeath()
{
	LevelManager::GetInstance()->CheckHighscore();
	GameManager::GetInstance()->ShowEndScreen(true);
}

/************************************************************
#--Description--#:  On level won display end screen
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void GameManager::LevelWon()
{
	LevelManager::GetInstance()->CheckHighscore();
	GameManager::GetInstance()->ShowEndScreen(false);
}

/************************************************************
#--Description--#:  Hide end screen
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void GameManager::HideEndScreen()
{
	SceneManager::GetInstance()->RemoveScene("EndScreen");
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