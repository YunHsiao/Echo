#pragma once

class CPoint {

public:
	CPoint::CPoint(const D3DXVECTOR3& pos, const UINT& color)
		:m_vPos(pos)
		,m_nColor(color)
	{
	}

	inline D3DXVECTOR3& GetPos() { return m_vPos; }
	inline UINT GetCol() { return m_nColor; }

private:
	D3DXVECTOR3 m_vPos;
	UINT m_nColor;
};
