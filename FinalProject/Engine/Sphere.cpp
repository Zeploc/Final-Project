//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Pyramid.cpp
// Description    	:    Pyramid Mesh
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// This Includes //
#include "Sphere.h"

// Engine Includes //
#include "Shader.h"
#include "Lighting.h"

// OpenGL Library //
#include <SOIL.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Sphere::Sphere(float fWidth, float fHeight, float fDepth, glm::vec4 _Colour)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;
	Colour = _Colour;
	TextureSource = "";
	bHasTexture = false;
	BindSphere();
	m_eShape = Utils::SPHERE;
	if (bHasTexture)
	{
		if (bIsLit)
		{
			program = Shader::LitTextureprogram;
		}
		else
		{
			program = Shader::Textureprogram;
		}
	}
	else
	{
		if (bIsLit)
		{
			program = Shader::LitTextureprogram;
		}
		else
		{
			program = Shader::program;
		}
	}
}

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Sphere::Sphere(float fWidth, float fHeight, float fDepth, glm::vec4 _Colour, const char * _TextureSource, glm::vec4 _UVCoords)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;
	Colour = _Colour;
	TextureSource = _TextureSource;
	UVCoords = _UVCoords;
	bHasTexture = true;
	BindSphere();
	m_eShape = Utils::SPHERE;
	if (bHasTexture)
	{
		if (bIsLit)
		{
			program = Shader::LitTextureprogram;
		}
		else
		{
			program = Shader::Textureprogram;
		}
	}
	else
	{
		if (bIsLit)
		{
			program = Shader::LitTextureprogram;
		}
		else
		{
			program = Shader::program;
		}
	}
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Sphere::~Sphere()
{
}

/************************************************************
#--Description--#: 	Binds sphere with sepcified attributes
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Sphere::BindSphere()
{
	const int sections = 20;
	const int vertexAttrib = 12;
	const int indexPerQuad = 6;

	double phi = 0;
	double theta = 0;

	float vertices[(sections) * (sections)* vertexAttrib];
	int offset = 0;
	for (int i = 0; i < sections; i++)
	{
		theta = 0;

		for (int j = 0; j < sections; j++)
		{
			float x = cos(phi) * sin(theta);
			float y = cos(theta);
			float z = sin(phi) * sin(theta);

			vertices[offset++] = x * m_fWidth;
			vertices[offset++] = y * m_fHeight;
			vertices[offset++] = z * m_fDepth;

			vertices[offset++] = Colour.x;
			vertices[offset++] = Colour.y;
			vertices[offset++] = Colour.z;
			vertices[offset++] = Colour.a;

			vertices[offset++] = (float)i / (sections - 1);
			vertices[offset++] = (float)j / (sections - 1);

			vertices[offset++] = x;
			vertices[offset++] = y;
			vertices[offset++] = z;

			theta += (3.14159265359 / (sections - 1));
			//theta += (M_PI / (sections - 1));
		}

		//phi += (2 * M_PI) / (sections - 1);
		phi += (2 * 3.14159265359) / (sections - 1);
	}

	GLuint indices[(sections) * (sections)* indexPerQuad];
	offset = 0;
	for (int i = 0; i < sections; i++)
	{
		for (int j = 0; j < sections; j++)
		{
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			indices[offset++] = (((i + 1) % sections) * sections) + (j);
			indices[offset++] = (i * sections) + (j);

			indices[offset++] = (i * sections) + ((j + 1) % sections);
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
		}
	}

	GLuint VBO, EBO;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexAttrib * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexAttrib * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexAttrib * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertexAttrib * sizeof(GLfloat), (void*)(9 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	m_iIndicies = sizeof(indices) / sizeof(GLuint);

	if (TextureSource != "")
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height;
		unsigned char* image = SOIL_load_image(TextureSource, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

	}
}

/************************************************************
#--Description--#: 	Rebinds the vao with the colour (not texture)
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Sphere::Rebind()
{
	BindSphere();
}

/************************************************************
#--Description--#:	Render Current Mesh to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Sphere::Render(Utils::Transform Newtransform)
{
	if (bHasTexture)
	{		
		if (bIsLit)
		{
			glUseProgram(program);
			glm::mat4 translate = glm::translate(glm::mat4(), Newtransform.Position);
			glm::mat4 scale = glm::scale(glm::mat4(), Newtransform.Scale);
			glm::mat4 rotation = glm::rotate(glm::mat4(), glm::radians(Newtransform.Rotation.x), glm::vec3(1, 0, 0));
			rotation = glm::rotate(rotation, glm::radians(Newtransform.Rotation.y), glm::vec3(0, 1, 0));
			rotation = glm::rotate(rotation, glm::radians(Newtransform.Rotation.z), glm::vec3(0, 0, 1));
			glm::mat4 model = translate * rotation * scale;
			GLint UVCoordsLoc = glGetUniformLocation(Shader::LitTextureprogram, "model");
			glUniformMatrix4fv(UVCoordsLoc, 1, GL_FALSE, glm::value_ptr(model));
			Lighting::m_v3SunDirection = { 10, 1, 10 };
			GLint LightPosLoc = glGetUniformLocation(Shader::LitTextureprogram, "lightPos");
			glUniform3fv(LightPosLoc, 3, glm::value_ptr(Lighting::m_v3SunDirection));
			glUniform1i(glGetUniformLocation(Shader::LitTextureprogram, "bIsTex"), bHasTexture);
		}
		else
		{
			glUseProgram(program);
		}

		glEnable(GL_BLEND);
	}
	else
	{
		if (bIsLit)
		{
			glUseProgram(program);
			glUniform1i(glGetUniformLocation(Shader::LitTextureprogram, "bIsTex"), bHasTexture);
		}
		else
		{
			glUseProgram(program);
		}
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
void Sphere::Update()
{
}
