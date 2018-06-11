//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name		:    main.cpp
// Description		:    main implementation of the program
// Author			:    Alex Coultas
// Mail				:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <iostream>
#include <time.h>
#include <vld.h>

// OpenGL Library Includes //
#include <glew.h>
#include <freeglut.h>
#include <fmod.hpp>
#include <soil\SOIL.h>
#include <glm/common.hpp>
#include <glm/gtc\type_ptr.hpp>
#include <glm/gtc\type_ptr.hpp>
#include <glm/gtc\matrix_transform.hpp>

// Engine Includes //
#include "Engine\Input.h"
#include "Engine\Camera.h"
#include "Engine\Shader.h"
#include "Engine\ShaderLoader.h"
#include "Engine\Entity.h"
#include "Engine\UIText.h"
#include "Engine\Scene.h"
#include "Engine\SceneManager.h"
#include "Engine\SoundManager.h"
#include "Engine\Time.h"

// Local Includes //
#include "Level.h"
#include "Menu.h"
#include "Player.h"
#include "GameSettings.h"
#include "GameManager.h"
#include "LevelManager.h"
#include "UIManager.h"
#include "NetworkSystem.h"
#include "NetworkManager.h"
#include "AI.h"
#include "Engine/LogManager.h"

// make sure the winsock lib is included...
#pragma comment(lib,"ws2_32.lib")

// Static Function Prototypes //

// Types //
using namespace std;

#define SI Input::GetInstance()
#define CAM Camera::GetInstance() 
#define SM SceneManager::GetInstance()

// Prototypes //
void renderScene(void);
void changeSize(int w, int h);
void Update();
void Init();
void OnExit();

bool bLoading = true;

/************************************************************
#--Description--#: 	The main function of the program
#--Author--#: 		Alex Coultas
#--Parameters--#: 	Count of how many arguments and a dynamic array of each argument
#--Return--#: 		Return int
************************************************************/
int main(int argc, char **argv)
{
	srand(unsigned int(time(NULL)));
	// init GLUT and create Window
	CAM->Init(1280, 720, glm::vec3(0, 7, 10), glm::vec3(0, 1, -1), glm::vec3(0, 1.0f, 0.0f));
	CAM->SwitchProjection(Camera::PERSPECTIVE);
	glutInit(&argc, argv);
	//glutInitWindowPosition(0, 0);
	glutInitWindowSize(CAM->SCR_WIDTH, CAM->SCR_HEIGHT);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Game Window");
	//glutFullScreen();
	
	glewInit();				// OpenGL init
	Init();					// Settings Initialised

	// register callbacks
	glutDisplayFunc(renderScene);

	glutCloseFunc(OnExit); // Set close function if window is closed
	
	// here is the idle func registration
	glutIdleFunc(Update);
	
	// the input function registration
	SI->Init();
	
	// Window Resize Function
	glutReshapeFunc(changeSize);
	
	// enter GLUT event processing cycle
	glutMainLoop();

	
	return 0;
}

/************************************************************
#--Description--#: 	Renders the elements every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (bLoading)
	{
		LogManager::GetInstance()->Render();
	}
	else
	{
		Camera::GetInstance()->Update();

		SM->RenderCurrentScene();
		UIManager::GetInstance()->Render();
	}	
	glutSwapBuffers();
}

/************************************************************
#--Description--#: 	Updated when the window changes size
#--Author--#: 		Alex Coultas
#--Parameters--#: 	The width and height of the window
#--Return--#: 		NA
************************************************************/
void changeSize(int w, int h)
{
	CAM->SCR_HEIGHT = h;
	CAM->SCR_WIDTH = w;
}

/************************************************************
#--Description--#: 	Update for every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Update()
{
	if (bLoading)
	{
		SoundManager::GetInstance()->InitFMod();
		LevelManager::GetInstance()->Init();
		bLoading = false;
	}
	else
	{
		SceneManager::GetInstance()->UpdateCurrentScene();
		UIManager::GetInstance()->Update();
		Time::Update();
		NetworkManager::GetInstance()->m_Network.Update();
		SI->Update(); // HAS TO BE LAST TO HAVE FIRST PRESS AND RELEASE
	}
	glutPostRedisplay();
}

/************************************************************
#--Description--#: 	Initialises the program
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Init()
{
	Shader::AddProgram("Resources/Shaders/VertexShader.vs", "Resources/Shaders/FragmentShader.fs", "program");
	//Shader::program = loader.CreateProgram("Resources/Shaders/VertexShader.vs", "Resources/Shaders/FragmentShader.fs");
	Shader::AddProgram("Resources/Shaders/TextureVertexShader.vs", "Resources/Shaders/TextureFragmentShader.fs", "Textureprogram");
	//Shader::Textureprogram = loader.CreateProgram("Resources/Shaders/TextureVertexShader.vs", "Resources/Shaders/TextureFragmentShader.fs");
	Shader::AddProgram("Resources/Shaders/LitVertexShader.vs", "Resources/Shaders/LitFragmentShader.fs", "LitTextureprogram");
	//Shader::LitTextureprogram = loader.CreateProgram("Resources/Shaders/LitVertexShader.vs", "Resources/Shaders/LitFragmentShader.fs");
	Shader::AddProgram("Resources/Shaders/Text.vs", "Resources/Shaders/Text.fs", "TextUIprogram");
	//Shader::TextUIprogram = loader.CreateProgram("Resources/Shaders/Text.vs", "Resources/Shaders/Text.fs");
	Shader::AddProgram("Resources/Shaders/UI.vs", "Resources/Shaders/UI.fs", "UIprogram");
	//Shader::UIprogram = loader.CreateProgram("Resources/Shaders/UI.vs", "Resources/Shaders/UI.fs");
	Shader::AddProgram("Resources/Shaders/CubeMapVertexShader.vs", "Resources/Shaders/CubeMapFragmentShader.fs", "CubeMapProgram");
	//Shader::CubeMapProgram = loader.CreateProgram("Resources/Shaders/CubeMapVertexShader.vs", "Resources/Shaders/CubeMapFragmentShader.fs");
	Shader::AddProgram("Resources/Shaders/ModelVertexShader.vs", "Resources/Shaders/ModelFragmentShader.fs", "ModelProgram");
	//Shader::ModelProgram = loader.CreateProgram("Resources/Shaders/ModelVertexShader.vs", "Resources/Shaders/ModelFragmentShader.fs");
	Shader::AddProgram("Resources/Shaders/ModelVertexShaderLit.vs", "Resources/Shaders/ModelFragmentShaderLit.fs", "ModelProgramLit");
	//Shader::ModelProgramLit = loader.CreateProgram("Resources/Shaders/ModelVertexShaderLit.vs", "Resources/Shaders/ModelFragmentShaderLit.fs");
	Shader::AddProgram("Resources/Shaders/ReflectionShader.vs", "Resources/Shaders/ReflectionShader.fs", "ReflectionProgram");
	//Shader::ReflectionProgram = loader.CreateProgram("Resources/Shaders/ReflectionShader.vs", "Resources/Shaders/ReflectionShader.fs");
	
	glCullFace(GL_BACK); // Cull the Back faces
	glFrontFace(GL_CW); // Front face is Clockwise order
	glEnable(GL_CULL_FACE); // Turn on the back face culling

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//glutSetCursor(GLUT_CURSOR_NONE);

	//glutSetCursor(GLUT_CURSOR_CROSSHAIR);

	glutIgnoreKeyRepeat(1);

	glClearColor(0.8f, 0.8f, 0.8f, 1.0); // clear grey
	LogManager::GetInstance()->Init();
}

/************************************************************
#--Description--#: 	Runs when the application closes
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void OnExit()
{
	Shader::CleanUp();
	SceneManager::DestoryInstance();
	Camera::DestoryInstance();
	Input::DestoryInstance();
	SoundManager::DestoryInstance();
	GameManager::DestoryInstance();
	LevelManager::DestoryInstance();
	UIManager::DestoryInstance();
	NetworkManager::DestoryInstance();
	AI::CleanUp();
	Text::Fonts.~vector();
}


