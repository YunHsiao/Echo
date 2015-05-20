#include "stdafx.h"
#include "Footprint.h"
#include "Direct3D9.h"

CFootPrint::CFootPrint(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, 
					   BOOL left, DWORD color)
:CElement(left?RL_LEFT_FOOTPRINT:RL_RIGHT_FOOTPRINT, 
		  pos, dir, 8000.f, 1.f, .7f, 0.f, FALSE, color)
,m_bLeft(left)
{	
}

CFootPrint::~CFootPrint() {
	
}

void CFootPrint::Tick(DWORD fElapsedTime, SMap *map, UINT index) {
	CElement::Tick(fElapsedTime, map, index);
}

ID3DXSprite* CFootPrint::Render(const D3DXVECTOR3& pos, SMap *map, BOOL highQuality) {
	return CElement::Render(pos, map, highQuality);
}