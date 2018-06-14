//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    UIManager.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//


// Local Includes //
#include "Chat.h"
#include "EndGameScreen.h"
#include "OptionsScreen.h"
#include "MessageBox.h"
#include "HUD.h"

// Library Includes //
#include <memory>
//#pragma once­­­

class UIManager
{
public:
	Chat m_ChatInstance;
	EndScreen m_EndScreen;
	OptionsScreen m_OptionsScreenInstance;
	UIMessageBox m_MessageBoxInstance;
	HUD m_HUDInstance;

	void Update();
	void Render();

	void ShowMessageBox(std::string _Message);

	bool m_bDisplayChat = false;
	bool m_bEndScreen = false;
	bool m_bDisplayPauseOptions = false;
	bool m_bDisplayMessageBox = false;
	bool m_bDisplayHUD = false;

	void SwitchUIMode(bool _bNewMode);
	bool GetUIMode() { return m_bUIMode; };
	bool m_bFPS = false;
	bool m_bLoadingScreen = false;

private:
	bool m_bUIMode = true;
	std::shared_ptr<UIImage> ControlsImage;

	// Singleton
public:
	static std::shared_ptr<UIManager> GetInstance();
	static void DestoryInstance();
	~UIManager(); // Shared pointer has to be able to call destructor so can't be private

private:
	static std::shared_ptr<UIManager> m_pUIManager;
	UIManager();
	UIManager(UIManager const&);              // Don't Implement
	void operator=(UIManager const&); // Don't implement
};

