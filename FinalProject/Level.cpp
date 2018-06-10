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
#include "SpeedBoostPickUp.h"

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
	SpawnPos = glm::vec3(17, -2.5f, 20);
	// Add cube map first so transpancy works
	std::shared_ptr<Player> Player(new Player(Utils::Transform{ SpawnPos, glm::vec3(0, 0, 0), glm::vec3(0.01f, 0.01f, 0.01f) }, 0.5f, 1.0f, 0.5f, Utils::CENTER, glm::vec4(0.1, 1.0, 0.1, 1.0)));
	AddEntity(Player);
	Player->EntityMesh->AddCollisionBounds(0.6f, 1.0f, 0.6f, Player);
	EPlayer = Player;
	EPlayer->SetActive(true);

	std::shared_ptr<Spectator> Spec = std::make_shared<Spectator>(Spectator(Utils::Transform{ SpawnPos, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, Utils::CENTER));
	ESpectator = Spec;
	AddEntity(ESpectator);
	ESpectator->SetActive(false);

	CurrentController = Player;

	std::shared_ptr<SpeedBoostPickUp> NewPickup = std::make_shared<SpeedBoostPickUp>(SpeedBoostPickUp(Utils::Transform{ { 7.5f, -2.0f, 10.0f }, { 45, 45, 45 }, {1, 1, 1} }, Utils::CENTER, EPlayer));
	std::shared_ptr<Cube> NewPickupMesh = std::make_shared<Cube>(Cube(0.5f, 0.5f, 0.5f, { 0.4, 0.1, 0.6, 1.0f }));
	NewPickup->AddMesh(NewPickupMesh);
	NewPickupMesh->AddCollisionBounds(0.5f, 0.5f, 0.5f, NewPickup);
	NewPickupMesh->SetLit(true);
	AddEntity(NewPickup);

	std::shared_ptr<Entity> CubeCollision = std::make_shared<Entity>(Entity(Utils::Transform{ { 15.0f, -2.5f, 18.0f },{ 0, 0, 0 },{ 1, 1, 1 } }, Utils::BOTTOM_CENTER));
	std::shared_ptr<Cube> CubeCollisionMesh = std::make_shared<Cube>(Cube(2.0f, 2.0f, 2.0f, { 0.1, 0.3, 0.7, 1.0f }));
	CubeCollision->AddMesh(CubeCollisionMesh);
	CubeCollisionMesh->AddCollisionBounds(2.0f, 2.0f, 2.0f, CubeCollision);
	CubeCollisionMesh->SetLit(true);
	AddCollidable(CubeCollision);

	FPSCounterText = std::make_shared<UIText>(glm::vec2(Camera::GetInstance()->SCR_WIDTH - 30.0f, Camera::GetInstance()->SCR_HEIGHT - 20.0f), 0.0f, glm::vec4(0.6, 0.6, 0.6, 1.0), "FPS:", "Resources/Fonts/Roboto-Condensed.ttf", 20, Utils::TOP_RIGHT);
	//std::shared_ptr<UIImage> ScoreBack(new UIImage(glm::vec2(Camera::GetInstance()->SCR_WIDTH - 20.0f, 18.0f), Utils::TOP_RIGHT, 0.0f, glm::vec4(0.3, 0.3, 0.3, 1.0), 160, 50));
	//ScoreText = std::make_shared<UIText>(glm::vec2(Camera::GetInstance()->SCR_WIDTH - 30.0f, 20.0f), 0.0f, glm::vec4(0.36, 0.219, 0.188, 1.0), "Score: ", "Resources/Fonts/Pixeled.ttf", 30, Utils::TOP_RIGHT);

	AddUIElement(FPSCounterText);
	//AddUITextElement(ScoreText);
	fCameraSpeed = 0;// GameSettings::fMoveSpeed;

	//std::shared_ptr<Entity> Floor = std::make_shared<Entity>(Entity({ glm::vec3(0, -3, 0), glm::vec3(-90, 0, 0), glm::vec3(1, 1 ,1) }, Utils::CENTER));
	//std::shared_ptr<Plane> FloorMesh = std::make_shared<Plane>(20.0f, 20.0f, glm::vec4(0.3f, 0.5f, 1.0f, 1.0f), "Resources/BlockWithEdge.png", glm::vec4(0, 10, 0, 10));
	//Floor->AddMesh(FloorMesh);
	////std::shared_ptr<Entity> Floor = std::make_shared<Entity>(Entity({ glm::vec3(0, -3, 0), glm::vec3(-90, 0, 0), glm::vec3(1, 1 ,1) }, 20, 20, Utils::CENTER, glm::vec4(0.3f, 0.5f, 1.0f, 1.0f), "Resources/BlockWithEdge.png", Utils::PLANE, glm::vec4(0, 10, 0, 10)));
	//AddEntity(Floor);

	//std::shared_ptr<Entity> ImagePlane = std::make_shared<Entity>(Entity({ glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::CENTER));
	//std::shared_ptr<Plane> ImagePlaneMesh = std::make_shared<Plane>(1.0f, 1.0f, glm::vec4(0.5f, 0.8f, 1.0f, 1.0f), "Resources/AwesomeFace.png");
	//ImagePlane->AddMesh(ImagePlaneMesh);
	//AddEntity(ImagePlane);

	//std::shared_ptr<Entity> PlainPyamid = std::make_shared<Entity>(Entity({ glm::vec3(0, -3, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	//std::shared_ptr<Pyramid> PlainPyramidMesh = std::make_shared<Pyramid>(1.0f, 1.0f, 1.0f, glm::vec4(0.5f, 0.2f, 0.7f, 1.0f));
	//PlainPyamid->AddMesh(PlainPyramidMesh);
	//AddEntity(PlainPyamid);

	//std::shared_ptr<Entity> TexturedPyramid = std::make_shared<Entity>(Entity({ glm::vec3(-1, -3, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	//std::shared_ptr<Pyramid> TexturedPyramidMesh = std::make_shared<Pyramid>(1.0f, 1.0f, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "Resources/AwesomeFace.png");
	//TexturedPyramid->AddMesh(TexturedPyramidMesh);
	//AddEntity(TexturedPyramid);

	//std::shared_ptr<Entity> TexturedLitSphere = std::make_shared<Entity>(Entity({ glm::vec3(3, -3, 3), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	//std::shared_ptr<Sphere> TexturedLitSphereMesh = std::make_shared<Sphere>(1.0f, 1.0f, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "Resources/Rayman.jpg");
	//TexturedLitSphere->AddMesh(TexturedLitSphereMesh);
	//TexturedLitSphereMesh->SetLit(true);
	//AddEntity(TexturedLitSphere);

	std::shared_ptr<Entity> Target = std::make_shared<Entity>(Entity({ glm::vec3(0, -3, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	std::shared_ptr<Sphere> TargetMesh = std::make_shared<Sphere>(0.5f, 2.0f, 0.5f, glm::vec4(1.0f, 0.5f, 0.1f, 1.0f));
	TargetMesh->SetLit(true);
	Target->AddMesh(TargetMesh);
	TargetMesh->AddCollisionBounds(0.5f, 2.0f, 0.5f, Target);
	//TargetMesh->LightProperties.v3LightColour = {0.8f, 0.1f, 0.1f};
	//AddEntity(Target);
	AddCollidable(Target);
	PersuitTarget = Target;

	std::shared_ptr<Cube> EnemeyMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(0.1f, 1.0f, 0.1f, 1.0f), "Resources/Enemy1.png");

	//for (int i = 0; i < 20; i++)
	//{
	//	std::shared_ptr<Enemy1> NewEnemy = std::make_shared<Enemy1>(Enemy1({ glm::vec3(-5 + i * 0.3f, -2, -5), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER, { 0,0,0 }));
	//	EnemeyMesh->SetLit(true);
	//	NewEnemy->AddMesh(EnemeyMesh);
	//	EnemeyMesh->MeshCollisionBounds = std::make_shared<CollisionBounds>(1, 1, 1, NewEnemy);
	//	AddEnemy(NewEnemy);
	//	//AddEntity(NewEnemy);
	//}
	for (int i = 0; i < 20; i++)
	{
		std::shared_ptr<Enemy2> NewEnemy = std::make_shared<Enemy2>(Enemy2({ glm::vec3(-5 + i * 0.3f, -2, -5), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
		EnemeyMesh->SetLit(true);
		NewEnemy->AddMesh(EnemeyMesh);
		EnemeyMesh->AddCollisionBounds(1, 1, 1, NewEnemy);
		AddEnemy(NewEnemy);
		NewEnemy->SetTarget(PersuitTarget);
		//AddEntity(NewEnemy);
	}

	//Enemy1Ref = NewEnemy;

	/*TargetRef = Target;


	TempTarget = std::make_shared<Entity>(Entity({ glm::vec3(5, -3, 5), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, Utils::BOTTOM_CENTER));
	TargetMesh = std::make_shared<Sphere>(1.0f, 2.0f, 1.0f, glm::vec4(0.0f, 0.9f, 0.1f, 1.0f));
	TargetMesh->SetLit(true);
	TempTarget->AddMesh(TargetMesh);
	AddEntity(TempTarget);

	std::shared_ptr<Enemy2> NewPersueEnemy = std::make_shared<Enemy2>(Enemy2({ glm::vec3(5, -2, -5), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	std::shared_ptr<Cube> EnemeyPursueMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(1.0f, 1.0f, 0.1f, 1.0f), "Resources/Enemy1.png");
	NewPersueEnemy->AddMesh(EnemeyPursueMesh);
	NewPersueEnemy->SetTarget(TempTarget);
	AddEntity(NewPersueEnemy);*/

	//std::shared_ptr<Enemy3> NewWanderEnemy = std::make_shared<Enemy3>(Enemy3({ glm::vec3(-5, -2.5, 5), glm::vec3(0, 0, 0), glm::vec3(0.1f, 0.1f, 0.1f) }, Utils::BOTTOM_CENTER));
	////std::shared_ptr<Cube> EnemeyBlueMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(0.1f, 0.1f, 1.0f, 1.0f), "Resources/Enemy1.png");
	//std::shared_ptr<Model> SkullMesh = std::make_shared<Model>(Model({ 1.0f, 1.0f, 1.0f, 1.0f },"Resources/Models/LowPoly_Pixel_RPG_Assets_DevilsGarage_v01/3D/skull.obj"));
	//NewWanderEnemy->AddMesh(SkullMesh);
	//AddEntity(NewWanderEnemy);


	/*std::shared_ptr<Entity> ModelEnt = std::make_shared<Entity>(Entity({ glm::vec3(5, -5.0, 8), glm::vec3(0, 0, 0), glm::vec3(0.1f, 0.1f, 0.1f) }, Utils::BOTTOM_CENTER));
	std::shared_ptr<Model> NewModelMesh = std::make_shared<Model>(Model({ 1.0f, 1.0f, 1.0f, 1.0f }, "Resources/Models/Isometric_3D_Hex_Pack/groundEarth.fbx"));
	ModelEnt->AddMesh(NewModelMesh);
	AddEntity(ModelEnt);*/


	//std::shared_ptr<Cursor> NewCursor = std::make_shared<Cursor>("Resources/Grey_Cursor.png");
	//NewCursor->SetVisibleRange({ 500, 150 });
	//AddUIElement(NewCursor);
	//TestModel = new Model("Resources/Models/Tank/Tank.obj", Camera::GetInstance());
	
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
	Enemies.clear();
	//delete TestModel;
	//TestModel = nullptr;
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Level::Update()
{
	float fDotProductDirections = glm::dot(Camera::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos), glm::vec3(0, 1, 0));
	if (fDotProductDirections == 0) // Perpendicular
	{
		std::cout << "Perpendicular to plane\n";
		PersuitTarget->transform.Position = Camera::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos) * 15.0f + Camera::GetInstance()->GetCameraPosition();
	}
	else
	{
		float fDistance = -((glm::dot(Camera::GetInstance()->GetCameraPosition(), glm::vec3(0, 1, 0)) + 2.5f) /
			(fDotProductDirections));
		PersuitTarget->transform.Position = Camera::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos) * fDistance + Camera::GetInstance()->GetCameraPosition();
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
	/*if (EPlayer->EntityMesh->MeshCollisionBounds->isColliding(TargetRef))
	{
		std::cout << "Player Colliding with Target!\n";
	}*/
	//if (!TestParticleSystem)
	//	std::cout << "particle system destroyed\n";
	FPSCounterText->sText = "FPS: " + std::to_string(Time::dFPS);
	Scene::Update(); // Call super/base Update

	if (Input::GetInstance()->KeyState[(unsigned char)'r'] == Input::INPUT_FIRST_PRESS)
	{
		RestartLevel();
	}

	/*if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_FIRST_PRESS)
	{
		std::shared_ptr<Entity> Bullet = std::make_shared<Entity>(Entity({ EPlayer->transform.Position, EPlayer->transform.Rotation, glm::vec3(0.1f, 0.1f, 0.1f) }, Utils::BOTTOM_CENTER));
		std::shared_ptr<Cube> BulletCube = std::make_shared<Cube>(Cube(1, 1, 1, { 1,1,1,1 }));
		Bullet->AddMesh(BulletCube);
		AddEntity(Bullet);
	}*/

	/*TempTarget->transform.Position += glm::vec3(4 * Time::dTimeDelta, 0, 0);*/

	/*if (EPlayer->transform.Position.y < -5)
	{
		GameManager::GetInstance()->PlayerDeath();
	}

	if (EPlayer->transform.Position.x > Camera::GetInstance()->GetCameraPosition().x + 2)
	{
		Camera::GetInstance()->MoveCamera(glm::vec3(fCameraSpeed * Time::dTimeDelta, 0, 0));
	}
	else if (EPlayer->transform.Position.x < Camera::GetInstance()->GetCameraPosition().x - 2)
	{
		Camera::GetInstance()->MoveCamera(glm::vec3(-fCameraSpeed * (float)Time::dTimeDelta, 0, 0));
	}*/

	//ScoreText->sText = "Score: " + std::to_string(iScore);
}

void Level::RenderScene()
{
	Scene::RenderScene();

	//TestModel->Render();
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
	AddEntity(Enemy);
	Enemies.push_back(Enemy);
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
	std::shared_ptr<Entity> ModelEnt = std::make_shared<Entity>(Entity({ _v3Postion, Rotation, glm::vec3(0.1f, 0.1f, 0.1f) }, Utils::BOTTOM_CENTER));
	std::shared_ptr<Model> NewModelMesh = std::make_shared<Model>(Model({ 1.0f, 1.0f, 1.0f, 1.0f }, _ModelPath.c_str()));
	ModelEnt->AddMesh(NewModelMesh);
	NewModelMesh->SetLit(true);
	NewModelMesh->LightProperties.fAmbientStrength = 0.7f;
	AddEntity(ModelEnt);
}

/************************************************************
#--Description--#:  Respawns current level enemies
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Level::RespawnEnemies()
{
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
	int iRandTrack = 3;//rand() % 5;
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
	//PlayRandomTrack();
	UIManager::GetInstance()->SwitchUIMode(false);
	for (auto& it : Enemies)
	{
		std::shared_ptr<Enemy1> IsEnemy1 = std::dynamic_pointer_cast<Enemy1>(it);
		if (IsEnemy1)
		{
			IsEnemy1->AddPathPoints();
		}
		
	}
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
	Camera::GetInstance()->SetCameraPos(glm::vec3(0, 0, 3));
	iScore = iCoinsCollected * 5;
	RespawnEnemies();
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
