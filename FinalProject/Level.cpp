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

// Local Includes //
#include "Player.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"

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
	Camera::GetInstance()->m_bFPS = false;
	std::shared_ptr<Entity> WorldCubeMap = std::make_shared<Entity>(Entity({ { 0, 0, 0 },{ 0, 0, 0 },{ 1, 1, 1 } }, Utils::CENTER));
	char *  TextureSources[6] = { "right.jpg", "left.jpg" , "top.jpg" , "bottom.jpg" , "back.jpg" , "front.jpg" };
	std::shared_ptr<CubeMap> WorldCubeMapMesh = std::make_shared<CubeMap>(CubeMap(1000.0f, 1000.0f, 1000.0f, TextureSources));
	WorldCubeMap->AddMesh(WorldCubeMapMesh);
	AddEntity(WorldCubeMap);
	// Add cube map first so transpancy works
	std::shared_ptr<Player> Player(new Player(Utils::Transform{ glm::vec3(SpawnPos, 0.1f), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1) }, 0.5f, 1.0f, 0.5f, Utils::CENTER, glm::vec4(0.1, 1.0, 0.1, 1.0)));
	AddEntity(Player);
	EPlayer = Player;                


	FPSCounterText = std::make_shared<UIText>(glm::vec2(Camera::GetInstance()->SCR_WIDTH - 30.0f, Camera::GetInstance()->SCR_HEIGHT - 20.0f), 0.0f, glm::vec4(0.6, 0.6, 0.6, 1.0), "FPS:", "Resources/Fonts/Roboto-Condensed.ttf", 20, Utils::TOP_RIGHT);
	//std::shared_ptr<UIImage> ScoreBack(new UIImage(glm::vec2(Camera::GetInstance()->SCR_WIDTH - 20.0f, 18.0f), Utils::TOP_RIGHT, 0.0f, glm::vec4(0.3, 0.3, 0.3, 1.0), 160, 50));
	//ScoreText = std::make_shared<UIText>(glm::vec2(Camera::GetInstance()->SCR_WIDTH - 30.0f, 20.0f), 0.0f, glm::vec4(0.36, 0.219, 0.188, 1.0), "Score: ", "Resources/Fonts/Pixeled.ttf", 30, Utils::TOP_RIGHT);

	AddUIElement(FPSCounterText);
	//AddUITextElement(ScoreText);
	fCameraSpeed = 0;// GameSettings::fMoveSpeed;

	std::shared_ptr<Entity> Floor = std::make_shared<Entity>(Entity({ glm::vec3(0, -3, 0), glm::vec3(-90, 0, 0), glm::vec3(1, 1 ,1) }, Utils::CENTER));
	std::shared_ptr<Plane> FloorMesh = std::make_shared<Plane>(20.0f, 20.0f, glm::vec4(0.3f, 0.5f, 1.0f, 1.0f), "Resources/BlockWithEdge.png", glm::vec4(0, 10, 0, 10));
	Floor->AddMesh(FloorMesh);
	//std::shared_ptr<Entity> Floor = std::make_shared<Entity>(Entity({ glm::vec3(0, -3, 0), glm::vec3(-90, 0, 0), glm::vec3(1, 1 ,1) }, 20, 20, Utils::CENTER, glm::vec4(0.3f, 0.5f, 1.0f, 1.0f), "Resources/BlockWithEdge.png", Utils::PLANE, glm::vec4(0, 10, 0, 10)));
	AddEntity(Floor);

	std::shared_ptr<Entity> ImagePlane = std::make_shared<Entity>(Entity({ glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::CENTER));
	std::shared_ptr<Plane> ImagePlaneMesh = std::make_shared<Plane>(1.0f, 1.0f, glm::vec4(0.5f, 0.8f, 1.0f, 1.0f), "Resources/AwesomeFace.png");
	ImagePlane->AddMesh(ImagePlaneMesh);
	AddEntity(ImagePlane);

	std::shared_ptr<Entity> PlainPyamid = std::make_shared<Entity>(Entity({ glm::vec3(0, -3, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	std::shared_ptr<Pyramid> PlainPyramidMesh = std::make_shared<Pyramid>(1.0f, 1.0f, 1.0f, glm::vec4(0.5f, 0.2f, 0.7f, 1.0f));
	PlainPyamid->AddMesh(PlainPyramidMesh);
	AddEntity(PlainPyamid);
	std::shared_ptr<Entity> TexturedPyramid = std::make_shared<Entity>(Entity({ glm::vec3(-1, -3, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	std::shared_ptr<Pyramid> TexturedPyramidMesh = std::make_shared<Pyramid>(1.0f, 1.0f, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "Resources/AwesomeFace.png");
	TexturedPyramid->AddMesh(TexturedPyramidMesh);
	AddEntity(TexturedPyramid);

	std::shared_ptr<Entity> TexturedLitSphere = std::make_shared<Entity>(Entity({ glm::vec3(3, -3, 3), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	std::shared_ptr<Sphere> TexturedLitSphereMesh = std::make_shared<Sphere>(1.0f, 1.0f, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "Resources/Rayman.jpg");
	TexturedLitSphere->AddMesh(TexturedLitSphereMesh);
	TexturedLitSphereMesh->bIsLit = true;
	AddEntity(TexturedLitSphere);

	std::shared_ptr<Entity> Target = std::make_shared<Entity>(Entity({ glm::vec3(5, -3, 5), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	std::shared_ptr<Sphere> TargetMesh = std::make_shared<Sphere>(1.0f, 2.0f, 1.0f, glm::vec4(1.0f, 0.1f, 0.1f, 1.0f));
	TargetMesh->bIsLit = true;
	Target->AddMesh(TargetMesh);
	AddEntity(Target);

	std::shared_ptr<Enemy1> NewEnemy = std::make_shared<Enemy1>(Enemy1({ glm::vec3(-5, -2, -5), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER, { 2,0,0 }));
	std::shared_ptr<Cube> EnemeyMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(0.1f, 1.0f, 0.1f, 1.0f), "Resources/Enemy1.png");
	NewEnemy->AddMesh(EnemeyMesh);
	NewEnemy->Target = Target;
	AddEntity(NewEnemy);

	TempTarget = std::make_shared<Entity>(Entity({ glm::vec3(5, -3, 5), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	TargetMesh = std::make_shared<Sphere>(1.0f, 2.0f, 1.0f, glm::vec4(0.0f, 0.9f, 0.1f, 1.0f));
	TargetMesh->bIsLit = true;
	TempTarget->AddMesh(TargetMesh);
	AddEntity(TempTarget);

	std::shared_ptr<Enemy2> NewPersueEnemy = std::make_shared<Enemy2>(Enemy2({ glm::vec3(5, -2, -5), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	std::shared_ptr<Cube> EnemeyPursueMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(0.1f, 1.0f, 0.1f, 1.0f), "Resources/Enemy1.png");
	NewPersueEnemy->AddMesh(EnemeyMesh);
	NewPersueEnemy->SetTarget(TempTarget);
	AddEntity(NewPersueEnemy);
	
	std::shared_ptr<Enemy3> NewWanderEnemy = std::make_shared<Enemy3>(Enemy3({ glm::vec3(-5, -2.5, 5), glm::vec3(0, 0, 0), glm::vec3(1, 1 ,1) }, Utils::BOTTOM_CENTER));
	std::shared_ptr<Cube> EnemeyBlueMesh = std::make_shared<Cube>(1.0f, 1.0f, 1.0f, glm::vec4(0.1f, 0.1f, 1.0f, 1.0f), "Resources/Enemy1.png");
	NewWanderEnemy->AddMesh(EnemeyBlueMesh);
	AddEntity(NewWanderEnemy);

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
	//if (!TestParticleSystem)
	//	std::cout << "particle system destroyed\n";
	FPSCounterText->sText = "FPS: " + std::to_string(Time::dFPS);
	Scene::Update(); // Call super/base Update

	if (Input::GetInstance()->KeyState[(unsigned char)'r'] == Input::INPUT_FIRST_PRESS)
	{
		RestartLevel();
	}

	TempTarget->transform.Position += glm::vec3(4 * Time::dTimeDelta, 0, 0);

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
	return;
	SoundManager::GetInstance()->StopAudio("BackgroundC");
	const char* MusicOptions[] = { "Resources/Sound/Ludum Dare 28 - Track 1.wav",
		"Resources/Sound/Ludum Dare 28 - Track 3.wav", "Resources/Sound/Ludum Dare 30 - Track 6.wav",
		"Resources/Sound/Ludum Dare 30 - Track 7.wav", "Resources/Sound/Ludum Dare 38 - Track 10.wav" };
	int iRandTrack = rand() % 5;
	std::cout << "Playing " << MusicOptions[iRandTrack] << " | Number " << iRandTrack << std::endl;
	SoundManager::GetInstance()->AddAudio(MusicOptions[iRandTrack], true, "GameBackgroundTrack " + std::to_string(iRandTrack));
	SoundManager::GetInstance()->PlayAudio("GameBackgroundTrack " + std::to_string(iRandTrack), "BackgroundC");
	SoundManager::GetInstance()->SetChannelVolume("BackgroundC", 0.3f);
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
	UIManager::GetInstance()->m_bDisplayChat = true;
	UIManager::GetInstance()->SwitchUIMode(false);
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
void Level::SetPlayerPosition(glm::vec2 Pos)
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
