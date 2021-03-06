// DialogRecipe.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApp.h"
#include "DialogRecipe.h"
#include "afxdialogex.h"


// CDialogRecipe dialog

IMPLEMENT_DYNAMIC(CDialogRecipe, CThemeDialog)

CDialogRecipe::CDialogRecipe(CWnd* pParent /*=nullptr*/)
	: CThemeDialog(IDD_DIALOG_RECIPE, pParent)
{

}

CDialogRecipe::~CDialogRecipe()
{
}

void CDialogRecipe::DoDataExchange(CDataExchange* pDX)
{
	CThemeDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogRecipe, CThemeDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDialogRecipe message handlers

void CDialogRecipe::SaveControlInitPosition()
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

void CDialogRecipe::ChangeControlPosition()
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

// Theme
void CDialogRecipe::InitDialogSkin()
{
	SetMouseDragMove(FALSE);
	SetStyleResize(FALSE);
	SetTransparentByColor(TRUE);
	SetFontSize(14);
	SetFontBold(FALSE);
	SetAlignFormat(DT_CENTER | DT_VCENTER);
	//SetCaptionArea(CRect(1, 1, -1, 57));
	//SetCaptionText(TEXT("Theme Control Example"));
	SetBackColor(RGB(0, 0, 0));
}

void CDialogRecipe::InitControlSkin()
{
	CThemeEdit* pEdit = nullptr;
	pEdit = (CThemeEdit*)AddThemeEditCtrl(IDC_EDIT1);
	gf_SetSkinEdit(pEdit);
}


BOOL CDialogRecipe::OnInitDialog()
{
	CThemeDialog::OnInitDialog();

	SaveControlInitPosition();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogRecipe::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CThemeDialog::OnShowWindow(bShow, nStatus);

	if (mOnShowed == false)
	{
		mOnShowed = true;

		InitDialogSkin();
		InitControlSkin();
	}
}


BOOL CDialogRecipe::PreTranslateMessage(MSG* pMsg)
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


void CDialogRecipe::OnSize(UINT nType, int cx, int cy)
{
	CThemeDialog::OnSize(nType, cx, cy);

	ChangeControlPosition();
}
