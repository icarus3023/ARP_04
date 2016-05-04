
// arp_testDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "arp_test.h"
#include "arp_testDlg.h"
#include "afxdialogex.h"
#include <map>
#include <list>
#include <pcap.h>
#include <WinSock2.h>
#pragma  comment(lib, "ws2_32.lib")
#include <Packet32.h>
#pragma  comment(lib, "packet.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Carp_testDlg ��ȭ ����



Carp_testDlg::Carp_testDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ARP_TEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Carp_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_DstAddr);
	DDX_Control(pDX, IDC_ARP_CACHE, ArpCacheTable);
	DDX_Control(pDX, IDC_IPADDRESS, m_DstAddr);
}

void Carp_testDlg::OnBnClickedSend()
{
	static char errbuf[PCAP_ERRBUF_SIZE];
	CString strMsg,devname;
	pcap_if_t *alldevs;
	int status = pcap_findalldevs(&alldevs, errbuf);
	if (status != 0) {
		strMsg.Format(_T("%s"), errbuf);
		ArpCacheTable.AddString(strMsg);
		return;
	}
	char* device = pcap_lookupdev(errbuf);
	
	pcap_t* pd = pcap_open_live(device, 1500, PCAP_OPENFLAG_PROMISCUOUS, 2000, errbuf);
	if (pd == NULL) {
		perror(errbuf);
		exit(-1);
	}
	bpf_u_int32 localnet;
	bpf_u_int32	netmask;
	if (pcap_lookupnet(device, &localnet, &netmask, errbuf) < 0) {
		perror(errbuf);
		exit(-1);
	}
	pcap_addr_t *a;
	CString strMsg;
	strMsg = pcap_lookupdev(errbuf);
	for (pcap_if_t *d = alldevs; d != NULL; d = d->next) {
		for (pcap_addr_t *a = d->addresses; a != NULL; a = a->next) {
			if (  a->broadaddr && a->addr->sa_family == AF_INET6  && a->addr && a->netmask ) {
				devname = d->description;
				strMsg = inet_ntoa(((struct sockaddr_in*)a->addr)->sin_addr);
				//printf("Found a device %s on address %s with netmask %s\n", dev->name, iptos(((struct sockaddr_in *)dev_addr->addr)->sin_addr.s_addr), iptos(((struct sockaddr_in *)dev_addr->netmask)->sin_addr.s_addr);
				ArpCacheTable.AddString(strMsg);
			}
		}
	}

	unsigned char src[4];
	m_DstAddr.GetAddress(src[0], src[1], src[2], src[3]);
	strMsg.Format(_T("%d.%d.%d.%d"), src[0], src[1], src[2], src[3]);

	ArpCacheTable.AddString(strMsg);
	pcap_freealldevs(alldevs);
}
void Carp_testDlg::OnBnClickedAllDelete()
{
	ArpCacheTable.ResetContent();
}
void Carp_testDlg::OnBnClickedItemDelete()
{
	ArpCacheTable.DeleteString(ArpCacheTable.GetCurSel());
}
BEGIN_MESSAGE_MAP(Carp_testDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, &Carp_testDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_ALL_DELETE, &Carp_testDlg::OnBnClickedAllDelete)
	ON_BN_CLICKED(IDC_ITEM_DELETE, &Carp_testDlg::OnBnClickedItemDelete)
END_MESSAGE_MAP()


// Carp_testDlg �޽��� ó����

BOOL Carp_testDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void Carp_testDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void Carp_testDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR Carp_testDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}







