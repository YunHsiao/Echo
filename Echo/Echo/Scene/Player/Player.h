#pragma once
#include "Map.h"
#include "Direct3D9.h"

enum PLAYER_STATUS {
	PS_DEAD,
	PS_ALIVE,
	PS_STANDING,
	PS_SNEAKING,
	PS_WALKING,
	PS_RUNNING
};

struct PLAYER_PARAMETER {
	FLOAT fSneakSpeed, fWalkSpeed, fRunSpeed;
	INT nSneakInterval, nWalkInterval, nRunInterval;
	FLOAT fShoulderWidth, fSwimmingDelay, fFootSize, fFootScale;
	PLAYER_PARAMETER(FLOAT ss = 0.025f, FLOAT ws = 0.05f, FLOAT rs = 0.08f, 
		INT si = 1000, INT wi = 500, INT ri = 333, FLOAT sw = 5.f, 
		FLOAT sd = .588f/*10/17*/, FLOAT fs = .7f) 
		:fSneakSpeed(ss), fWalkSpeed(ws), fRunSpeed(rs)
		,nSneakInterval(si), nWalkInterval(wi), nRunInterval(ri)
		,fShoulderWidth(sw), fSwimmingDelay(sd), fFootScale(fs)
	{
		fFootSize = CDirect3D9::GetInstance()->
			GetTextureSize(RL_LEFT_FOOTPRINT).Height / 2.f * fFootScale;
		fFootSize = sqrt(pow(fFootSize, 2) + pow(fShoulderWidth, 2));
	}
};

#define SNEAKING_ECHO 2.f/9
#define WALKING_ECHO 1.f/3

class CPlayer 
{
public:
	CPlayer(D3DXVECTOR3&, D3DXVECTOR3&, PLAYER_PARAMETER, const SMap*);
	~CPlayer();
	void Tick(DWORD fElapsedTime);
	BOOL IsFinished() { return m_status < PS_STANDING; }
	void Stamp(FLOAT intensity);
	void InitFootprints();
	void Die(BOOL IsTrap);
	void CreateLine(D3DXVECTOR3& pos, D3DXVECTOR3& dir);
	inline PLAYER_STATUS *GetStatus() { return &m_status; }
	inline void SetStatus(PLAYER_STATUS status) { m_status = status; }
	inline D3DXVECTOR3 *GetScreenCoord() { return &m_vPos; }
	inline void SetDirection(D3DXVECTOR3& dir) 
	{ m_vDir = dir; D3DXVec3Normalize(&m_vDir, &m_vDir); }

private:
	void CreateFootprint();
	void HandleSceneEvent();
	inline BOOL IsWater(UINT sound) 
	{ return sound > AL_WATER_RUNNING && sound < AL_WATER_SPLASH_EXIT; }
	PLAYER_STATUS m_status;
	PLAYER_PARAMETER m_param;
	const SMap *m_pMap;
	BOOL m_bLeft;
	UINT m_cSound, m_pSound;
	D3DXVECTOR3 m_vPos, m_vDir;
	FLOAT m_fElapsedTime;

};