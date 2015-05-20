#include "stdafx.h"
#include "GameManager.h"

#include "Logo.h"
#include "Direct3D9.h"
#include "Controller.h"
#include "SceneManager.h"
#include "AudioManager.h"

CGame CGame::s_game;

CGame::CGame() 
:m_logo(NULL)
,m_bStarted(FALSE)
,m_bHQ(FALSE)
{
}

CGame::~CGame() 
{
	SAFE_DELETE(m_logo);
}

HRESULT CGame::Initialize(HWND hWnd, int iWindowsWidth, int iWindowsHeight) {
	HRESULT hr;
	if(FAILED(hr = CDirect3D9::GetInstance()->Initialize(hWnd, 
		iWindowsWidth, iWindowsHeight, std::ifstream("Resources\\list.txt"))))
		return hr;
	if(FAILED(hr = CAudioManager::GetInstance()->Initialize(hWnd, 
		std::ifstream("Sound\\list.txt"))))
		return hr;

	m_logo = new CLogo();
	return hr;
}

LRESULT CGame::MessageHandler(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CController::GetInstance()->MessageHandler(message, wParam, lParam);
}


void CGame::Tick(DWORD fElapsedTime) {
	BOOL skip = CController::GetInstance()->Tick(fElapsedTime, m_bStarted);
	if (CController::GetInstance()->NeedToggleQuality()) m_bHQ = !m_bHQ;
	if (m_bStarted) {
		if (CSceneManager::GetInstance()->Tick(fElapsedTime)) QuitGame();
	} else if (m_logo) {
		if (m_logo->Tick(fElapsedTime, skip)) StartGame();
	}
}

void CGame::Render() {
	CDirect3D9::GetInstance()->PreRender();
	if (m_bStarted) CSceneManager::GetInstance()->Render(m_bHQ);
	else if (m_logo) m_logo->Render(m_bHQ);
	CDirect3D9::GetInstance()->PostRender();
}

void CGame::StartGame() {
	m_bStarted = TRUE;
	SAFE_DELETE(m_logo);
	CSceneManager::GetInstance()->LoadScene();
}

void CGame::QuitGame() {
	m_bStarted = FALSE;
	if (!m_logo) m_logo = new CLogo();
}