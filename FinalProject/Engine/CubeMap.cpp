//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Level.cpp
// Description    	:    main implementation for Level
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "CubeMap.h"

// Local Includes //

// Engine Includes //
#include "Shader.h"
#include "Camera.h"

// OpenGL Includes //
#include <SOIL.h>

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
CubeMap::CubeMap(float fWidth, float fHeight, float fDepth, char *  _TextureSources[6])
{
	TextureSources[0] = _TextureSources[0];
	TextureSources[1] = _TextureSources[1];
	TextureSources[2] = _TextureSources[2];
	TextureSources[3] = _TextureSources[3];
	TextureSources[4] = _TextureSources[4];
	TextureSources[5] = _TextureSources[5];
	m_iIndicies = 36;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;
	bHasTexture = false;
	m_eShape = Utils::CUBE;
	BindCubeMap();

}


/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
CubeMap::~CubeMap()
{
}

/************************************************************
#--Description--#: 	Binds pyramid with sepcified attributes
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void CubeMap::BindCubeMap()
{
	float fHalfWidth = m_fWidth / 2;
	float fHalfHeight = m_fHeight / 2;
	float fHalfDepth = m_fDepth / 2;

	GLfloat vertices[] = {
		// Positions								// Colors			
		-fHalfWidth, fHalfHeight, fHalfDepth,	// Top left front 0
		fHalfWidth, fHalfHeight, fHalfDepth,	// Top right front 1
		fHalfWidth, -fHalfHeight, fHalfDepth,	// Bottom left front 2
		-fHalfWidth, -fHalfHeight, fHalfDepth,	// bottom right front 3
		// Back Face
		-fHalfWidth, fHalfHeight, -fHalfDepth,	//top left back 4
		fHalfWidth, fHalfHeight, -fHalfDepth,	// top right back 5
		-fHalfWidth, -fHalfHeight, -fHalfDepth,	// bottom left back 6
		fHalfWidth, -fHalfHeight, -fHalfDepth	// bottom right back 7
	};

	GLuint indices[] = {
		// Right Face
		1, 5, 7,
		1, 7, 3,
		// Left Face
		0, 4, 6,
		0, 6, 2,
		// Top Face
		0, 4, 5,
		0, 5, 1,
		// Bottom Face
		2, 6, 7,
		2, 7, 3,
		// Back Face
		5, 4, 6,
		5, 6, 7,
		// Front Face
		0, 1, 2,
		0, 2, 3
	};
	GLuint vbo;
	GLuint ebo;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	int width, height;
	unsigned char* image;
	for (GLuint i = 0; i < 6; i++)
	{
		std::string fullPathName = "Resources/Textures/CubeMap/";
		fullPathName.append(TextureSources[i]);
		image = SOIL_load_image(fullPathName.c_str(), &width, &height, 0,
			SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
			width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

/************************************************************
#--Description--#: 	Rebinds the vao with the colour (not texture)
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void CubeMap::Rebind()
{
	BindCubeMap();
}

/************************************************************
#--Description--#:	Render Current Mesh to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void CubeMap::Render(Utils::Transform Newtransform)
{
	glUseProgram(Shader::CubeMapProgram);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glUniform1i(glGetUniformLocation(Shader::CubeMapProgram, "cubeSampler"), 0);

	Camera::GetInstance()->SetMVP(Newtransform);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, sizeof(m_iIndicies) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void CubeMap::Update()
{
}