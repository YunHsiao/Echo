// MaskToolDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TG2DMapEditor.h"
#include "ToolBrushDialog.h"

// CMaskToolDialog 对话框

IMPLEMENT_DYNAMIC(CToolBrushDialog, CDialog)

CToolBrushDialog::CToolBrushDialog(CWnd* pParent /*=NULL*/)
: CDialog(CToolBrushDialog::IDD, pParent)
, m_nBrushMaxNum  (0)
, m_nNpcMaxNum    (0)
, m_nMonsterMaxNum(0)
, m_nCollMaxNum   (0)
, m_nTrigMaxNum   (0)
, m_dwMessage     (WM_BRUSH)
{
}

CToolBrushDialog::~CToolBrushDialog()
{
}

void CToolBrushDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1,  m_Button[0]);
	DDX_Control(pDX, IDC_BUTTON2,  m_Button[1]);
	DDX_Control(pDX, IDC_BUTTON3,  m_Button[2]);
	DDX_Control(pDX, IDC_BUTTON4,  m_Button[3]);
	DDX_Control(pDX, IDC_BUTTON5,  m_Button[4]);
	DDX_Control(pDX, IDC_BUTTON6,  m_Button[5]);
	DDX_Control(pDX, IDC_BUTTON7,  m_Button[6]);
	DDX_Control(pDX, IDC_BUTTON8,  m_Button[7]);
	DDX_Control(pDX, IDC_BUTTON9,  m_Button[8]);
	DDX_Control(pDX, IDC_BUTTON10, m_Button[9]);
	DDX_Control(pDX, IDC_BUTTON11, m_Button[10]);
	DDX_Control(pDX, IDC_BUTTON12, m_Button[11]);
	DDX_Control(pDX, IDC_BUTTON13, m_Button[12]);
	DDX_Control(pDX, IDC_BUTTON14, m_Button[13]);
	DDX_Control(pDX, IDC_BUTTON15, m_Button[14]);
	DDX_Control(pDX, IDC_BUTTON16, m_Button[15]);
	
	DDX_Control(pDX, IDC_TAB, m_Tab);
}


BEGIN_MESSAGE_MAP(CToolBrushDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1,  &CToolBrushDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2,  &CToolBrushDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3,  &CToolBrushDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4,  &CToolBrushDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5,  &CToolBrushDialog::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6,  &CToolBrushDialog::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7,  &CToolBrushDialog::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8,  &CToolBrushDialog::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9,  &CToolBrushDialog::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CToolBrushDialog::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CToolBrushDialog::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CToolBrushDialog::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CToolBrushDialog::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CToolBrushDialog::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CToolBrushDialog::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &CToolBrushDialog::OnBnClickedButton16)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CToolBrushDialog::OnTcnSelchangeMasktab)
	ON_LBN_SELCHANGE(IDC_BRUSHLIST, &CToolBrushDialog::OnLbnSelchangeBrushlist)
END_MESSAGE_MAP()



BOOL CToolBrushDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 获取自身的宽高
	GetWindowRect(&m_rWndRect);
	m_rWndRect.right  -= m_rWndRect.left;
	m_rWndRect.bottom -= m_rWndRect.top;

	// 添加LIST控件项
	CListBox* list = (CListBox*)GetDlgItem(IDC_BRUSHLIST);
	for(int i(0);i < 10;++i)
	{
		CString str = _T("");
		str.Format(L"%d",i);
		list->InsertString(i,str.GetBuffer());
	}

	tinyxml2::XMLDocument File;
	if(File.LoadFile("Res/ButtonStr.xml"))
	{
		AfxMessageBox(L"Res/ButtonStr.xml 文件加载失败!");
		return FALSE;
	}

	tinyxml2::XMLElement* tRoot = File.FirstChildElement();
	tinyxml2::XMLElement* tName = tRoot->FirstChildElement();
	int tabNum = atoi(tName->FirstChild()->Value());

	// 初始化标签
	for(int i(0);i<tabNum;++i)
	{
		tName = tName->NextSiblingElement();
		WCHAR str[MAX_PATH] = {0};
		LPCSTR buff = tName->Attribute("str");
		MultiByteToWideChar(CP_ACP,0,buff,-1,str,(int)strlen(buff)+1);
		m_Tab.InsertItem(i,str);
	}

	// 字符串的初始化
	// 画刷层字符串
	tinyxml2::XMLElement* tTemp = tName->NextSiblingElement();
	m_nBrushMaxNum = atoi(tTemp->FirstChild()->Value());

	for(int i(0);i < m_nBrushMaxNum;++i)
	{
		tTemp = tTemp->NextSiblingElement();
		WCHAR str[MAX_PATH] = {0};
		LPCSTR buff = tTemp->Attribute("str");
		MultiByteToWideChar(CP_ACP,0,buff,-1,str,(int)strlen(buff)+1);
		m_pStrBrush[i] = str;
	}

	// NPC层字符串
	tTemp = tTemp->NextSiblingElement();
	m_nNpcMaxNum = atoi(tTemp->FirstChild()->Value());

	for(int i(0);i < m_nNpcMaxNum;++i)
	{
		tTemp = tTemp->NextSiblingElement();
		WCHAR str[MAX_PATH] = {0};
		LPCSTR buff = tTemp->Attribute("str");
		MultiByteToWideChar(CP_ACP,0,buff,-1,str,(int)strlen(buff)+1);
		m_pStrNpc[i] = str;
	}

	// 怪物层字符串
	tTemp = tTemp->NextSiblingElement();
	m_nMonsterMaxNum = atoi(tTemp->FirstChild()->Value());

	for(int i(0);i < m_nMonsterMaxNum;++i)
	{
		tTemp = tTemp->NextSiblingElement();
		WCHAR str[MAX_PATH] = {0};
		LPCSTR buff = tTemp->Attribute("str");
		MultiByteToWideChar(CP_ACP,0,buff,-1,str,(int)strlen(buff)+1);
		m_pStrMonster[i] = str;
	}

	// 碰撞层字符串
	tTemp = tTemp->NextSiblingElement();
	m_nCollMaxNum = atoi(tTemp->FirstChild()->Value());

	for(int i(0);i < m_nCollMaxNum;++i)
	{
		tTemp = tTemp->NextSiblingElement();
		WCHAR str[MAX_PATH] = {0};
		LPCSTR buff = tTemp->Attribute("str");
		MultiByteToWideChar(CP_ACP,0,buff,-1,str,(int)strlen(buff)+1);
		m_pStrColl[i] = str;
	}

	// 触发层字符串
	tTemp = tTemp->NextSiblingElement();
	m_nTrigMaxNum = atoi(tTemp->FirstChild()->Value());

	for(int i(0);i < m_nTrigMaxNum;++i)
	{
		tTemp = tTemp->NextSiblingElement();
		WCHAR str[MAX_PATH] = {0};
		LPCSTR buff = tTemp->Attribute("str");
		MultiByteToWideChar(CP_ACP,0,buff,-1,str,(int)strlen(buff)+1);
		m_pStrTrig[i] = str;
	}

	// 设置默认层
	ProcBrushState();

	ShowWindow(SW_SHOW);
	UpdateWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// CMaskToolDialog 消息处理程序

// 过滤掉系统关闭按键	
BOOL CToolBrushDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;

	if(::GetAsyncKeyState(VK_LMENU) & ::GetAsyncKeyState(VK_F4))
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}

void CToolBrushDialog::OnBnClickedButton1()
{
	static BOOL b(TRUE);
	m_Button[0].SetState(b);
	b = !b;

	// 消息,第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,0);
}

void CToolBrushDialog::OnBnClickedButton2()
{
	static BOOL b(TRUE);
	m_Button[1].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,1);
}
void CToolBrushDialog::OnBnClickedButton3()
{
	static BOOL b(TRUE);
	m_Button[2].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,2);
}

void CToolBrushDialog::OnBnClickedButton4()
{
	static BOOL b(TRUE);
	m_Button[3].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,3);
}

void CToolBrushDialog::OnBnClickedButton5()
{
	static BOOL b(TRUE);
	m_Button[4].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,4);
}

void CToolBrushDialog::OnBnClickedButton6()
{
	static BOOL b(TRUE);
	m_Button[5].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,5);
}

void CToolBrushDialog::OnBnClickedButton7()
{
	static BOOL b(TRUE);
	m_Button[6].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,6);
}

void CToolBrushDialog::OnBnClickedButton8()
{
	static BOOL b(TRUE);
	m_Button[7].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,7);
}

void CToolBrushDialog::OnBnClickedButton9()
{
	static BOOL b(TRUE);
	m_Button[8].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,8);
}

void CToolBrushDialog::OnBnClickedButton10()
{
	static BOOL b(TRUE);
	m_Button[9].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,9);
}

void CToolBrushDialog::OnBnClickedButton11()
{
	static BOOL b(TRUE);
	m_Button[10].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,10);
}

void CToolBrushDialog::OnBnClickedButton12()
{
	static BOOL b(TRUE);
	m_Button[11].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,11);
}

void CToolBrushDialog::OnBnClickedButton13()
{
	static BOOL b(TRUE);
	m_Button[12].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,12);
}

void CToolBrushDialog::OnBnClickedButton14()
{
	static BOOL b(TRUE);
	m_Button[13].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,13);
}

void CToolBrushDialog::OnBnClickedButton15()
{
	static BOOL b(TRUE);
	m_Button[14].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,14);
}

void CToolBrushDialog::OnBnClickedButton16()
{
	static BOOL b(TRUE);
	m_Button[15].SetState(b);
	b = !b;

	// s1,s2 == 消息 n == 第几个按钮
	BUTTON_POSTMESSAGE(m_dwMessage,15);
}

void CToolBrushDialog::OnTcnSelchangeMasktab(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch(m_Tab.GetCurSel())
	{
	case 0: // 画刷		
		{
			ProcBrushState();
			CWnd* cView = AfxGetMainWnd()->GetWindow(GW_CHILD); 
			::PostMessage(cView->m_hWnd,m_dwMessage,MODIFY,0);
		}
		break;
	case 1: // NPC
		{
			ProcNpcState();
			CWnd* cView = AfxGetMainWnd()->GetWindow(GW_CHILD); 
			::PostMessage(cView->m_hWnd,m_dwMessage,MODIFY,0);
		}
		break;
	case 2: // 怪物
		{
			ProcMonsterState(); 
			CWnd* cView = AfxGetMainWnd()->GetWindow(GW_CHILD); 
			::PostMessage(cView->m_hWnd,m_dwMessage,MODIFY,0);
		}
		break;
	case 3: // 碰撞
		{
			ProcCollState();
		}
		break;
	case 4: // 触发
		{
			ProcTrigState();
			CWnd* cView = AfxGetMainWnd()->GetWindow(GW_CHILD); 
			::PostMessage(cView->m_hWnd,m_dwMessage,MODIFY,0);
		}
		break;
	}

	*pResult = 0;
}

void CToolBrushDialog::ProcBrushState()
{
	m_dwMessage = WM_BRUSH;
	int n = IDI_ICON_B01;
	for(int i(0);i < TAB_BUTTON_MAXNUM;++i)
	{
		if(n < (IDI_ICON_B01 + m_nBrushMaxNum))
		{
			m_Button[i].SetIcon(::LoadIcon(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(n)));
			m_Button[i].SetWindowText(m_pStrBrush[i].c_str());
			m_Button[i].ShowWindow(SW_SHOW);
			++n;
		}
		else
		{
			m_Button[i].ShowWindow(SW_HIDE);
		}
	}
}

void CToolBrushDialog::ProcNpcState()
{
	m_dwMessage = WM_NPC;
	int n = IDI_ICON_NPC1;
	for(int i(0);i < TAB_BUTTON_MAXNUM;++i)
	{
		if(n < (IDI_ICON_NPC1 + m_nNpcMaxNum))
		{
			m_Button[i].SetIcon(::LoadIcon(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(n)));
			m_Button[i].SetWindowText(m_pStrNpc[i].c_str());
			m_Button[i].ShowWindow(SW_SHOW);
			++n;
		}
		else
		{
			m_Button[i].ShowWindow(SW_HIDE);
		}
	}
}

void CToolBrushDialog::ProcMonsterState()
{
	m_dwMessage = WM_MONSTER;
	int n = IDI_ICON_MON1;
	for(int i(0);i < TAB_BUTTON_MAXNUM;++i)
	{
		if(n < (IDI_ICON_MON1 + m_nMonsterMaxNum))
		{
			m_Button[i].SetIcon(::LoadIcon(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(n)));
			m_Button[i].SetWindowText(m_pStrMonster[i].c_str());
			m_Button[i].ShowWindow(SW_SHOW);
			++n;
		}
		else
		{
			m_Button[i].ShowWindow(SW_HIDE);
		}
	}
}

void CToolBrushDialog::ProcCollState()
{
	m_dwMessage = WM_COLL;
	int n = IDI_ICONCOLL;
	for(int i(0);i < TAB_BUTTON_MAXNUM;++i)
	{
		if(n < (IDI_ICONCOLL + m_nCollMaxNum))
		{
			m_Button[i].SetIcon(::LoadIcon(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(n)));
			m_Button[i].SetWindowText(m_pStrColl[i].c_str());
			m_Button[i].ShowWindow(SW_SHOW);
			++n;
		}
		else
		{
			m_Button[i].ShowWindow(SW_HIDE);
		}
	}
}

void CToolBrushDialog::ProcTrigState()
{
	m_dwMessage = WM_TRIG;
	int n = IDI_ICONTRIG;
	for(int i(0);i < TAB_BUTTON_MAXNUM;++i)
	{
		if(n < (IDI_ICONTRIG + m_nTrigMaxNum))
		{
			m_Button[i].SetIcon(::LoadIcon(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(n)));
			m_Button[i].SetWindowText(m_pStrTrig[i].c_str());
			m_Button[i].ShowWindow(SW_SHOW);
			++n;
		}
		else
		{
			m_Button[i].ShowWindow(SW_HIDE);
		}
	}
}

void CToolBrushDialog::OnLbnSelchangeBrushlist()
{
	CListBox* list = (CListBox*)GetDlgItem(IDC_BRUSHLIST);
	int num = list->GetCurSel();

	CWnd* cView = AfxGetMainWnd()->GetWindow(GW_CHILD);
	::PostMessage(cView->m_hWnd,WM_TRIGMAPID,num,num);
}
