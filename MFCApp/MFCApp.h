
// MFCApp.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCAppApp:
// See MFCApp.cpp for the implementation of this class
//

class CMFCAppApp : public CWinApp
{	
public:
	CMFCAppApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

public:
	jmp::IScanLabRtc* mScanLabRtc;
	//std::unique_ptr<jmp::CScanLabRtc5> mScanLabRtc;
	std::unique_ptr<jmp::CTurboPmacManager> mTurboPmacMotionManager;
};

extern CMFCAppApp theApp;