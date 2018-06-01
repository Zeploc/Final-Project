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


//************************************************************
//#--Description--#:	Constructor function with base position
//#--Author--#: 		Alex Coultas
//#--Parameters--#:		Takes contructor values
//#--Return--#: 		NA
//************************************************************/
Entity::Entity(Utils::Transform _Transform, Utils::EANCHOR _Anchor)
	: transform(_Transform), EntityAnchor(_Anchor)
{

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
