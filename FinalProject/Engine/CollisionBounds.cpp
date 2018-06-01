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

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
CollisionBounds::CollisionBounds(float _fWidth, float _fHeight, float _fDepth, std::shared_ptr<Entity> _EntityRef)
	: fHeight(_fHeight), fWidth(_fWidth), fDepth(_fDepth), EntityRef(_EntityRef)
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

bool CollisionBounds::isColliding(std::shared_ptr<Entity> Entity2)
{
	std::shared_ptr<Mesh> Entity1Mesh = std::dynamic_pointer_cast<Mesh>(EntityRef->EntityMesh);
	std::shared_ptr<Mesh> Entity2Mesh = std::dynamic_pointer_cast<Mesh>(Entity2->EntityMesh);
	float HalfWidth1 = (fWidth / 2.0f) * abs(EntityRef->transform.Scale.x);
	float HalfHeight1 = (fHeight / 2.0f) * abs(EntityRef->transform.Scale.y);
	float HalfDepth1 = (fDepth / 2.0f) * abs(EntityRef->transform.Scale.z);
	float HalfWidth2 = (Entity2Mesh->MeshCollisionBounds->fWidth / 2.0f) * abs(Entity2->transform.Scale.x);
	float HalfHeight2 = (Entity2Mesh->MeshCollisionBounds->fHeight / 2.0f) * abs(Entity2->transform.Scale.y);
	float HalfDepth2 = (Entity2Mesh->MeshCollisionBounds->fDepth / 2.0f) * abs(Entity2->transform.Scale.z);
	
	glm::vec3 Entity1Pos = EntityRef->transform.Position;//Utils::GetAncoredPosition(EntityRef->transform.Position, glm::vec2(Entity1Mesh->MeshCollisionBounds->fWidth, Entity1Mesh->MeshCollisionBounds->fHeight) * (glm::vec2)EntityRef->transform.Scale, EntityRef->EntityAnchor);
	glm::vec3 Entity2Pos = Entity2->transform.Position;//Utils::GetAncoredPosition(Entity2->transform.Position, glm::vec2(Entity2Mesh->m_fWidth, Entity2Mesh->MeshCollisionBounds->fHeight) * (glm::vec2)Entity2->transform.Scale, Entity2->EntityAnchor);

	if (Entity1Pos.x + Entity1Mesh->MeshCollisionBounds->v3Offset.x + HalfWidth1 > Entity2Pos.x + Entity2Mesh->MeshCollisionBounds->v3Offset.x - HalfWidth2
		&& Entity1Pos.x + Entity1Mesh->MeshCollisionBounds->v3Offset.x - HalfWidth1 < Entity2Pos.x + Entity2Mesh->MeshCollisionBounds->v3Offset.x + HalfWidth2
		&& Entity1Pos.y + Entity1Mesh->MeshCollisionBounds->v3Offset.y + HalfHeight1 > Entity2Pos.y + Entity2Mesh->MeshCollisionBounds->v3Offset.y - HalfHeight2
		&& Entity1Pos.y + Entity1Mesh->MeshCollisionBounds->v3Offset.y - HalfHeight1 < Entity2Pos.y + Entity2Mesh->MeshCollisionBounds->v3Offset.y + HalfHeight2
		&& Entity1Pos.z + Entity1Mesh->MeshCollisionBounds->v3Offset.z + HalfDepth1 > Entity2Pos.z + Entity2Mesh->MeshCollisionBounds->v3Offset.z - HalfDepth2
		&& Entity1Pos.z + Entity1Mesh->MeshCollisionBounds->v3Offset.z - HalfDepth1 < Entity2Pos.z + Entity2Mesh->MeshCollisionBounds->v3Offset.z + HalfDepth2)
		return true;
	return false;
}

bool CollisionBounds::CheckCollision(std::shared_ptr<Entity> Entity2, glm::vec3 Movement)
{
	std::shared_ptr<Mesh> Entity1Mesh = std::dynamic_pointer_cast<Mesh>(EntityRef->EntityMesh);
	std::shared_ptr<Mesh> Entity2Mesh = std::dynamic_pointer_cast<Mesh>(Entity2->EntityMesh);
	float HalfWidth1 = (Entity1Mesh->MeshCollisionBounds->fWidth / 2) * abs(EntityRef->transform.Scale.x);
	float HalfHeight1 = (Entity1Mesh->MeshCollisionBounds->fHeight / 2) * abs(EntityRef->transform.Scale.y);
	float HalfDepth1 = (Entity1Mesh->MeshCollisionBounds->fDepth / 2) * abs(EntityRef->transform.Scale.z);
	float HalfWidth2 = (Entity2Mesh->MeshCollisionBounds->fWidth / 2) * abs(Entity2->transform.Scale.x);
	float HalfHeight2 = (Entity2Mesh->MeshCollisionBounds->fHeight / 2) * abs(Entity2->transform.Scale.y);
	float HalfDepth2 = (Entity2Mesh->MeshCollisionBounds->fDepth / 2) * abs(Entity2->transform.Scale.z);

	glm::vec3 Entity1Pos = Utils::GetAncoredPosition(EntityRef->transform.Position, glm::vec2(Entity1Mesh->MeshCollisionBounds->fWidth, Entity1Mesh->MeshCollisionBounds->fHeight) * (glm::vec2)EntityRef->transform.Scale, EntityRef->EntityAnchor);
	glm::vec3 Entity2Pos = Utils::GetAncoredPosition(Entity2->transform.Position, glm::vec2(Entity2Mesh->m_fWidth, Entity2Mesh->MeshCollisionBounds->fHeight) * (glm::vec2)Entity2->transform.Scale, Entity2->EntityAnchor);

	if (Entity1Pos.x + Entity1Mesh->MeshCollisionBounds->v3Offset.x + HalfWidth1 + Movement.x > Entity2Pos.x + Entity2Mesh->MeshCollisionBounds->v3Offset.x - HalfWidth2
		&& Entity1Pos.x + Entity1Mesh->MeshCollisionBounds->v3Offset.x - HalfWidth1 + Movement.x < Entity2Pos.x + Entity2Mesh->MeshCollisionBounds->v3Offset.x + HalfWidth2
		&& Entity1Pos.y + Entity1Mesh->MeshCollisionBounds->v3Offset.y + HalfHeight1 + Movement.y > Entity2Pos.y + Entity2Mesh->MeshCollisionBounds->v3Offset.y - HalfHeight2
		&& Entity1Pos.y + Entity1Mesh->MeshCollisionBounds->v3Offset.y - HalfHeight1 + Movement.y < Entity2Pos.y + Entity2Mesh->MeshCollisionBounds->v3Offset.y + HalfHeight2
		&& Entity1Pos.z + Entity1Mesh->MeshCollisionBounds->v3Offset.z + HalfDepth1 + Movement.z > Entity2Pos.z + Entity2Mesh->MeshCollisionBounds->v3Offset.z - HalfDepth2
		&& Entity1Pos.z + Entity1Mesh->MeshCollisionBounds->v3Offset.z - HalfDepth1 + Movement.z < Entity2Pos.z + Entity2Mesh->MeshCollisionBounds->v3Offset.z + HalfDepth2)
		return true;
	return false;
}
