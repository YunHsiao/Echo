#pragma once
#include "Scene.h"
#include "Map.h"

class CSceneManager {

private:
	CSceneManager();
	~CSceneManager();

public:
	static CSceneManager* GetInstance() { return &s_scenes; }

public:
	BOOL Tick(DWORD fElapsedTime);
	void Render(BOOL highQuality);
	void LoadScene();
	inline const SMap *GetMap() { return m_scene->GetMap(); }

private:
	CScene *m_scene;
	UINT m_nLevel;

private:
	static CSceneManager s_scenes;
};