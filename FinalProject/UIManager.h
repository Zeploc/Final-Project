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
#include "ScoreTab.h"
#include "OptionsScreen.h"

// Library Includes //
#include <memory>
//#pragma once­­­

class UIManager
{
public:
	Chat m_ChatInstance;
	ScoreTab m_ScoreTabInstance;
	OptionsScreen m_OptionsScreenInstance;

	void Update();
	void Render();

	bool m_bDisplayChat = false;
	bool m_bDisplayTabScores = false;
	bool m_bDisplayPauseOptions = false;

	void SwitchUIMode(bool _bNewMode);
	bool m_bFPS = false;

private:
	bool m_bUIMode = true;

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

