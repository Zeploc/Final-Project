
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name		:    Camera.h
// Description		:    Header file outlining the Class
// Author			:    Alex Coultas
// Mail				:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //

// OpenGL Library Includes //

// Engine Includes //
#include "Engine\Scene.h"
#include "Engine\UIImage.h"
#include "Engine\UIButton.h"

// Local Includes //

// This Includes //

// Static Variables //

#pragma once

class Menu : public Scene
{
public:
	enum MENUSECTION
	{
		MAIN,
		OPTIONS,
		HOST,
		JOIN
	};

	Menu(std::string sSceneName);
	~Menu();
	
	void Update();
	
	void OnLoadScene();
	void PlayRandomTrack();
	void ToggleMenuSection(MENUSECTION);
	//void ToggleInstructions();
private:
	glm::vec2 ButtonPositions[3];
	int m_iCurrentSelected = 0;
	std::vector<std::shared_ptr<UIElement>> v_MenuElements;
	std::vector<std::shared_ptr<UIElement>> v_OptionsElements;
	std::vector<std::shared_ptr<UIElement>> v_HostGameElements;
	std::vector<std::shared_ptr<UIElement>> v_JoinGameElements;
	MENUSECTION m_CurrentSection = MAIN;
	std::shared_ptr<UIImage> m_Arrow;
};

