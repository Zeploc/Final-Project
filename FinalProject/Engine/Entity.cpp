//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Entity.cpp
// Description    	:    main implementation for Entity
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "Entity.h"

// Library Includes //
#include <iostream>

// OpenGL Library Includes //
#include <glm\gtx\string_cast.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

// Engine Includes //
#include "Shader.h"
#include "Camera.h"
#include "Time.h"
#include "Plane.h"
#include "Pyramid.h"
#include "Sphere.h"


//
///************************************************************
//#--Description--#:  Constructor function
//#--Author--#: 		Alex Coultas
//#--Parameters--#:	Takes contructor values
//#--Return--#: 		NA
//************************************************************/
//Entity::Entity(Utils::Transform _Transform, float _fWidth, float _fHeight, Utils::EANCHOR _Anchor, glm::vec4 _Colour, Utils::ESHAPE _eShape)
//	: transform(_Transform), m_fWidth(_fWidth), m_fHeight(_fHeight), EntityAnchor(_Anchor), Colour(_Colour), m_eShape(_eShape)
//{
//	switch (_eShape)
//	{
//	case Utils::PYRAMID:
//		vao = Shader::BindPyramidArray(m_fWidth, m_fHeight, m_fWidth, Colour);
//		m_iIndicies = 18;
//		CollisionBox.fHeight = m_fHeight;
//		CollisionBox.fWidth = m_fWidth;
//		break;
//	case Utils::PLANE:
//		vao = Shader::BindArray(m_fWidth, m_fHeight, Colour);
//		m_iIndicies = 6;
//		break;
//	case Utils::SPHERE:
//		vao = Shader::BindSphereArray(m_fWidth, m_fHeight, m_fWidth, Colour, m_iIndicies);
//		break;
//	default:
//		break;
//	}
//	bHasTexture = false;
//}
//
///************************************************************
//#--Description--#:  Constructor function with texture
//#--Author--#: 		Alex Coultas
//#--Parameters--#:	Takes contructor values
//#--Return--#: 		NA
//************************************************************/
//Entity::Entity(Utils::Transform _Transform, float _fWidth, float _fHeight, Utils::EANCHOR _Anchor, glm::vec4 _Colour, const char * TextureSource, Utils::ESHAPE _eShape, glm::vec4 UVCoords)
//	: transform(_Transform), m_fWidth(_fWidth), m_fHeight(_fHeight), EntityAnchor(_Anchor), Colour(_Colour)
//{
//	switch (_eShape)
//	{
//	case Utils::PYRAMID:
//		vao = Shader::BindPyramidArray(m_fWidth, m_fHeight, m_fWidth, Colour, TextureSource, texture, UVCoords);
//		m_iIndicies = 18;
//		break;
//	case Utils::PLANE:
//		vao = Shader::BindArray(m_fWidth, m_fHeight, Colour, TextureSource, texture, UVCoords);
//		CollisionBox.fHeight = m_fHeight;
//		CollisionBox.fWidth = m_fWidth;
//		m_iIndicies = 6;
//		break;
//	case Utils::SPHERE:
//		vao = Shader::BindSphereArray(m_fWidth, m_fHeight, m_fWidth, Colour, TextureSource, texture, m_iIndicies, UVCoords);
//		break;
//	default:
//		break;
//	}
//	bHasTexture = true;
//}
///************************************************************
//#--Description--#:  Constructor function with texture and animation
//#--Author--#: 		Alex Coultas
//#--Parameters--#:	Takes contructor values
//#--Return--#: 		NA
//************************************************************/
//Entity::Entity(Utils::Transform _Transform, float _fWidth, float _fHeight, Utils::EANCHOR _Anchor, glm::vec4 _Colour, const char * TextureSource, glm::vec2 v2FrameCounts, int _iFPS)
//	: transform(_Transform), m_fWidth(_fWidth), m_fHeight(_fHeight), EntityAnchor(_Anchor), Colour(_Colour)
//{
//	vao = Shader::BindArray(m_fWidth, m_fHeight, Colour, TextureSource, texture, v2FrameCounts, AnimationInfo);
//	bHasTexture = true;
//	AnimationInfo.iFPS = _iFPS;
//	m_fFrameCheck = 1.0f / AnimationInfo.iFPS;
//	CollisionBox.fHeight = m_fHeight;
//	CollisionBox.fWidth = m_fWidth;
//}
//
///************************************************************
//#--Description--#:  Constructor function with texture and tiling method
//#--Author--#: 		Alex Coultas
//#--Parameters--#:	Takes contructor values
//#--Return--#: 		NA
//************************************************************/
//Entity::Entity(Utils::Transform _Transform, float _fWidth, float _fHeight, Utils::EANCHOR _Anchor, glm::vec4 _Colour, const char * TextureSource, int iCount, bool bHorizontal)
//	: transform(_Transform), m_fWidth(_fWidth), m_fHeight(_fHeight), EntityAnchor(_Anchor), Colour(_Colour)
//{
//	vao = Shader::BindArray(m_fWidth, m_fHeight, Colour, TextureSource, texture, iCount, bHorizontal);
//	bHasTexture = true;
//	CollisionBox.fHeight = m_fHeight;
//	CollisionBox.fWidth = m_fWidth;
//}

Entity::Entity(Utils::Transform _Transform, Utils::EANCHOR _Anchor)
	: transform(_Transform), EntityAnchor(_Anchor)
{
	//EntityMesh->EntityRef = this->shared_from_this();
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Entity::~Entity()
{

}

void Entity::AddMesh(std::shared_ptr<Mesh> _NewMesh)
{
	EntityMesh = _NewMesh;
}

//void Entity::AddMesh(Utils::ESHAPE _NewShape)
//{
//	switch (_NewShape)
//	{
//	case Utils::PLANE:
//		EntityMesh = std::make_shared<Plane>(Plane(1, 1, { 1.0f, 1.0f, 1.0f,  1.0f }));
//		break;
//	case Utils::PYRAMID:
//		EntityMesh = std::make_shared<Pyramid>(Pyramid(1, 1, 1, { 1.0f, 1.0f, 1.0f,  1.0f }));
//		break;
//	case Utils::CUBE:
//		break;
//	case Utils::SPHERE:
//		EntityMesh = std::make_shared<Sphere>(Sphere(1, 1, 1, { 1.0f, 1.0f, 1.0f,  1.0f }));
//		break;
//	default:
//		break;
//	}
//	EntityMesh->EntityRef = this->shared_from_this();
//}

/************************************************************
#--Description--#: 	Draws the entity on the screen at the using the transform
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Entity::DrawEntity()
{
	Utils::Transform AnchoredTransform = transform;
	AnchoredTransform.Position = Utils::GetAncoredPosition(transform.Position, glm::vec2(EntityMesh->m_fWidth * transform.Scale.x, EntityMesh->m_fHeight * transform.Scale.y), EntityAnchor);
	EntityMesh->Render(AnchoredTransform);
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Entity::Update()
{
	EntityMesh->Update();

}

/************************************************************
#--Description--#: 	Virtual function ran when the entity is destroyed
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Entity::OnDestroy()
{
	
}


/************************************************************
#--Description--#: 	Moves the position by adding on the movement
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the movement to move the entity by
#--Return--#: 		NA
************************************************************/
void Entity::Translate(glm::vec3 _Movement)
{
	transform.Position += _Movement;
}

/************************************************************
#--Description--#: 	Rotates the entity by a certain amount
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the rotation to add
#--Return--#: 		NA
************************************************************/
void Entity::Rotate(glm::vec3 Rotate)
{
	transform.Rotation += Rotate;
}

/************************************************************
#--Description--#: 	Sets the entity scale
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Takes in the new scale
#--Return--#: 		NA
************************************************************/
void Entity::SetScale(glm::vec3 _NewScale)
{
	transform.Scale = _NewScale;
}
