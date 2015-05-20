#include "stdafx.h"
#include "Controller.h"

CController CController::s_controller;

CController::CController() 
:m_player(NULL)
,m_bEnter(FALSE)
{
	
}

CController::~CController()
{

}

void CController::KeyboardTick(DWORD fElapsedTime) 
{
	SHORT sKeyState = 0;
	if (::GetAsyncKeyState(VK_LEFT) & 0x8000) sKeyState += KS_LEFT;
	if (::GetAsyncKeyState(VK_UP) & 0x8000) sKeyState += KS_UP;
	if (::GetAsyncKeyState(VK_RIGHT) & 0x8000) sKeyState += KS_RIGHT;
	if (::GetAsyncKeyState(VK_DOWN) & 0x8000) sKeyState += KS_DOWN;
	if (::GetAsyncKeyState(VK_SPACE) & 0x8000) m_lPressedTime += fElapsedTime;

	if (sKeyState) {
		if (m_bMouseState) m_bMouseState = FALSE;
		if (::GetAsyncKeyState(VK_SHIFT) & 0x8000) m_player->SetStatus(PS_SNEAKING);
		else m_player->SetStatus(PS_WALKING);
	} else { if (!m_bMouseState) m_player->SetStatus(PS_STANDING); return; }

	if (sKeyState == KS_UPPER_LEFT) m_player->SetDirection(D3DXVECTOR3(-1,-1,0));
	else if (sKeyState == KS_UPPER_RIGHT) m_player->SetDirection(D3DXVECTOR3(1,-1,0));
	else if (sKeyState == KS_LOWER_LEFT) m_player->SetDirection(D3DXVECTOR3(-1,1,0));
	else if (sKeyState == KS_LOWER_RIGHT) m_player->SetDirection(D3DXVECTOR3(1,1,0));
	else if (sKeyState == KS_LEFT) m_player->SetDirection(D3DXVECTOR3(-1,0,0));
	else if (sKeyState == KS_UP) m_player->SetDirection(D3DXVECTOR3(0,-1,0));
	else if (sKeyState == KS_RIGHT) m_player->SetDirection(D3DXVECTOR3(1,0,0));
	else if (sKeyState == KS_DOWN) m_player->SetDirection(D3DXVECTOR3(0,1,0));
}

LRESULT CController::MessageHandler(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_KEYDOWN:
		if (wParam == VK_RETURN) 
			m_bEnter = TRUE;
		else if (wParam == VK_F11)
			m_bHQ = TRUE;
		break;
	case WM_KEYUP:
		if (wParam == VK_SPACE) {
			m_fIntensity = m_lPressedTime / 1000.f;
			m_lPressedTime = 0;
			if (m_player) m_player->Stamp(m_fIntensity);
		} else if (wParam == VK_RETURN)
			m_bEnter = FALSE;
		break;
	case WM_LBUTTONDOWN:
		m_vMse.x = LOWORD(lParam);
		m_vMse.y = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		m_vDir.x = LOWORD(lParam) - m_vMse.x;
		m_vDir.y = HIWORD(lParam) - m_vMse.y;
		if (m_player) m_player->CreateLine(m_vMse, m_vDir);
		break;
	}
	return 0;
}

BOOL CController::Tick(DWORD fElapsedTime, BOOL started) 
{
	if (started) {
		if (m_player) KeyboardTick(fElapsedTime);
	} else return m_bEnter;
	return FALSE;
}