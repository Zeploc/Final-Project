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
Model::Model(glm::vec4 _Colour, const char *  ModelSource)
{
	m_fWidth = 0;// fWidth;
	m_fHeight = 0;// fHeight;
	m_fDepth = 0;// fDepth;
	Colour = _Colour;
	TextureSource = ModelSource;
	bHasTexture = true;
	program = Shader::ModelProgram;
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
	pModelObject = std::make_shared<ModelObject>(TextureSource);
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
	//glUniform4fv(glGetUniformLocation(program, "fragcolor"), 4, glm::value_ptr(Colour));
	pModelObject->Render(Newtransform);
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