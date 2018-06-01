//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Cube.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

#pragma once
// This Includes //
#include "Mesh.h"

class Model : public Mesh
{
public:
	Model(float fWidth, float fHeight, float fDepth, glm::vec4 Colour, const char *  ModelSource);
	~Model();

	void BindModel();
	void Rebind();

	void Render(Utils::Transform Newtransform);
	void Update();	
};

