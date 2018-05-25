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
			_Scene->SetPlayerPosition({ 0, -0.5f });
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(-8.25,-2.25f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 0.5, 6, Utils::BOTTOM_CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block4.png", 1, true);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(-2, -1.75f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 2, 1, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block4Ice.png", 2, true);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(-6, -1.5f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 2, 1.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block2Ice.png", 4, true);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(-6, 1.5f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 2, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block2Ice.png", 1, false);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(-6, 1.0f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 2, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block1.png", 1, false);
			/*std::shared_ptr<Entity> BottomPlat = std::make_shared<Entity>(Utils::Transform{ glm::vec3(-6, 0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 2.0f, 0.5f, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block3.png", 1, false);
			BottomPlat->CollisionBox.fHeight /= 2.0f;*/
			//_Scene->AddCollidable(BottomPlat);

			break;
		}
		case 2:
		{
			_Scene->SetPlayerPosition({ -3, 2.0f });
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(-8.25,-2.25f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 0.5, 6, Utils::BOTTOM_CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block4Ice.png", 1, true);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(-7, -1.0f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 2, 1.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block2Ice.png", 4, true);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(5, -0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 2, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block2Ice.png", 1, false);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(5, -1.0f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 2, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block1.png", 1, false);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(5, -1.5f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 2, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block3.png", 1, false);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(9.5, -0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 2, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block2Ice.png", 1, false);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(9.5, -1.0f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 2, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block3.png", 1, false);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(14, -2.0f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 1, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block3.png", 1, false);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(16, -0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 1, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block3.png", 1, false);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(17, 0.25f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 1, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block3.png", 1, false);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(19, 1.0f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 1, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block3.png", 1, false);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(28, -1.0f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 0.5, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block3.png", 1, false);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(29, 0.1f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 0.5, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block3.png", 1, false);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(30, 1.0f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 0.5, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block3.png", 1, false);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(26, 0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 0.5, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block3.png", 1, false);
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(25, -0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 0.5, 0.5, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block3.png", 1, false);
			
			//At 0 player cant move/strafe//_Scene->AddCollidable(Utils::Transform{ glm::vec3(-7, -0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 2, 1.5, Utils::CENTER, glm::vec4(0.8, 0.8, 0.8, 1.0), "Resources/Level/Block2.png", 4, true);

			_Scene->AddCollidable(Utils::Transform{ glm::vec3(-1, -2.75f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 2, 1, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block4.png", 2, true);
			// add coin on the 3 enemies
			_Scene->AddCollidable(Utils::Transform{ glm::vec3(32, -2.0f, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 1.5f, 0.5f, Utils::CENTER, glm::vec4(1, 1, 1, 1.0), "Resources/Level/Block3.png", 1, false);
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