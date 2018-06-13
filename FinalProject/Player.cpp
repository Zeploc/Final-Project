//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Player.cpp
// Description    	:    main implementation for Player
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <iostream>

// OpenGL Library Includes //
#include <glm\gtx\string_cast.hpp>

// Engine Includes //
#include "Engine/Utils.h"
#include "Engine/SceneManager.h"
#include "Engine/Scene.h"
#include "Engine\Entity.h"
#include "Engine\Input.h"
#include "Engine\Time.h"
#include "Engine\Sphere.h"
#include "Engine\Model.h"
#include "Engine\Cube.h"
#include "Engine\CollisionBounds.h"

// Local Includes //
#include "Level.h"
#include "UIManager.h"
#include "Level.h"
#include "Boss.h"
#include "GameManager.h"


// This Includes //
#include "Player.h"

// Static Variables //


// Types //
#define GetSign(a) (a < 0 ? -1 : (a > 0 ? 1 : 0)) // If num is smaller than 0, ie negative, return -1, else return 1

// Math Library //
#define _USE_MATH_DEFINES
#include <math.h>

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Player::Player(Utils::Transform _Transform, float _fWidth, float _fHeight, float _fDepth, Utils::EANCHOR _Anchor, glm::vec4 _Colour)
	: Entity(_Transform, _Anchor)
{
	//std::shared_ptr<Sphere> NewMesh = std::make_shared<Sphere>(_fWidth, _fHeight, _fDepth, _Colour);
	std::shared_ptr<Model> NewModelMesh = std::make_shared<Model>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "Resources/Models/LowPoly_Pixel_RPG_Assets_DevilsGarage_v01/3D/char01.obj");
	AddMesh(NewModelMesh);
	EntityMesh->SetLit(true);
	SetHealth(100);
}

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Player::Player(Utils::Transform _Transform, float _fWidth, float _fHeight, float _fDepth, Utils::EANCHOR _Anchor, glm::vec4 _Colour, const char * TextureSource, glm::vec4 UVCoords)
	: Entity(_Transform, _Anchor)
{
	std::shared_ptr<Sphere> NewMesh = std::make_shared<Sphere>(_fWidth, _fHeight, _fDepth, _Colour, TextureSource, UVCoords);
	AddMesh(NewMesh);
	EntityMesh->SetLit(true);
	SetHealth(100);
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Player::~Player()
{	
	for (auto& it : Bullets)
	{
		SceneManager::GetInstance()->GetCurrentScene()->DestroyEntity(it.BulletEntity);
		it.BulletEntity = nullptr;
	}
	Bullets.clear();
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Player::Update()
{
	std::shared_ptr<Level> GotLevel = std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->GetCurrentScene());
	if (!GotLevel || !bActive || !UIManager::GetInstance()->m_bFPS || !GameManager::GetInstance()->IsPlayerAlive()) return;
	
	if (transform.Position.y < -20.0f)
	{		
		GameManager::GetInstance()->PlayerDeath();
		return;
	}

	glm::vec3 MouseWorldPosition = { 0,0,0 };

	float fDotProductDirections = glm::dot(Camera::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos), glm::vec3(0, 1, 0));
	if (fDotProductDirections != 0) // Perpendicular
	{
	
		float fDistance = -((glm::dot(Camera::GetInstance()->GetCameraPosition(), glm::vec3(0, 1, 0)) + 2.5f) /
		(fDotProductDirections));
		MouseWorldPosition = Camera::GetInstance()->ScreenToWorldDirection(Input::GetInstance()->MousePos) * fDistance + Camera::GetInstance()->GetCameraPosition();
	}

	glm::vec3 VectorToMouseFromPlayer = MouseWorldPosition - this->transform.Position;
	VectorToMouseFromPlayer = glm::normalize(VectorToMouseFromPlayer);
	float fDir = glm::dot(glm::cross(VectorToMouseFromPlayer, glm::vec3(0, 0, -1)), { 0, 1, 0 });
	float AngleToMouse = acos(glm::dot(VectorToMouseFromPlayer, glm::vec3(0, 0, -1)));
	if (fDir > 0) AngleToMouse = M_PI + ((2 * M_PI) - AngleToMouse + M_PI);
	this->transform.Rotation.y = (AngleToMouse / (M_PI * 2)) * 360 + 180;
	 
		
	if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_HOLD || Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_FIRST_PRESS)
	{
		if (BulletTimer <= 0)
		{
			Bullet NewBullet;
			std::shared_ptr<Entity> Bullet = std::make_shared<Entity>(Entity({ this->transform.Position, this->transform.Rotation, glm::vec3(0.1f, 0.1f, 0.1f) }, Utils::BOTTOM_CENTER));
			std::shared_ptr<Cube> BulletCube = std::make_shared<Cube>(Cube(1, 1, 1, { 1,0,0,1 }));
			BulletCube->AddCollisionBounds(0.3f, 0.3f, 0.3f, Bullet);
			Bullet->AddMesh(BulletCube);
			SceneManager::GetInstance()->GetCurrentScene()->AddEntity(Bullet);
			glm::vec3 BulletDirection = glm::normalize(VectorToMouseFromPlayer);
			NewBullet.CurrentVelocity = (BulletDirection*BulletSpeed);
			NewBullet.BulletEntity = Bullet;
			Bullets.push_back(NewBullet);
			if (FireRatePickup)
			{
				BulletTimer = 0.5f;
			}
			else
			{
				BulletTimer = 0.12f;
			}
			
		}
	}

	BulletTimer -= Time::dTimeDelta;
	if (Bullets.size() != 0)
	{
		HandleBullets();
	}
	
	if (Input::GetInstance()->KeyState[(unsigned char)'d'] == Input::INPUT_HOLD || Input::GetInstance()->KeyState[(unsigned char)'d'] == Input::INPUT_FIRST_PRESS)
	{
		MoveHorizontally(false);
	}
	else if (Input::GetInstance()->KeyState[(unsigned char)'a'] == Input::INPUT_HOLD || Input::GetInstance()->KeyState[(unsigned char)'a'] == Input::INPUT_FIRST_PRESS)
	{
		MoveHorizontally(true);
	}
	else
	{
		v3Speed.x = 0;
	}

	if (Input::GetInstance()->KeyState[(unsigned char)'w'] == Input::INPUT_HOLD || Input::GetInstance()->KeyState[(unsigned char)'w'] == Input::INPUT_FIRST_PRESS)
	{
		MoveVertical(true);
	}
	else if (Input::GetInstance()->KeyState[(unsigned char)'s'] == Input::INPUT_HOLD || Input::GetInstance()->KeyState[(unsigned char)'s'] == Input::INPUT_FIRST_PRESS)
	{
		MoveVertical(false);
	}
	else
	{
		v3Speed.z = 0;
	}
	
	if (RollTimer > 0 && Input::GetInstance()->KeyState[(unsigned char)' '] == Input::INPUT_FIRST_PRESS && bHasDodged == true)
	{
		v3Speed.x *= 35;
		v3Speed.z *= 35;
		bHasDodged = false;
		DodgeCooldown = 1.0f;
	}

	if (Input::GetInstance()->KeyState[(unsigned char)' '] == Input::INPUT_FIRST_PRESS && bHasDodged == false && DodgeCooldown < 0)
	{
		v3Speed.x *= 15;
		v3Speed.z *= 15;
		bHasDodged = true;
		RollTimer = 0.5f;		
	}

	/*if (m_fHealth <= 0)
	{
		this->shared_from_this()->SetActive(false);
		this->shared_from_this()->SetVisible(false);
	}*/


	RollTimer -= Time::dTimeDelta;
	

	if (RollTimer < 0.0f)
	{
		bHasDodged = false; 
	}
	
	DodgeCooldown -= Time::dTimeDelta;

	bool bAddGrav = true;
	for (auto it : GotLevel->Collidables)
	{
		if (EntityMesh->GetCollisionBounds()->CheckCollision(it, glm::vec3(v3Speed.x, v3Speed.y - GameSettings::fGravity * (float)Time::dTimeDelta, v3Speed.z))) // Checks if will collide at - gravity
		{
			if (it->transform.Position.y >= transform.Position.y)
				v3Speed.y = EntityMesh->GetCollisionBounds()->GetDistance(it).y; // Moves up to object (moves up the distance from object)				
			else
			{
				v3Speed.y = -EntityMesh->GetCollisionBounds()->GetDistance(it).y; // Moves up to object (moves down the distance from object)
				//bJump = false;
			}
			bAddGrav = false;				
			break;
		}
	}

	if (GotLevel->Collidables.size() == 0 || bAddGrav)
		if (v3Speed.y > -GameSettings::fTerminalVelocity * (float)Time::dTimeDelta) v3Speed.y -= GameSettings::fGravity * (float)Time::dTimeDelta; // moves down with -gravity
		else v3Speed.y = -GameSettings::fTerminalVelocity * (float)Time::dTimeDelta;
	
	Translate(glm::vec3(v3Speed.x, v3Speed.y, v3Speed.z));
	//GotLevel->DebugText->sText = std::to_string(v2HSpeed.z);

	if (CurrentPowerUp != NONE)
	{
		m_fPowerUpTimer -= Time::dTimeDelta;
		if (m_fPowerUpTimer <= 0)
		{
			m_fPowerUpTimer = 0;
			PowerUpComplete();
			CurrentPowerUp = NONE;
		}
	}
}

/************************************************************
#--Description--#:  Respawn player
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Player::Reset()
{
	std::shared_ptr<Level> GotLevel = std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->Scenes[SceneManager::GetInstance()->CurrentScene]);
	v3Speed.x = 0;
	v3Speed.z = 0;
	v3Speed.y = 0;
	transform.Position = GotLevel->SpawnPos;
	SetHealth(100);
	SetScore(0);
}


void Player::SetHealth(float _fNewHealth)
{
	m_fHealth = _fNewHealth;
	UIManager::GetInstance()->m_HUDInstance.SetHealth(m_fHealth);
	//if (m_fHealth <= 0)
		//GameManager::GetInstance()->PlayerDeath();
}

void Player::ApplyHealth(float _fmodify)
{
	m_fHealth += _fmodify;
	m_fHealth = max(m_fHealth, 0.0f);
	m_fHealth = min(m_fHealth, 100.0f);
	UIManager::GetInstance()->m_HUDInstance.SetHealth(m_fHealth);
	//if (m_fHealth <= 0)
		//GameManager::GetInstance()->PlayerDeath();
}

void Player::AddScore(int _iAddScore)
{
	m_iScore += _iAddScore;
	UIManager::GetInstance()->m_HUDInstance.SetScore(m_iScore);
}

void Player::SetScore(int _iNewScore)
{
	m_iScore = _iNewScore;
	UIManager::GetInstance()->m_HUDInstance.SetScore(m_iScore);
}

void Player::ApplyPowerUp(POWERUPS _PowerUp, float _fPowerUpTime)
{
	m_fPowerUpTimer = _fPowerUpTime;
	CurrentPowerUp = _PowerUp;
	switch (_PowerUp)
	{
	case NONE:
		break;
	case SPEEDBOOST:
		m_fCurrentPlayerSpeed = GameSettings::fMoveSpeed * 2;
		break;
	case FIRERATE:
	{
		FireRatePickup = true;
		break;
	}
	default:
		break;
	}
}

void Player::PowerUpComplete()
{
	switch (CurrentPowerUp)
	{
	case SPEEDBOOST:
		m_fCurrentPlayerSpeed = GameSettings::fMoveSpeed;
		break;
	case FIRERATE:
		FireRatePickup = false;
	default:
		break;
	}
}

void Player::HandleBullets()
{
	bool bBackToStart = false;
	auto iEndPos = Bullets.end();
	for (auto it = Bullets.begin(); it != iEndPos; it++)
	{
		if (bBackToStart)
		{
			it = Bullets.begin();
			bBackToStart = false;
		}
		it->BulletEntity->transform.Position += it->CurrentVelocity * (float)Time::dTimeDelta;
		it->Timer -= Time::dTimeDelta;

		if (it->Timer <= 0)
		{
			SceneManager::GetInstance()->GetCurrentScene()->DestroyEntity(it->BulletEntity);
			it = Bullets.erase(it);
			if (it == Bullets.begin())
			{
				bBackToStart = true;
			}
			else
				it--;
			if (Bullets.size() == 0) break;
			iEndPos = Bullets.end();
		}
	}
	bBackToStart = false;
	std::shared_ptr<Level> LevelRef = dynamic_pointer_cast<Level>(SceneManager::GetInstance()->GetCurrentScene());
	auto BulletEnd = Bullets.end();
	for (auto Bulletit = Bullets.begin(); Bulletit != BulletEnd; ++Bulletit)
	{
		if (bBackToStart)
		{
			Bulletit = Bullets.begin();
			bBackToStart = false;
		}
		for (auto& it : LevelRef->CurrentEnemies)
		{
			if (Bulletit->BulletEntity->EntityMesh->GetCollisionBounds()->isColliding(it))
			{
				std::shared_ptr<Boss> IsBoss = std::dynamic_pointer_cast<Boss>(it);
				if (IsBoss)
				{
					IsBoss->OnBulletCollision();
					if (IsBoss->BossHealth <= 0 && IsBoss->IsActive())
					{
						it->SetVisible(false);
						it->SetActive(false);
						//Bulletit.BulletEntity->SetActive(false);
						//Bulletit.BulletEntity->SetVisible(false);
						LevelRef->DestroyCollidable(Bulletit->BulletEntity);
						Bulletit = Bullets.erase(Bulletit);
						BulletEnd = Bullets.end();
						AddScore(50);
						if (Bulletit == Bullets.begin())
						{
							bBackToStart = true;
						}
						else
							Bulletit--;
						if (Bullets.size() == 0) break;
					}
				}
				else if (it->IsActive())
				{
					// Temp bullet kill
					it->SetVisible(false);
					it->SetActive(false);
					//Bulletit.BulletEntity->SetActive(false);
					//Bulletit.BulletEntity->SetVisible(false);
					LevelRef->DestroyCollidable(Bulletit->BulletEntity);
					Bulletit = Bullets.erase(Bulletit);
					BulletEnd = Bullets.end();
					AddScore(10);
					if (Bulletit == Bullets.begin())
					{
						bBackToStart = true;
					}
					else
						Bulletit--;
					if (Bullets.size() == 0) break;
				}
			}
		}
		if (Bullets.size() == 0) break;

	}
}

/************************************************************
#--Description--#:  Move player in specified direction
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes bool for direction true if left
#--Return--#: 		NA
************************************************************/
void Player::MoveHorizontally(bool bLeft)
{
	int Direction;
	if (bLeft) Direction = -1;
	else Direction = 1;
	std::shared_ptr<Level> GotLevel = std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->Scenes[SceneManager::GetInstance()->CurrentScene]);
	if (GotLevel)
	{
		if (GotLevel->Collidables.size() == 0)
		{
			if (bLeft)
				v3Speed.x = -m_fCurrentPlayerSpeed * (float)Time::dTimeDelta;
			else
				v3Speed.x = m_fCurrentPlayerSpeed * (float)Time::dTimeDelta;
			return;
		}
		for (auto it : GotLevel->Collidables)
		{
			if (EntityMesh->GetCollisionBounds()->CheckCollision(it, glm::vec3(m_fCurrentPlayerSpeed * Time::dTimeDelta * Direction, 0, 0)))
			{
					v3Speed.x = Direction * abs(EntityMesh->GetCollisionBounds()->GetDistance(it).x);
					break;
			}
			else
			{
				if (bLeft)
					v3Speed.x = -m_fCurrentPlayerSpeed * (float)Time::dTimeDelta;
				else
					v3Speed.x = m_fCurrentPlayerSpeed * (float)Time::dTimeDelta;
			}
		}
	}
}

void Player::MoveVertical(bool bUp)
{
	int Direction;
	if (bUp) Direction = -1;
	else Direction = 1;
	std::shared_ptr<Level> GotLevel = std::dynamic_pointer_cast<Level>(SceneManager::GetInstance()->Scenes[SceneManager::GetInstance()->CurrentScene]);
	if (GotLevel)
	{
		if (GotLevel->Collidables.size() == 0)
		{
			if (bUp)
				v3Speed.z = -m_fCurrentPlayerSpeed * (float)Time::dTimeDelta;
			else
				v3Speed.z = m_fCurrentPlayerSpeed * (float)Time::dTimeDelta;
			return;
		}
		for (auto it : GotLevel->Collidables)
		{

			if (EntityMesh->GetCollisionBounds()->CheckCollision(it, glm::vec3(0, 0, m_fCurrentPlayerSpeed * Time::dTimeDelta * Direction)))
			{
				v3Speed.z = Direction * abs(EntityMesh->GetCollisionBounds()->GetDistance(it).z);
				break;
			}
			else
			{
				if (bUp)
					v3Speed.z = -m_fCurrentPlayerSpeed * (float)Time::dTimeDelta;
				else
					v3Speed.z = m_fCurrentPlayerSpeed * (float)Time::dTimeDelta;
			}
		}
	}
}

void Player::HurtPlayer(float Damage)
{
	ApplyHealth(-Damage);
}

