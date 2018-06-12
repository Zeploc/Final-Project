//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    LobbyMenu.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <memory>

// Engine Includes //
#include "Engine\Scene.h"

class UIButton;

#pragma once
class LobbyMenu
{
public:
	LobbyMenu();
	~LobbyMenu();

	void Init(std::shared_ptr<Scene> _Scene);

	void HideElements();
	void ShowElements();

	void ClientConnected(std::string _UseName, std::string Address);

	std::string GetCurrentServerName() { return CurrentServerName->sText; };
	std::string GetPlayerName() { return PlayerName->sText; };

	void SetPlayerNameText(std::string _PlayerName) { PlayerName->sText = _PlayerName; };
	void SetServerNameText(std::string _ServerName) { ServerName->sText = _ServerName; };

	std::vector<std::shared_ptr<UIElement>> v_ScreenElements;
private:
	std::shared_ptr<UIText> CurrentServerName;
	std::shared_ptr<UIText> PlayerName;
	std::shared_ptr<UIText> ErrorText;
	std::shared_ptr<UIButton> StartServerBtn;
	std::shared_ptr<UIText> ServerName;

	std::vector<std::shared_ptr<UIElement>> v_PlayersConnected;
};

