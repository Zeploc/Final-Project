//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    NetworkEntity.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <thread>

#pragma once
class NetworkEntity
{
public:
	NetworkEntity();
	~NetworkEntity();
	NetworkEntity(const NetworkEntity&) { };

	virtual void Initialise() = 0;
	//virtual void ReceiveData(char* _pcBufferToReceiveData) = 0;
	virtual void Update() = 0;

	std::thread m_ReceiveThread;
protected:
};

