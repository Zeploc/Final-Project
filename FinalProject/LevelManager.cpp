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
#include "Engine\Time.h"
#include "Engine\Cube.h"
#include "Engine\Model.h"
#include "Engine\Shader.h"

// Local Includes //
#include "Menu.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "EnemySeek.h"
#include "Level.h"
#include "Player.h"
#include "SpeedBoostPickUp.h"
#include "FireRatePickUp.h"
#include "HeatSeekerPickUp.h"
#include "Boss.h"
#include "UIManager.h"
#include "HUD.h"

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
			CurrentRoundTime = WAVE1;
			fCurrentRoundElapsed = CurrentRoundTime;
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
			std::shared_ptr<SpeedBoostPickUp> NewPickup = std::make_shared<SpeedBoostPickUp>(SpeedBoostPickUp(Utils::Transform{ { 7.5f, -2.0f, 10.0f },{ 45, 45, 45 },{ 1, 1, 1 } }, Utils::CENTER, _Scene->EPlayer));
			std::shared_ptr<Cube> NewPickupMesh = std::make_shared<Cube>(Cube(0.5f, 0.5f, 0.5f, { 0.4, 0.1, 0.6, 1.0f }, "Resources/Box.png"));
			NewPickup->AddMesh(NewPickupMesh);
			NewPickupMesh->AddCollisionBounds(0.5f, 0.5f, 0.5f, NewPickup);
			NewPickupMesh->SetLit(true);
			_Scene->AddEntity(NewPickup);
			NewPickupMesh->program = Shader::Programs["ReflectionProgram"];

			
			std::shared_ptr<FireRatePickup> FireRatePickupInstance = std::make_shared<FireRatePickup>(FireRatePickup(Utils::Transform{ { 5.5f, -2.0f, 10.0f },{ 45, 45, 45 },{ 1, 1, 1 } }, Utils::CENTER, _Scene->EPlayer));
			std::shared_ptr<Cube> FireRateMesh = std::make_shared<Cube>(Cube(0.5f, 0.5f, 0.5f, { 0.4, 0.1, 0.6, 1.0f }, "Resources/Box.png"));
			FireRatePickupInstance->AddMesh(FireRateMesh);
			FireRateMesh->AddCollisionBounds(0.5f, 0.5f, 0.5f, FireRatePickupInstance);
			FireRateMesh->SetLit(true);
			_Scene->AddEntity(FireRatePickupInstance);
			FireRateMesh->program = Shader::Programs["ReflectionProgram"];


			std::shared_ptr<HeatSeekerPickUp> HeatSeekingPickupInstance = std::make_shared<HeatSeekerPickUp>(HeatSeekerPickUp(Utils::Transform{ { 2.5f, -2.0f, 10.0f },{ 45, 45, 45 },{ 1, 1, 1 } }, Utils::CENTER, _Scene->EPlayer));
			std::shared_ptr<Cube> HeatSeekMesh = std::make_shared<Cube>(Cube(0.5f, 0.5f, 0.5f, { 0.4, 0.1, 0.6, 1.0f }, "Resources/Box.png"));
			HeatSeekingPickupInstance->AddMesh(HeatSeekMesh);
			HeatSeekMesh->AddCollisionBounds(0.5f, 0.5f, 0.5f, HeatSeekingPickupInstance);
			HeatSeekMesh->SetLit(true);
			_Scene->AddEntity(HeatSeekingPickupInstance);
			HeatSeekMesh->program = Shader::Programs["ReflectionProgram"];

			std::shared_ptr<Entity> CubeCollision = std::make_shared<Entity>(Entity(Utils::Transform{ { 17.0f, -2.5f, 20.0f },{ 0, 0, 0 },{ 1, 1, 1 } }, Utils::BOTTOM_CENTER));
			std::shared_ptr<Cube> CubeCollisionMesh = std::make_shared<Cube>(Cube(2.0f, 2.0f, 2.0f, { 0.1, 0.3, 0.7, 1.0f }));
			CubeCollision->AddMesh(CubeCollisionMesh);
			CubeCollisionMesh->AddCollisionBounds(2.0f, 2.0f, 2.0f, CubeCollision);
			CubeCollisionMesh->SetLit(true);
			_Scene->AddCollidable(CubeCollision);

			_Scene->SetPlayerPosition({ 17, 1.0f, 20 });
			_Scene->AddEntity(_Scene->MouseAimTarget);

			break;
		}
		case 2:
		{
			CurrentRoundTime = WAVE2;
			fCurrentRoundElapsed = CurrentRoundTime;
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

			std::shared_ptr<Boss> BossObject = std::make_shared<Boss>(Boss(Utils::Transform{ { 20.5f, -1.0f, -10.0f },{ 0, 0, 0 },{ 0.5f, 0.5f, 0.5f } }, Utils::CENTER));
			std::shared_ptr<Model> SkullMesh = std::make_shared<Model>(Model({ 0.7f, 0.1f, 0.1f, 1.0f }, "Resources/Models/LowPoly_Pixel_RPG_Assets_DevilsGarage_v01/3D/skull.obj"));
			BossObject->AddMesh(SkullMesh);
			SkullMesh->AddCollisionBounds(3.0f, 5.0f, 3.0f, BossObject);
			SkullMesh->SetLit(true);
			SkullMesh->LightProperties.fShininess = 50.0f;
			SkullMesh->LightProperties.fLightSpecStrength = 0.3f;
			SkullMesh->LightProperties.fAmbientStrength = 0.05f;
			_Scene->AddEnemy(BossObject);

			_Scene->SetPlayerPosition({ 3, 2.0f, 0 });
			_Scene->AddEntity(_Scene->MouseAimTarget);
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
	std::shared_ptr<Level> LevelRef = LevelManager::GetInstance()->GetCurrentActiveLevel();

	int RandomEnemyNum = rand() % 4 + 1;
	int RandomSideNum = rand() % 4 + 1;
	if (SpawnTimer <= 0)
	{
		switch (RandomEnemyNum)
		{
		case 1:
		{
			for (int i = 0; i < 5; i++)
			{
				std::shared_ptr<EnemySeek> NewSeekEnemy;
				//Basic seeker
				switch (RandomSideNum)
				{
				case 1:
				{
					NewSeekEnemy = std::make_shared<EnemySeek>(EnemySeek({ glm::vec3(-20, -2.5, -20), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
					break;
				}
				case 2:
				{
					NewSeekEnemy = std::make_shared<EnemySeek>(EnemySeek({ glm::vec3(-20, -2.5, 50), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
					break;
				}
				case 3:
				{
					NewSeekEnemy = std::make_shared<EnemySeek>(EnemySeek({ glm::vec3(50, -2.5, 50), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
					break;
				}
				case 4:
				{
					NewSeekEnemy = std::make_shared<EnemySeek>(EnemySeek({ glm::vec3(50, -2.5, -20), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
					break;
				}
				default:
					break;
				}

				std::shared_ptr<Cube> SeekEnemyMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(0.4f, 0.5f, 0.8f, 1.0f), "Resources/Enemy1.png");
				SeekEnemyMesh->SetLit(true);
				NewSeekEnemy->AddMesh(SeekEnemyMesh);
				NewSeekEnemy->Target = LevelRef->EPlayer;
				SeekEnemyMesh->AddCollisionBounds(1, 1, 1, NewSeekEnemy);
				LevelRef->AddTempEnemy(NewSeekEnemy);
				int RandomSideNum = rand() % 4 + 1;
			}
			SpawnTimer += 3.0f;
			break;
		}

		case 2:
		{
			for (int i = 0; i < 5; i++)
			{
				std::shared_ptr<Enemy3> PersueEnemy;
				switch (RandomSideNum)
				{
				case 1:
				{
					PersueEnemy = std::make_shared<Enemy3>(Enemy3({ glm::vec3(-20, -2.5, -20), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
					break;
				}
				case 2:
				{
					PersueEnemy = std::make_shared<Enemy3>(Enemy3({ glm::vec3(-20, -2.5, 50), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
					break;
				}
				case 3:
				{
					PersueEnemy = std::make_shared<Enemy3>(Enemy3({ glm::vec3(50, -2.5, 50), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
					break;
				}
				case 4:
				{
					PersueEnemy = std::make_shared<Enemy3>(Enemy3({ glm::vec3(50, -2.5, -20), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
					break;
				}
				default:
					break;
				}
				//Pursue lad
				std::shared_ptr<Cube> PersueEnemyMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(0.9f, 0.8f, 0.0f, 1.0f), "Resources/Enemy1.png");
				PersueEnemyMesh->SetLit(true);
				PersueEnemy->AddMesh(PersueEnemyMesh);
				PersueEnemy->Target = LevelRef->EPlayer;
				PersueEnemyMesh->AddCollisionBounds(1, 1, 1, PersueEnemy);
				LevelRef->AddTempEnemy(PersueEnemy);
			}
			SpawnTimer += 3.0f;
			break;
		}

		case 3:
		{
			// Path Followers
			std::shared_ptr<Cube> CrowdPathFollowingEnemeyMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(0.2f, 0.1f, 0.7f, 1.0f), "Resources/Enemy1.png");
			std::shared_ptr<Enemy1> CrowdPathFollowingEnemy;

			for (int i = 0; i < 4; i++)
			{
				switch (RandomSideNum)
				{
				case 1:
				{
					CrowdPathFollowingEnemy = std::make_shared<Enemy1>(Enemy1({ glm::vec3(0 + i * 0.3f, -2.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER, { 5,0,0 }));
					break;
				}
				case 2:
				{
					CrowdPathFollowingEnemy = std::make_shared<Enemy1>(Enemy1({ glm::vec3(0 + i * 0.3f, -2.5, 30), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER, { 5,0,0 }));
					break;
				}
				case 3:
				{
					CrowdPathFollowingEnemy = std::make_shared<Enemy1>(Enemy1({ glm::vec3(30 + i * 0.3f, -2.5, 30), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER, { 5,0,0 }));
					break;
				}
				case 4:
				{
					CrowdPathFollowingEnemy = std::make_shared<Enemy1>(Enemy1({ glm::vec3(30 + i * 0.3f, -2.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER, { 5,0,0 }));
					break;
				}
				default:
					break;
				}
				
				CrowdPathFollowingEnemeyMesh->SetLit(true);
				CrowdPathFollowingEnemy->AddMesh(CrowdPathFollowingEnemeyMesh);
				CrowdPathFollowingEnemeyMesh->AddCollisionBounds(1, 1, 1, CrowdPathFollowingEnemy);
				LevelRef->AddTempEnemy(CrowdPathFollowingEnemy);
				//AddEntity(NewEnemy);
			}
			SpawnTimer += 3.0f;
			break;
		}

		case 4:
		{
			//Flock
			std::shared_ptr<Cube> EnemeyMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), "Resources/Enemy1.png");
			std::shared_ptr<Enemy2> FlockingEnemy;

			for (int i = 0; i < 5; i++)
			{
				switch (RandomSideNum)
				{
				case 1:
				{
					FlockingEnemy = std::make_shared<Enemy2>(Enemy2({ glm::vec3(0 + i * 0.3f, -2.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
					break;
				}
				case 2:
				{
					FlockingEnemy = std::make_shared<Enemy2>(Enemy2({ glm::vec3(0 + i * 0.3f, -2.5, 30), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
					break;
				}
				case 3:
				{
					FlockingEnemy = std::make_shared<Enemy2>(Enemy2({ glm::vec3(30 + i * 0.3f, -2.5, 30), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
					break;
				}
				case 4:
				{
					FlockingEnemy = std::make_shared<Enemy2>(Enemy2({ glm::vec3(30 + i * 0.3f, -2.5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
					break;
				}
				default:
					break;
				}
				EnemeyMesh->SetLit(true);
				FlockingEnemy->AddMesh(EnemeyMesh);
				EnemeyMesh->AddCollisionBounds(1, 1, 1, FlockingEnemy);
				LevelRef->AddTempEnemy(FlockingEnemy);
				FlockingEnemy->SetTarget(LevelRef->EPlayer);
			}
			SpawnTimer += 3.0f;
			break;
		}
		
		default:
			break;
		}
		//Random number chooses what type of enemy to spawn

		//Random timer chooses when to spawn them

		//Switch spawns an enemy at a random edge on the map
	}
}

void LevelManager::ResetWaveTimer()
{
	fCurrentRoundElapsed = CurrentRoundTime;
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

void LevelManager::Update()
{
	if (GetCurrentActiveLevel())
	{
		SpawnTimer -= Time::dTimeDelta;
		fCurrentRoundElapsed -= Time::dTimeDelta;
		UIManager::GetInstance()->m_HUDInstance.SetWaveTimer(fCurrentRoundElapsed);
		if (fCurrentRoundElapsed <= 0)
		{
			NextLevel();
		}
	}
}

