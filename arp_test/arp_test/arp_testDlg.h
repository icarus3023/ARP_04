
// arp_testDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// Carp_testDlg ��ȭ ����
class Carp_testDlg : public CDialogEx
{
// �����Դϴ�.
public:
	Carp_testDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ARP_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
		CListBox ArpCacheTable;
		CIPAddressCtrl m_DstAddr;
		void Carp_testDlg::OnBnClickedSend();
		afx_msg void OnBnClickedAllDelete();
		afx_msg void OnBnClickedItemDelete();
};
