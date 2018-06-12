//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    GroupEnemies.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <memory>
#include <vector>

// Forward Declaration //
class Entity;

#pragma once
class GroupEnemies
{
public:
	GroupEnemies();
	~GroupEnemies();

	void Update();

	//std::vector<std::shared_ptr<Entity>> Enemies;
};

