
// arp_test.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif
// IPCAppDlg.h : header file
//

#if !defined(AFX_IPCAPPDLG_H__FE9C37CB_4C90_4772_99AD_50A9B24BCE62__INCLUDED_)
#define AFX_IPCAPPDLG_H__FE9C37CB_4C90_4772_99AD_50A9B24BCE62__INCLUDED_

#include "resource.h"		// 주 기호입니다.


// Carp_testApp:
// 이 클래스의 구현에 대해서는 arp_test.cpp을 참조하십시오.
//

class Carp_testApp : public CWinApp
{
public:
	Carp_testApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.

	DECLARE_MESSAGE_MAP()
};

extern Carp_testApp theApp;
#endif