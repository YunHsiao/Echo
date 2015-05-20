#pragma once
#include "Line.h"
#include "Element.h"

extern D3DXVECTOR3 g_vScreenCenter;
extern D3DXVECTOR3 g_vDefaultDir;

typedef std::list<CElement*> Elements;
typedef std::list<CElement*>::iterator ElementsIt;

class CElementManager {

private:
	CElementManager();
	~CElementManager();

public:
	static CElementManager* GetInstance() { return &s_elements; }

public:
	void CreateElement(UINT texture, const SMap *map, FLOAT lastTime = 5000.f, BOOL fadeIn = FALSE,
		DWORD color = 0xFFFFFF, BOOL ui = FALSE, const D3DXVECTOR3& pos = g_vScreenCenter, 
		const D3DXVECTOR3& dir = g_vDefaultDir);
	void CreateLine(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, const SMap *map, DWORD color = 0xFFFFFF);
	void CreateCircle(const D3DXVECTOR3& pos, FLOAT intensity, const SMap *map, 
		FLOAT lastTime = 5000.f, FLOAT length = 500.f, INT max = 72, 
		DWORD color = 0xFFFFFF, BOOL isPlayer = FALSE);
	void CreateFootprint(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, 
		BOOL left, const SMap *map, DWORD color = 0xFFFFFF, FLOAT scale = .7f);
	void Tick(DWORD fElapsedTime);
	void Render(const D3DXVECTOR3& pos, BOOL highQuality);
	BOOL IsGone() { return m_pAmbience.empty() && m_pAmbience.empty(); }
	BOOL IsEchoInside(D3DXVECTOR3& pos);
	void Clear();
private:
	Elements m_pAmbience, m_pEcho;

private:
	static CElementManager s_elements;

};