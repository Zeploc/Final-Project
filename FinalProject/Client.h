//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Client.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//
#pragma once
// Local Includes //
#include "NetworkEntity.h"

class Client :	public NetworkEntity
{
public:
	Client();
	~Client();

	void Initialise();
	void ReceiveData(char* _pcBufferToReceiveData);
	void Update();
};

