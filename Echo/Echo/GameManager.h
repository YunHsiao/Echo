#pragma once
#include "Logo.h"

class CGame 
{
private:
	CGame();
	~CGame();

public:
	static CGame* GetInstance() { return &s_game; }

public:
	LRESULT MessageHandler(UINT message, WPARAM wParam, LPARAM lParam);
	HRESULT Initialize(HWND hWnd, int iWindowsWidth, int iWindowsHeight);
	void StartGame();
	void QuitGame();
	void Tick(DWORD fElapsedTime);
	void Render();

private:
	CLogo *m_logo;
	BOOL m_bStarted, m_bHQ;
	static CGame s_game;
};