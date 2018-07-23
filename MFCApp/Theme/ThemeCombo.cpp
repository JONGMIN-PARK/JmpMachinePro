// ThemeCombo.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "ThemeCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThemeCombo

CThemeCombo::CThemeCombo()
{

	m_pListbox = NULL;											// 리스트박스 컨트롤 초기화
	m_pBtnDropDown = NULL;										// 아래쪽 방향 버튼 컨트롤 초기화
	m_pEditbox = NULL;											// 에디트박스 컨트롤 초기화

	m_stThemeInfo.strIniCtrlName = "";							// INI 파일 조회용 컨트롤명

	// 테마 관련 변수 초기화
	InitThemeVariable();

}

// 테마 관련 변수 초기화
void CThemeCombo::InitThemeVariable()
{
	// 아래쪽 방향 버튼 컨트롤 전역변수 초기화
	if( m_pBtnDropDown )
		m_pBtnDropDown->InitThemeVariable();
	
	// 콤보박스 컨트롤 설정값 초기화
	m_stThemeInfo.crFontColor = RGB(0,0,0);						// 글자 색깔
	m_stThemeInfo.nFontSize = 12;								// 폰트 크기
	m_stThemeInfo.bFontBold = FALSE;							// 폰트 두껍기 여부
	m_stThemeInfo.crEditboxBack = RGB(255,255,255);				// 배경 색깔
	m_stThemeInfo.nOutlineThick = 1;							// 경계선 두께
	m_stThemeInfo.crOutlineLT = RGB(78,123,164);				// 경계선 색깔 (왼쪽 & 위쪽)
	m_stThemeInfo.crOutlineRB = RGB(192,195,200);				// 경계선 색깔 (오른쪽 & 아래쪽)
	m_stThemeInfo.crFrame = RGB(200,200,200);					// 항목 구분석 색깔
	m_stThemeInfo.nAlignFormat = DT_CENTER|DT_VCENTER;			// 텍스트 정렬 방식
	m_stThemeInfo.nBtnWidth = dfBUTTON_WIDTH;					// 버튼 컨트롤 넓이
	m_stThemeInfo.nEditboxHeight = dfEDITBOX_HEIGHT;			// 에디트박스 높이
	m_stThemeInfo.nListBoxHeight = dfLISTBOX_HEIGHT_MIN;		// 리스트박스 높이
	m_stThemeInfo.rInitPos = CRect(-1,-1,-1,-1);				// 초기 위치 정보

	// 버튼 컨트롤 설정값 초기화
	m_stBtnCtrlInfo.uIDImage = 0;
	m_stBtnCtrlInfo.strImageFile = "";
	m_stBtnCtrlInfo.nImageCount = 4;
	m_stBtnCtrlInfo.bMakeRgn = FALSE;

}

CThemeCombo::~CThemeCombo()
{
}


BEGIN_MESSAGE_MAP(CThemeCombo, CWnd)
	//{{AFX_MSG_MAP(CThemeCombo)
	ON_LBN_SELCHANGE(dfIDC_LISTBOX_DROP_CTRL, OnSelchangeListBox)
	ON_LBN_KILLFOCUS(dfIDC_LISTBOX_DROP_CTRL, OnKillfocusListBox)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	ON_BN_CLICKED(dfIDC_BUTTON_CTRL, OnBtnDropDown)
	//ON_MESSAGE(LBN_SELCHANGE, OnMsgListboxSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CThemeCombo message handlers

void CThemeCombo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	//테두리 선을 그리는 함수
	DrawEdgeLine(&dc);
	
}

// 테두리 선을 그리는 함수
void CThemeCombo::DrawEdgeLine(CPaintDC *pdc)
{
	CRect rWnd;
	
	// 테두리 사각형 영역을 구해서 반환하는 함수
	rWnd = GetOutlineRect();

	COLORREF crBack;
	crBack = m_stThemeInfo.crEditboxBack;
	
	CBrush brNew(crBack), *pOldBrush;
	pOldBrush = pdc->SelectObject(&brNew);
	pdc->Rectangle(rWnd);
	pdc->SelectObject(pOldBrush);

	rWnd.right --;
	rWnd.bottom --;

	for(int i=0 ; i < m_stThemeInfo.nOutlineThick ; i++)
	{
		//사각형 선을 그리는 함수
		gf_DrawRectLine(rWnd, m_stThemeInfo.crOutlineLT, m_stThemeInfo.crOutlineLT, m_stThemeInfo.crOutlineRB, m_stThemeInfo.crOutlineRB, pdc);
		//사각형 좌표 변경
		rWnd.left ++;
		rWnd.top ++;
		rWnd.right --;
		rWnd.bottom --;
	}
}

// 컨트롤의 폰트를 설정
void CThemeCombo::SetFont(CFont *pFont)
{
	m_pEditbox->SetFont(pFont);
}

// 컨트롤에 문자열을 입력
void CThemeCombo::SetWindowText(CString strText)
{
	m_pEditbox->SetWindowText(strText);
}

void CThemeCombo::ModifyEditStyle(DWORD remove, DWORD add)
{
	m_pEditbox->ModifyStyle( remove, add );
}

void CThemeCombo::SetSel(int nStart, int nEnd)
{
	m_pEditbox->SetSel( nStart, nEnd );
}

void CThemeCombo::SetFocus(BOOL f)
{
	m_pEditbox->SetFocus(f);
}

// 클래스 삭제시에 실행되는 함수
void CThemeCombo::OnDestroy() 
{
	CWnd::OnDestroy();

	// 리스트박스를 보여주는 타이머 종료
	KillTimer(dfID_TIMER_SHOW_LISTBOX);
	
	// 에디트박스 컨트롤 삭제
	if(m_pEditbox) 
	{ 
		m_pEditbox->DestroyWindow(); 
		delete m_pEditbox;
		m_pEditbox = NULL;
	}
	// 아래쪽 방향 버튼 컨트롤 삭제
	if( m_pBtnDropDown )
	{
		m_pBtnDropDown->DestroyWindow();
		delete m_pBtnDropDown;
		m_pBtnDropDown = NULL;
	}
	// 리스트박스 컨트롤 삭제
	if( m_pListbox )
	{
		m_pListbox->DestroyWindow();
		delete m_pListbox;
		m_pListbox = NULL;
	}	
}

// 테두리 사각형 영역을 구해서 반환하는 함수
CRect CThemeCombo::GetOutlineRect()
{
	CRect rWnd;

	//클래스의 크기를 구한다
	this->GetClientRect(&rWnd);
	// 테두리 높이
	rWnd.bottom = rWnd.top + m_stThemeInfo.nEditboxHeight;
	//rWnd.bottom = rWnd.top + dfEDITBOX_HEIGHT;

	return rWnd;
}

// 클래스 생성시에 실행되는 함수
int CThemeCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	DWORD dwStyle = lpCreateStruct->style;

	// 수평 정렬 방식을 구해서 전역변수에 저장
	if( (dwStyle & ES_CENTER) == ES_CENTER )
		m_stThemeInfo.nAlignFormat |= DT_CENTER;
	else if( (dwStyle & ES_LEFT) == ES_LEFT )
		m_stThemeInfo.nAlignFormat |= DT_LEFT;
	else if( (dwStyle & ES_RIGHT) == ES_RIGHT )
		m_stThemeInfo.nAlignFormat |= DT_RIGHT;
	else
		m_stThemeInfo.nAlignFormat |= DT_LEFT;

	// 컨트롤 생성 함수
	CreateControl(dwStyle);
	
	return 0;
}

// 리스트박스에 항목을 추가
int CThemeCombo::AddString(CString strItem)
{
	return m_pListbox->AddString( strItem );
}

// 부모 윈도우 화면의 리스트박스 컨트롤 영역을 새로 그리는 함수
void CThemeCombo::RedrawParentListCtrlArea()
{
	CRect rCtrl;

	// 리스트박스 컨트롤의 위치좌표를 구한다
	m_pListbox->GetWindowRect(&rCtrl);
	this->GetParent()->ScreenToClient(&rCtrl);
	// 부모 윈도우 화면을 새로 그린다
	this->GetParent()->InvalidateRect(rCtrl, TRUE);
}

// 이벤트 메시지를 부모 윈도우에 알려주는 함수
LRESULT CThemeCombo::SendMessageToParent(int nRow, int nCol, int nMessage)
{
	if (!IsWindow(m_hWnd))
        return 0;

    NM_COMBOBOXVIEW nmgv;
    nmgv.iRow         = nRow;
    nmgv.iColumn      = nCol;
    nmgv.hdr.hwndFrom = m_hWnd;
    nmgv.hdr.idFrom   = GetDlgCtrlID();
    nmgv.hdr.code     = nMessage;

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
	//if( m_pParent && IsWindow(m_pParent->m_hWnd))
		//return m_pParent->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
        //return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
		return pOwner->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), nMessage), (LPARAM)m_hWnd);

    else
        return 0;
}

// 현재 선택된 항목 번호를 반환
int CThemeCombo::GetCurSel()
{
	return m_pListbox->GetCurSel();
}

// 에디트박스 컨트롤에 표시된 문자열을 반환
void CThemeCombo::GetWindowText(CString &rString)
{
	m_pEditbox->GetWindowText(rString);
}

// 문자열을 검색하여 해당 항목 번호를 반환
int CThemeCombo::FindString(int nStartAfter, LPCTSTR lpszString)
{
	return m_pListbox->SelectString( nStartAfter, lpszString );
}

// 항목을 삭제하는 함수
int CThemeCombo::DeleteString(UINT nIndex)
{
	return m_pListbox->DeleteString( nIndex );
}

// 리스트 중간에 항목을 삽입하는 함수
int CThemeCombo::InsertString(int nIndex, LPCTSTR lpszItem)
{
	return m_pListbox->InsertString( nIndex, lpszItem );
}

// 항목 개수를 반환하는 함수
int CThemeCombo::GetCount()
{
	return m_pListbox->GetCount();
}

// 특정 항목의 내용을 구하는 함수
void CThemeCombo::GetLBText(int nIndex, CString &rString)
{
	m_pListbox->GetText( nIndex, rString );
}

// 리스트박스 항목을 더블클릭하면 실행되는 함수
void CThemeCombo::OnDblclkListbox()
{
	// 이벤트 메시지를 부모 윈도우에 알려주는 함수
	SendMessageToParent(0, 0, CBN_DBLCLK);

	/*********************************************************************/
	// 부모 윈도우쪽 코딩
	// BEGIN_MESSAGE_MAP에 다음 코딩 추가
	//		ON_NOTIFY(CBN_DBLCLK, IDC_COMBO_COUNTRY, OnDblclkComboCountry)
	// 이벤트 발생시 실행되는 함수 생성 - OnDblclkComboCountry()
	/*********************************************************************/
}

// 특정 항목을 선택표시하는 함수
int CThemeCombo::SetCurSel(int nIndex)
{
	int nSelItemNo=-1;

	nSelItemNo = m_pListbox->SetCurSel( nIndex );

	// 리스트박스 컨트롤 선택 항목 변경시 실행되는 함수
	OnSelchangeListBox();

	return nSelItemNo;
}

// 컨트롤 경계선 정보를 받아들이는 함수
void CThemeCombo::SetOutline(COLORREF crOutlineLT, COLORREF crOutlineRB, int nOutlineThick)
{
	//전역변수 설정값 변경
	m_stThemeInfo.nOutlineThick = nOutlineThick;
	m_stThemeInfo.crOutlineLT = crOutlineLT;
	m_stThemeInfo.crOutlineRB = crOutlineRB;

	// 리스트박스에 경계선 정보를 전달
	m_pListbox->SetOutline( crOutlineLT, crOutlineRB, nOutlineThick);

	//컨트롤을 재배치하는 함수
	RepositionControl();
}

// 컨트롤 경계선 정보를 받아들이는 함수
void CThemeCombo::SetOutline(COLORREF crOutlineLT, COLORREF crOutlineRB, int nOutlineThick, COLORREF crFrame)
{
	//전역변수 설정값 변경
	m_stThemeInfo.nOutlineThick = nOutlineThick;
	m_stThemeInfo.crOutlineLT = crOutlineLT;
	m_stThemeInfo.crOutlineRB = crOutlineRB;
	m_stThemeInfo.crFrame = crFrame;

	// 리스트박스에 경계선 정보를 전달
	m_pListbox->SetOutline( crOutlineLT, crOutlineRB, nOutlineThick, crFrame, m_stThemeInfo.crEditboxBack);

	//컨트롤을 재배치하는 함수
	RepositionControl();
}

// 에디트박스 컨트롤에 폰트 특성을 설정
void CThemeCombo::SetFontEditbox()
{
	// 에디트박스 컨트롤에 배경색 지정
	//m_pEditbox->SetBackgroundColor(FALSE, m_stThemeInfo.crEditboxBack);
	m_pEditbox->SetBackColor(m_stThemeInfo.crEditboxBack);

	// 폰트 크기 설정
	m_pEditbox->SetFontSize(m_stThemeInfo.nFontSize);
	// 폰트 컬러 설정
	m_pEditbox->SetFontColor(m_stThemeInfo.crFontColor);
	// 폰트 두께 설정
	m_pEditbox->SetFontBold(m_stThemeInfo.bFontBold);
	// 외곽선 두께 설정
	m_pEditbox->SetOutlineThick(0);
	
	/*CHARFORMAT cfText;

	//폰트 특성 생성
	m_pEditbox->GetDefaultCharFormat(cfText);
	cfText.bCharSet = OUT_DEFAULT_PRECIS | CLIP_DEFAULT_PRECIS | DEFAULT_QUALITY;
	cfText.dwEffects -= CFE_AUTOCOLOR;
	cfText.crTextColor = m_stThemeInfo.crFontColor;
	//cfText.bPitchAndFamily = DEFAULT_PITCH;
	cfText.bPitchAndFamily = m_stThemeInfo.nFontSize;
	strcpy (cfText.szFaceName, m_sName);    // cf.szFaceName = m_sName;
	m_pEditbox->SetDefaultCharFormat(cfText);*/
}

// 폰트 특성을 설정하는 함수
void CThemeCombo::SetFontStyle(COLORREF crFontColor, COLORREF crBackColor, int nFontSize, BOOL bBold)
{
	m_stThemeInfo.crFontColor = crFontColor;
	m_stThemeInfo.crEditboxBack = crBackColor;
	m_stThemeInfo.nFontSize = nFontSize;
	m_stThemeInfo.bFontBold = bBold;
	
	// 에디트박스 컨트롤에 폰트 특성을 설정
	SetFontEditbox();

	// 리스트박스 컨트롤에 폰트 특성을 설정
	m_pListbox->SetFontColor(crFontColor);
	m_pListbox->SetBackColor(crBackColor);
	m_pListbox->SetFontSize(nFontSize);
	m_pListbox->SetFontBold(bBold);
	//m_pListbox->SetFontStyle(crFontColor, crBackColor, nFontSize, bBold);
}

// 폰트 특성을 설정하는 함수
void CThemeCombo::SetFontStyle(COLORREF crFontColor, COLORREF crBackColor, int nFontSize, BOOL bBold, int nAlignFormat)
{
	m_stThemeInfo.crFontColor = crFontColor;
	m_stThemeInfo.crEditboxBack = crBackColor;
	m_stThemeInfo.nFontSize = nFontSize;
	m_stThemeInfo.bFontBold = bBold;
	m_stThemeInfo.nAlignFormat = nAlignFormat;
	
	// 에디트박스 컨트롤에 폰트 특성을 설정
	SetFontEditbox();

	// 리스트박스 컨트롤에 폰트 특성을 설정
	m_pListbox->SetFontColor(crFontColor);
	m_pListbox->SetBackColor(crBackColor);
	m_pListbox->SetFontSize(nFontSize);
	m_pListbox->SetFontBold(bBold);
	m_pListbox->SetAlignFormat(nAlignFormat);
	//m_pListbox->SetFontStyle(crFontColor, crBackColor, nFontSize, bBold, nAlignFormat);
}

// 선택항목의 색깔을 설정하는 함수
void CThemeCombo::SetSelectItemColor(COLORREF crSelItemFont, COLORREF crSelItemBack)
{
	m_pListbox->SetSelectItemColor( crSelItemFont, crSelItemBack);
}

// 버튼 컨트롤 배경 이미지 정보를 계산
void CThemeCombo::CalculateBtnImageInfo()
{
	CBitmap bmpBitmap;
	BITMAP bm;
	BOOL bLoad=FALSE;

	if( m_stBtnCtrlInfo.uIDImage > 0 )
	{
		// 버튼 이미지 로딩
		bLoad = bmpBitmap.LoadBitmap(m_stBtnCtrlInfo.uIDImage);
		if( !bLoad )
			return;
		bmpBitmap.GetBitmap(&bm);

		// 버튼 크기를 구한다
		m_stThemeInfo.nBtnWidth = bm.bmWidth / m_stBtnCtrlInfo.nImageCount;
		m_stThemeInfo.nEditboxHeight = bm.bmHeight + (m_stThemeInfo.nOutlineThick * 2);
		/*CString strMsg;
		strMsg.Format("Height %d", m_stThemeInfo.nEditboxHeight);
		MessageBox(strMsg);*/
	}
	else if( m_stBtnCtrlInfo.strImageFile.GetLength() > 3 )
	{		
		// 이미지 파일을 읽어서 CBitmap으로 반환
		bLoad = gf_LoadPictureFile(m_stBtnCtrlInfo.strImageFile, &bmpBitmap);
		if( !bLoad )
			return;
		bmpBitmap.GetBitmap(&bm);

		// 버튼 크기를 구한다
		m_stThemeInfo.nBtnWidth = bm.bmWidth / m_stBtnCtrlInfo.nImageCount;
		m_stThemeInfo.nEditboxHeight = bm.bmHeight + (m_stThemeInfo.nOutlineThick * 2);
	}
}

// 버튼 컨트롤 배경 이미지 설정(파일)
BOOL CThemeCombo::SetButtonImage(CString strFileName)
{
	BOOL bLoad=FALSE;

	// 버튼 컨트롤에 이미지 파일 경로 전달
	bLoad = m_pBtnDropDown->SetBackImage(strFileName);
	if( !bLoad )
		return bLoad;

	// 버튼 Caption 텍스트를 삭제
	m_pBtnDropDown->SetCaptionText(TEXT(""));

	// 파일 경로를 전역변수에 저장
	m_stBtnCtrlInfo.strImageFile = strFileName;
	m_stBtnCtrlInfo.uIDImage = 0;

	// 버튼 컨트롤 배경 이미지 정보를 계산
	CalculateBtnImageInfo();

	//컨트롤을 재배치하는 함수
	RepositionControl();

	return bLoad;
}

// 버튼 컨트롤 배경 이미지 설정(Resource ID)
BOOL CThemeCombo::SetButtonImage(UINT nImageID)
{
	BOOL bLoad=FALSE;

	// 버튼 컨트롤에 이미지 Resource ID 전달
	bLoad = m_pBtnDropDown->SetBackImage(nImageID);
	if( !bLoad )
		return bLoad;

	// 버튼 Caption 텍스트를 삭제
	m_pBtnDropDown->SetCaptionText(TEXT(""));

	// 파일 경로를 전역변수에 저장
	m_stBtnCtrlInfo.strImageFile = "";
	m_stBtnCtrlInfo.uIDImage = nImageID;

	// 버튼 컨트롤 배경 이미지 정보를 계산
	CalculateBtnImageInfo();

	// 새로운 위치정보를 계산
	CRect rCtrlOld, rCtrlNew, rCtrlSize;
	// 현재 컨트롤 위치좌표
	this->GetWindowRect(&rCtrlOld);
	// 새로운 컨트롤 크기
	rCtrlSize = GetOutlineRect();

	/*CString strMsg;
	strMsg.Format("Height : %d", rCtrlSize.Height());
	MessageBox(strMsg);*/

	//m_stThemeInfo.nEditboxHeight
	// 높이가 서로 다르다면 위치좌표를 변경
	if( rCtrlSize.Height() != rCtrlOld.Height() )
	{
		// 새로운 위치좌표를 구한다
		rCtrlNew.left = rCtrlOld.left;
		rCtrlNew.top = rCtrlOld.top;
		rCtrlNew.right = rCtrlNew.left + rCtrlSize.Width();
		rCtrlNew.bottom = rCtrlNew.top + rCtrlSize.Height();
		// 컨트롤을 새로운 위치좌표로 이동
		this->MoveWindow(rCtrlNew);
	}

	//컨트롤을 재배치하는 함수
	//RepositionControl();

	return bLoad;
}

// 버튼 컨트롤 배경 이미지 블럭 개수 설정
void CThemeCombo::SetButtonImageBlockCount(int nBlockCount)
{
	// 버튼 컨트롤 배경 이미지 블럭 개수 설정
	m_pBtnDropDown->SetBackImageBlockCount(nBlockCount);

	m_stBtnCtrlInfo.nImageCount = nBlockCount;

	// 버튼 컨트롤 배경 이미지 정보를 계산
	CalculateBtnImageInfo();

	//컨트롤을 재배치하는 함수
	RepositionControl();
}

// 왼쪽 마우스 버튼 클릭시 실행되는 함수
void CThemeCombo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	this->SetFocus(TRUE);
	
	CWnd::OnLButtonDown(nFlags, point);
}

long CThemeCombo::OnMsgListboxSelChange(WPARAM wParam, LPARAM lParam)
{
	int i=0;

	return 0;
}

// 리스트박스 컨트롤에 포커스가 사라졌을때 실행되는 함수
void CThemeCombo::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	//NM_COMBOBOXVIEW *pItem = (NM_COMBOBOXVIEW *)pNMHDR;
	
	// 사용자가 아래쪽방향 버튼을 눌렀다면 무시하고 함수 탈출
	if( pNewWnd == m_pBtnDropDown )
		return;

	// 리스트박스가 보여진 상태라면 감춘다
	if( m_pListbox->IsWindowVisible() )
	{
		// 리스트박스를 감추는 함수
		HideListbox();	
	}
}

// 리스트박스 컨트롤에 포커스가 사라졌을때 실행되는 함수
void CThemeCombo::OnKillfocusListBox()
{
	// 마우스가 버튼 컨트롤 영역 위에 있다면
	// 사용자가 드롭다운 버튼을 클릭한것으로 판단하고 함수 탈출
	if( m_pBtnDropDown->IsMouseHover() )
		return;

	// 리스트박스가 보여진 상태라면 감춘다
	if( m_pListbox->IsWindowVisible() )
	{
		// 리스트박스를 감추는 함수
		HideListbox();
	}
}

// 아래쪽 방향 버튼 컨트롤 클릭시 실행되는 함수
void CThemeCombo::OnBtnDropDown()
{
	if( m_pListbox == NULL )
		return;

	CRect rCtrl, rWnd;
	//m_pListbox->GetWindowRect(rCtrl);

	// 리스트박스가 보여진 상태라면 감춘다
	if( m_pListbox->IsWindowVisible() )
	{
		// 리스트박스를 감추는 함수
		HideListbox();
	}
	// 리스트박스가 감춰진 상태라면 보여준다
	else
	{
		// 리스트박스를 보여주는 타이머 발생
		SetTimer(dfID_TIMER_SHOW_LISTBOX, 50, NULL);
	}
}

// 타이머 이벤트 발생시 실행되는 함수
void CThemeCombo::OnTimer(UINT_PTR nIDEvent) 
{
	switch( nIDEvent )
	{
	case dfID_TIMER_SHOW_LISTBOX :			// 리스트박스를 보여주는 타이머 일때
		// 타이머 중지
		KillTimer(dfID_TIMER_SHOW_LISTBOX);

		// 리스트박스를 보여주는 함수
		ShowListbox();
		break;
	}
	
	CWnd::OnTimer(nIDEvent);
}

// 리스트박스를 감추는 함수
void CThemeCombo::HideListbox()
{
	// 리스트박스 컨트롤을 감춘다
	m_pListbox->ShowWindow(FALSE);

	// 부모 윈도우 화면의 리스트컨트롤 영역을 새로 그리는 함수
	RedrawParentListCtrlArea();

	// 부모 윈도우로 포커스를 전달
	this->GetParent()->SetFocus();
}

// 클래스 생성
BOOL CThemeCombo::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, pContext);
}

// 리스트박스를 보여주는 함수
void CThemeCombo::ShowListbox()
{
	CRect rCtrl;
	
	// 리스트박스의 위치좌표를 구해서 반환하는 함수
	rCtrl = GetListboxRect();
	// 리스트박스 컨트롤의 위치 변경
	m_pListbox->MoveWindow(&rCtrl);
	
	// 리스트박스 컨트롤을 화면에 표시
	m_pListbox->ShowWindow(TRUE);

	// 리스트박스 컨트롤에 포커스 지정
	m_pListbox->SetFocus();

}


// 위치가 변경될때 실행되는 함수
void CThemeCombo::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	//현재 클래스 사이즈가 너무 작으면 함수 탈출
	if( cx < 10 || cy < 10 )
		return;

	// 자기 자신의 크기를 조정
	ResizeThis();
	
	//컨트롤을 재배치하는 함수
	RepositionControl();	
}

// 자기 자신의 크기를 조정
void CThemeCombo::ResizeThis()
{
	CRect rScreen, rView;

	// 테두리 사각형 영역을 구해서 반환하는 함수
	rView = GetOutlineRect();
	// 자신의 위치좌표
	this->GetWindowRect(&rScreen);

	// 컨트롤 높이가 올바르지 않다면 자신의 크기 변경
	if( rScreen.Height() != rView.Height() )
	{
		// 리스트박스 높이 설정
		m_stThemeInfo.nListBoxHeight = rScreen.Height() - rView.Height();
		if( m_stThemeInfo.nListBoxHeight < dfLISTBOX_HEIGHT_MIN )
			m_stThemeInfo.nListBoxHeight = dfLISTBOX_HEIGHT_MIN;

		// 자신의 크기를 조정
		this->GetParent()->ScreenToClient(&rScreen);
		rScreen.bottom = rScreen.top + rView.Height();
		this->MoveWindow(rScreen);
	}
}

// 컨트롤 생성 함수
void CThemeCombo::CreateControl(DWORD dwStyle)
{
	CRect rCtrl;
	DWORD dwEdit=ES_CENTER;
	
	// 에디트박스 컨트롤 생성
	//m_pEditbox = new CRichEditCtrl();CBS_DROPDOWNLIST
	// 'DropList' 스타일이라면 에디트박스에 '읽기전용' 스타일 적용
	if( (dwStyle & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST )
		dwEdit |= ES_READONLY;
	m_pEditbox = new CThemeEdit();
	m_pEditbox->Create(WS_TABSTOP|WS_VISIBLE|dwEdit , CRect(0,0,40,20), this, dfIDC_EDITBOX_CTRL);
	// 에디트박스 컨트롤에 폰트 특성을 설정
	SetFontEditbox();

	// '아래쪽 삼각형' 버튼 컨트롤 생성
	rCtrl = CRect(0,0, m_stThemeInfo.nBtnWidth, m_stThemeInfo.nEditboxHeight-2);
	m_pBtnDropDown = new CThemeButton();
	// 버튼 이미지가 존재한다면
	if( m_stBtnCtrlInfo.uIDImage > 0 && m_stBtnCtrlInfo.nImageCount > 1 )
	{
		m_pBtnDropDown->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP, rCtrl, this, dfIDC_BUTTON_CTRL);
		m_pBtnDropDown->MoveWindow(rCtrl);
	}	
	else
	{
		m_pBtnDropDown->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP, rCtrl, this, dfIDC_BUTTON_CTRL);
		m_pBtnDropDown->SetBackColor(RGB(193,211,251));
		m_pBtnDropDown->GetClientRect(&rCtrl);
		m_pBtnDropDown->SetCaptionText(TEXT("▽"));
		m_pBtnDropDown->SetFontColor(RGB(37,57,93));
		m_pBtnDropDown->SetFontSize(12);
		m_pBtnDropDown->SetFontBold(TRUE);
		//m_pBtnDropDown->SetShadowColor(RGB(0,0,70));
	}
	
	// 리스트박스 컨트롤 생성
	m_pListbox = new CThemeListBox();
	m_pListbox->m_bStylePopUp = TRUE;
	if( !m_pListbox->Create(WS_TABSTOP|WS_CHILD, CRect(0,20,100,80), this, dfIDC_LISTBOX_DROP_CTRL) )
	{
		m_pListbox->DestroyWindow();
		delete m_pListbox;
		m_pListbox = NULL;
	}
	m_pListbox->SetParent(this);
	m_pListbox->SetOutline(m_stThemeInfo.crOutlineLT, m_stThemeInfo.crOutlineRB, m_stThemeInfo.nOutlineThick, 
							m_stThemeInfo.crFrame, m_stThemeInfo.crFrame);
	m_pListbox->SetFontSize(m_stThemeInfo.nFontSize);
	m_pListbox->SetFontBold(m_stThemeInfo.bFontBold);
	m_pListbox->SetAlignFormat(m_stThemeInfo.nAlignFormat);
	m_pListbox->ShowWindow(FALSE);

}

// 리스트박스의 위치좌표를 구해서 반환하는 함수
CRect CThemeCombo::GetListboxRect()
{
	CRect rCtrl, rWnd, rCombo;
	
	// 리스트박스 컨트롤의 위치좌표를 계산
	this->GetWindowRect(&rCtrl);				// 이 윈도우의 영역
	rWnd = GetOutlineRect();						// 테두리 사각형 영역을 구해서 반환하는 함수

	rCtrl.top += rWnd.Height();
	rCtrl.bottom = rCtrl.top + m_stThemeInfo.nListBoxHeight;

	// 리스트박스 컨트롤의 높이가 최소값 보다 작다면
	if( rCtrl.Height() < dfLISTBOX_HEIGHT_MIN )
		rCtrl.bottom = rCtrl.top + dfLISTBOX_HEIGHT_MIN;
	
	// 윈도우 화면 해상도를 구한다
	int cy = GetSystemMetrics(SM_CYSCREEN);

	/// 리스트박스가 윈도우 화면 아래까지 내려간다면
	if( rCtrl.bottom > cy )
	{
		// 리스트박스의 위치를 콤보박스 위쪽으로 이동
		rCtrl.bottom = rCtrl.top - rWnd.Height();
		rCtrl.top = rCtrl.bottom - m_stThemeInfo.nListBoxHeight;

		// 리스트박스 컨트롤의 높이가 최소값 보다 작다면
		if( rCtrl.Height() < dfLISTBOX_HEIGHT_MIN )
			rCtrl.top = rCtrl.bottom - dfLISTBOX_HEIGHT_MIN;
	}
	
	return rCtrl;
}

// 배경 컬러 설정
void CThemeCombo::SetBackColor(COLORREF crBack)
{
	// 배경 컬러를 전역변수에 저장
	m_stThemeInfo.crEditboxBack = crBack;

	// 에디트박스 컨트롤에 배경 컬러 설정
	m_pEditbox->SetBackColor(crBack);
	// 리스트박스 컨트롤에 배경 컬러 설정
	m_pListbox->SetBackColor(crBack);
}

// 폰트 컬러 설정
void CThemeCombo::SetFontColor(COLORREF crFont)
{
	// 폰트 컬러를 전역변수에 저장
	m_stThemeInfo.crFontColor = crFont;

	// 에디트박스 컨트롤에 폰트 컬러 설정
	m_pEditbox->SetFontColor(crFont);
	// 리스트박스 컨트롤에 폰트 컬러 설정
	m_pListbox->SetFontColor(crFont);
}

// 리스트박스 일반 항목 배경 컬러 설정
void CThemeCombo::SetNormalItemBackColor(COLORREF crItemBack)
{
	m_pListbox->SetNormalItemBackColor(crItemBack);
}

// 리스트박스 일반 항목 배경색을 지정하는 함수
void CThemeCombo::SetNormalItemBackColor(COLORREF crItem1, COLORREF crItem2)
{
	m_pListbox->SetNormalItemBackColor(crItem1, crItem2);
}

// 일반 항목(짝수번째) 배경 컬러 설정
void CThemeCombo::SetNormalItem1BackColor(COLORREF crItem1Back)
{
	m_pListbox->SetNormalItem1BackColor(crItem1Back);
}

// 일반 항목(홀수번째) 배경 컬러 설정
void CThemeCombo::SetNormalItem2BackColor(COLORREF crItem2Back)
{
	m_pListbox->SetNormalItem2BackColor(crItem2Back);
}

// 항목 구분선 컬러를 지정하는 함수
void CThemeCombo::SetItemFrameColor(COLORREF crFrame)
{
	// 항목 구분선 컬러를 전역변수에 저장
	m_stThemeInfo.crFrame = crFrame;

	// 리스트박스에 항목 구분선 컬러 설정
	m_pListbox->SetItemFrameColor(crFrame);
}

// 외곽선 컬러를 설정
void CThemeCombo::SetOutlineColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineLT = crOutline;
	m_stThemeInfo.crOutlineRB = crOutline;

	// 리스트박스에 외곽선 컬러 설정
	m_pListbox->SetOutlineColor(crOutline);
}

// 외곽선 컬러를 설정
void CThemeCombo::SetOutlineColor(COLORREF crOutlineLT, COLORREF crOutlineRB)
{
	m_stThemeInfo.crOutlineLT = crOutlineLT;
	m_stThemeInfo.crOutlineRB = crOutlineRB;

	// 리스트박스에 외곽선 컬러 설정
	m_pListbox->SetOutlineColor(crOutlineLT, crOutlineRB);
}

// 외곽선(Left-Top) 컬러를 설정
void CThemeCombo::SetOutlineLTColor(COLORREF crOutlineLT)
{
	m_stThemeInfo.crOutlineLT = crOutlineLT;
	
	// 리스트박스에 외곽선 컬러 설정
	m_pListbox->SetOutlineLTColor(crOutlineLT);
}

// 외곽선(Right-Bottom) 컬러를 설정
void CThemeCombo::SetOutlineRBColor(COLORREF crOutlineRB)
{
	m_stThemeInfo.crOutlineRB = crOutlineRB;
	
	// 리스트박스에 외곽선 컬러 설정
	m_pListbox->SetOutlineRBColor(crOutlineRB);
}

// 외곽선 두께를 설정
void CThemeCombo::SetOutlineThick(int nOutlineThick)
{
	m_stThemeInfo.nOutlineThick = nOutlineThick;

	// 컨트롤 위치 재배치
	RepositionControl();
}

// 텍스트 정렬 방식 설정
void CThemeCombo::SetAlignFormat(int nAlignFormat)
{
	// 텍스트 정렬 방식을 전역변수에 저장
	m_stThemeInfo.nAlignFormat = nAlignFormat;

	// 리스트박스 컨트롤에 텍스트 정렬 방식을 설정
	m_pListbox->SetAlignFormat(nAlignFormat);
}

// 툴팁을 생성
void CThemeCombo::MakeToolTip(CString strToolTip)
{
	m_pEditbox->MakeToolTip(strToolTip);
}

// 툴팁 텍스트 변경
void CThemeCombo::ChangeToolTip(CString strToolTip)
{
	m_pEditbox->ChangeToolTip(strToolTip);
}

// 툴팁 삭제
void CThemeCombo::DeleteToolTip()
{
	m_pEditbox->DeleteToolTip();
}

// 툴팁 문자열 설정
void CThemeCombo::SetToolTip(CString strText)
{
	m_pEditbox->SetToolTip(strText);
}

// 리스트박스 컨트롤 선택 항목 변경시 실행되는 함수
void CThemeCombo::OnSelchangeListBox()
{
	CString strSelItem;

	// 리스트박스 컨트롤의 선택 항목 내용을 구한다
	m_pListbox->GetText(m_pListbox->GetCurSel(), strSelItem);

	// 리스트박스를 감추는 함수
	HideListbox();

	// 에디트박스 컨트롤에 선택 항목 내용을 표시
	m_pEditbox->SetWindowText(strSelItem);

	// 에디트박스에 포커스 지정
	m_pEditbox->SetFocus(TRUE);

	// 이벤트 메시지를 부모 윈도우에 알려주는 함수
	//SendMessageToParent(0, 0, CBN_SELCHANGE);

	// 이벤트 메시지를 부모 윈도우에 전달
	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM)m_hWnd);

	/*********************************************************************/
	// 부모 윈도우쪽 코딩
	// BEGIN_MESSAGE_MAP에 다음 코딩 추가
	//		ON_CBN_SELCANCEL(IDC_COMBO_COUNTRY, OnSelchangeComboCountry)
	// 이벤트 발생시 실행되는 함수 생성 - OnSelchangeComboCountry()
	/*********************************************************************/
}

// 리스트박스 높이 설정
void CThemeCombo::SetListBoxHeight(int nHeight)
{
	m_stThemeInfo.nListBoxHeight = nHeight;
}

// 선택된 항목 폰트 컬러 설정
void CThemeCombo::SetSelectItemFontColor(COLORREF crFont)
{
	m_pListbox->SetSelectItemFontColor(crFont);
}

// 선택된 항목 배경 컬러 설정
void CThemeCombo::SetSelectItemBackColor(COLORREF crBack)
{
	m_pListbox->SetSelectItemBackColor(crBack);
}

// 폰트 크기 설정
void CThemeCombo::SetFontSize(int nFontSize)
{
	m_stThemeInfo.nFontSize = nFontSize;

	m_pEditbox->SetFontSize(nFontSize);
	m_pListbox->SetFontSize(nFontSize);
}

// 폰트 두껍기 설정
void CThemeCombo::SetFontBold(BOOL bFontBold)
{
	m_stThemeInfo.bFontBold = bFontBold;

	m_pEditbox->SetFontBold(bFontBold);
	m_pListbox->SetFontBold(bFontBold);
}

// 항목 높이 설정
void CThemeCombo::SetItemHeight(int nItemHeight)
{
	m_pListbox->SetItemHeight(nItemHeight);
}

// 왼쪽 여백 설정
void CThemeCombo::SetLeftGap(int nLeftGap)
{
	m_pListbox->SetLeftGap(nLeftGap);
}

// 오른쪽 여백 설정
void CThemeCombo::SetRightGap(int nRightGap)
{
	m_pListbox->SetRightGap(nRightGap);
}

//컨트롤을 재배치하는 함수
void CThemeCombo::RepositionControl()
{
	CRect rWnd, rCtrl, rListBox, rCombo;

	// 외곽선 사각형 영역을 구해서 반환하는 함수
	rWnd = GetOutlineRect();
	// 테두리 라인이 차지하는 영역을 제외
	rWnd.left += m_stThemeInfo.nOutlineThick;
	rWnd.top += m_stThemeInfo.nOutlineThick;
	rWnd.right -= m_stThemeInfo.nOutlineThick;
	rWnd.bottom -= m_stThemeInfo.nOutlineThick;

	// 에디트박스 컨트롤의 위치좌표를 계산
	rCtrl = rWnd;
	rCtrl.top += 2;
	rCtrl.right -= m_stThemeInfo.nBtnWidth;
	// 에디트박스 컨트롤의 위치 변경
	m_pEditbox->MoveWindow(&rCtrl);

	// 버튼 컨트롤의 위치좌표를 계산
	rCtrl = rWnd;
	rCtrl.left = rCtrl.right - m_stThemeInfo.nBtnWidth;
	// 버튼 컨트롤의 위치 변경
	m_pBtnDropDown->MoveWindow(&rCtrl);

	// 리스트박스의 위치좌표를 구해서 반환하는 함수
	rCtrl = GetListboxRect();
	// 리스트박스 컨트롤의 위치 변경
	m_pListbox->MoveWindow(&rCtrl);
	
	//배경을 새로 그린다
	Invalidate(FALSE);
}

// 리스트박스 투명도 설정
BOOL CThemeCombo::SetListBoxTransparent(BYTE byteDegree)
{
	return m_pListbox->SetTransparent(byteDegree);
}
