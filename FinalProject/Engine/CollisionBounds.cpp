//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    CollisionBoudns.cpp
// Description    	:    Collision Bound system
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "CollisionBounds.h"

// Engine Includes //
#include "Mesh.h"
#include "Utils.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
CollisionBounds::CollisionBounds()
{
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
CollisionBounds::~CollisionBounds()
{
}

bool CollisionBounds::isColliding(std::shared_ptr<Entity> Entity1, std::shared_ptr<Entity> Entity2)
{
	std::shared_ptr<Mesh> Entity1Mesh = std::dynamic_pointer_cast<Mesh>(Entity1->EntityMesh);
	std::shared_ptr<Mesh> Entity2Mesh = std::dynamic_pointer_cast<Mesh>(Entity2->EntityMesh);
	float HalfWidth1 = (Entity1Mesh->MeshCollisionBounds->fWidth / 2) * abs(Entity1->transform.Scale.x);
	float HalfHeight1 = (Entity1Mesh->MeshCollisionBounds->fHeight / 2) * abs(Entity1->transform.Scale.y);
	float HalfWidth2 = (Entity2Mesh->MeshCollisionBounds->fWidth / 2) * abs(Entity2->transform.Scale.x);
	float HalfHeight2 = (Entity2Mesh->MeshCollisionBounds->fHeight / 2) * abs(Entity2->transform.Scale.y);


	glm::vec3 Entity1Pos = Utils::GetAncoredPosition(Entity1->transform.Position, glm::vec2(Entity1Mesh->m_fWidth, Entity1Mesh->m_fHeight) * (glm::vec2)Entity1->transform.Scale, Entity1->EntityAnchor);
	glm::vec3 Entity2Pos = Utils::GetAncoredPosition(Entity2->transform.Position, glm::vec2(Entity2Mesh->m_fWidth, Entity2Mesh->m_fHeight) * (glm::vec2)Entity2->transform.Scale, Entity2->EntityAnchor);

	if (Entity1Pos.x + Entity1Mesh->MeshCollisionBounds->v2Offset.x + HalfWidth1 > Entity2Pos.x + Entity2Mesh->MeshCollisionBounds->v2Offset.x - HalfWidth2
		&& Entity1Pos.x + Entity1Mesh->MeshCollisionBounds->v2Offset.x - HalfWidth1 < Entity2Pos.x + Entity2Mesh->MeshCollisionBounds->v2Offset.x + HalfWidth2
		&& Entity1Pos.y + Entity1Mesh->MeshCollisionBounds->v2Offset.y + HalfHeight1 > Entity2Pos.y + Entity2Mesh->MeshCollisionBounds->v2Offset.y - HalfHeight2
		&& Entity1Pos.y + Entity1Mesh->MeshCollisionBounds->v2Offset.y - HalfHeight1 < Entity2Pos.y + Entity2Mesh->MeshCollisionBounds->v2Offset.y + HalfHeight2)
		return true;
	return false;
}

bool CollisionBounds::CheckCollision(std::shared_ptr<Entity> Entity1, std::shared_ptr<Entity> Entity2, glm::vec3 Movement)
{
	std::shared_ptr<Mesh> Entity1Mesh = std::dynamic_pointer_cast<Mesh>(Entity1->EntityMesh);
	std::shared_ptr<Mesh> Entity2Mesh = std::dynamic_pointer_cast<Mesh>(Entity2->EntityMesh);
	float HalfWidth1 = (Entity1Mesh->MeshCollisionBounds->fWidth / 2) * abs(Entity1->transform.Scale.x);
	float HalfHeight1 = (Entity1Mesh->MeshCollisionBounds->fHeight / 2) * abs(Entity1->transform.Scale.y);
	float HalfWidth2 = (Entity2Mesh->MeshCollisionBounds->fWidth / 2) * abs(Entity2->transform.Scale.x);
	float HalfHeight2 = (Entity2Mesh->MeshCollisionBounds->fHeight / 2) * abs(Entity2->transform.Scale.y);

	//glm::vec3 Entity1Pos = Utils::GetAncoredPosition(Entity1->transform.Position, glm::vec3(Entity1Mesh->m_fWidth, Entity1Mesh->m_fHeight) * (glm::vec3)Entity1->transform.Scale, Entity1->EntityAnchor);
	//glm::vec3 Entity2Pos = Utils::GetAncoredPosition(Entity2->transform.Position + glm::vec3(Entity2Mesh->MeshCollisionBounds->v2Offset, 0), glm::vec2(Entity2Mesh->m_fWidth, Entity2Mesh->m_fHeight) * (glm::vec2)Entity2->transform.Scale, Entity2->EntityAnchor);

	if (Entity1Pos.x + Entity1Mesh->MeshCollisionBounds->v2Offset.x + HalfWidth1 + Movement.x > Entity2Pos.x + Entity2Mesh->MeshCollisionBounds->v2Offset.x - HalfWidth2
		&& Entity1Pos.x + Entity1Mesh->MeshCollisionBounds->v2Offset.x - HalfWidth1 + Movement.x < Entity2Pos.x + Entity2Mesh->MeshCollisionBounds->v2Offset.x + HalfWidth2
		&& Entity1Pos.y + Entity1Mesh->MeshCollisionBounds->v2Offset.y + HalfHeight1 + Movement.y > Entity2Pos.y + Entity2Mesh->MeshCollisionBounds->v2Offset.y - HalfHeight2
		&& Entity1Pos.y + Entity1Mesh->MeshCollisionBounds->v2Offset.y - HalfHeight1 + Movement.y < Entity2Pos.y + Entity2Mesh->MeshCollisionBounds->v2Offset.y + HalfHeight2)
		return true;
	return false;
}
