//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Cube.cpp
// Description    	:    Cube Mesh
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "Model.h"

// Engine Includes //
#include "Shader.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Model::Model(float fWidth, float fHeight, float fDepth, glm::vec4 _Colour, const char *  ModelSource)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;
	Colour = _Colour;
	TextureSource = ModelSource;
	bHasTexture = true;
	//m_iIndicies = 36;
	BindModel();
	m_eShape = Utils::MODEL;

}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Model::~Model()
{
}

/************************************************************
#--Description--#: 	Binds pyramid with sepcified attributes
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Model::BindModel()
{
	float fHalfWidth = m_fWidth / 2;
	float fHalfHeight = m_fHeight / 2;
	float fHalfDepth = m_fDepth / 2;
	
	GLfloat Texturedvertices[] = {
		// Positions								// Colors									// UV Cords
		// Front Face
		-fHalfWidth, fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		0.0f, 0.0f,
		fHalfWidth, fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		1.0f, 0.0f,
		fHalfWidth, -fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		1.0f, 1.0f,
		-fHalfWidth, -fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		0.0f, 1.0f,
		// Right Face
		fHalfWidth, fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		0.0f, 0.0f,
		fHalfWidth, fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		1.0f, 0.0f,
		fHalfWidth, -fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		1.0f, 1.0f,
		fHalfWidth, -fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		0.0f, 1.0f,
		// Back Face
		fHalfWidth, fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		0.0f, 0.0f,
		-fHalfWidth, fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		1.0f, 0.0f,
		-fHalfWidth, -fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		1.0f, 1.0f,
		fHalfWidth, -fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		0.0f, 1.0f,
		// Left Face
		-fHalfWidth, fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		0.0f, 0.0f,
		-fHalfWidth, fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		1.0f, 0.0f,
		-fHalfWidth, -fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		1.0f, 1.0f,
		-fHalfWidth, -fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		0.0f, 1.0f,
		// Top Face
		-fHalfWidth, fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		0.0f, 0.0f,
		fHalfWidth, fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		1.0f, 0.0f,
		fHalfWidth, fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		1.0f, 1.0f,
		-fHalfWidth, fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		0.0f, 1.0f,
		// Bottom Face
		-fHalfWidth, -fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		0.0f, 0.0f,
		fHalfWidth, -fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		1.0f, 0.0f,
		fHalfWidth, -fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		1.0f, 1.0f,
		-fHalfWidth, -fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,		0.0f, 1.0f

	};
	GLfloat vertices[] = {
		// Positions								// Colors			
		// Front Face
		-fHalfWidth, fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		fHalfWidth, fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		fHalfWidth, -fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		-fHalfWidth, -fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		// Right Face
		fHalfWidth, fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		fHalfWidth, fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		fHalfWidth, -fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		-fHalfWidth, -fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		// Back Face
		fHalfWidth, fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		-fHalfWidth, fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		-fHalfWidth, -fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		fHalfWidth, -fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		// Left Face
		-fHalfWidth, fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		-fHalfWidth, fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		-fHalfWidth, -fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		-fHalfWidth, -fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		// Top Face
		-fHalfWidth, fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		fHalfWidth, fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		fHalfWidth, fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		-fHalfWidth, fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		// Bottom Face
		-fHalfWidth, -fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		fHalfWidth, -fHalfHeight, fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		fHalfWidth, -fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a,
		-fHalfWidth, -fHalfHeight, -fHalfDepth,		Colour.r, Colour.g, Colour.b, Colour.a


	};

	GLuint indices[] = {
		// Front Face
		0, 1, 2,
		0, 2, 3,
		// Right Face
		4, 5, 6,
		4, 6, 7,
		// Back Face
		8, 9, 10,
		8, 10, 11,
		// Left Face
		12, 13, 14,
		12, 14, 15,
		// Top Face
		16, 17, 18,
		16, 18, 19, 
		// Bottom Face
		20, 21, 22,
		20, 22, 23
	};

	vao = Shader::CreateBuffer(TextureSource, texture, true);
	if (TextureSource != "")
		glBufferData(GL_ARRAY_BUFFER, sizeof(Texturedvertices), Texturedvertices, GL_STATIC_DRAW);
	else
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

/************************************************************
#--Description--#: 	Rebinds the vao with the colour (not texture)
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Model::Rebind()
{
	BindModel();
}

/************************************************************
#--Description--#:	Render Current Mesh to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Model::Render(Utils::Transform Newtransform)
{
	if (bHasTexture)
	{
		glUseProgram(Shader::Textureprogram);
		glEnable(GL_BLEND);
	}
	else
	{
		glUseProgram(Shader::program);
		glDisable(GL_BLEND);
	}
	Mesh::Render(Newtransform);
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Model::Update()
{
}
