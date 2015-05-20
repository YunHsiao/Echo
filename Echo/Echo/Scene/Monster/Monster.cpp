#include "stdafx.h"
#include "Monster.h"
#include "ElementManager.h"
#include "AudioManager.h"

CMonster::CMonster(D3DXVECTOR3& pos, MONSTER_PARAMETER param, const SMap *map,
				   const PLAYER_STATUS *pStatus, const D3DXVECTOR3 *pPos)
:m_vPos(pos)
,m_param(param)
,m_pStatus(pStatus)
,m_pPos(pPos)
,m_pMap(map)
,m_lElapsedTime(0)
,m_lLost(0)
,m_status(MS_HIBERNATING)
{
}

CMonster::~CMonster()
{
}

BOOL CMonster::Tick(DWORD fElapsedTime, BOOL wake) 
{
	m_lElapsedTime += fElapsedTime;
	m_vDir = *m_pPos - m_vPos; 
	BOOL captured(D3DXVec3LengthSq(&m_vDir) < m_param.fSniffingRadiusSq);
	if (wake && *m_pStatus != PS_SNEAKING) { 
		m_status = MS_TRACKING; m_lLost = 0;
	} else if (*m_pStatus == PS_STANDING) {
		m_lLost += fElapsedTime;
		if (m_lLost > 1000) m_status = MS_HIBERNATING;
	}
	switch (m_status) {
	case MS_TRACKING:
		CAudioManager::GetInstance()->ChannelPlay(AL_MONSTER, 0, MAX_VOLUME, FALSE);
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_vPos += m_vDir * (fElapsedTime * m_param.fTrackingSpeed);
		if (m_lElapsedTime > 200) { Emerge(); m_lElapsedTime = 0; }
		break;
	case MS_HIBERNATING:
		CAudioManager::GetInstance()->ChannelStop(AL_MONSTER);
		break;
	}
	return captured;
}

void CMonster::Emerge() {
	CElementManager::GetInstance()->CreateCircle(m_vPos, 1.f, m_pMap, 400.f, 125.f, 5, 0xFF0000);
}