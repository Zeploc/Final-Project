//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Chat.cpp
// Description    	:    The Chat UI System for the game
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "ScoreTab.h"

// Engine Includes //
#include "Engine\Camera.h"


/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
ScoreTab::ScoreTab()
{
	m_pBackImage = std::make_shared<UIImage>(UIImage({ Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 }, Utils::CENTER, 0, { 0.5f, 0.5f, 0.5f, 1.0f }, 600, 300));
	m_pScoresTitle = std::make_shared<UIText>(UIText({ Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 - 120 }, 0, { 0.9f, 0.9f, 0.9f, 1.0f }, "Scores:", "Resources/Fonts/Roboto-Black.ttf", 45, Utils::CENTER));;
	
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
ScoreTab::~ScoreTab()
{
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void ScoreTab::Update()
{
	m_pBackImage->Update();
	m_pScoresTitle->Update();
}

/************************************************************
#--Description--#:	Render Scene to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void ScoreTab::Render()
{
	m_pBackImage->DrawUIElement();
	m_pScoresTitle->DrawUIElement();
}
