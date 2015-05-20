#pragma once
#include "Player.h"

enum KEYSTATE {
	KS_DOWN		=	(1 << 0),
	KS_LEFT		=	(1 << 1),
	KS_RIGHT	=	(1 << 2),
	KS_UP		=	(1 << 3),
	KS_UPPER_LEFT	=	KS_UP | KS_LEFT,
	KS_UPPER_RIGHT	=	KS_UP | KS_RIGHT,
	KS_LOWER_LEFT	=	KS_DOWN | KS_LEFT,
	KS_LOWER_RIGHT	=	KS_DOWN | KS_RIGHT
};

class CController {

private:
	CController();
	~CController();

public:
	static CController* GetInstance() { return &s_controller; }

public:
	LRESULT MessageHandler(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL Tick(DWORD fElapsedTime, BOOL started);
	inline BOOL NeedToggleQuality() 
	{ if (m_bHQ) { m_bHQ = FALSE; return TRUE; } return FALSE; }
	inline void BindPlayer(CPlayer* player) { m_player = player; }

private:
	void KeyboardTick(DWORD fElapsedTime);
	CPlayer* m_player;
	BOOL m_bMouseState, m_bEnter, m_bHQ;
	DWORD m_lPressedTime;
	FLOAT m_fIntensity;
	D3DXVECTOR3 m_vMse, m_vDir;

private:
	static CController s_controller;
};