//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    UIManager.cpp
// Description    	:    Manages the UI System for the game
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "UIManager.h"

// Engine Includes //
#include "Engine\Input.h"
#include "Engine\SceneManager.h"
#include "Engine\Scene.h"

// Static Variables //
std::shared_ptr<UIManager> UIManager::m_pUIManager;

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
UIManager::UIManager()
{
}


/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
UIManager::~UIManager()
{
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void UIManager::Update()
{
	// Toggle Chat System
	if (Input::GetInstance()->KeyState[(const int)'c'] == Input::INPUT_FIRST_PRESS || Input::GetInstance()->KeyState[13] == Input::INPUT_FIRST_PRESS)
	{
		m_bDisplayChat = !m_bDisplayChat;
		SwitchUIMode(m_bDisplayChat);
	}
	// Toggle Options Menu
	if (Input::GetInstance()->KeyState[27] == Input::INPUT_FIRST_PRESS)
	{
		// Switch Options Displayed
		m_bDisplayPauseOptions = !m_bDisplayPauseOptions;
		// Move pointer to center before resuming play
		glutWarpPointer((float)Camera::GetInstance()->SCR_WIDTH * 0.5f, (float)Camera::GetInstance()->SCR_HEIGHT * 0.5f);
		// Switch UI Mode
		SwitchUIMode(m_bDisplayPauseOptions);
	}
	// If current scene is main menu do not show options menu
	if (SceneManager::GetInstance()->GetCurrentScene()->SceneName == "MainMenu")
	{
		if (m_bDisplayPauseOptions) m_bDisplayPauseOptions = false;
		if (m_bDisplayChat) m_bDisplayChat = false;
		if (m_bUIMode == false)	SwitchUIMode(true);
	}
	// Show Tab Scores
	if (Input::GetInstance()->KeyState[(const int)'\t'] == Input::INPUT_FIRST_PRESS)
	{
		m_bDisplayTabScores = true;
	}
	// Hide Tab Scores
	else if (Input::GetInstance()->KeyState[(const int)'\t'] == Input::INPUT_FIRST_RELEASE)
	{
		m_bDisplayTabScores = false;
	}
	// FPS mode only on when UI Mode is off
	if (Camera::GetInstance()->m_bFPS == m_bUIMode)
		Camera::GetInstance()->m_bFPS = !m_bUIMode;
	
	// Update system instances
	m_ChatInstance.Update();
	m_ScoreTabInstance.Update();
	if (m_bDisplayPauseOptions) m_OptionsScreenInstance.Update();
}

/************************************************************
#--Description--#:	Render Scene to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void UIManager::Render()
{
	// Render System instances
	if (m_bDisplayChat) m_ChatInstance.Render();
	if (m_bDisplayTabScores) m_ScoreTabInstance.Render();
	if (m_bDisplayPauseOptions) m_OptionsScreenInstance.Render();
}

void UIManager::SwitchUIMode(bool _bNewMode)
{
	m_bUIMode = _bNewMode;

	// Setting things based on UI mode
	if (m_bUIMode)
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	else
		glutSetCursor(GLUT_CURSOR_NONE);

	// FPS mode only on when UI Mode is off
	if (Camera::GetInstance()->m_bFPS == m_bUIMode)
		Camera::GetInstance()->m_bFPS = !m_bUIMode;
}

/************************************************************
#--Description--#:  Retrieves static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		Returns static pointer to self
************************************************************/
std::shared_ptr<UIManager> UIManager::GetInstance()
{
	if (!m_pUIManager) // null or doesn't exist
	{
		m_pUIManager = std::shared_ptr<UIManager>(new UIManager());
	}
	return m_pUIManager;
}

/************************************************************
#--Description--#:  Destroys static instance pointer to this class
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void UIManager::DestoryInstance()
{
	m_pUIManager = nullptr;
}