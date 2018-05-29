
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
#include <WinSock2.h>
// OpenGL Library Includes //

// Engine Includes //
#include "Engine\Scene.h"
#include "Engine\UIImage.h"
#include "Engine\UIButton.h"

// Local Includes //
#include "ServerItem.h"

// This Includes //

// Static Variables //

#pragma once


enum MENUSECTION
{
	MAIN,
	OPTIONS,
	HOST,
	JOIN
};


class Menu : public Scene
{
public:

	Menu(std::string sSceneName);
	~Menu();
	
	void Update();
	
	void OnLoadScene();
	void PlayRandomTrack();
	void ToggleMenuSection(MENUSECTION);
	void ClearServerList() { v_ServerList.clear(); };

	void AddServers(std::vector<ServerInfo> Servers);

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

	std::vector<ServerItem> v_ServerList;
	glm::vec2 ServerListPos;
};

