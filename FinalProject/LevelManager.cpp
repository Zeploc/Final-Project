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
#include <random>
#include <cstdlib>
// OpenGL Library Includes //

// Engine Includes //
#include "Engine\SceneManager.h"
#include "Engine\Plane.h"

// Local Includes //
#include "Menu.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "Engine\Cube.h"
#include "Level.h"
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
			float Width = 8.6f;
			float Height = 7.5f;
			int BoardSize = 5;
			for (int z = 0; z < BoardSize; z++)
			{
				for (int x = 0; x < BoardSize; x++)
				{
					float fCurrentX = Width * x;
					if (z % 2 != 0)
					{
						fCurrentX -= Width / 2;
					}
					_Scene->AddHexPlatform("Resources/Models/Isometric_3D_Hex_Pack/ground.fbx", { fCurrentX, -5.0, Height * z }, { 0,30,0 });
				}
			}

			_Scene->SetPlayerPosition({ 17, 1.0f, 20 });
			_Scene->AddEntity(_Scene->MouseAimTarget);

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
		LevelManager::GetInstance()->GetCurrentActiveLevel()->PlayRandomTrack();
	}
}
/************************************************************
#--Description--#:  Switches to current level
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void LevelManager::SwitchToCurrentLevel()
{
	SceneManager::GetInstance()->SwitchScene("Level " + std::to_string(iCurrentLevelID));
}

/************************************************************
#--Description--#:  Retreive current level
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		Returns current level pointer
************************************************************/
std::shared_ptr<Level> LevelManager::GetCurrentActiveLevel()
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

void LevelManager::EnemySpawner()
{
	int RandomNum;
	std::shared_ptr<Level> LevelRef = std::dynamic_pointer_cast <Level>(SceneManager::GetInstance()->GetCurrentScene());

	RandomNum = rand() % 5 + 1;

	switch (RandomNum)
	{
	case 1:
	{
		//Basic seeker
		break;
	}

	case 2:
	{
		//Pursue lad
		break;
	}

	case 3:
	{
		// Path Followers
		break;
	}

	case 4:
	{
		//Flock
		std::shared_ptr<Cube> EnemeyMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(0.1f, 1.0f, 0.1f, 1.0f), "Resources/Enemy1.png");

		for (int i = 0; i < 5; i++)
		{
			std::shared_ptr<Enemy2> NewEnemy = std::make_shared<Enemy2>(Enemy2({ glm::vec3(-5 + i * 0.3f, -2, -5), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
			EnemeyMesh->SetLit(true);
			NewEnemy->AddMesh(EnemeyMesh);
			EnemeyMesh->AddCollisionBounds(1, 1, 1, NewEnemy);
			LevelRef->AddEnemy(NewEnemy);
			NewEnemy->SetTarget(LevelRef->MouseAimTarget);
			
		}
		break;
	}
	case 5:
	{
		// Evade?
	}
	default:
		break;
	}
	//Random number chooses what type of enemy to spawn

	//Random timer chooses when to spawn them

	//Switch spawns an enemy at a random edge on the map
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

