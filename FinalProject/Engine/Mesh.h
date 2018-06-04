//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name		:    Mesh.h
// Description		:    Header file outlining the Class
// Author			:    Alex Coultas
// Mail				:    alex.cou7417@mediadesign.school.nz
//

//#pragma once
//Mesh.h
#ifndef _MESH_H
#define _MESH_H

// OpenGL Library Includes //
#include <glew.h>
#include <freeglut.h>
#include <glm/common.hpp>

// Engine Includes //
#include "Utils.h"
//#include "Entity.h"

// Library Include //
#include <memory>


class CollisionBounds;

class Mesh
{
public:
	Mesh();
	~Mesh();
	
	virtual void Render(Utils::Transform Newtransform);
	virtual void Update() {};
	virtual void OnDestroy() {};

	virtual void Rebind() {}; // Will replace if texture exists
	virtual void SetLit(bool _bIsLit) { bIsLit = _bIsLit; };

	Utils::ESHAPE m_eShape;
	float m_fWidth;
	float m_fHeight;
	float m_fDepth = 0;
	glm::vec4 Colour;
	GLuint program;
	GLuint vao;
	GLuint texture;
	const char * TextureSource;
	glm::vec4 UVCoords;
	bool bHasTexture = false;
	int m_iIndicies;
	std::shared_ptr<CollisionBounds> MeshCollisionBounds;
	//std::shared_ptr<Entity> EntityRef;
protected:
	bool bIsLit = false;
};

#endif