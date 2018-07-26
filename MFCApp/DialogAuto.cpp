// DialogAuto.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApp.h"
#include "DialogAuto.h"
#include "afxdialogex.h"


// CDialogAuto dialog

IMPLEMENT_DYNAMIC(CDialogAuto, CThemeDialog)

CDialogAuto::CDialogAuto(CWnd* pParent /*=NULL*/)
	: CThemeDialog(IDD_DIALOG_AUTO, pParent)
{

}

CDialogAuto::~CDialogAuto()
{
}

void CDialogAuto::DoDataExchange(CDataExchange* pDX)
{
	CThemeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_PERSON, m_pListCtrlPerson);
}


BEGIN_MESSAGE_MAP(CDialogAuto, CThemeDialog)
	ON_EN_CHANGE(IDC_EDIT1, &CDialogAuto::OnEnChangeEdit1)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
ON_BN_CLICKED(IDC_BUTTON1, &CDialogAuto::OnBnClickedButton1)
ON_NOTIFY(NM_DBLCLK, IDC_GRID_PERSON, &CDialogAuto::OnNMDblclkGridPerson)
ON_BN_CLICKED(IDC_BUTTON2, &CDialogAuto::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDialogAuto message handlers


BOOL CDialogAuto::OnInitDialog()
{
	CThemeDialog::OnInitDialog();

	SaveControlInitPosition();

	mUIDataManager = std::make_unique<jmp::CUIDataManager>();	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogAuto::InitDialogSkin()
{
	SetMouseDragMove(FALSE);
	SetStyleResize(FALSE);
	SetTransparentByColor(TRUE);
	SetFontSize(14);
	SetFontBold(FALSE);
	SetAlignFormat(DT_CENTER | DT_VCENTER);
	//SetCaptionArea(CRect(1, 1, -1, 57));
	//SetCaptionText(TEXT("Theme Control Example"));
	SetBackColor(RGB(62, 62, 62));
}

void CDialogAuto::InitControlSkin()
{
	CThemeEdit* pEdit = nullptr;
	CThemeButton* pButton = nullptr;
	CThemeGroup* pGroup = nullptr;
	CThemeStatic* pStatic = nullptr;

	// 배경 부터 추가...
	pGroup = (CThemeGroup*)AddThemeGroupCtrl(IDC_STATIC_WORK_STATE);
	gf_SetSkinGroup(pGroup);
	pGroup->SetFontColor(RGB(255, 255, 255));
	pGroup->SetFontBold(true);	
	pGroup->SetFontSize(14);
	pGroup->SetEdgeRoundValue(15);
	pGroup->SetBackDrawType(dfBACK_DRAW_TYPE_COLOR);
	pGroup->SetOutlineLTColor(RGB(40,185,252));
	pGroup->SetOutlineRBColor(RGB(40,185,245));
	//pGroup->SetOutlineColor(RGB(0, 0, 0));
	pGroup->SetBackColor(RGB(68, 68, 68));
	pGroup->Invalidate();

	pGroup = (CThemeGroup*)AddThemeGroupCtrl(IDC_STATIC_TIME);
	gf_SetSkinGroup(pGroup);
	pGroup->SetFontColor(RGB(255, 255, 255));
	pGroup->SetFontBold(true);
	pGroup->SetFontSize(14);
	pGroup->SetEdgeRoundValue(15);
	pGroup->SetBackDrawType(dfBACK_DRAW_TYPE_COLOR);
	pGroup->SetOutlineLTColor(RGB(40, 185, 252));
	pGroup->SetOutlineRBColor(RGB(40, 185, 245));
	//pGroup->SetOutlineColor(RGB(0, 0, 0));
	pGroup->SetBackColor(RGB(68, 68, 68));
	pGroup->Invalidate();

	pGroup = (CThemeGroup*)AddThemeGroupCtrl(IDC_STATIC_PRODUCTION);
	gf_SetSkinGroup(pGroup);
	pGroup->SetFontColor(RGB(255, 255, 255));
	pGroup->SetFontBold(true);
	pGroup->SetFontSize(14);
	pGroup->SetEdgeRoundValue(15);
	pGroup->SetBackDrawType(dfBACK_DRAW_TYPE_COLOR);
	pGroup->SetOutlineLTColor(RGB(40, 185, 252));
	pGroup->SetOutlineRBColor(RGB(40, 185, 245));
	//pGroup->SetOutlineColor(RGB(0, 0, 0));
	pGroup->SetBackColor(RGB(68, 68, 68));
	pGroup->Invalidate();

	pEdit = (CThemeEdit*)AddThemeEditCtrl(IDC_EDIT1);
	gf_SetSkinEdit(pEdit);

	// Button
	pButton = (CThemeButton*)AddThemeButtonCtrl(IDC_BUTTON1);
	gf_SetSkinBtnInside(pButton);

	pButton = (CThemeButton*)AddThemeButtonCtrl(IDC_BUTTON2);
	gf_SetSkinBtnInside(pButton);

	pStatic = (CThemeStatic*)AddThemeStaticCtrl(IDC_STATIC_PROCESS);	
	gf_SetSkinStaticTitle(pStatic);
	pStatic->SetBackDrawType(dfBACK_DRAW_TYPE_COLOR);
	pStatic->SetBackColor(RGB(125, 125, 125));
	pStatic->SetAlignFormat(DT_CENTER | DT_VCENTER);
}


void CDialogAuto::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CThemeDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

BOOL CDialogAuto::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_TIMER && pMsg->hwnd == nullptr)
	{
		return false;
	}

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

void CDialogAuto::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CThemeDialog::OnShowWindow(bShow, nStatus);

	if (mOnShowed == false)
	{
		mOnShowed = true;

		InitDialogSkin();
		InitControlSkin();

		CreateControl();
		InitGridPerson();

		mUIDataManager->add(GetDlgItem(IDC_EDIT1), "IDC_EDIT1", IDC_EDIT1);
		mUIDataManager->load("");

		double value = 0.0;
		CThemeEdit* pEdit = static_cast<CThemeEdit*>(GetDlgItem(IDC_EDIT1));
		value = pEdit->GetDouble();

		//AfxBeginThread(executeLoop, this);
		loopThread_ = std::thread(&executeLoop, this); //std::unique_ptr<std::thread>(new std::thread(&IMotionManager::executeLoop));
	}	
}

void CDialogAuto::OnSize(UINT nType, int cx, int cy)
{
	CThemeDialog::OnSize(nType, cx, cy);

	if (mOnShowed == false)
	{
		return;
	}

	//ChangeControlPosition();
	SetTimer(1, 500, nullptr);
}

void CDialogAuto::SaveControlInitPosition()
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

void CDialogAuto::ChangeControlPosition()
{
	if (mControlPositionArray.GetSize() <= 1)
		return;

	CRect rect, rect_org;
	int i = 0;
	CThemeEdit* pEdit = nullptr;

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


void CDialogAuto::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CThemeDialog::OnClose();
}

void CDialogAuto::OnTimer(UINT_PTR nIDEvent)
{
	static bool mIsBlink;
	static int count;
	CString str;

	switch (nIDEvent)
	{
	case 1 :
		count++;
		if (mIsBlink == true)
		{
			mIsBlink = false;
			str.Format(TEXT("%d"), count);
			static_cast<CThemeStatic*>(GetDlgItem(IDC_STATIC_PROCESS))->SetCaptionText(str);
			static_cast<CThemeStatic*>(GetDlgItem(IDC_STATIC_PROCESS))->SetBackColor(RGB(255, 0, 0));
			m_pGridPerson->SetCellColor(0, 0, RGB(255, 0, 0));
		}
		else
		{
			mIsBlink = true;
			str.Format(TEXT("%d"), count);
			static_cast<CThemeStatic*>(GetDlgItem(IDC_STATIC_PROCESS))->SetCaptionText(str);
			static_cast<CThemeStatic*>(GetDlgItem(IDC_STATIC_PROCESS))->SetBackColor(RGB(0, 0, 255));
			m_pGridPerson->SetCellColor(0, 0, RGB(0, 0, 255));
		}
		
		count = count >= 100 ? 0 : count;
		break;

	case 2 : 
		break;
	}

	CThemeDialog::OnTimer(nIDEvent);
}


void CDialogAuto::OnDestroy()
{
	CThemeDialog::OnDestroy();

	KillTimer(0);
}

void CDialogAuto::OnBnClickedButton1()
{
	double value = 0.0;
	CThemeEdit* pEdit = static_cast<CThemeEdit*>(GetDlgItem(IDC_EDIT1));
	value = pEdit->GetDouble();

	return;
	theApp.mTurboPmacMotionManager->startRotaryBuffer();

	/*int nSelItem = 0;
	CString strText;
	nSelItem = 0;
	m_pGridPerson->SetRow(nSelItem + 1);
	for (int i = 0; i < 3; i++)
	{
		strText = TEXT("gggg");
		m_pGridPerson->SetCol(i + 1);
		m_pGridPerson->SetText(strText);
	}
	
	m_pGridPerson->SetEditable(false);	*/
}

void CDialogAuto::CreateControl()
{
	m_pGridPerson = AddThemeGridCtrl(IDC_GRID_PERSON);
	gf_SetSkinGrid(m_pGridPerson);
}

void CDialogAuto::InitGridPerson()
{
	CString strColList[] = { TEXT("No"), TEXT("Name"), TEXT("Gender"), TEXT("Score") };
	int nColWidth[] = { 30, 50, 55, 50 };
	int i = 0, nColCount = 4;
	CString strText;
	m_pGridPerson->SetRows(11);
	m_pGridPerson->SetCols(4);
	m_pGridPerson->SetFixedRows(1);
	m_pGridPerson->SetFixedCols(1);

	m_pGridPerson->SetRow(0);
	for (int i = 0; i < nColCount; i++)
	{
		m_pGridPerson->SetColWidth(i, nColWidth[i]);
		m_pGridPerson->SetCol(i);
		m_pGridPerson->SetText(strColList[i]);
	}
	m_pGridPerson->SetCol(0);
	for (int i = 0; i < 10; i++)
	{
		m_pGridPerson->SetRow(i + 1);
		strText.Format(TEXT("%d"), i + 1);
		m_pGridPerson->SetText(strText);
	}
}


void CDialogAuto::OnNMDblclkGridPerson(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	

	*pResult = 0;
}


//void CDialogAuto::OnLvnItemchangedGridPerson(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}


void CDialogAuto::OnBnClickedButton2()
{
	theApp.mTurboPmacMotionManager->stopRotaryBuffer();
}

unsigned int CDialogAuto::executeLoop(void* obj)
{
	CDialogAuto* dlgAuto = (CDialogAuto*)(obj);
	CThemeEdit* pEdit = nullptr;
	
	jmp::CUIDataManager& dataManager = *dlgAuto->mUIDataManager;
	//CUIDataManager* dataManager = &(*dlgAuto->mUIDataManager);
	
	double value = 0.0;
	while (1)
	{		
		value = dataManager["IDC_EDIT1"]; //(*dlgAuto->mUIDataManager)["IDC_EDIT1"];
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}