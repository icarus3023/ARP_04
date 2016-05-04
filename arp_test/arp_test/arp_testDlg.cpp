
// arp_testDlg.cpp : 구현 파일
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


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// Carp_testDlg 대화 상자



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


// Carp_testDlg 메시지 처리기

BOOL Carp_testDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Carp_testDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR Carp_testDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}







