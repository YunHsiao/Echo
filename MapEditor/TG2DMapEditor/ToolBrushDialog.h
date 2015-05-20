#pragma once
#include "afxwin.h"
#include "afxext.h"
#define   MODIFY   9999 //��ʾ���Ա༭��ǰ���Ԫ�ء�

// CMaskToolDialog �Ի���

class CToolBrushDialog : public CDialog
{
	DECLARE_DYNAMIC(CToolBrushDialog)

public:
	CToolBrushDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CToolBrushDialog();

// �Ի�������
	enum { IDD = IDD_MASKDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	std::wstring m_pStrBrush[TAB_BUTTON_MAXNUM];    // Ԫ���ַ���
	std::wstring m_pStrNpc[TAB_BUTTON_MAXNUM];      // NPC�ַ���
	std::wstring m_pStrMonster[TAB_BUTTON_MAXNUM];  // �����ַ���
	std::wstring m_pStrColl[TAB_BUTTON_MAXNUM];     // ��ײ�ַ���
	std::wstring m_pStrTrig[TAB_BUTTON_MAXNUM];     // �����ַ���
	INT          m_nBrushMaxNum;                    // Ԫ�ز�����ť��
	INT          m_nNpcMaxNum;                      // NPC������ť��
	INT          m_nMonsterMaxNum;                  // ���������ť��
	INT          m_nCollMaxNum;                     // ��ײ������ť��
	INT          m_nTrigMaxNum;                     // ����������ť��
	DWORD        m_dwMessage;
	RECT         m_rWndRect;
};