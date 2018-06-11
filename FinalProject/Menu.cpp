//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    Menu.cpp
// Description    	:    main implementation for Menu
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

// Library Includes //
#include <iostream>
#include <random>

// OpenGL Library Includes //

// Engine Includes //

#include "Engine\UISlider.h"
#include "Engine\SceneManager.h"
#include "Engine\SoundManager.h"
#include "Engine\Cursor.h"
#include "Engine\Input.h"

// Local Includes //
#include "GameSettings.h"
#include "LevelManager.h"
#include "UIManager.h"
#include "NetworkSystem.h"
#include "Client.h"
#include "Server.h"
#include "NetworkManager.h"

// This Includes //
#include "Menu.h"

// Static Variables //

// Prototpyes //
void StartGameBtn();
void ExitGameBtn();
void OptionsScreenBtn();
void MenuScreenBtn();
void HostGameScreenBtn();
void JoinGameScreenBtn();

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Menu::Menu(std::string sSceneName)
	: Scene(sSceneName)
{
	// Add Title Elements
	std::shared_ptr<UIText> Title(new UIText(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, 100.0f), 0, glm::vec4(0.6, 0.6, 0.6, 1.0), "TEST MENU", "Resources/Fonts/Roboto-Black.ttf", 100, Utils::CENTER));
	std::shared_ptr<UIButton> StartBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 - 100), Utils::CENTER, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 480, 70, StartGameBtn));
	StartBtn->AddText("START GAME", "Resources/Fonts/Roboto-Thin.ttf", 34, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	std::shared_ptr<UIButton> HostGameBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 - 20), Utils::CENTER, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 480, 70, HostGameScreenBtn));
	HostGameBtn->AddText("HOST GAME", "Resources/Fonts/Roboto-Thin.ttf", 34, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	std::shared_ptr<UIButton> JoinGameBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 + 60), Utils::CENTER, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 480, 70, JoinGameScreenBtn));
	JoinGameBtn->AddText("JOIN GAME", "Resources/Fonts/Roboto-Thin.ttf", 34, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	std::shared_ptr<UIButton> OptionsBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 + 140), Utils::CENTER, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 480, 70, OptionsScreenBtn));
	OptionsBtn->AddText("OPTIONS", "Resources/Fonts/Roboto-Thin.ttf", 34, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	std::shared_ptr<UIButton> QuitBtn(new UIButton(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 + 220), Utils::CENTER, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 480, 70, ExitGameBtn));
	QuitBtn->AddText("QUIT GAME", "Resources/Fonts/Roboto-Thin.ttf", 34, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	std::shared_ptr<UIImage> BackImage(new UIImage(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2), Utils::CENTER, 0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Camera::GetInstance()->SCR_WIDTH, Camera::GetInstance()->SCR_HEIGHT, "Resources/ExampleBackground.png", 2));
	std::shared_ptr<UIImage> UIIMG(new UIImage(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, 100.0f), Utils::CENTER, 0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 600, 100, "Resources/Box.png", 2));
	// Add elements to scene UI elements
	AddUIElement(BackImage);
	AddUITextElement(Title);
	AddUIElement(StartBtn);
	AddUIElement(OptionsBtn);
	AddUIElement(HostGameBtn);
	AddUIElement(JoinGameBtn);
	AddUIElement(QuitBtn);
	// Add elements to vector list
	v_MenuElements.push_back(Title);
	v_MenuElements.push_back(StartBtn);
	v_MenuElements.push_back(OptionsBtn);
	v_MenuElements.push_back(HostGameBtn);
	v_MenuElements.push_back(JoinGameBtn);
	v_MenuElements.push_back(QuitBtn);
	//// Set sounds for buttons
	//StartBtn->SetPressSound("Resources/Sound/Button.wav");
	//OptionsBtn->SetPressSound("Resources/Sound/Button.wav");
	//QuitBtn->SetPressSound("Resources/Sound/Button.wav");
	//// Set positions
	//ButtonPositions[0] = StartBtn->GetPosition() - glm::vec2((float)StartBtn->ImageComponent.GetWidth() / 2.0f, 0.0f);
	//ButtonPositions[1] = OptionsBtn->GetPosition() - glm::vec2((float)OptionsBtn->ImageComponent.GetWidth() / 2.0f, 0.0f);
	//ButtonPositions[2] = QuitBtn->GetPosition() - glm::vec2((float)QuitBtn->ImageComponent.GetWidth() / 2.0f, 0.0f);
	
	// Add Options Elements
	std::shared_ptr<UIText> OptionsText(new UIText(glm::vec2(Camera::GetInstance()->SCR_WIDTH / 2, Camera::GetInstance()->SCR_HEIGHT / 2 - 200.0f), 0, glm::vec4(0.2, 0.2, 0.2, 1.0), "OPTIONS:", "Resources/Fonts/Roboto-Bold.ttf", 80, Utils::CENTER));
	OptionsText->SetActive(false);
	std::shared_ptr<UIButton> BackBtn(new UIButton(glm::vec2(0, Camera::GetInstance()->SCR_HEIGHT), Utils::BOTTOM_LEFT, 0.0f, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), 480, 80, MenuScreenBtn));
	BackBtn->AddText("Back", "Resources/Fonts/Roboto-Thin.ttf", 34, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Utils::CENTER, { 0, 0 });
	BackBtn->SetActive(false);
	m_VolumeSlider = std::make_shared<UISlider>(UISlider({ Camera::GetInstance()->SCR_WIDTH / 2,Camera::GetInstance()->SCR_HEIGHT / 2 }, 0, { 0.2f, 0.2f, 0.2f, 1.0f }, { 0.6f, 0.6f, 0.6f, 1.0f }, 300, 20, 40, 10, Utils::CENTER, "Main Volume:"));
	m_VolumeSlider->SetMinimumPosition(0.0f);
	m_VolumeSlider->SetMaximumPosition(10.0f);
	m_VolumeSlider->SetLockSize(0.5f);
	m_VolumeSlider->SetStartPosition(GameSettings::fVolumeLevel);
	m_VolumeSlider->SetActive(false);

	// Add elements to scene UI elements
	AddUITextElement(OptionsText);
	AddUIElement(BackBtn);
	AddUIElement(m_VolumeSlider);
	// Add elements to vector list
	v_OptionsElements.push_back(OptionsText);
	v_OptionsElements.push_back(BackBtn);
	v_OptionsElements.push_back(m_VolumeSlider);
	
	// Add cursor
	//std::shared_ptr<Cursor> NewCursor = std::make_shared<Cursor>("Resources/Grey_Cursor.png");
	//AddUIElement(NewCursor);

	//m_Arrow = std::make_shared<UIImage>(ButtonPositions[0], Utils::BOTTOM_RIGHT, 0.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 32, 24, "Resources/Grey_Arrow.png", 1);
	//AddUIElement(m_Arrow);
	/*SoundManager::GetInstance()->AddChannel("BackgroundC");
	SoundManager::GetInstance()->AddChannel("FXC");
	PlayRandomTrack();*/
	UIManager::GetInstance()->m_bDisplayChat = false;
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Menu::~Menu()
{
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Menu::Update()
{
	Scene::Update();	

	if (m_CurrentSection == OPTIONS)
	{
		float CurrentVolumeSlider = m_VolumeSlider->GetValue();
		if (CurrentVolumeSlider != GameSettings::fVolumeLevel)
		{
			SoundManager::GetInstance()->SetChannelVolume("BackgroundC", CurrentVolumeSlider / 20.0f);
		}
	}
	
}

/************************************************************
#--Description--#:  When scene is loaded
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Menu::OnLoadScene()
{
	UIManager::GetInstance()->m_bDisplayChat = false;
	UIManager::GetInstance()->m_bDisplayHUD = false;
	UIManager::GetInstance()->SwitchUIMode(true);
	LobbyScreen.Init(this->shared_from_this());
	JoinGameScreen.Init(this->shared_from_this());
	HostGameScreen.Init(this->shared_from_this());
	//Camera::GetInstance()->m_bFPS = false;
	SoundManager::GetInstance()->StopAudio("BackgroundC");
	PlayRandomTrack();
}

/************************************************************
#--Description--#:  Play random menu music track
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void Menu::PlayRandomTrack()
{
	const char* MusicOptions[] = { "Resources/Sound/Story of Maple.mp3", "Resources/Sound/Ludum Dare 28 - Track 4.wav",
		"Resources/Sound/Ludum Dare 30 - Track 1.wav", "Resources/Sound/Ludum Dare 32 - Track 2.wav",
		"Resources/Sound/Ludum Dare 38 - Track 3.wav" };
	int iRandTrack = 2;// rand() % 5;
	std::cout << "Playing " << MusicOptions[iRandTrack] << " | Number " << iRandTrack << std::endl;
	SoundManager::GetInstance()->AddAudio(MusicOptions[iRandTrack], true, "BackgroundTrack " + std::to_string(iRandTrack));
	SoundManager::GetInstance()->PlayAudio("BackgroundTrack " + std::to_string(iRandTrack), "BackgroundC");
	SoundManager::GetInstance()->SetChannelVolume("BackgroundC", GameSettings::fVolumeLevel / 20.0f);
}

/************************************************************
#--Description--#:  Toggle menu section
#--Author--#: 		Alex Coultas
#--Parameters--#:	Menu Section to switch to
#--Return--#: 		NA
************************************************************/
void Menu::ToggleMenuSection(MENUSECTION _NewSection)
{
	// Hide all menu elements
	for (auto it : v_MenuElements)
		it->SetActive(false);
	for (auto it : v_OptionsElements)
		it->SetActive(false);
	HostGameScreen.HideElements();
	/*for (auto it : v_HostGameElements)
		it->SetActive(false);*/
	JoinGameScreen.HideElements();
	/*for (auto it : v_JoinGameElements)
		it->SetActive(false);*/
	LobbyScreen.HideElements();
	/*for (auto it : v_LobbyElements)
		it->SetActive(false);*/
	
	// Set current Section to new section
	m_CurrentSection = _NewSection;

	// Show all elements in the corresponding section
	switch (m_CurrentSection)
	{
	case MAIN:
		for (auto it : v_MenuElements)
			it->SetActive(true);
		break;
	case OPTIONS:
		for (auto it : v_OptionsElements)
			it->SetActive(true);
		break;
	case HOST:
		HostGameScreen.ShowElements();
		/*for (auto it : v_HostGameElements)
			it->SetActive(true);*/
		break;
	case JOIN:
		JoinGameScreen.ShowElements();
		/*for (auto it : v_JoinGameElements)
			it->SetActive(true);*/
		break;
	case LOBBY:
		LobbyScreen.ShowElements();
		/*for (auto it : v_LobbyElements)
			it->SetActive(true);*/
		break;
	default:
		break;
	}
}



/************************************************************
#--Description--#:  Local start game button
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void StartGameBtn()
{
	std::cout << "Start Game (Switch to Level)\n";
	SoundManager::GetInstance()->StopAudio("BackgroundC");
	SceneManager::GetInstance()->SwitchScene("Level " + std::to_string(LevelManager::GetInstance()->CurrentLevel()));
	LevelManager::GetInstance()->GetCurrentActiveLevel()->PlayRandomTrack();
}

/************************************************************
#--Description--#:  Local exit game button
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void ExitGameBtn()
{
	glutLeaveMainLoop();
}

/************************************************************
#--Description--#:  Local instructions game button
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void MenuScreenBtn()
{
	std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());
	MenuRef->ToggleMenuSection(MAIN);
}

/************************************************************
#--Description--#:  Local instructions game button
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void OptionsScreenBtn()
{
	std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());
	MenuRef->ToggleMenuSection(OPTIONS);
}

/************************************************************
#--Description--#:  Local instructions game button
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void HostGameScreenBtn()
{
	std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());
	MenuRef->ToggleMenuSection(HOST);
}

/************************************************************
#--Description--#:  Local instructions game button
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
void JoinGameScreenBtn()
{
	std::shared_ptr<Menu> MenuRef = std::dynamic_pointer_cast<Menu>(SceneManager::GetInstance()->GetCurrentScene());
	MenuRef->ToggleMenuSection(JOIN);
	NetworkManager::GetInstance()->m_Network.InitClient();
}



