//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name		:    GameSettings.h
// Description		:    Header file outlining the Class
// Author			:    Alex Coultas
// Mail				:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //

// OpenGL Library Includes //

// Engine Includes //

// Local Includes //

// This Includes //

// Static Variables //

#pragma once
class GameSettings
{
public:

	enum Scenes
	{
		MainMenu,
		Levels
	};
	
	static float fMoveSpeed;
	static float fGravity;
	static float fJumpSize;
	static float fTerminalVelocity;
};

