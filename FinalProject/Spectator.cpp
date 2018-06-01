//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Spectator.cpp
// Description    	:    Spectator controller entity for game system
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "Spectator.h"

// Engine Includes //
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Time.h"
#include "UIManager.h"

// OpenGL Library Includes //
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc/matrix_transform.hpp>

Spectator::Spectator(Utils::Transform _Transform, Utils::EANCHOR _Anchor)
	: Entity(_Transform, _Anchor)
{
	LastPos = glm::vec2((float)Camera::GetInstance()->SCR_WIDTH * 0.5f, (float)Camera::GetInstance()->SCR_HEIGHT * 0.5f);
}


Spectator::~Spectator()
{
}

void Spectator::Update()
{
	if (UIManager::GetInstance()->m_bFPS) FPSControls();
	Entity::Update();
}

/************************************************************
#--Description--#: 	Enables First person controls for perspective camera
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Spectator::FPSControls()
{
	glm::vec2 Offset = glm::vec2(Input::GetInstance()->MousePos - glm::vec2((float)Camera::GetInstance()->SCR_WIDTH * 0.5f, (float)Camera::GetInstance()->SCR_HEIGHT * 0.5f));
	Offset *= MouseSensitivity;
	Yaw -= Offset.x;
	Pitch -= Offset.y;

	glm::clamp((float)Pitch, 89.0f, -89.0f);
	glm::vec3 frontVector(-cos(glm::radians(Pitch))*sin(glm::radians(Yaw)),
		sin(glm::radians(Pitch)),
		-cos(glm::radians(Pitch)) * cos(glm::radians(Yaw)));
	Camera::GetInstance()->SetCameraForwardVector(glm::normalize(frontVector));

	if (Input::GetInstance()->KeyState[(unsigned char)'w'] == Input::INPUT_HOLD)
		Camera::GetInstance()->SetCameraPos(Camera::GetInstance()->GetCameraPosition() + Camera::GetInstance()->GetCameraForwardVector() * cameraSpeed * (float)Time::dTimeDelta);
	else if (Input::GetInstance()->KeyState[(unsigned char)'s'] == Input::INPUT_HOLD)
		Camera::GetInstance()->SetCameraPos(Camera::GetInstance()->GetCameraPosition() - Camera::GetInstance()->GetCameraForwardVector() * cameraSpeed * (float)Time::dTimeDelta);

	if (Input::GetInstance()->KeyState[(unsigned char)'a'] == Input::INPUT_HOLD)
		Camera::GetInstance()->SetCameraPos(Camera::GetInstance()->GetCameraPosition() - glm::normalize(glm::cross(Camera::GetInstance()->GetCameraForwardVector(), Camera::GetInstance()->GetCameraUpVector())) * cameraSpeed * (float)Time::dTimeDelta);
	else if (Input::GetInstance()->KeyState[(unsigned char)'d'] == Input::INPUT_HOLD)
		Camera::GetInstance()->SetCameraPos(Camera::GetInstance()->GetCameraPosition() + glm::normalize(glm::cross(Camera::GetInstance()->GetCameraForwardVector(), Camera::GetInstance()->GetCameraUpVector())) * cameraSpeed * (float)Time::dTimeDelta);

	if (Input::GetInstance()->KeyState[(unsigned char)' '] == Input::INPUT_HOLD)
		Camera::GetInstance()->SetCameraPos(Camera::GetInstance()->GetCameraPosition() + Camera::GetInstance()->GetCameraUpVector() * cameraSpeed * (float)Time::dTimeDelta);
	else if (Input::GetInstance()->KeyState[(unsigned char)'q'] == Input::INPUT_HOLD)
		Camera::GetInstance()->SetCameraPos(Camera::GetInstance()->GetCameraPosition() - Camera::GetInstance()->GetCameraUpVector() * cameraSpeed * (float)Time::dTimeDelta);
		
	glutWarpPointer((float)Camera::GetInstance()->SCR_WIDTH * 0.5f, (float)Camera::GetInstance()->SCR_HEIGHT * 0.5f);
}