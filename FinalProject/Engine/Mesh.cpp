//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Entity.cpp
// Description    	:    Mesh Component for entity
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "Mesh.h"

// Engine Includes //
#include "Camera.h"
#include "CollisionBounds.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Mesh::Mesh()
{
	/*switch (_Shape)
	{
	case Utils::PLANE:
		break;
	case Utils::PYRAMID:
		break;
	case Utils::CUBE:
		break;
	case Utils::SPHERE:
		break;
	default:
		break;
	}*/
}

/************************************************************
#--Description--#:  Constructor function with texture
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Mesh::~Mesh()
{
}

/************************************************************
#--Description--#:	Render Current Mesh to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Mesh::Render(Utils::Transform Newtransform)
{
	// ABOVE CALLED FROM DERIVED RENDER
	if (bHasTexture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	
	}
	glFrontFace(GL_CW);
	Camera::GetInstance()->SetMVP(Newtransform, program);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, m_iIndicies, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
