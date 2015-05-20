#pragma once
#include "Player.h"
#include "Map.h"

enum MONSTER_STATUS {
	MS_HIBERNATING,
	MS_TRACKING
};

struct MONSTER_PARAMETER {
	FLOAT fTrackingSpeed, fBodyRadius, fSniffingRadiusSq;
	MONSTER_PARAMETER(FLOAT ts = 0.04f, FLOAT br = 10.f, FLOAT sr = 900.f)
		:fTrackingSpeed(ts), fBodyRadius(br), fSniffingRadiusSq(sr)
	{
	}
};

class CMonster {
public:
	CMonster(D3DXVECTOR3&, MONSTER_PARAMETER, const SMap *map,
		const PLAYER_STATUS *ps, const D3DXVECTOR3 *pos);
	~CMonster();
	BOOL Tick(DWORD fElapsedTime, BOOL wake);
	BOOL IsAwakened() { return m_status == MS_TRACKING; }
	inline void SetStatus(MONSTER_STATUS status) { m_status = status; }
	inline D3DXVECTOR3& GetCoord() { return m_vPos; }
	inline void SetDirection(D3DXVECTOR3& dir) 
	{ m_vDir = dir; D3DXVec3Normalize(&m_vDir, &m_vDir); }


private:
	void Emerge();
	const PLAYER_STATUS *m_pStatus;
	const D3DXVECTOR3 *m_pPos;
	const SMap *m_pMap;
	MONSTER_STATUS m_status;
	MONSTER_PARAMETER m_param;
	D3DXVECTOR3 m_vPos, m_vDir;
	DWORD m_lElapsedTime, m_lLost;
};