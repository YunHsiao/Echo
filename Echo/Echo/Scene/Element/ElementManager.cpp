#include "stdafx.h"
#include "ElementManager.h"

CElementManager CElementManager::s_elements;

CElementManager::CElementManager() 
{

}

CElementManager::~CElementManager() 
{
	Clear();
}

void CElementManager::Clear() {
	for (ElementsIt it(m_pEcho.begin()); it != m_pEcho.end(); ++it) 
		SAFE_DELETE(*it) m_pEcho.clear();
	for (ElementsIt it(m_pAmbience.begin()); it != m_pAmbience.end(); ++it) 
		SAFE_DELETE(*it) m_pAmbience.clear();
}

void CElementManager::CreateElement(UINT texture, const SMap *map, FLOAT lastTime, BOOL fadeIn,
						DWORD color, BOOL ui, const D3DXVECTOR3& pos, const D3DXVECTOR3& dir) 
{
	m_pAmbience.push_back(new CElement(texture, map, pos, dir, lastTime, 
		1.f, 1.f, 0.f, fadeIn, color, ui));
}

void CElementManager::CreateLine(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, const SMap *map, DWORD color) {
	m_pEcho.push_back(new CLine(pos, dir, 1.f, map, FALSE, 5000.f, 500.f, color));
}

void CElementManager::CreateCircle(const D3DXVECTOR3& pos, FLOAT intensity, const SMap *map, 
			FLOAT lastTime, FLOAT length, INT max, DWORD color, BOOL isPlayer) 
{
	D3DXVECTOR3 dir;
	D3DXMATRIX trans;
	if (intensity > 1.f) intensity = 1.f;
	INT cnt = static_cast<INT>(max*intensity+0.5f);
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
	if (isPlayer) 
		for (INT i(0); i < cnt; ++i) {
			D3DXVec3TransformNormal(&dir, &dir, &trans);
			m_pEcho.push_back(new CLine(pos, dir, 
				intensity, map, intensity <= 2.f/9, lastTime, length, color));
		}
	else 
		for (INT i(0); i < cnt; ++i) {
			D3DXVec3TransformNormal(&dir, &dir, &trans);
			m_pAmbience.push_back(new CLine(pos, dir, 
				intensity, map, intensity <= 2.f/9, lastTime, length, color));
		}

}

void CElementManager::CreateFootprint(const D3DXVECTOR3& pos, 
		const D3DXVECTOR3& dir, BOOL left, const SMap *map, DWORD color, FLOAT scale) 
{
	m_pAmbience.push_back(new CElement(left?RL_LEFT_FOOTPRINT:RL_RIGHT_FOOTPRINT, map,
		pos, dir, 8000.f, 1.f, scale, 0.f, FALSE, color));
}

BOOL CElementManager::IsEchoInside(D3DXVECTOR3& pos) {
	for (ElementsIt it(m_pEcho.begin()); it != m_pEcho.end(); ++it)
		if (D3DXVec3LengthSq(&((*it)->GetCoord() - pos)) < 900.f) 
			return TRUE;
	return FALSE;
}

void CElementManager::Tick(DWORD fElapsedTime) {
	for (ElementsIt it(m_pEcho.begin()); it != m_pEcho.end();) {
		(*it)->Tick(fElapsedTime);
		if ((*it)->IsGone()) {
			SAFE_DELETE(*it);
			it = m_pEcho.erase(it);
			continue;
		}
		++it;
	}
	for (ElementsIt it(m_pAmbience.begin()); it != m_pAmbience.end();) {
		(*it)->Tick(fElapsedTime);
		if ((*it)->IsGone()) {
			SAFE_DELETE(*it);
			it = m_pAmbience.erase(it);
			continue;
		}
		++it;
	}
}

void CElementManager::Render(const D3DXVECTOR3& pos, BOOL highQuality) {
	for (ElementsIt it(m_pEcho.begin()); it != m_pEcho.end(); ++it)
		(*it)->Render(pos, highQuality);
	for (ElementsIt it(m_pAmbience.begin()); it != m_pAmbience.end(); ++it)
		(*it)->Render(pos, highQuality);
}