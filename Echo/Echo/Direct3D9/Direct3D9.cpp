#include "stdafx.h"
#include "Direct3D9.h"

CDirect3D9 CDirect3D9::s_Direct3D9;

CDirect3D9::CDirect3D9():
m_pDirect3D9(NULL),
m_pD3D9Device(NULL),
m_pSprite(NULL)
{
	ZeroMemory(&m_D3DPresentParam, sizeof(D3DPRESENT_PARAMETERS));
	m_rWnd.left = 0;
	m_rWnd.top = 0;
	//D3DXMatrixIdentity(&m_mTra);
}

CDirect3D9::~CDirect3D9()
{
	for (UINT i(0); i < m_vpText.size(); ++i)
		SAFE_RELEASE(m_vpText[i])
	m_vpText.clear();
	SAFE_RELEASE(m_pSprite)
	SAFE_RELEASE(m_pFont)
	SAFE_RELEASE(m_pD3D9Device)
	SAFE_RELEASE(m_pDirect3D9)
}

HRESULT CDirect3D9::Initialize(HWND hWnd, int iWindowsWidth, int iWindowsHeight, std::ifstream fileList)
{
	// 创建D3D对象
	m_pDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == m_pDirect3D9)
		return E_FAIL;

	m_rWnd.right = iWindowsWidth;
	m_rWnd.bottom = iWindowsHeight;

	D3DDISPLAYMODE d3ddm;
	m_pDirect3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	// 初始化参数
	m_D3DPresentParam.BackBufferWidth            = iWindowsWidth;					// 后台缓冲区的宽度
	m_D3DPresentParam.BackBufferHeight           = iWindowsHeight;					// 后台缓冲区的高度
	m_D3DPresentParam.BackBufferFormat           = d3ddm.Format;					// 后台缓冲区的像素格式
	m_D3DPresentParam.BackBufferCount            = 1;								// 后台缓冲区的数量
	m_D3DPresentParam.MultiSampleType            = D3DMULTISAMPLE_NONE;				// 多重采样的类型
	m_D3DPresentParam.MultiSampleQuality         = 0;								// 多重采样的质量
	m_D3DPresentParam.SwapEffect                 = D3DSWAPEFFECT_DISCARD;			// 后台缓冲区的页面置换方式
	m_D3DPresentParam.hDeviceWindow              = hWnd;							// 窗口句柄
	m_D3DPresentParam.Windowed                   = TRUE;							// 窗口还是全屏
	m_D3DPresentParam.EnableAutoDepthStencil     = TRUE;							// 深度与模板缓存
	m_D3DPresentParam.AutoDepthStencilFormat     = D3DFMT_D24S8;					// 深度缓存与模板缓存的像素格式
	m_D3DPresentParam.Flags                      = 0;								// 附加的特性
	m_D3DPresentParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// 刷新频率
	m_D3DPresentParam.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// 刷新时间间隔(垂直同步)

	HRESULT hr;
	IDirect3DTexture9* text(NULL);
	D3DSURFACE_DESC surfDesc;
	if (FAILED(hr = m_pDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_D3DPresentParam, &m_pD3D9Device)))
		return hr;

	if (FAILED(hr = D3DXCreateSprite(m_pD3D9Device, &m_pSprite))) 
		return hr;

	if (FAILED(hr = D3DXCreateFont(m_pD3D9Device, 20, 0, FW_DONTCARE, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft JhengHei", &m_pFont)))
		if (FAILED(hr = D3DXCreateFont(m_pD3D9Device, 20, 0, FW_DONTCARE, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"微软雅黑", &m_pFont)))
			hr = D3DXCreateFont(m_pD3D9Device, 20, 0, FW_DONTCARE, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"宋体", &m_pFont);

	std::string fileName("");
	if (!getline(fileList, fileName)) return LoadBackupResource(text, surfDesc);
	do {
		if (fileName.empty()) continue;
		if (FAILED(hr = D3DXCreateTextureFromFileExA(m_pD3D9Device, fileName.c_str(),
			D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 
			D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
			D3DX_FILTER_LINEAR, D3DX_FILTER_NONE,
			D3DCOLOR_ARGB(255, 255, 0, 255),
			NULL, NULL,
			&text))) {
				hr = LoadBackupResource(text, surfDesc);
				break;
		}			
		text->GetLevelDesc(0, &surfDesc);
		m_sdSize.push_back(surfDesc);
		m_vpText.push_back(text);
	} while (getline(fileList, fileName));
	
	return hr;
}

HRESULT CDirect3D9::LoadBackupResource(IDirect3DTexture9* text, D3DSURFACE_DESC& surfDesc) {
	HRESULT hr;
	for (UINT i(0); i < m_vpText.size(); ++i)
		SAFE_RELEASE(m_vpText[i])
	m_vpText.clear();
	m_sdSize.clear();
	INT resource(0);
	for (INT i(0); i < 20; ++i) {
		if (i == 2) resource = IDB_ECHO;
		else if (i == 3 || i == 5) resource = IDB_404;
		else resource = IDB_PH;
		hr = D3DXCreateTextureFromResourceEx(m_pD3D9Device, 
				GetModuleHandle(NULL), MAKEINTRESOURCE(resource),
				D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 
				D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
				D3DX_FILTER_LINEAR, D3DX_FILTER_NONE,
				D3DCOLOR_ARGB(255, 255, 0, 255),
				NULL, NULL,
				&text);
		text->GetLevelDesc(0, &surfDesc);
		m_sdSize.push_back(surfDesc);
		m_vpText.push_back(text);
	}
	return hr;
}

void CDirect3D9::PreRender()
{
	if (NULL == m_pD3D9Device) return;
	m_pD3D9Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00), 1.0f, 0);
	m_pD3D9Device->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDirect3D9::PostRender() 
{
	m_pSprite->End();
	m_pD3D9Device->EndScene();
	m_pD3D9Device->Present(NULL, NULL, NULL, NULL);
}

void CDirect3D9::DrawText(CHAR *strText, LPRECT rect, D3DCOLOR color)
{
	if (!m_pFont || !strText) return;
	m_pFont->DrawTextA(NULL, strText, -1, rect?rect:&m_rWnd, DT_LEFT, color);
}