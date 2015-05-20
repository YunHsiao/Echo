#include "stdafx.h"
#include "Element.h"
#include "Direct3D9.h"

CElement::CElement(UINT texture, const SMap *map, const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, 
				   FLOAT lastTime, FLOAT intensity, FLOAT scale, FLOAT speed, 
				   BOOL fadeIn, DWORD color, BOOL ui)
:m_vPos(pos)
,m_pMap(map)
,m_vDir(dir)
,m_fSpeed(speed)
,m_fIntensity(intensity)
,m_nLastTime(lastTime)
,m_pRect(NULL)
,m_bFadeIn(fadeIn)
,m_color(color)
,m_bUI(ui)
{	
	if (m_bFadeIn) m_fIntensity = 1 - m_fIntensity;
	m_pText = CDirect3D9::GetInstance()->GetTexture(texture);
	D3DSURFACE_DESC sdSize = CDirect3D9::GetInstance()->GetTextureSize(texture);
	m_vSize.x = sdSize.Width / 2.f;
	m_vSize.y = sdSize.Height / 2.f;
	m_vSize.z = 0.f;
	D3DXMatrixScaling(&m_mSca, scale, scale, 1.f);
	Update();
}

CElement::~CElement() {
	m_fIntensity = 0;
}

void CElement::Update() {
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	FLOAT angle = acos(D3DXVec3Dot(&m_vDir, &D3DXVECTOR3(0.f, -1.f, 0.f)));
	if (D3DXVec3Dot(&m_vDir, &D3DXVECTOR3(1.f, 0.f, 0.f)) < 0) 
		angle = 2*D3DX_PI-angle;
	D3DXMatrixRotationZ(&m_mRot, angle);
	D3DXMatrixTranslation(&m_mTra, m_vPos.x, m_vPos.y, m_vPos.z);
	m_mWor = m_mRot * m_mSca * m_mTra;
}

void CElement::Tick(DWORD fElapsedTime) {
	if (IsGone()) return;
	if (m_nLastTime) m_fIntensity += fElapsedTime/m_nLastTime*(m_bFadeIn?1.f:-1.f);
	m_vPos += m_vDir*(fElapsedTime*m_fSpeed);
	Update();
}

ID3DXSprite* CElement::Render(const D3DXVECTOR3& pos, BOOL highQuality) {
	if (IsGone()) return NULL;
	ID3DXSprite* sprite = CDirect3D9::GetInstance()->GetSprite();
	if (!m_bUI) {
		m_mWor._41 -= pos.x;
		m_mWor._42 -= pos.y;
	}
	sprite->SetTransform(&m_mWor);
	sprite->Draw(m_pText, m_pRect, &m_vSize, NULL, 
		((((DWORD)((m_fIntensity)*255.f))&0xff)<<24)|m_color);
	return sprite;
}