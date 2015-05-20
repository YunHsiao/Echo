#pragma once
#include "afxwin.h"
#include "afxext.h"
#define   MODIFY   9999 //表示可以编辑当前类的元素。

// CMaskToolDialog 对话框

class CToolBrushDialog : public CDialog
{
	DECLARE_DYNAMIC(CToolBrushDialog)

public:
	CToolBrushDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CToolBrushDialog();

// 对话框数据
	enum { IDD = IDD_MASKDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnTcnSelchangeMasktab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnSelchangeBrushlist();

public:
	virtual BOOL OnInitDialog();

public:
	inline RECT GetWndRect()
	{
		return m_rWndRect;
	}
	void ProcBrushState();
	void ProcNpcState();
	void ProcMonsterState();
	void ProcCollState();
	void ProcTrigState();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CButton      m_Button[TAB_BUTTON_MAXNUM];
	CTabCtrl     m_Tab;
	std::wstring m_pStrBrush[TAB_BUTTON_MAXNUM];    // 元素字符串
	std::wstring m_pStrNpc[TAB_BUTTON_MAXNUM];      // NPC字符串
	std::wstring m_pStrMonster[TAB_BUTTON_MAXNUM];  // 怪物字符串
	std::wstring m_pStrColl[TAB_BUTTON_MAXNUM];     // 碰撞字符串
	std::wstring m_pStrTrig[TAB_BUTTON_MAXNUM];     // 触发字符串
	INT          m_nBrushMaxNum;                    // 元素层的最大按钮数
	INT          m_nNpcMaxNum;                      // NPC层的最大按钮数
	INT          m_nMonsterMaxNum;                  // 怪物层的最大按钮数
	INT          m_nCollMaxNum;                     // 碰撞层的最大按钮数
	INT          m_nTrigMaxNum;                     // 触发层的最大按钮数
	DWORD        m_dwMessage;
	RECT         m_rWndRect;
};