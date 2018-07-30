
// MFCApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MFCApp.h"
#include "MFCAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace jmp;

// CMFCAppApp

BEGIN_MESSAGE_MAP(CMFCAppApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMFCAppApp construction

CMFCAppApp::CMFCAppApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMFCAppApp object

CMFCAppApp theApp;


// CMFCAppApp initialization

BOOL CMFCAppApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));	
	
	// 프로그램 중복 실행 방지
	CString machineName;
	machineName = "UV Cutting Machine";

	HANDLE hMutex = CreateMutex(nullptr, true, machineName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		ReleaseMutex(hMutex);

		CWnd* pWndPrev;
		CWnd* pWndChild;
		pWndPrev = CWnd::FindWindow(nullptr, machineName);
		if (pWndPrev)
		{
			pWndChild = pWndPrev->GetLastActivePopup();

			if (pWndChild->IsIconic())
				pWndPrev->ShowWindow(SW_RESTORE);

			pWndChild->SetForegroundWindow();
		}
		else
		{
			CString str = machineName + TEXT(" is already running.");
			AfxMessageBox(str);
		}
		return false;
	}
	ReleaseMutex(hMutex);

	// 디바이스 초기화
	mScanLabRtc = CreateRtc6();
	UINT result = mScanLabRtc->initialize();

	mTurboPmacMotionManager = std::make_unique<jmp::CTurboPmacManager>();
	std::map<int, std::vector<int>> deviceNumbers;
	deviceNumbers[0] = { 0, 1, 2, 3 };
	
	mTurboPmacMotionManager->init(deviceNumbers);

	//mTurboPmacMotionManager->loadProgramFile("D:\\0001. Document\\001. 모션관련\\000. Turbo PMAC\\Rotary Buffer\\BinaryRotaryBufferEx\\RotaryPG\\RotaryEx.pmc");
	//mTurboPmacMotionManager->initRotaryBuffer();

	CMFCAppDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
	}	
	
#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	DestroyRtc(&mScanLabRtc);
	mTurboPmacMotionManager->dispose();	

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}



int CMFCAppApp::ExitInstance()
{	

	return CWinApp::ExitInstance();
}
