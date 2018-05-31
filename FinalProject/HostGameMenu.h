//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    HostGameMenu.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <memory>

// Engine Includes //
#include "Engine\Scene.h"
#include "Engine\UIButton.h"
#include "Engine\UISlider.h"
#include "Engine\UITextField.h"

#pragma once
class HostGameMenu
{
public:
	HostGameMenu();
	~HostGameMenu();

	void Init(std::shared_ptr<Scene> _Scene);

	void HideElements();
	void ShowElements();
	void SetValues();

	int GetCurrentPlayersCounter() { return (int)PlayersCountSlider->GetValue(); };
	std::string GetServerName() { return ServerName->sText; };
	std::string GetPlayerName() { return PlayerName->FieldText.sText; };

	std::vector<std::shared_ptr<UIElement>> v_ScreenElements;
private:
	std::shared_ptr<UISlider> PlayersCountSlider;
	std::shared_ptr<UIText> ServerName;
	std::shared_ptr<UITextField> PlayerName;
};

