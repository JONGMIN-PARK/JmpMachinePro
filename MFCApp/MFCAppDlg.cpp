
// MFCAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApp.h"
#include "MFCAppDlg.h"
#include "afxdialogex.h"

//──────────────────────────────────────────────────────────────────────────────────────────
// 서브 화면
//──────────────────────────────────────────────────────────────────────────────────────────
#include "DialogAuto.h"
#include "DialogManual.h"
#include "DialogRecipe.h"
#include "DialogMaint.h"
#include "DialogConfig.h"
#include "DialogLog.h"
//──────────────────────────────────────────────────────────────────────────────────────────

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMFCAppDlg dialog



CMFCAppDlg::CMFCAppDlg(CWnd* pParent /*=NULL*/)
	: CThemeDialog(IDD_MFCAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CThemeDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCAppDlg, CThemeDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_COMMAND_RANGE(IDC_BUTTON_AUTO, IDC_BUTTON_LOG, &CMFCAppDlg::OnSelectedMainMenu)
ON_BN_CLICKED(IDC_BUTTON_EXIT, &CMFCAppDlg::OnBnClickedButtonExit)
ON_WM_TIMER()
END_MESSAGE_MAP()


// CMFCAppDlg message handlers

BOOL CMFCAppDlg::OnInitDialog()
{
	CThemeDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	SetWindowPos(nullptr, 0, 0, 1920, 1080, SWP_NOREPOSITION);

	SaveControlInitPosition();

	SetTimer(0, 20, nullptr);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void OffsetChildWindow(CWnd* child, int dx, int dy, int dw, int dh)
{
	if (!child) return;
	if (!child->GetParent()) return;
	if (!IsWindow(child->m_hWnd)) return;

	//find child window's coordinates relative to top-left of parent
	CRect rc;
	child->GetWindowRect(&rc);
	CPoint p(0, 0);
	ScreenToClient(child->GetParent()->GetSafeHwnd(), &p);
	rc.OffsetRect(p);

	//prevent negative size
	if ((rc.Width() + dw) < 0) dw = -rc.Width();
	if ((rc.Height() + dh) < 0) dh = -rc.Height();

	child->SetWindowPos(0, rc.left + dx, rc.top + dy, rc.Width() + dw, rc.Height() + dh, SWP_NOZORDER);
	child->Invalidate(FALSE);
}

void CMFCAppDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);	
	
	//ChangeControlPosition();
}

void CMFCAppDlg::InitDialogSkin()
{
	SetMouseDragMove(FALSE);
	SetStyleResize(FALSE);
	SetTransparentByColor(TRUE);
	SetBackImage(TEXT("IDB_DLG_MAIN_BACK.gif"));
	SetFontSize(30);
	SetShadowColor(RGB(0, 0, 0));
	SetFontColor(RGB(255, 255, 255));
	SetFontBold(TRUE);
	SetAlignFormat(DT_CENTER | DT_VCENTER);
	SetCaptionArea(CRect(1, 1, -1, 57));
	SetCaptionText(TEXT("JmpMachinePro V1.0"));
}

void CMFCAppDlg::InitControlSkin()
{
	CThemeButton* pButton = nullptr;
	pButton = (CThemeButton*)AddThemeButtonCtrl(IDC_BUTTON_AUTO);
	pButton->SetFontBold(true);
	pButton->SetFontSize(14);
	gf_SetSkinBtnInside(pButton);

	pButton = (CThemeButton*)AddThemeButtonCtrl(IDC_BUTTON_MANUAL);
	pButton->SetFontBold(true);
	pButton->SetFontSize(14);
	gf_SetSkinBtnInside(pButton);

	pButton = (CThemeButton*)AddThemeButtonCtrl(IDC_BUTTON_RECIPE);
	pButton->SetFontBold(true);
	pButton->SetFontSize(14);
	gf_SetSkinBtnInside(pButton);

	pButton = (CThemeButton*)AddThemeButtonCtrl(IDC_BUTTON_MAINT);
	pButton->SetFontBold(true);
	pButton->SetFontSize(14);
	gf_SetSkinBtnInside(pButton);

	pButton = (CThemeButton*)AddThemeButtonCtrl(IDC_BUTTON_CONFIG);
	pButton->SetFontBold(true);
	pButton->SetFontSize(14);
	gf_SetSkinBtnInside(pButton);

	pButton = (CThemeButton*)AddThemeButtonCtrl(IDC_BUTTON_LOG);
	pButton->SetFontBold(true);
	pButton->SetFontSize(14);
	gf_SetSkinBtnInside(pButton);

	pButton = (CThemeButton*)AddThemeButtonCtrl(IDC_BUTTON_EXIT);
	pButton->SetFontBold(true);
	pButton->SetFontSize(14);
	gf_SetSkinBtnExit(pButton);
}

BOOL CMFCAppDlg::PreTranslateMessage(MSG* pMsg)
{
	// ALT + F4 방지
	if (pMsg->message == WM_SYSKEYDOWN)
	{
		if (pMsg->wParam == VK_F4)
		{
			return true;
		}
	}

	// Edit box등 엔터 키 누를 때 종료 방지
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
	{
		return true;
	}

	return CThemeDialog::PreTranslateMessage(pMsg);
}

void CMFCAppDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CThemeDialog::OnShowWindow(bShow, nStatus);	
		
	InitDialogSkin();
	InitControlSkin();

	CWnd* pParentWindow = GetDlgItem(IDC_STATIC);
	CRect client;
	pParentWindow->GetClientRect(&client);

	mDialogAuto = new CDialogAuto();
	mDialogAuto->Create(IDD_DIALOG_AUTO, pParentWindow);
	mDialogAuto->ShowWindow(SW_SHOW);
	mDialogAuto->MoveWindow(0, 0, client.Width(), client.Height());

	mDialogManual = new CDialogManual();
	mDialogManual->Create(IDD_DIALOG_MANUAL, pParentWindow);
	//mDialogManual->ShowWindow(SW_SHOW);
	mDialogManual->MoveWindow(0, 0, client.Width(), client.Height());

	mDialogRecipe = new CDialogRecipe();
	mDialogRecipe->Create(IDD_DIALOG_MANUAL, pParentWindow);
	//mDialogManual->ShowWindow(SW_SHOW);
	mDialogRecipe->MoveWindow(0, 0, client.Width(), client.Height());

	mDialogMaint = new CDialogMaint();
	mDialogMaint->Create(IDD_DIALOG_MAINT, pParentWindow);		
	//mDialogMaint->ShowWindow(SW_SHOW);
	mDialogMaint->MoveWindow(0, 0, client.Width(), client.Height());

	mDialogConfig = new CDialogConfig();
	mDialogConfig->Create(IDD_DIALOG_MANUAL, pParentWindow);
	//mDialogManual->ShowWindow(SW_SHOW);
	mDialogConfig->MoveWindow(0, 0, client.Width(), client.Height());

	mDialogLog = new CDialogLog();
	mDialogLog->Create(IDD_DIALOG_MANUAL, pParentWindow);
	//mDialogManual->ShowWindow(SW_SHOW);
	mDialogLog->MoveWindow(0, 0, client.Width(), client.Height());
	
	/*CString strTab = TEXT("");
	strTab.Format(TEXT("TAB1"));
	this->tabControl_.InsertItem(0, strTab, 0);
	strTab.Format(TEXT("TAB2"));
	this->tabControl_.InsertItem(1, strTab, 0);

	CRect rect;
	tabControl_.GetClientRect(&rect);
	mDialogAuto = new CDialogAuto();
	mDialogAuto->Create(IDD_DIALOG_AUTO, &tabControl_);
	mDialogAuto->SetWindowPos(nullptr, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);

	mDialogMaint = new CDialogMaint();
	mDialogMaint->Create(IDD_DIALOG_MAINT, &tabControl_);
	mDialogMaint->SetWindowPos(nullptr, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_NOZORDER);*/

	mLastDisplay = mDialogAuto;
}

void CMFCAppDlg::SaveControlInitPosition()
{
	CRect rect;
	TCHAR sz[256];

	GetClientRect(rect);
	mControlPositionArray.Add(rect);

	CWnd* pWnd = GetTopWindow();

	if (pWnd != NULL)
	{
		pWnd->GetWindowText(sz, 256);
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);
		mControlPositionArray.Add(rect);

		CWnd* tmpWnd = pWnd->GetNextWindow(GW_HWNDNEXT);

		while (tmpWnd != NULL)
		{
			tmpWnd->GetWindowText(sz, 256);
			tmpWnd->GetWindowRect(rect);
			ScreenToClient(rect);
			mControlPositionArray.Add(rect);
			tmpWnd = tmpWnd->GetNextWindow(GW_HWNDNEXT);
		}
	}
}

void CMFCAppDlg::ChangeControlPosition()
{
	if (mControlPositionArray.GetSize() <= 1)
		return;

	CRect rect, rect_org;
	int i = 0;

	rect_org = mControlPositionArray.GetAt(i);
	i++;

	//GetClientRect(rect);
	rect = CRect(0, 0, MAIN_DISPLAY_WIDTH, MAIN_DISPLAY_HEIGHT);

	double dXRatio = ((double)rect.Width()) / ((double)rect_org.Width());
	double dYRatio = ((double)rect.Height()) / ((double)rect_org.Height());

	CWnd* pWnd = GetTopWindow();

	rect = mControlPositionArray.GetAt(i);
	i++;

	pWnd->MoveWindow((int)(rect.left*dXRatio),
		(int)(rect.top*dYRatio),
		(int)(rect.Width()*dXRatio),
		(int)(rect.Height()*dYRatio));

	CWnd* tmpWnd = pWnd->GetNextWindow(GW_HWNDNEXT);

	while (tmpWnd != NULL)
	{
		rect = mControlPositionArray.GetAt(i);
		i++;
		tmpWnd->MoveWindow((int)(rect.left*dXRatio),
			(int)(rect.top*dYRatio),
			(int)(rect.Width()*dXRatio),
			(int)(rect.Height()*dYRatio));

		tmpWnd = tmpWnd->GetNextWindow(GW_HWNDNEXT);
	}
}

void CMFCAppDlg::OnBnClickedButtonExit()
{
	OnBtnClickExit();
}


void CMFCAppDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	switch (nIDEvent)
	{
	case 0:

		break;
	}

	CThemeDialog::OnTimer(nIDEvent);
}


BOOL CMFCAppDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	KillTimer(0);

	mDialogAuto->DestroyWindow();
	delete mDialogAuto;
	mDialogAuto = nullptr;

	mDialogManual->DestroyWindow();
	delete mDialogManual;
	mDialogManual = nullptr;

	mDialogRecipe->DestroyWindow();
	delete mDialogRecipe;
	mDialogRecipe = nullptr;

	mDialogMaint->DestroyWindow();
	delete mDialogMaint;
	mDialogMaint = nullptr;

	mDialogConfig->DestroyWindow();
	delete mDialogConfig;
	mDialogConfig = nullptr;

	mDialogLog->DestroyWindow();
	delete mDialogLog;
	mDialogLog = nullptr;

	return CThemeDialog::DestroyWindow();
}

void CMFCAppDlg::OnSelectedMainMenu(UINT id)
{
	if (id == mLastDisplayIndex)
	{
		return;
	}

	mLastDisplay->ShowWindow(SW_HIDE);
	switch (id)
	{
	case IDC_BUTTON_AUTO :		
		mDialogAuto->ShowWindow(SW_SHOW);
		mLastDisplay = mDialogAuto;
		break;

	case IDC_BUTTON_MANUAL :
		mDialogManual->ShowWindow(SW_SHOW);
		mLastDisplay = mDialogManual;
		break;

	case IDC_BUTTON_RECIPE :
		mDialogRecipe->ShowWindow(SW_SHOW);
		mLastDisplay = mDialogRecipe;
		break;

	case IDC_BUTTON_MAINT :		
		mDialogMaint->ShowWindow(SW_SHOW);
		mLastDisplay = mDialogMaint;
		break;

	case IDC_BUTTON_CONFIG :
		mDialogConfig->ShowWindow(SW_SHOW);
		mLastDisplay = mDialogConfig;
		break;

	case IDC_BUTTON_LOG :
		mDialogLog->ShowWindow(SW_SHOW);
		mLastDisplay = mDialogLog;
		break;
	}
	mLastDisplayIndex = id;
}
