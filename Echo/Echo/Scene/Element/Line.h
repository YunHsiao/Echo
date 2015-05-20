#pragma once
#include "Element.h"

class CLine : public CElement {

public:
	CLine(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, FLOAT intensity, const SMap *map,
		BOOL quiet = FALSE, FLOAT lastTime = 5000.f, FLOAT length = 500.f, DWORD color = 0xFFFFFF, UINT index = 0);
	~CLine();
	virtual void Tick(DWORD fElapsedTime);
	virtual ID3DXSprite *Render(const D3DXVECTOR3& pos, BOOL highQuality);
	typedef std::deque<D3DXVECTOR3> EchoPoints;
	typedef std::deque<D3DXVECTOR3>::iterator EchoPointsIt;
	typedef std::deque<D3DXVECTOR3>::size_type EchoPointsIndex;

protected:
	void RenderPoint(ID3DXSprite *sprite, const D3DXVECTOR3& lerp, 
					 const D3DXVECTOR3& pos, UINT& type, FLOAT& intensity);
	void DetectCollision();
	EchoPoints m_pEcho;
	RECT m_rHead, m_rTail;
	BOOL m_bQuiet, m_bTurned, m_bFaded;
	FLOAT m_fLength;
	UINT m_mapIndex;
};