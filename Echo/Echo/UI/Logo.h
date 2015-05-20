#pragma once
#include "Scene.h"
#include "Polygon.h"
#include "Element.h"
#include "Line.h"

class CLogo {

public:
	CLogo();
	~CLogo();
	BOOL Tick(DWORD fElapsedTime, BOOL skip);
	void Render(BOOL highQuality);
	void CreateCircle(const D3DXVECTOR3& pos, FLOAT intensity, const SMap *map, 
		FLOAT lastTime = 5000.f, FLOAT length = 500.f, UINT index = 0, INT max = 72, 
		DWORD color = 0xFFFFFF, BOOL isPlayer = FALSE);
	typedef std::list<CElement*> Elements;
	typedef std::list<CElement*>::iterator ElementsIt;
	typedef std::vector<CLine*> Echoes;
	typedef std::vector<CLine*>::iterator EchoesIt;

private:	
	void Fire();
	void ClearCircles();
	void Light(UINT texture, FLOAT lastTime = 5000.f);
	SMap maps;
	Echoes echoes;
	LONG m_lElapsedTime, m_lText;
	Elements elements;
	BOOL m_bEnableSkip, m_bEntering;
};