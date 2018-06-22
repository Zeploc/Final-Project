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

// Library Includes //
#include <iostream>

// Local Includes //
#include "NetworkManager.h"
#include "Client.h"
//#include "UIManager.h"

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
	m_pBackImage = std::make_shared<UIImage>(UIImage({10, Camera::GetInstance()->SCR_HEIGHT - 60 }, Utils::BOTTOM_LEFT, 0, { 0.3f, 0.3f, 0.3f, 0.5f }, 400, 200));
	m_pTypingField = std::make_shared<UITextField>(UITextField({ 25, Camera::GetInstance()->SCR_HEIGHT - 70 }, Utils::CENTER_LEFT, 0, { 0.5f, 0.5f, 0.5f, 0.5f }, 260, 20, { 0.8f, 0.8f, 0.8f, 1.0f }, "Enter Message...", "Resources/Fonts/Arial.ttf", 20, Utils::CENTER_LEFT));
	m_pChatBackImage = std::make_shared<UIImage>(UIImage({ 25, Camera::GetInstance()->SCR_HEIGHT - 85 }, Utils::BOTTOM_LEFT, 0, { 0.2f, 0.2f, 0.2f, 0.5f }, 370, 160));
	m_pEnterButton = std::make_shared<UIButton>(UIButton({ 280, Camera::GetInstance()->SCR_HEIGHT - 60 }, Utils::BOTTOM_LEFT, 0, { 0.4f, 0.4f, 0.4f, 0.5f }, { 0.8f, 0.8f, 0.8f, 1.0f }, 110, 20, SendChat));
	m_pEnterButton->AddText("Send", "Resources/Fonts/Roboto-Thin.ttf", 20, { 1.0f, 1.0f, 1.0f, 1.0f }, Utils::CENTER);
	SetChatMaximised(false);
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

	if (bIsMaximised && !m_pTypingField->GetFocussed())
		SetChatMaximised(false);
	else if (!bIsMaximised && m_pTypingField->GetFocussed())
		SetChatMaximised(true);
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
	for (auto& it : ChatText)
	{
		it->DrawUIElement();
	}
}

void Chat::SetChatMaximised(bool _bIsMaximied)
{
	bIsMaximised = _bIsMaximied;
	m_pBackImage->SetActive(_bIsMaximied);
	m_pChatBackImage->SetActive(_bIsMaximied);

	m_pTypingField->SetFocussed(_bIsMaximied);

}

void Chat::SendChatMessage()
{
	NetworkManager::GetInstance()->m_Network.m_pNetworkEntity->SendMessageNE(m_pTypingField->FieldText.sText, CHAT);
	m_pTypingField->ResetField();
	SetChatMaximised(false);
}

void Chat::AddChatMessage(ChatMessage NewMessage)
{
	FullChat.push_back(NewMessage);
	glm::vec2 NewPosition = m_pTypingField->GetPosition();
	NewPosition.x -= m_pTypingField->BackImage.GetWidth() / 2;
	NewPosition.y -= (ChatText.size() + 1) * 25;
	std::string ChatMessage = "[" + NewMessage.Username + "] " + NewMessage.Message;
	std::shared_ptr<UIText> NewMessageText = std::make_shared<UIText>(UIText(NewPosition, 0, { 0.8f, 0.8f, 0.8f, 1.0f }, ChatMessage, ChatFont, 20, Utils::CENTER_LEFT));
	ChatText.push_back(NewMessageText);
	RepositionChat();
	// Add visual New Chat Message
}

void Chat::RepositionChat()
{
	while ((ChatText.size()) * 25 > m_pChatBackImage->GetHeight())
	{
		ChatText.erase(ChatText.begin());
	}
	glm::vec2 NewPosition = m_pTypingField->GetPosition();
	NewPosition.x -= m_pTypingField->BackImage.GetWidth() / 2;
	NewPosition.y -= (ChatText.size() + 1) * 25;
	for (auto& it : ChatText)
	{
		NewPosition.y += 25;
		it->SetPosition(NewPosition);
	}
}

void SendChat()
{
	//UIManager::GetInstance()->m_ChatInstance.SendChatMessage();
}
