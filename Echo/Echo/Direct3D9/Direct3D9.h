#pragma once
#include <vector>

class CDirect3D9
{
private:
	CDirect3D9();
	~CDirect3D9();

public:
	static CDirect3D9* GetInstance() { return &s_Direct3D9; }

public:
	HRESULT Initialize(HWND hWnd, int iWindowsWidth, int iWindowsHeight, std::ifstream fileList);
	void PreRender();
	void PostRender();
	void DrawText(CHAR *strText, LPRECT rect = NULL, D3DCOLOR color = 0xFFFFFFFF);
	inline ID3DXSprite* GetSprite() { return m_pSprite; }
	inline LPDIRECT3DTEXTURE9 GetTexture(UINT index) { 
		if (index >= m_vpText.size()) index = m_vpText.size()-1;
		return m_vpText[index]; }
	inline D3DSURFACE_DESC& GetTextureSize(UINT index) { 
		if (index >= m_sdSize.size()) index = m_sdSize.size()-1;
		return m_sdSize[index]; }
private:
	HRESULT LoadBackupResource(IDirect3DTexture9* text, D3DSURFACE_DESC& surfDesc);
	IDirect3D9* m_pDirect3D9;					// D3D9对象
	IDirect3DDevice9* m_pD3D9Device;			// D3D9设备

	ID3DXSprite* m_pSprite;
	ID3DXFont* m_pFont;
	std::vector<LPDIRECT3DTEXTURE9> m_vpText;
	std::vector<D3DSURFACE_DESC> m_sdSize;

	D3DPRESENT_PARAMETERS m_D3DPresentParam;	// D3D9初始化参数

	RECT m_rWnd;
	//D3DXMATRIX m_mTra;

private:
	static CDirect3D9 s_Direct3D9;
};