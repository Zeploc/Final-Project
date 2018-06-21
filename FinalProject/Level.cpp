//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Level.cpp
// Description    	:    main implementation for Level
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <iostream>
#include <algorithm>    // std::min

// OpenGL Library Includes //

// Engine Includes //
#include "Engine\Input.h"
#include "Engine\UIButton.h"
#include "Engine\SceneManager.h"
#include "Engine\SoundManager.h"
#include "Engine\Time.h"
#include "Engine\ParticleSystem.h"
#include "Engine\Cursor.h"
#include "Engine\Mesh.h"
#include "Engine\Plane.h"
#include "Engine\Pyramid.h"
#include "Engine\Sphere.h"
#include "Engine\Cube.h"
#include "Engine\CubeMap.h"
#include "Engine\CollisionBounds.h"
#include "Engine\Model.h"

// Local Includes //
#include "Player.h"
#include "Spectator.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "EnemySeek.h"
#include "SpeedBoostPickUp.h"
#include "Boss.h"
#include "LevelManager.h"

#include "NetworkManager.h"
#include "Server.h"

// This Includes //
#include "Level.h"

// Static Variables //

// Prototypes //


/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Level::Level(std::string sSceneName)
	: Scene(sSceneName)
{
	
	Camera::GetInstance()->SetCameraForwardVector({0, -1, -1});
	Camera::GetInstance()->SetCameraPos({ 17, 15, 30 });
	std::shared_ptr<Entity> WorldCubeMap = std::make_shared<Entity>(Entity({ { 0, 0, 0 },{ 0, 0, 0 },{ 1, 1, 1 } }, Utils::CENTER));
	char *  TextureSources[6] = { "right.jpg", "left.jpg" , "top.jpg" , "bottom.jpg" , "back.jpg" , "front.jpg" };
	std::shared_ptr<CubeMap> WorldCubeMapMesh = std::make_shared<CubeMap>(CubeMap(1000.0f, 1000.0f, 1000.0f, TextureSources));
	WorldCubeMap->AddMesh(WorldCubeMapMesh);
	AddEntity(WorldCubeMap);
	Utils::WorldCubeMap = WorldCubeMap;

	SpawnPos = glm::vec3(17, 1.0f, 20);
	// Add cube map first so transpancy works
	/*if (!NetworkManager::GetInstance()->m_Network.m_pNetworkEntity)
	{
		std::shared_ptr<Player> Player(new Player(Utils::Transform{ SpawnPos, glm::vec3(0, 0, 0), glm::vec3(0.01f, 0.01f, 0.01f) }, 0.5f, 1.0f, 0.5f, Utils::CENTER, glm::vec4(0.1, 1.0, 0.1, 1.0)));
		AddEntity(Player);
		Player->EntityMesh->AddCollisionBounds(0.6f, 1.0f, 0.6f, Player);
		EPlayer = Player;
		EPlayer->SetActive(true);
	}*/

	std::shared_ptr<Spectator> Spec = std::make_shared<Spectator>(Spectator(Utils::Transform{ SpawnPos, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, Utils::CENTER));
	ESpectator = Spec;
	AddEntity(ESpectator);
	ESpectator->SetActive(false);

	CurrentController = EPlayer;

	//std::shared_ptr<SpeedBoostPickUp> NewPickup = std::make_shared<SpeedBoostPickUp>(SpeedBoostPickUp(Utils::Transform{ { 7.5f, -2.0f, 10.0f }, { 45, 45, 45 }, {1, 1, 1} }, Utils::CENTER, EPlayer));
	//std::shared_ptr<Cube> NewPickupMesh = std::make_shared<Cube>(Cube(0.5f, 0.5f, 0.5f, { 0.4, 0.1, 0.6, 1.0f }, "Resources/Box.png"));
	//NewPickup->AddMesh(NewPickupMesh);
	//NewPickupMesh->AddCollisionBounds(0.5f, 0.5f, 0.5f, NewPickup);
	//NewPickupMesh->SetLit(true);
	//AddEntity(NewPickup);
	//NewPickupMesh->program = Shader::Programs["ReflectionProgram"];

	//std::shared_ptr<Boss> BossObject = std::make_shared<Boss>(Boss(Utils::Transform{ { 20.5f, -1.0f, -10.0f },{ 0, 0, 0 },{ 0.5f, 0.5f, 0.5f } }, Utils::CENTER));
	//std::shared_ptr<Model> SkullMesh = std::make_shared<Model>(Model({ 0.7f, 0.1f, 0.1f, 1.0f }, "Resources/Models/LowPoly_Pixel_RPG_Assets_DevilsGarage_v01/3D/skull.obj"));
	//BossObject->AddMesh(SkullMesh);
	//SkullMesh->AddCollisionBounds(3.0f, 5.0f, 3.0f, BossObject);
	//SkullMesh->SetLit(true);
	//SkullMesh->LightProperties.fShininess = 50.0f;
	//SkullMesh->LightProperties.fLightSpecStrength = 0.3f;
	//SkullMesh->LightProperties.fAmbientStrength = 0.05f;
	//AddEnemy(BossObject);
	////BossRef = BossObject;

	//std::shared_ptr<Entity> CubeCollision = std::make_shared<Entity>(Entity(Utils::Transform{ { 17.0f, -2.5f, 20.0f },{ 0, 0, 0 },{ 1, 1, 1 } }, Utils::BOTTOM_CENTER));
	//std::shared_ptr<Cube> CubeCollisionMesh = std::make_shared<Cube>(Cube(2.0f, 2.0f, 2.0f, { 0.1, 0.3, 0.7, 1.0f }));
	//CubeCollision->AddMesh(CubeCollisionMesh);
	//CubeCollisionMesh->AddCollisionBounds(2.0f, 2.0f, 2.0f, CubeCollision);
	//CubeCollisionMesh->SetLit(true);
	//AddCollidable(CubeCollision);

	FPSCounterText = std::make_shared<UIText>(glm::vec2(Camera::GetInstance()->SCR_WIDTH - 30.0f, Camera::GetInstance()->SCR_HEIGHT - 20.0f), 0.0f, glm::vec4(0.6, 0.6, 0.6, 1.0), "FPS:", "Resources/Fonts/Roboto-Condensed.ttf", 20, Utils::TOP_RIGHT);
	AddUIElement(FPSCounterText);
	
	//std::shared_ptr<Entity> ImagePlane = std::make_shared<Entity>(Entity({ glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::CENTER));
	//std::shared_ptr<Plane> ImagePlaneMesh = std::make_shared<Plane>(1.0f, 1.0f, glm::vec4(0.5f, 0.8f, 1.0f, 1.0f), "Resources/AwesomeFace.png");
	//ImagePlane->AddMesh(ImagePlaneMesh);
	//AddEntity(ImagePlane);
	
	//std::shared_ptr<Entity> TexturedPyramid = std::make_shared<Entity>(Entity({ glm::vec3(-1, -3, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	//std::shared_ptr<Pyramid> TexturedPyramidMesh = std::make_shared<Pyramid>(1.0f, 1.0f, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "Resources/AwesomeFace.png");
	//TexturedPyramid->AddMesh(TexturedPyramidMesh);
	//AddEntity(TexturedPyramid);
		
	std::shared_ptr<Entity> Target = std::make_shared<Entity>(Entity({ glm::vec3(0, -2.5, 0), glm::vec3(-90, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	//std::shared_ptr<Sphere> TargetMesh = std::make_shared<Sphere>(0.5f, 2.0f, 0.5f, glm::vec4(1.0f, 0.5f, 0.1f, 1.0f));
	std::shared_ptr<Plane> TargetMesh = std::make_shared<Plane>(Plane(1.0f, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, "Resources/Textures/MouseTarget.png"));
	TargetMesh->SetLit(true);
	Target->AddMesh(TargetMesh);
	//TargetMesh->AddCollisionBounds(0.5f, 2.0f, 0.5f, Target);
	//TargetMesh->LightProperties.v3LightColour = {0.8f, 0.1f, 0.1f};
	//AddEntity(Target);
	//AddCollidable(Target);
	MouseAimTarget = Target;

	//std::shared_ptr<Cube> CrowdPathFollowingEnemeyMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(0.1f, 1.0f, 0.1f, 1.0f), "Resources/Enemy1.png");

	//for (int i = 0; i < 10; i++)
	//{
	//	std::shared_ptr<Enemy1> CrowdPathFollowingEnemy = std::make_shared<Enemy1>(Enemy1({ glm::vec3(-5 + i * 0.3f, -2.5, -5), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER, { 5,0,0 }));
	//	CrowdPathFollowingEnemeyMesh->SetLit(true);
	//	CrowdPathFollowingEnemy->AddMesh(CrowdPathFollowingEnemeyMesh);
	//	CrowdPathFollowingEnemeyMesh->AddCollisionBounds(1, 1, 1, CrowdPathFollowingEnemy);
	//	AddEnemy(CrowdPathFollowingEnemy);
	//	//AddEntity(NewEnemy);
	//}
	//std::shared_ptr<Cube> FlockingEnemyMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(0.9f, 0.3f, 0.1f, 1.0f), "Resources/Enemy1.png");
	//for (int i = 0; i < 8; i++)
	//{
	//	std::shared_ptr<Enemy2> FlockingEnemy = std::make_shared<Enemy2>(Enemy2({ glm::vec3(-5 + i * 0.3f, -2.5, -5), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	//	FlockingEnemyMesh->SetLit(true);
	//	FlockingEnemy->AddMesh(FlockingEnemyMesh);
	//	FlockingEnemyMesh->AddCollisionBounds(1, 1, 1, FlockingEnemy);
	//	AddEnemy(FlockingEnemy);
	//	FlockingEnemy->SetTarget(EPlayer);
	//	//AddEntity(NewEnemy);
	//}

	////Enemy1Ref = NewEnemy;

	//std::shared_ptr<Entity> NewTarget = std::make_shared<Entity>(Entity({ glm::vec3(0, -3, 0), glm::vec3(-90, 0, 0), glm::vec3(1, 1 ,1) }, Utils::CENTER));
	//std::shared_ptr<Sphere> NewTargetMesh = std::make_shared<Sphere>(0.5f, 1.0f, 0.5f, glm::vec4(1.0f, 0.2f, 0.5f, 1.0f));
	//NewTargetMesh->SetLit(true);
	//NewTarget->AddMesh(NewTargetMesh);
	////AddEntity(NewTarget);
	////TargetRef = NewTarget;
	//
	/*std::shared_ptr<EnemySeek> NewSeekEnemy = std::make_shared<EnemySeek>(EnemySeek({ glm::vec3(0, -2.5, -5), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	std::shared_ptr<Cube> SeekEnemyMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(0.4f, 0.5f, 0.8f, 1.0f), "Resources/Enemy1.png");
	SeekEnemyMesh->SetLit(true);
	NewSeekEnemy->AddMesh(SeekEnemyMesh);
	NewSeekEnemy->Target = BossRef;
	SeekEnemyMesh->AddCollisionBounds(1, 1, 1, NewSeekEnemy);
	AddEnemy(NewSeekEnemy);

	std::shared_ptr<Enemy3> PersueEnemy = std::make_shared<Enemy3>(Enemy3({ glm::vec3(0, -2.5, -5), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	std::shared_ptr<Cube> PersueEnemyMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(0.9f, 0.8f, 0.0f, 1.0f), "Resources/Enemy1.png");
	PersueEnemyMesh->SetLit(true);
	PersueEnemy->AddMesh(PersueEnemyMesh);
	PersueEnemy->Target = EPlayer;
	PersueEnemyMesh->AddCollisionBounds(1, 1, 1, PersueEnemy);
	AddEnemy(PersueEnemy);*/

	//std::shared_ptr<Cursor> NewCursor = std::make_shared<Cursor>("Resources/Grey_Cursor.png");
	//NewCursor->SetVisibleRange({ 500, 150 });
	//AddUIElement(NewCursor);
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Level::~Level()
{
	Collidables.clear();
	CurrentEnemies.clear();
	EnemiesTemplate.clear();
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Level::Update()
{
	LevelManager::GetInstance()->EnemySpawner();
	MouseAimTarget->transform.Rotation.z += 10.0f *  (float)Time::dTimeDelta;
	float fDotProductDirections = glm::dot(Camera::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos), glm::vec3(0, 1, 0));
	if (fDotProductDirections == 0) // Perpendicular
	{
		std::cout << "Perpendicular to plane\n";
		MouseAimTarget->transform.Position = Camera::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos) * 15.0f + Camera::GetInstance()->GetCameraPosition();
	}
	else
	{
		float fDistance = -((glm::dot(Camera::GetInstance()->GetCameraPosition(), glm::vec3(0, 1, 0)) + 3.0f) /
			(fDotProductDirections));
		MouseAimTarget->transform.Position = Camera::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos) * fDistance + Camera::GetInstance()->GetCameraPosition();
		MouseAimTarget->transform.Position.y += 0.1f;
	}
	
	if (CurrentController == EPlayer)
	{
		if (GameManager::GetInstance()->IsPlayerAlive())
			CameraMovement();
	}

	if (Input::GetInstance()->KeyState['g'] == Input::INPUT_FIRST_PRESS && !UIManager::GetInstance()->GetUIMode())
	{
		if (CurrentController == EPlayer)
		{
			CurrentController = ESpectator;
			EPlayer->SetActive(false);
			ESpectator->SetActive(true);
		}
		else
		{
			CurrentController = EPlayer;
			EPlayer->SetActive(true);
			ESpectator->SetActive(false);
		}
	}
	FPSCounterText->sText = "FPS: " + std::to_string(Time::dFPS);

	if (NetworkManager::GetInstance()->m_Network.IsServer()) // is Server
	{
		if (NetworkEntity)
		{
			NetworkEntity->transform = MouseAimTarget->transform;
			NetworkEntity->transform.Position.y = 1.0f;
		}

		if (Input::GetInstance()->KeyState[(unsigned int) 'f'] == Input::INPUT_FIRST_PRESS)
		{
			DestroyNetworkEntity(NetworkEntity);
		}
	}


	Scene::Update(); // Call super/base Update

	if (Input::GetInstance()->KeyState[(unsigned char)'r'] == Input::INPUT_FIRST_PRESS)
	{
		RestartLevel();
	}	
}

void Level::RenderScene()
{
	Scene::RenderScene();
	
}

/************************************************************
#--Description--#:  Add Collidable entity
#--Author--#: 		Alex Coultas
#--Parameters--#:	Entity to add
#--Return--#: 		NA
************************************************************/
void Level::AddCollidable(std::shared_ptr<Entity> Ent)
{
	AddEntity(Ent);
	Collidables.push_back(Ent);
}

/************************************************************
#--Description--#:  Add Enemey entity
#--Author--#: 		Alex Coultas
#--Parameters--#:	Entity to add
#--Return--#: 		NA
************************************************************/
void Level::AddEnemy(std::shared_ptr<Entity> Enemy)
{
	EnemiesTemplate.push_back(Enemy);
	AddTempEnemy(Enemy);
}

void Level::AddTempEnemy(std::shared_ptr<Entity> NewEnemy)
{
	std::shared_ptr<Enemy1> IsEnemy1 = std::dynamic_pointer_cast<Enemy1>(NewEnemy);
	std::shared_ptr<Enemy2> IsEnemy2 = std::dynamic_pointer_cast<Enemy2>(NewEnemy);
	std::shared_ptr<Enemy3> IsEnemy3 = std::dynamic_pointer_cast<Enemy3>(NewEnemy);
	std::shared_ptr<EnemySeek> IsEnemySeek = std::dynamic_pointer_cast<EnemySeek>(NewEnemy);
	std::shared_ptr<Boss> IsBoss = std::dynamic_pointer_cast<Boss>(NewEnemy);
	bool bIsEnemy1 = IsEnemy1 != nullptr;
	bool bIsEnemy2 = IsEnemy2 != nullptr;
	bool bIsEnemy3 = IsEnemy3 != nullptr;
	bool bIsEnemySeek = IsEnemySeek != nullptr;
	bool bIsBoss = IsBoss != nullptr;
	if (bIsEnemy1)
	{
		std::shared_ptr<Enemy1> NewEnemyCopy = std::make_shared<Enemy1>(Enemy1(IsEnemy1->transform, IsEnemy1->EntityAnchor, { 0, 0, 0 }));
		CurrentEnemies.push_back(NewEnemyCopy);
		NewEnemyCopy->EntityMesh = IsEnemy1->EntityMesh;
		AddEntity(NewEnemyCopy);
	}
	else if (IsEnemy2)
	{
		std::shared_ptr<Enemy2> NewEnemyCopy = std::make_shared<Enemy2>(Enemy2(IsEnemy2->transform, IsEnemy2->EntityAnchor));
		CurrentEnemies.push_back(NewEnemyCopy);
		NewEnemyCopy->SetTarget(EPlayer);
		NewEnemyCopy->EntityMesh = IsEnemy2->EntityMesh;
		AddEntity(NewEnemyCopy);
	}
	else if (bIsEnemy3)
	{
		std::shared_ptr<Enemy3> NewEnemyCopy = std::make_shared<Enemy3>(Enemy3(IsEnemy3->transform, IsEnemy3->EntityAnchor));
		CurrentEnemies.push_back(NewEnemyCopy);
		NewEnemyCopy->Target = EPlayer;
		NewEnemyCopy->EntityMesh = IsEnemy3->EntityMesh;
		AddEntity(NewEnemyCopy);
	}
	else if (bIsEnemySeek)
	{
		std::shared_ptr<EnemySeek> NewEnemyCopy = std::make_shared<EnemySeek>(EnemySeek(IsEnemySeek->transform, IsEnemySeek->EntityAnchor));
		CurrentEnemies.push_back(NewEnemyCopy);
		NewEnemyCopy->Target = EPlayer;
		NewEnemyCopy->EntityMesh = IsEnemySeek->EntityMesh;
		AddEntity(NewEnemyCopy);
	}
	else if (bIsBoss)
	{
		std::shared_ptr<Boss> NewEnemyCopy = std::make_shared<Boss>(Boss(IsBoss->transform, IsBoss->EntityAnchor));
		NewEnemyCopy->AddMesh(IsBoss->EntityMesh);
		CurrentEnemies.push_back(NewEnemyCopy);
		AddEntity(NewEnemyCopy);
		NewEnemyCopy->EntityMesh = NewEnemyCopy->EntityMesh;
		BossRef = NewEnemyCopy;
	}
}

void Level::DestroyEnemy(std::shared_ptr<Entity> _DeleteEnemy)
{
	for (auto it = CurrentEnemies.begin(); it != CurrentEnemies.end(); ++it)
	{
		if (*it == _DeleteEnemy)
		{
			//*it = nullptr;
			CurrentEnemies.erase(it);
			break;
		}
	}
	DestroyEntity(_DeleteEnemy);
	_DeleteEnemy = nullptr;
}

void Level::DestroyNetworkEntity(std::shared_ptr<Entity> EntityToDestroy)
{
	NetworkManager::GetInstance()->DestroyNetworkEntity(EntityToDestroy);
	DestroyEntity(EntityToDestroy);
}

/************************************************************
#--Description--#:  Add Collidable entity
#--Author--#: 		Alex Coultas
#--Parameters--#:	Entity to add
#--Return--#: 		NA
************************************************************/
std::shared_ptr<Entity> Level::AddCollidable(Utils::Transform _Transform, float _fWidth, float _fHeight, Utils::EANCHOR _Anchor, glm::vec4 _Colour, const char* TextureSource, int iCount, bool bHorizontal)
{
	/*std::shared_ptr<Entity> Ent(new Entity(_Transform, _fWidth, _fHeight, _Anchor, _Colour, TextureSource, iCount, bHorizontal));
	AddEntity(Ent);
	Collidables.push_back(Ent);*/
	return nullptr;
}

/************************************************************
#--Description--#:  Destroy Collidable entity
#--Author--#: 		Alex Coultas
#--Parameters--#:	Entity to destroy
#--Return--#: 		NA
************************************************************/
void Level::DestroyCollidable(std::shared_ptr<Entity> _Entity)
{
	for (auto it = Collidables.begin(); it != Collidables.end(); ++it)
	{
		if (*it == _Entity)
		{
			*it = nullptr;
			Collidables.erase(it);
			break;
		}
	}
	DestroyEntity(_Entity);
	_Entity = nullptr;
}

void Level::AddHexPlatform(std::string _ModelPath, glm::vec3 _v3Postion, glm::vec3 Rotation)
{
	std::shared_ptr<Entity> ModelEnt = std::make_shared<Entity>(Entity({ _v3Postion, Rotation, glm::vec3(0.1f, 0.1f, 0.1f) }, Utils::CENTER));
	std::shared_ptr<Model> NewModelMesh = std::make_shared<Model>(Model({ 1.0f, 1.0f, 1.0f, 1.0f }, _ModelPath.c_str()));
	ModelEnt->AddMesh(NewModelMesh);
	NewModelMesh->SetLit(true);
	NewModelMesh->LightProperties.fAmbientStrength = 0.7f;
	NewModelMesh->AddCollisionBounds(10, 4, 10, ModelEnt);
	AddCollidable(ModelEnt);
}

void Level::DestroyAllEnemies()
{
	for (auto& it : CurrentEnemies)
	{
		DestroyEntity(it);
	}
	CurrentEnemies.clear();
}

/************************************************************
#--Description--#:  Respawns current level enemies
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Level::RespawnEnemies()
{
	DestroyAllEnemies();

	for (auto& TemplateE : EnemiesTemplate)
	{
		AddTempEnemy(TemplateE);
	}
	//for (unsigned int i = 0; i < Enemies.size(); i++)
	//{
	//	DestroyCollidable(Enemies[i]);
	//	Enemies[i] = nullptr;
	//}
	//Enemies.clear();
	//for (auto it : EnemiesTemplate)
	//{
	//	std::shared_ptr<Enemy1> IsEnemy1 = std::dynamic_pointer_cast<Enemy1>(it);
	//	std::shared_ptr<Enemy2> IsEnemy2 = std::dynamic_pointer_cast<Enemy2>(it);
	//	std::shared_ptr<Enemy3> IsEnemy3 = std::dynamic_pointer_cast<Enemy3>(it);

	//	if (IsEnemy1)
	//		AddTemporaryEnemy(std::make_shared<Enemy1>(*IsEnemy1));
	//	else if (IsEnemy2)
	//		AddTemporaryEnemy(std::make_shared<Enemy2>(*IsEnemy2));
	//	else if (IsEnemy3)
	//		AddTemporaryEnemy(std::make_shared<Enemy3>(*IsEnemy3));
	//	//AddTemporaryEnemy(std::make_shared<Entity>(*it));		
	//}
}

/************************************************************
#--Description--#:  play random level music track
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Level::PlayRandomTrack()
{
	SoundManager::GetInstance()->StopAudio("BackgroundC");
	const char* MusicOptions[] = { "Resources/Sound/Ludum Dare 28 - Track 1.wav",
		"Resources/Sound/Ludum Dare 28 - Track 3.wav", "Resources/Sound/Ludum Dare 30 - Track 6.wav",
		"Resources/Sound/Ludum Dare 30 - Track 7.wav", "Resources/Sound/Ludum Dare 38 - Track 10.wav" };
	int iRandTrack = rand() % 5;
	std::cout << "Playing " << MusicOptions[iRandTrack] << " | Number " << iRandTrack << std::endl;
	SoundManager::GetInstance()->AddAudio(MusicOptions[iRandTrack], true, "GameBackgroundTrack " + std::to_string(iRandTrack));
	SoundManager::GetInstance()->PlayAudio("GameBackgroundTrack " + std::to_string(iRandTrack), "BackgroundC");
	SoundManager::GetInstance()->SetChannelVolume("BackgroundC", GameSettings::fVolumeLevel / 20.0f);
}

/************************************************************
#--Description--#:  When the level scene loads
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Level::OnLoadScene()
{
	PlayRandomTrack();
	UIManager::GetInstance()->SwitchUIMode(false);
	UIManager::GetInstance()->m_bDisplayHUD = true;
	UIManager::GetInstance()->m_bLoadingScreen = false;
	RestartLevel();	
}
/************************************************************
#--Description--#:  Restarts the level
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Level::RestartLevel()
{
	//EPlayer->Reset();
	//Camera::GetInstance()->SetCameraPos(glm::vec3(0, 0, 3));
	//iScore = iCoinsCollected * 5;
	RespawnEnemies();
	if (BossRef) std::dynamic_pointer_cast<Boss>(BossRef)->ResetHealth();
	GameManager::GetInstance()->RespawnPlayer();
	GameManager::GetInstance()->HideEndScreen();
	for (auto& it : CurrentEnemies)
	{
		std::shared_ptr<Enemy1> IsEnemy1 = std::dynamic_pointer_cast<Enemy1>(it);
		if (IsEnemy1)
		{
			IsEnemy1->AddPathPoints();
			break;
		}
	}

	for (auto& it : Entities)
	{
		std::shared_ptr<PickUpBase> IsPickup = std::dynamic_pointer_cast<PickUpBase>(it);
		if (IsPickup)
		{
			IsPickup->Init();
		}
	}
}

/************************************************************
#--Description--#:  Sets player spawn position
#--Author--#: 		Alex Coultas
#--Parameters--#:	Vector 2 for new position
#--Return--#: 		NA
************************************************************/
void Level::SetPlayerPosition(glm::vec3 Pos)
{
	//EPlayer->transform.Position = glm::vec3(Pos, 0);	
	SpawnPos = Pos;
}

void Level::CameraMovement()
{

	glm::vec3 CurrentCamPos = Camera::GetInstance()->GetCameraPosition();
	float fCamHeight = 15;
	float fBackDistance = 15;

	glm::vec3 Difference = glm::vec3();
	if (BossRef && EPlayer)
		Difference = BossRef->transform.Position - EPlayer->transform.Position;
	float fDistance = abs(glm::length(Difference));
	glm::vec3 Direction = fDistance != 0 ? glm::normalize(Difference) : Difference;

	float fBackMultiplier = fDistance / 20;

	glm::vec3 CenterPosition = EPlayer ? EPlayer->transform.Position + Direction * (fDistance / 2) : SpawnPos;
	glm::vec3 NewCameraTargetPostion = CenterPosition;
	NewCameraTargetPostion.y += max(fCamHeight * fBackMultiplier, 12.0f);
	NewCameraTargetPostion.z += max(fBackDistance * fBackMultiplier, 12.0f);
	
	glm::vec3 AdditionVec = NewCameraTargetPostion - CurrentCamPos;
	AdditionVec *= 0.3f;
	
	glm::vec3 NewCameraPos = CurrentCamPos + AdditionVec;
	Camera::GetInstance()->SetCameraPos(NewCameraPos);

}

/************************************************************
#--Description--#:  Local function for back button
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void BackButton()
{
	SceneManager::GetInstance()->SwitchScene("MainMenu");
}
