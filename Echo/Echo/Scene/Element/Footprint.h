#pragma once
#include "Element.h"

class CFootPrint : public CElement {

public:
	CFootPrint(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, BOOL left, DWORD color = 0xFFFFFF);
	~CFootPrint();
	virtual void Tick(DWORD fElapsedTime, SMap *map, UINT index = 0);
	virtual ID3DXSprite* Render(const D3DXVECTOR3& pos, SMap *map, BOOL highQuality);

protected:
	BOOL m_bLeft;
};