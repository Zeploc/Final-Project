//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Chat.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

#pragma once
// Engine Includes //
#include "Engine\UIImage.h"
#include "Engine\UIButton.h"
#include "Engine\UITextField.h"

// Library Includes //
#include <memory>
#include <vector>


struct ChatMessage
{
	std::string Username;
	std::string Message;
};

class Chat
{
public:
	Chat();
	~Chat();
	
	void Update();
	void Render();

	void SetChatMaximised(bool _bIsMaximied);
	bool GetChatMaximsied() { return bIsMaximised; };

	void SendChatMessage();
	void AddChatMessage(ChatMessage NewMessage);
	void RepositionChat();
	
private:
	bool bIsMaximised = false;
	std::vector<ChatMessage> FullChat;

	std::vector<std::shared_ptr<UIText>> ChatText;
	std::string ChatFont = "Resources/Fonts/Arial.ttf";

	std::shared_ptr<UIImage> m_pBackImage;
	std::shared_ptr<UIImage> m_pChatBackImage;
	std::shared_ptr<UITextField> m_pTypingField;
	std::shared_ptr<UIButton> m_pEnterButton;
};

