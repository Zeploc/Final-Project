//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    MessageBox.cpp
// Description    	:    Message box for messages
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "MessageBox.h"

// Local Includes //
#include "UIManager.h"

// Local Functions //
void AcceptBtn();

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
UIMessageBox::UIMessageBox()
{
	// Add MessageBox Elements
	m_pBackImage = std::make_shared<UIImage>(UIImage({ Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 }, Utils::CENTER, 0, { 0.3f, 0.3f, 0.3f, 1.0f }, 600, 150));
	m_pMessage = std::make_shared<UIText>(UIText({ Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 - 40 }, 0, glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), "Message", "Resources/Fonts/Roboto-Bold.ttf", 30, Utils::CENTER));
	m_pAcceptButton = std::make_shared<UIButton>(UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 + 30), Utils::CENTER, 0.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 300, 70, AcceptBtn));
	m_pAcceptButton->AddText("Accept", "Resources/Fonts/Roboto-Thin.ttf", 34, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
UIMessageBox::~UIMessageBox()
{

}

void UIMessageBox::SetMessageText(std::string _Message)
{
	m_pMessage->sText = _Message;
}

void UIMessageBox::Update()
{
	m_pBackImage->Update();
	m_pMessage->Update();
	m_pAcceptButton->Update();
}

void UIMessageBox::Render()
{
	m_pBackImage->DrawUIElement();
	m_pMessage->DrawUIElement();
	m_pAcceptButton->DrawUIElement();
}

void AcceptBtn()
{
	UIManager::GetInstance()->m_bDisplayMessageBox = false;
}
