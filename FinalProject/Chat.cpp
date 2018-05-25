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
#include "Chat.h"

// Engine Includes //
#include "Engine\Camera.h"

// Local Functions //
void SendChat();

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Chat::Chat()
{
	m_pBackImage = std::make_shared<UIImage>(UIImage({10, Camera::GetInstance()->SCR_HEIGHT }, Utils::BOTTOM_LEFT, 0, { 0.3f, 0.3f, 0.3f, 0.5f }, 400, 200));
	m_pTypingField = std::make_shared<UIImage>(UIImage({ 25, Camera::GetInstance()->SCR_HEIGHT - 5 }, Utils::BOTTOM_LEFT, 0, { 0.5f, 0.5f, 0.5f, 0.5f }, 260, 20));;
	m_pChatBackImage = std::make_shared<UIImage>(UIImage({ 25, Camera::GetInstance()->SCR_HEIGHT - 25 }, Utils::BOTTOM_LEFT, 0, { 0.2f, 0.2f, 0.2f, 0.5f }, 370, 160));
	m_pEnterButton = std::make_shared<UIButton>(UIButton({ 280, Camera::GetInstance()->SCR_HEIGHT }, Utils::BOTTOM_LEFT, 0, { 0.4f, 0.4f, 0.4f, 0.5f }, { 0.8f, 0.8f, 0.8f, 1.0f }, 110, 30, SendChat));
	m_pEnterButton->AddText("Send", "Resources/Fonts/Roboto-Thin.ttf", 20, { 1.0f, 1.0f, 1.0f, 1.0f }, Utils::CENTER);
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Chat::~Chat()
{
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Chat::Update()
{
	m_pBackImage->Update();
	m_pChatBackImage->Update();
	m_pTypingField->Update();
	m_pEnterButton->Update();
}

/************************************************************
#--Description--#:	Render Scene to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Chat::Render()
{
	m_pBackImage->DrawUIElement();
	m_pChatBackImage->DrawUIElement();
	m_pTypingField->DrawUIElement();
	m_pEnterButton->DrawUIElement();
}

void SendChat()
{
}
