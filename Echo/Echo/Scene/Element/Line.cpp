#include "stdafx.h"
#include "Line.h"
#include "Direct3D9.h"

CLine::CLine(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir, FLOAT intensity, 
		const SMap *map, BOOL quiet, FLOAT lastTime, FLOAT length, DWORD color, UINT index)
:CElement(RL_ECHO, map, pos, dir, lastTime, intensity, 1.f/32, .2f, FALSE, color)
,m_bQuiet(quiet)
,m_fLength(1/length)
,m_bFaded(FALSE)
,m_mapIndex(index)
{
	D3DSURFACE_DESC sd = CDirect3D9::GetInstance()->GetTextureSize(RL_ECHO);
	m_rHead.top = 120; m_rHead.bottom = sd.Height;
	m_rHead.left = 0; m_rHead.right = sd.Width;
	m_rTail.top = 0; m_rTail.bottom = 119;
	m_rTail.left = 0; m_rTail.right = sd.Width;
	m_vSize.y *= 0.5f;
}

CLine::~CLine() {

}

void CLine::DetectCollision() {
	if (!m_pMap) return;
	/// Model 2: Turn back if throughed.
	/// Cons: Some will break through edges.
	m_bTurned = m_pMap->GetArea(MT_WALL, m_mapIndex)->
		ReflectCollision(m_vPos, m_vDir, m_bTurned);
	if (m_pEcho.empty()) {
		m_pEcho.push_back(CPoint(m_vPos, m_pMap->GetRenderEffect(m_vPos)));
	} else {
		if (m_bQuiet) {
			D3DXVECTOR3 *pre = &m_pEcho.back().GetPos();
			D3DXVECTOR3 lerp = m_vPos - *pre;
			FLOAT step = 1.f / D3DXVec3Length(&lerp);
			for (FLOAT s(0); s < 1.f; s += step) {
				D3DXVec3Lerp(&lerp, pre, &m_vPos, s);
				m_pEcho.push_back(CPoint(lerp, m_pMap->GetRenderEffect(lerp)));
			}
		} else {
			D3DXVECTOR3 *pre = &m_pEcho.front().GetPos();
			D3DXVECTOR3 lerp = m_vPos - *pre;
			FLOAT step = 1.f / D3DXVec3Length(&lerp);
			for (FLOAT s(0); s < 1.f; s += step) {
				D3DXVec3Lerp(&lerp, pre, &m_vPos, s);
				m_pEcho.push_front(CPoint(lerp, m_pMap->GetRenderEffect(lerp)));
			}
		}
	}

	/// Model 1: Roll back to previous if throughed.
	///	Cons: Some will stay still on egdes and flash.
	//if (map->GetArea(MT_WALL, index)->ReflectCollision(m_vPos, m_vDir)) {
	//	if (m_bQuiet) m_pEcho.push_back(m_vPos);
	//	else m_pEcho.push_front(m_vPos);
	//} else {
	//	if (m_bQuiet) {
	//		m_pEcho.pop_back();
	//		if (!m_pEcho.empty()) m_vPos = m_pEcho.back();
	//	} else { 
	//		m_pEcho.pop_front();
	//		if (!m_pEcho.empty()) m_vPos = m_pEcho.front();
	//	}
	//}

	/// Model 0: Judging from intersection point.
	/// Cons: Only tested in early version, unstable.
	//if (map->GetArea(MT_WALL, index)->Validate(m_vPos)) return;
	//for (EdgesCIt it(map->GetArea(MT_WALL, index)->edges.begin()); 
	//	it != map->GetArea(MT_WALL, index)->edges.end(); ++it) {
	//	if (it->Intersected(m_vPre, m_vPos)) {
	//		if (it->AimingInside(m_vDir)) return;
	//		FLOAT dis(fabsf(it->GetDistance(m_vPos)));
	//		//m_vPre = m_vPos;
	//		m_vPos += it->normal*dis*(it->inside?2.f:-2.f);
	//		m_vDir = it->GetReflectDirection(m_vDir);
	//		return;
	//	}
	//}
}

void CLine::Tick(DWORD fElapsedTime) {
	if (IsGone()) return;
	if (m_nLastTime) m_fIntensity += fElapsedTime/m_nLastTime*(m_bFadeIn?1.f:-1.f);
	if (m_bFaded) return;
	m_vPos += m_vDir*(fElapsedTime*m_fSpeed);
	DetectCollision();
	Update();
}

ID3DXSprite* CLine::Render(const D3DXVECTOR3& pos, BOOL highQuality) {
	if (IsGone()) return NULL;
	ID3DXSprite* sprite = CDirect3D9::GetInstance()->GetSprite();
	m_pRect = &m_rTail;
	FLOAT intensity(m_fIntensity);
	for (EchoPointsIt it(m_pEcho.begin()); it != m_pEcho.end(); ++it) {
		RenderPoint(sprite, it, pos, intensity);
		intensity -= m_fLength;
		if (intensity <= 0.f) { 
			//m_pEcho.erase(it+1, m_pEcho.end());
			if (m_bQuiet) m_bFaded = TRUE; 
			return sprite; 
		}
	}
	if (m_pEcho.empty()) return sprite;
	m_pRect = &m_rHead;
	if (m_bQuiet) RenderPoint(sprite, m_pEcho.end()-1, pos, intensity);
	else RenderPoint(sprite, m_pEcho.begin(), pos, m_fIntensity);
	return sprite;
}

void CLine::RenderPoint(ID3DXSprite *sprite, EchoPointsIt it, 
						const D3DXVECTOR3& pos, const FLOAT& intensity)
{
	if (it->GetCol() == MT_EXIT) {
		D3DXMatrixScaling(&m_mSca, 1.f/16, 1.f/16, 1.f);
	} else {
		D3DXMatrixScaling(&m_mSca, 1.f/32, 1.f/32, 1.f);
	}
	m_mWor = m_mRot * m_mSca * m_mTra;
	m_mWor._41 = it->GetPos().x - pos.x;
	m_mWor._42 = it->GetPos().y - pos.y;
	sprite->SetTransform(&m_mWor);
	if (m_color != 0xFFFFFF) {
		sprite->Draw(m_pText, m_pRect, &m_vSize, NULL, 
			((((DWORD)((intensity)*255.f))&0xff)<<24)|m_color);
		return;
	}
	switch (it->GetCol()) {
	case MT_EXIT:
		sprite->Draw(m_pText, m_pRect, &m_vSize, NULL, 
			((((DWORD)((intensity)*255.f))&0xff)<<24)|m_color);
		//intensity<.8f?intensity+.2f:1.f));
		break;
	case MT_TRAP:
		sprite->Draw(m_pText, m_pRect, &m_vSize, NULL, 
			((((DWORD)((intensity)*255.f))&0xff)<<24)|(m_color&0xFF0000));
		break;
	case MT_WATER:
		sprite->Draw(m_pText, m_pRect, &m_vSize, NULL, 
			((((DWORD)((intensity)*255.f))&0xff)<<24)|(m_color&0x0000FF));
		break;
	case MT_TRIGGER:
		sprite->Draw(m_pText, m_pRect, &m_vSize, NULL, 
			((((DWORD)((intensity)*255.f))&0xff)<<24)|(m_color&0xFFFF00));
		break;
	default:
		sprite->Draw(m_pText, m_pRect, &m_vSize, NULL, 
			((((DWORD)((intensity)*255.f))&0xff)<<24)|m_color);
		break;
	}
}
