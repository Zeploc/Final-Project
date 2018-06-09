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
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Player::~Player()
{
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
	if (!GotLevel || !bActive) return;
	
	/*glm::vec3 LookAtDirection = Target - Source;*/
	//if (Input::GetInstance()->KeyState[(unsigned char)'d'] == Input::INPUT_FIRST_PRESS)
	//{
	//	transform.Scale.x = 1;
	//	if (AnimationInfo.v2CurrentFrame.y < 2)
	//	{
	//		if (CollisionBox.v2Offset.y == -0.17f && ((AnimationInfo.v2CurrentFrame.x >= 4 && AnimationInfo.v2CurrentFrame.y == 0) || (AnimationInfo.v2CurrentFrame.x == 0 && AnimationInfo.v2CurrentFrame.y == 1)))
	//		{
	//			transform.Position.y += 0.02f;
	//		}
	//		CollisionBox.v2Offset.y = -0.10f;
	//		AnimationInfo.v2StartFrame = { 1, 1 };
	//		AnimationInfo.v2CurrentFrame = { 1, 1 };
	//		AnimationInfo.v2EndFrame = { 6, 1 };
	//		CollisionBox.fWidth = 0.8f;
	//		CollisionBox.fHeight = 0.86f;
	//	}
	//}
	//else if (Input::GetInstance()->KeyState[(unsigned char)'a'] == Input::INPUT_FIRST_PRESS)
	//{
	//	transform.Scale.x = -1;
	//	if (AnimationInfo.v2CurrentFrame.y < 2)
	//	{
	//		if (CollisionBox.v2Offset.y == -0.17f && ((AnimationInfo.v2CurrentFrame.x >= 4 && AnimationInfo.v2CurrentFrame.y == 0) || (AnimationInfo.v2CurrentFrame.x == 0 && AnimationInfo.v2CurrentFrame.y == 1)))
	//		{
	//			transform.Position.y += 0.02f;
	//		}
	//		CollisionBox.v2Offset.y = -0.10f;
	//		AnimationInfo.v2StartFrame = { 1, 1 };
	//		AnimationInfo.v2CurrentFrame = { 1, 1 };
	//		AnimationInfo.v2EndFrame = { 6, 1 };
	//		CollisionBox.fWidth = 0.8f;
	//		CollisionBox.fHeight = 0.86f;
	//	}
	//}
	//else if (Input::GetInstance()->KeyState[(unsigned char)'s'] == Input::INPUT_FIRST_PRESS)
	//{
	//	AnimationInfo.v2StartFrame = { 4, 0 };
	//	AnimationInfo.v2CurrentFrame = { 4, 0 };
	//	AnimationInfo.v2EndFrame = { 0, 1 };
	//	CollisionBox.fWidth = 0.6f;
	//	CollisionBox.fHeight = 0.7f;
	//	CollisionBox.v2Offset.y = -0.17f;
	//}
	//else if ((((AnimationInfo.v2CurrentFrame.y == 1 && AnimationInfo.v2CurrentFrame.x != 0)|| AnimationInfo.v2CurrentFrame.y == 3) && Input::GetInstance()->KeyState[(unsigned char)'d'] == Input::INPUT_RELEASED && Input::GetInstance()->KeyState[(unsigned char)'a'] == Input::INPUT_RELEASED) || AnimationInfo.v2CurrentFrame == glm::vec2(2, 3))
	//{
	//	AnimationInfo.v2CurrentFrame = { 0, 0 };
	//	AnimationInfo.v2StartFrame = { 0, 0 };
	//	AnimationInfo.v2EndFrame = { 3, 0 };
	//	CollisionBox.fWidth = 0.6f;
	//	CollisionBox.fHeight = 0.86f;
	//	CollisionBox.v2Offset.y = -0.10f;
	//}
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
	this->transform.Rotation.y = (AngleToMouse / (M_PI * 2)) * 360;
	 
		
	if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_HOLD || Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::INPUT_FIRST_PRESS)
	{
		if (BulletTimer <= 0)
		{
			Bullet NewBullet;
			std::shared_ptr<Entity> Bullet = std::make_shared<Entity>(Entity({ this->transform.Position, this->transform.Rotation, glm::vec3(0.1f, 0.1f, 0.1f) }, Utils::BOTTOM_CENTER));
			std::shared_ptr<Cube> BulletCube = std::make_shared<Cube>(Cube(1, 1, 1, { 1,0,0,1 }));
			Bullet->AddMesh(BulletCube);
			SceneManager::GetInstance()->GetCurrentScene()->AddEntity(Bullet);
			glm::vec3 BulletDirection = glm::normalize(VectorToMouseFromPlayer);
			NewBullet.CurrentVelocity = (BulletDirection*BulletSpeed);
			NewBullet.BulletEntity = Bullet;
			Bullets.push_back(NewBullet);
			BulletTimer = 0.12f;
		}
	}

	BulletTimer -= Time::dTimeDelta;



	for (auto it = Bullets.begin(); it != Bullets.end(); it++)
	{
		it->BulletEntity->transform.Position += it->CurrentVelocity;

		it->Timer -= Time::dTimeDelta;

		//if (it->Timer <= 0)
		//{
		//	if (Bullets.size() != 0)
		//	{
		//		unsigned int iEndPos = Bullets.size() - 1;
		//		for (unsigned int i = 0; i <= iEndPos; i++)
		//		{
		//			if (iEndPos >= Bullets.size())
		//			{
		//				iEndPos = Bullets.size() - 1;
		//				i--;
		//			}
		//			if (Bullets[iEndPos] != Bullets.back()) // if current last value is not equal to the back of the vector
		//			{
		//				iEndPos = Bullets.size() - 1;
		//				//i--;
		//			}
		//		}
		//	}
		//	Bullets.erase(it);
		//	 
		//}
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
		fHSpeed = 0;
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
		fVSpeed = 0;
	}
	
	


	if (RollTimer > 0 && Input::GetInstance()->KeyState[(unsigned char)' '] == Input::INPUT_FIRST_PRESS && bHasDodged == true)
	{
		fHSpeed *= 35;
		fVSpeed *= 35;
		bHasDodged = false;
		DodgeCooldown = 1.0f;
	}

	if (Input::GetInstance()->KeyState[(unsigned char)' '] == Input::INPUT_FIRST_PRESS && bHasDodged == false && DodgeCooldown < 0)
	{
		fHSpeed *= 15;
		fVSpeed *= 15;
		bHasDodged = true;
		RollTimer = 0.5f;
		
	}

	
	
	RollTimer -= Time::dTimeDelta;
	

	if (RollTimer < 0.0f)
	{
		bHasDodged = false; 
	}
	
	DodgeCooldown -= Time::dTimeDelta;


	//Reducing the velocity to 0 if the movement button is let go
	

	//else
	//{
	//	if (Input::GetInstance()->KeyState[(unsigned char)'s'] == Input::INPUT_HOLD || Input::GetInstance()->KeyState[(unsigned char)'s'] == Input::INPUT_FIRST_PRESS)
	//	{
	//		if (AnimationInfo.v2CurrentFrame.x >= 1 && AnimationInfo.v2CurrentFrame.y == 3) // If running animation
	//		{
	//			// Set to running
	//			AnimationInfo.v2StartFrame = { 1, 1 };
	//			AnimationInfo.v2CurrentFrame = { 1, 1 };
	//			AnimationInfo.v2EndFrame = { 6, 1 };
	//			CollisionBox.fWidth = 0.8f;
	//			CollisionBox.fHeight = 0.86f;
	//			CollisionBox.v2Offset.y = -0.17f;
	//		}
	//		else if (AnimationInfo.v2CurrentFrame.x >= 0 && AnimationInfo.v2CurrentFrame.x <= 3 && AnimationInfo.v2CurrentFrame.y == 0) // If Standing/ Idle animation
	//		{
	//			// Set to Crouched
	//			AnimationInfo.v2StartFrame = { 4, 0 };
	//			//AnimationInfo.v2CurrentFrame = { 4, 0 };
	//			AnimationInfo.v2EndFrame = { 0, 1 };
	//			CollisionBox.fWidth = 0.6f;
	//			CollisionBox.fHeight = 0.7f;
	//			CollisionBox.v2Offset.y = -0.17f;
	//		}
	//	}
	//	else
	//	{
	//		if (!bJump)
	//		{
	//			AnimationInfo.v2StartFrame = { 0, 0 };
	//			AnimationInfo.v2EndFrame = { 3, 0 };
	//			CollisionBox.fWidth = 0.6f;
	//			CollisionBox.fHeight = 0.86f;
	//			if (CollisionBox.v2Offset.y == -0.17f && ((AnimationInfo.v2CurrentFrame.x >= 4 && AnimationInfo.v2CurrentFrame.y == 0) || (AnimationInfo.v2CurrentFrame.x == 0 && AnimationInfo.v2CurrentFrame.y == 1)))
	//			{
	//				transform.Position.y += 0.02f;
	//			}
	//			CollisionBox.v2Offset.y = -0.10f;
	//		}
	//	}
	//	fHSpeed = 0;
	//}
	//if (Input::GetInstance()->KeyState[(unsigned char)'w'] == Input::INPUT_FIRST_PRESS || Input::GetInstance()->KeyState[(unsigned char)'w'] == Input::INPUT_HOLD)
	//{
	//	if (!bJump)
	//	{
	//		bJump = true;
	//		fVSpeed = GameSettings::fJumpSize / 10;
	//		AnimationInfo.v2CurrentFrame = { 0, 2 };
	//		AnimationInfo.v2StartFrame = { 4, 2 };
	//		AnimationInfo.v2EndFrame = { 4, 2 };
	//	}
	//}

	//if (GotLevel && Input::GetInstance()->KeyState[(unsigned char)'w'] != Input::INPUT_FIRST_PRESS)
	//{
	//	bool bAddGrav = true;
	//	for (auto it : GotLevel->Collidables)
	//	{
	//		// now its dis one?
	//		if (Utils::CheckCollision2D(std::make_shared<Entity>(*this), it, glm::vec2(fHSpeed, fVSpeed - GameSettings::fGravity * (float)Time::dTimeDelta))) // Checks if will collide at - gravity
	//		{
	//			if (it->transform.Position.y >= transform.Position.y)
	//				fVSpeed = Utils::GetDistance2D(std::make_shared<Entity>(*this), it).y; // Moves up to object (moves up the distance from object)				
	//			else
	//			{
	//				fVSpeed = -Utils::GetDistance2D(std::make_shared<Entity>(*this), it).y; // Moves up to object (moves down the distance from object)
	//				bJump = false;
	//			}
	//			bAddGrav = false;
	//			
	//			break;
	//		}
	//	}
	//	if (GotLevel->Collidables.size() == 0 || bAddGrav)
	//		if (fVSpeed > -GameSettings::fTerminalVelocity * (float)Time::dTimeDelta) fVSpeed -= GameSettings::fGravity * (float)Time::dTimeDelta; // moves down with -gravity
	//		else fVSpeed = -GameSettings::fTerminalVelocity * (float)Time::dTimeDelta;
	//}

	Translate(glm::vec3(fHSpeed, 0, fVSpeed));
	//GotLevel->DebugText->sText = std::to_string(fVSpeed);

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
	fHSpeed = 0;
	fVSpeed = 0;
	transform.Position = GotLevel->SpawnPos;
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
				fHSpeed = -GameSettings::fMoveSpeed * (float)Time::dTimeDelta;
			else
				fHSpeed = GameSettings::fMoveSpeed * (float)Time::dTimeDelta;
			return;
		}
		for (auto it : GotLevel->Collidables)
		{
			if (EntityMesh->MeshCollisionBounds->CheckCollision(it, glm::vec3(GameSettings::fMoveSpeed * Time::dTimeDelta * Direction, 0, 0)))
			{
					fHSpeed = Direction * abs(EntityMesh->MeshCollisionBounds->GetDistance(it).x);
					break;
			}
			else
			{
				if (bLeft)
					fHSpeed = -GameSettings::fMoveSpeed * (float)Time::dTimeDelta;
				else
					fHSpeed = GameSettings::fMoveSpeed * (float)Time::dTimeDelta;
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
				fVSpeed = -GameSettings::fMoveSpeed * (float)Time::dTimeDelta;
			else
				fVSpeed = GameSettings::fMoveSpeed * (float)Time::dTimeDelta;
			return;
		}
		for (auto it : GotLevel->Collidables)
		{

			if (EntityMesh->MeshCollisionBounds->CheckCollision(it, glm::vec3(0, 0, GameSettings::fMoveSpeed * Time::dTimeDelta * Direction)))
			{
				fVSpeed = Direction * abs(EntityMesh->MeshCollisionBounds->GetDistance(it).z);
				break;
			}
			else
			{
				if (bUp)
					fVSpeed = -GameSettings::fMoveSpeed * (float)Time::dTimeDelta;
				else
					fVSpeed = GameSettings::fMoveSpeed * (float)Time::dTimeDelta;
			}
		}
	}
}
