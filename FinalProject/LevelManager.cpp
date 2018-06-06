//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    LevelManager.cpp
// Description    	:    main implementation for LevelManager
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //

// OpenGL Library Includes //

// Engine Includes //
#include "Engine\SceneManager.h"

// Local Includes //
#include "Menu.h"

// This Includes //
#include "LevelManager.h"

// Static Variables //

std::shared_ptr<LevelManager> LevelManager::m_pLevelManager;

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
LevelManager::LevelManager()
{
	iCurrentLevelID = 1;
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
LevelManager::~LevelManager()
{
}

/************************************************************
#--Description--#:  Initialises the level manager
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void LevelManager::Init()
{
	std::shared_ptr<Menu> MenuScene = std::shared_ptr<Menu>(new Menu("MainMenu"));
	SceneManager::GetInstance()->AddScene(MenuScene);
	while (iCurrentLevelID > iNumberOfLevels)
	{
		std::shared_ptr<Level> LevelScene = std::shared_ptr<Level>(new Level("Level"));
		AddLevel(LevelScene);
	}
}

/************************************************************
#--Description--#:  Adds level scene
#--Author--#: 		Alex Coultas
#--Parameters--#:	New scene pointer
#--Return--#: 		Boolean false if it cannot add new level
************************************************************/
bool LevelManager::AddLevel(std::shared_ptr<Level> _Scene)
{
	iNumberOfLevels++;
	if (!PopulateLevel(_Scene, iNumberOfLevels)) return false;
	_Scene->SceneName = "Level " + std::to_string(iNumberOfLevels);
	SceneManager::GetInstance()->AddScene(_Scene);
	LevelHighScores.push_back(0);
	return true;
}

/************************************************************
#--Description--#:  Adds level elements to scene
#--Author--#: 		Alex Coultas
#--Parameters--#:	New scene pointer
#--Return--#: 		Boolean false doesn't contain level layout
************************************************************/
bool LevelManager::PopulateLevel(std::shared_ptr<Level> _Scene, int _iLevel)
{
	switch (_iLevel)
	{
		case 1:
		{
			_Scene->SetPlayerPosition({ 0, -2.5f, 0 });
			_Scene->AddHexPlatform("Resources/Models/Isometric_3D_Hex_Pack/groundEarth.fbx", { 5, -5.0, 8 }, { 0,0,0 });
			_Scene->AddHexPlatform("Resources/Models/Isometric_3D_Hex_Pack/groundEarth.fbx", { -5, -5.0, 15 }, { 0,0,0 });

			break;
		}
		case 2:
		{
			_Scene->SetPlayerPosition({ -3, 2.0f, 0 });
			//At 0 player cant move/strafe//_Scene->AddCollidable(Utils::Transform{ glm::vec3(-7, -0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 2, 1.5, Utils::CENTER, glm::vec4(0.8, 0.8, 0.8, 1.0), "Resources/Level/Block2.png", 4, true);
			break;
		}
		default:
			return false;
	}
	return true;
}

/************************************************************
#--Description--#:  Advances to next level
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void LevelManager::NextLevel()
{
	std::shared_ptr<Level> LevelScene = std::shared_ptr<Level>(new Level("New Level"));
	if (!AddLevel(LevelScene))
	{
		SceneManager::GetInstance()->SwitchScene("MainMenu");
	}
	else
	{
		iCurrentLevelID++;
		SceneManager::GetInstance()->SwitchScene(LevelScene->SceneName);
		LevelManager::GetInstance()->GetCurrentLevel()->PlayRandomTrack();
	}
}

/************************************************************
#--Description--#:  Retreive current level
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		Returns current level pointer
************************************************************/
std::shared_ptr<Level> LevelManager::GetCurrentLevel()
{
	return std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->GetCurrentScene());
}

/************************************************************
#--Description--#:  Retreive highscore for specific level
#--Author--#: 		Alex Coultas
#--Parameters--#:	Level int to get score for
#--Return--#: 		Returns level highscore
************************************************************/
int LevelManager::GetHighscore(int _iLevel)
{
	return LevelHighScores[_iLevel - 1];
}

/************************************************************
#--Description--#:  Checks if highscore is beaten by new score and sets
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void LevelManager::CheckHighscore()
{
	std::shared_ptr<Level> CurrentLevel = std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->GetCurrentScene());
	if (!CurrentLevel) return;
	if (CurrentLevel->GetTotalPoints() > LevelHighScores[iCurrentLevelID - 1])
	{
		LevelHighScores[iCurrentLevelID - 1] = CurrentLevel->GetTotalPoints();
	}
}

/************************************************************
#--Description--#:  Retrieves static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		Returns static pointer to self
************************************************************/
std::shared_ptr<LevelManager> LevelManager::GetInstance()
{
	if (!m_pLevelManager) // null or doesn't exist
	{
		m_pLevelManager = std::shared_ptr<LevelManager>(new LevelManager());
	}
	return m_pLevelManager;
}

/************************************************************
#--Description--#:  Destroys static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void LevelManager::DestoryInstance()
{
	m_pLevelManager = nullptr;
}