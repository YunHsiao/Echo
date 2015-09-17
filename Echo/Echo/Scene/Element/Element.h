#pragma once
#include "Map.h"

extern D3DXVECTOR3 g_vScreenCenter;
extern D3DXVECTOR3 g_vScreenOrigin;
extern D3DXVECTOR3 g_vDefaultDir;

class CElement {

public:
	CElement(UINT texture, const SMap *map, const D3DXVECTOR3& pos = g_vScreenCenter, 
		const D3DXVECTOR3& dir = g_vDefaultDir, FLOAT lastTime = 5000.f, 
		FLOAT intensity = 1.f, FLOAT scale = 1.f, FLOAT speed = 0.f, 
		BOOL fadeIn = FALSE, DWORD color = 0xFFFFFF, BOOL ui = FALSE);
	virtual ~CElement();
	virtual void Tick(DWORD fElapsedTime);
	virtual ID3DXSprite* Render(const D3DXVECTOR3& pos = g_vScreenOrigin);
	inline FLOAT GetIntensity() { return m_fIntensity; }
	inline BOOL IsGone() { return m_fIntensity < 0.f || m_fIntensity > 1.f; }
	inline D3DXVECTOR3& GetCoord() { return m_vPos; }

protected:
	void Update();
	const SMap *m_pMap;
	IDirect3DTexture9 *m_pText;
	D3DXMATRIX m_mRot, m_mSca, m_mTra, m_mWor;
	D3DXVECTOR3 m_vPos, m_vDir, m_vSize;
	LPRECT m_pRect;
	FLOAT m_fIntensity, m_fSpeed, m_nLastTime;
	BOOL m_bFadeIn, m_bUI;
	DWORD m_color;
};