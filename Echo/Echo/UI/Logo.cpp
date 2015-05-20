#include "stdafx.h"
#include "Logo.h"
#include "AudioManager.h"

//CLogo CLogo::s_logo;
extern D3DXVECTOR3 g_vScreenOrigin;
extern D3DXVECTOR3 g_vScreenCenter;
extern D3DXVECTOR3 g_vDefaultDir;

CLogo::CLogo() 
:m_lElapsedTime(5000)
,m_lText(-5000)
,m_bEnableSkip(FALSE)
,m_bEntering(FALSE)
{
	Points points;
	points.push_back(D3DXVECTOR3(50,  150, 0));
	points.push_back(D3DXVECTOR3(50,  450, 0));
	points.push_back(D3DXVECTOR3(185, 450, 0));
	points.push_back(D3DXVECTOR3(185, 390, 0));
	points.push_back(D3DXVECTOR3(80,  390, 0));
	points.push_back(D3DXVECTOR3(80,  330, 0));
	points.push_back(D3DXVECTOR3(185, 330, 0));
	points.push_back(D3DXVECTOR3(185, 270, 0));
	points.push_back(D3DXVECTOR3(80,  270, 0));
	points.push_back(D3DXVECTOR3(80,  210, 0));
	points.push_back(D3DXVECTOR3(185, 210, 0));
	points.push_back(D3DXVECTOR3(185, 150, 0));
	maps.insert(MT_WALL, SPolygon(points));
	points.clear();

	points.push_back(D3DXVECTOR3(235, 150, 0));
	points.push_back(D3DXVECTOR3(235, 450, 0));
	points.push_back(D3DXVECTOR3(370, 450, 0));
	points.push_back(D3DXVECTOR3(370, 390, 0));
	points.push_back(D3DXVECTOR3(265, 390, 0));
	points.push_back(D3DXVECTOR3(265, 210, 0));
	points.push_back(D3DXVECTOR3(370, 210, 0));
	points.push_back(D3DXVECTOR3(370, 150, 0));
	maps.insert(MT_WALL, SPolygon(points));
	points.clear();

	points.push_back(D3DXVECTOR3(420, 150, 0));
	points.push_back(D3DXVECTOR3(420, 450, 0));
	points.push_back(D3DXVECTOR3(450, 450, 0));
	points.push_back(D3DXVECTOR3(450, 330, 0));
	points.push_back(D3DXVECTOR3(525, 330, 0));
	points.push_back(D3DXVECTOR3(525, 450, 0));
	points.push_back(D3DXVECTOR3(555, 450, 0));
	points.push_back(D3DXVECTOR3(555, 150, 0));
	points.push_back(D3DXVECTOR3(525, 150, 0));
	points.push_back(D3DXVECTOR3(525, 270, 0));
	points.push_back(D3DXVECTOR3(450, 270, 0));
	points.push_back(D3DXVECTOR3(450, 150, 0));
	maps.insert(MT_WALL, SPolygon(points));
	points.clear();

	points.push_back(D3DXVECTOR3(635, 210, 0));
	points.push_back(D3DXVECTOR3(635, 390, 0));
	points.push_back(D3DXVECTOR3(710, 390, 0));
	points.push_back(D3DXVECTOR3(710, 210, 0));
	points.push_back(D3DXVECTOR3(635, 210, 0));
	points.push_back(D3DXVECTOR3(635, 150, 0));
	points.push_back(D3DXVECTOR3(740, 150, 0));
	points.push_back(D3DXVECTOR3(740, 450, 0));
	points.push_back(D3DXVECTOR3(605, 450, 0));
	points.push_back(D3DXVECTOR3(605, 150, 0));
	points.push_back(D3DXVECTOR3(635, 150, 0));
	maps.insert(MT_WALL, SPolygon(points));
	points.clear();

	Light(RL_EAR_PHONE);
}

CLogo::~CLogo() 
{
	for (ElementsIt it(elements.begin()); it != elements.end(); ++it) 
		SAFE_DELETE(*it);
	elements.clear();
	ClearCircles();
}

void CLogo::ClearCircles() {
	for (EchoesIt it(echoes.begin()); it != echoes.end(); ++it)
		SAFE_DELETE(*it);
	echoes.clear();
}

void CLogo::CreateCircle(const D3DXVECTOR3& pos, FLOAT intensity, const SMap *map, 
		FLOAT lastTime, FLOAT length, UINT index, INT max, DWORD color, BOOL isPlayer) 
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
			echoes.push_back(new CLine(pos, dir, 
				intensity, map, intensity <= 2.f/9, lastTime, length, color, index));
		}
	else 
		for (INT i(0); i < cnt; ++i) {
			D3DXVec3TransformNormal(&dir, &dir, &trans);
			echoes.push_back(new CLine(pos, dir, 
				intensity, map, intensity <= 2.f/9, lastTime, length, color, index));
		}
}

void CLogo::Fire() {
	FLOAT intensity = .4f, lastTime = 20000.f, length = 1000.f;
	CAudioManager::GetInstance()->ChannelPlay(AL_CLAP_LOUD);
	CreateCircle(D3DXVECTOR3(65,  180, 0), intensity, &maps, lastTime, length, 0);
	CreateCircle(D3DXVECTOR3(170, 420, 0), intensity, &maps, lastTime, length, 0);
	CreateCircle(D3DXVECTOR3(117, 300, 0), intensity, &maps, lastTime, length, 0);

	CreateCircle(D3DXVECTOR3(250, 180, 0), intensity, &maps, lastTime, length, 1);
	CreateCircle(D3DXVECTOR3(355, 420, 0), intensity, &maps, lastTime, length, 1);
	CreateCircle(D3DXVECTOR3(250, 300, 0), intensity, &maps, lastTime, length, 1);

	CreateCircle(D3DXVECTOR3(435, 300, 0), intensity, &maps, lastTime, length, 2);
	CreateCircle(D3DXVECTOR3(540, 300, 0), intensity, &maps, lastTime, length, 2);
	CreateCircle(D3DXVECTOR3(487, 300, 0), intensity, &maps, lastTime, length, 2);

	CreateCircle(D3DXVECTOR3(620, 180, 0), intensity, &maps, lastTime, length, 3);
	CreateCircle(D3DXVECTOR3(725, 420, 0), intensity, &maps, lastTime, length, 3);
	CreateCircle(D3DXVECTOR3(620, 300, 0), intensity, &maps, lastTime, length, 3);
}

void CLogo::Light(UINT texture, FLOAT lastTime) {
	elements.push_back(new CElement(texture, &maps, g_vScreenCenter, g_vDefaultDir, lastTime));
}

BOOL CLogo::Tick(DWORD fElapsedTime, BOOL skip)
{
	m_lText += fElapsedTime;
	m_lElapsedTime += fElapsedTime;
	if (!m_bEntering && m_lText > 2500) {
		Light(RL_START);
		m_lText = 0;
		m_bEnableSkip = TRUE;
	}
	if (skip && m_bEnableSkip) m_bEntering = TRUE;
	if (echoes.empty()) {
		if (m_bEntering) return TRUE;
		else if (m_lElapsedTime > 10000) {
			Fire();
			m_lElapsedTime = 0;
		}
	} else if (echoes[0]->IsGone()) {
		ClearCircles();
	}

	for (UINT i(0); i < echoes.size(); ++i) 
		echoes[i]->Tick(fElapsedTime);
	for (ElementsIt it(elements.begin()); it != elements.end();) {
		(*it)->Tick(fElapsedTime);
		if ((*it)->IsGone()) {
			SAFE_DELETE(*it);
			it = elements.erase(it);
			continue;
		}
		++it;
	}
	//return skip;	// Toggle comment here to enable skip anytime
	return FALSE;
}

void CLogo::Render(BOOL highQuality)
{
	for (EchoesIt it(echoes.begin()); it != echoes.end(); ++it) 
		(*it)->Render(g_vScreenOrigin, highQuality);
	for (ElementsIt it(elements.begin()); it != elements.end(); ++it) 
		(*it)->Render();
}