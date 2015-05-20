#include "StdAfx.h"
#include "DrawBrush.h"

CDrawBrush::CDrawBrush(void)
: m_nCurNum        (0)
, m_bIsActive      (FALSE)
, m_pCurWidth      (NULL)
, m_pCurHeight     (NULL)
, m_ppTexture      (NULL)
, m_pSprite        (NULL)
, m_dwColor        (0xFFFFFFFF)
, m_bIsSelect      (FALSE)
, m_fDepthZ        (0.5f)
, m_nTrigMapID     (0)
{
	D3DXMatrixIdentity(&m_matWorld);
}

CDrawBrush::~CDrawBrush(void)
{
	Destroy();
}

void CDrawBrush::Destroy()
{
	// 悬垂指针
	m_ppTexture = NULL;

	SAFE_DELETE_ARRAY(m_pCurWidth);
	SAFE_DELETE_ARRAY(m_pCurHeight);

	SAFE_RELEASE(m_pSprite);
}

HRESULT CDrawBrush::OnCreateByPoint(LPDIRECT3DDEVICE9 pd3dDevice, LPDIRECT3DTEXTURE9* ppTexture, INT num, BOOL bIsActive)
{
	HRESULT hr = S_OK;

	m_nMaxNum = num;

	m_ppTexture = ppTexture;

	m_pCurWidth  = new INT[m_nMaxNum];
	m_pCurHeight = new INT[m_nMaxNum];

	D3DXCreateSprite(pd3dDevice,&m_pSprite);

	for(int i(0);i<m_nMaxNum;++i)
	{
		// 获取图片信息
		m_ppTexture[i]->GetLevelDesc(0, &m_desc);
		m_pCurWidth[i]  = m_desc.Width;
		m_pCurHeight[i] = m_desc.Height;
	}

	// 是否激活
	m_bIsActive = bIsActive;

	return hr;
}

HRESULT CDrawBrush::Render(LPD3DXLINE pLine,POINT& offset,BOOL zDepth)
{
	HRESULT hr = S_OK;

	// 如果没有激活则不绘制
	if(!m_bIsActive)
		return hr;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	if(zDepth)
		m_fDepthZ = 1 - ((m_matWorld._42-offset.y + m_pCurHeight[m_nCurNum])/1000.f); 
	else
		m_fDepthZ = 0.f;

	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw(m_ppTexture[m_nCurNum],0,0,
		&D3DXVECTOR3((FLOAT)-offset.x,(FLOAT)-offset.y,m_fDepthZ), m_dwColor);
	m_pSprite->End();

	if(!pLine)
		return hr;

	float  x = m_matWorld._41 - offset.x;
	float  y = m_matWorld._42 - offset.y;
	float cx = m_matWorld._41 - offset.x + m_pCurWidth[m_nCurNum];
	float cy = m_matWorld._42 - offset.y + m_pCurHeight[m_nCurNum];

	D3DXVECTOR2 vecLine[8] = 
	{ 
		D3DXVECTOR2(x,y),
		D3DXVECTOR2(cx,y),   
		D3DXVECTOR2(cx,y),
		D3DXVECTOR2(cx,cy), 
		D3DXVECTOR2(cx,cy),
		D3DXVECTOR2(x,cy),   
		D3DXVECTOR2(x,cy),
		D3DXVECTOR2(x,y)
	};

	pLine->Draw(vecLine,8,0xFFFF0000); // 红色的线框
	return hr;
}