#include "stdafx.h"
#include "SceneManager.h"
#include "ElementManager.h"

CSceneManager CSceneManager::s_scenes;

CSceneManager::CSceneManager()
:m_scene(NULL)
,m_nLevel(1) // Level start from "SC_1.xml"
{
}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_scene)
}

void CSceneManager::LoadScene() {
	m_scene = new CScene(m_nLevel);
	m_nLevel = m_scene->GetLevel();
}

BOOL CSceneManager::Tick(DWORD fElapsedTime) {
	BOOL bNext(FALSE);
	if (!m_scene) return FALSE;
	if (m_scene->Tick(fElapsedTime)) {
		CElementManager::GetInstance()->Clear();
		if (m_scene->IsAlive()) { ++m_nLevel; bNext = TRUE; }
		SAFE_DELETE(m_scene)
		m_scene = new CScene(m_nLevel);
		m_nLevel = m_scene->GetLevel();
		if (bNext && m_nLevel == 1) return TRUE;
	}
	return FALSE;
}

void CSceneManager::Render(BOOL highQuality) {
	if (m_scene) m_scene->Render(highQuality);
}