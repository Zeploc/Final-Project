//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Spectator.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

#pragma once
#include "Engine\Entity.h"
class Spectator : public Entity
{
public:
	Spectator(Utils::Transform _Transform, Utils::EANCHOR _Anchor);
	~Spectator();
};

