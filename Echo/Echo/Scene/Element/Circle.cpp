#include "stdafx.h"
#include "Circle.h"

CCircle::CCircle(const D3DXVECTOR3& pos, FLOAT intensity, const SMap *map, 
				 FLOAT lastTime, FLOAT length, INT max, DWORD color)
				 :m_nMax(max)
{
	D3DXVECTOR3 dir;
	D3DXMATRIX trans;
	if (intensity > 1.f) intensity = 1.f;
	INT cnt = static_cast<INT>(m_nMax*intensity+0.5f);
	D3DXMatrixRotationZ(&trans, 2*D3DX_PI/cnt);
	UINT t;
	do {
		rand_s(&t);
		dir.x = t%10-5.f;
		rand_s(&t);
		dir.y = t%10-5.f;
	} while (dir.x == 0.f && dir.y == 0.f);
	dir.z = 0.f;
	D3DXVec3Normalize(&dir, &dir);
	for (INT i(0); i < cnt; ++i) {
		D3DXVec3TransformNormal(&dir, &dir, &trans);
		lines.push_back(new CLine(pos, dir, 
			intensity, map, intensity <= 2.f/9, lastTime, length, color));
	}
}

CCircle::~CCircle()
{
	for (LinesIt it(lines.begin()); it != lines.end(); ++it)
		SAFE_DELETE(*it);
	lines.clear();
}

void CCircle::Tick(DWORD fElapsedTime, UINT index)
{
	for (LinesIt it(lines.begin()); it != lines.end(); ++it) 
		(*it)->Tick(fElapsedTime, index);
}

void CCircle::Render(const D3DXVECTOR3& pos, BOOL highQuality)
{
	for (LinesIt it(lines.begin()); it != lines.end(); ++it)
		(*it)->Render(pos, highQuality);
}

BOOL CCircle::IsEchoInside(D3DXVECTOR3& pos) {
	for (LinesIt it(lines.begin()); it != lines.end(); ++it)
		if (D3DXVec3LengthSq(&((*it)->GetCoord() - pos)) < 900.f) 
			return TRUE;
	return FALSE;
}