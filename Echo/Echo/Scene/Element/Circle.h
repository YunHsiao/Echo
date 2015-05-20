#pragma once
#include "Line.h"

class CCircle {

public:
	CCircle(const D3DXVECTOR3& pos, FLOAT intensity, const SMap *map, FLOAT lastTime = 5000.f, 
		FLOAT length = 500.f, INT max = 72, DWORD color = 0xFFFFFF);
	~CCircle();
	void Tick(DWORD fElapsedTime, UINT index = 0);
	void Render(const D3DXVECTOR3& pos, BOOL highQuality);
	BOOL IsEchoInside(D3DXVECTOR3& pos);
	inline BOOL IsGone() { return lines.empty() || lines[0]->IsGone(); }
	typedef std::vector<CLine*> Lines;
	typedef std::vector<CLine*>::iterator LinesIt;

private:
	Lines lines;
	INT m_nMax;
};