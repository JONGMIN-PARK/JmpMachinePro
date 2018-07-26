// ThemeEdit.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "ThemeEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CThemeToolTip		m_ToolTip;
//WNDPROC				m_oldEditProc;
//MSG					m_msg;

//LRESULT CALLBACK EditProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////////////
// CThemeEdit

CThemeEdit::CThemeEdit()
{
	
	//컨트롤 초기화
	m_pEditbox = NULL;

	m_stThemeInfo.strIniCtrlName = "";							// INI 파일 조회용 컨트롤명
	m_stThemeInfo.bIntValue = FALSE;
	m_stThemeInfo.nMaxValue = 0.; 
	m_stThemeInfo.nMinValue = 0.;
	
	// 테마 관련 변수 초기화
	InitThemeVariable();

}

// 테마 관련 변수 초기화
void CThemeEdit::InitThemeVariable()
{

	// 테마 옵션 초기화
	m_stThemeInfo.nFontSize = 12;								//  폰트 크기
	m_stThemeInfo.bFontBold = FALSE;							//  폰트 두께
	m_stThemeInfo.crFont = RGB(0,0,0);							//  폰트 컬러
	m_stThemeInfo.crBackground = RGB(255,255,255);				// 배경 컬러
	m_stThemeInfo.crOutlineLT = RGB(78,123,164);				// 테두리 컬러(Left-Top)
	m_stThemeInfo.crOutlineRB = RGB(192,195,200);				// 테두리 컬러(Right-Bottom)
	m_stThemeInfo.nOutlineThick = 1;							// 테두리 두께
	m_stThemeInfo.rInitPos = CRect(-1,-1,-1,-1);				// 초기 위치 정보

}

CThemeEdit::~CThemeEdit()
{
}


BEGIN_MESSAGE_MAP(CThemeEdit, CWnd)
	//{{AFX_MSG_MAP(CThemeEdit)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(dfIDC_EDITBOX_CTRL, OnChangeEditbox)
	ON_EN_ERRSPACE(dfIDC_EDITBOX_CTRL, OnErrspaceEditbox)
	ON_EN_HSCROLL(dfIDC_EDITBOX_CTRL, OnHscrollEditbox)
	ON_EN_KILLFOCUS(dfIDC_EDITBOX_CTRL, OnKillfocusEditbox)
	ON_EN_MAXTEXT(dfIDC_EDITBOX_CTRL, OnMaxtextEditbox)
	ON_EN_SETFOCUS(dfIDC_EDITBOX_CTRL, OnSetfocusEditbox)
	ON_EN_UPDATE(dfIDC_EDITBOX_CTRL, OnUpdateEditbox)
	ON_EN_VSCROLL(dfIDC_EDITBOX_CTRL, OnVscrollEditbox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CThemeEdit message handlers

// 사용자 키보드 이벤트를 받아서 처리
BOOL CThemeEdit::PreTranslateMessage(MSG* pMsg) 
{
	//int i = 10;
	//if (pMsg->message == WM_KEYDOWN)
	//{
	//	switch (pMsg->wParam)
	//	{
	//	case 0x30:
	//	case 0x31:
	//	case 0x32:
	//	case 0x33:
	//	case 0x34:
	//	case 0x35:
	//	case 0x36:
	//	case 0x37:
	//	case 0x38:
	//	case 0x39:
	//	case VK_BACK:
	//	case VK_NUMPAD0:
	//	case VK_NUMPAD1:
	//	case VK_NUMPAD2:
	//	case VK_NUMPAD3:
	//	case VK_NUMPAD4:
	//	case VK_NUMPAD5:
	//	case VK_NUMPAD6:
	//	case VK_NUMPAD7:
	//	case VK_NUMPAD8:
	//	case VK_NUMPAD9:
	//		break;

	//	default:
	//		return TRUE;
	//	}
	//}


	return CWnd::PreTranslateMessage(pMsg);
}

void CThemeEdit::OnButtonUpDown(BOOL up)
{

}

//컨트롤을 재배치하는 함수
void CThemeEdit::RepositionControl()
{
	CRect rWnd, rCtrl;

	//클래스의 크기를 구한다
	this->GetClientRect(&rWnd);

	//컨트롤의 위치좌표를 계산
	rCtrl = rWnd;
	rCtrl.left += m_stThemeInfo.nOutlineThick;
	rCtrl.top += m_stThemeInfo.nOutlineThick;
	rCtrl.top += 2;
	rCtrl.right -= m_stThemeInfo.nOutlineThick;
	rCtrl.bottom -= m_stThemeInfo.nOutlineThick;

	//컨트롤의 위치변경
	m_pEditbox->MoveWindow(&rCtrl);	
	
	//배경을 새로 그린다
	Invalidate(FALSE);
}

// WM_PAINT 이벤트 발생시 실행되는 함수
void CThemeEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// 화면 영역좌표 구하기
	//CRect rScreen;
	//this->GetClientRect(&rScreen);

	//테두리 선을 그리는 함수
	DrawEdgeLine(&dc);
}

//테두리 선을 그리는 함수
void CThemeEdit::DrawEdgeLine(CPaintDC *pdc)
{
	CRect rWnd;
	
	//클래스의 크기를 구한다
	this->GetClientRect(&rWnd);

	COLORREF crBack;
	//DWORD dwStyle = m_pEditbox->GetStyle();

	// 에디트박스 활성화 상태에 따라 배경색을 설정
	// 읽기전용 일때도 어두운 배경색 적용
	//if( !m_pEditbox->IsWindowEnabled() || dwStyle & ES_READONLY )
	//	crBack = RGB(212,208,200);
	//else
		crBack = m_stThemeInfo.crBackground;
	
	/*CBrush brNew(crBack), *pOldBrush;
	pOldBrush = pdc->SelectObject(&brNew);
	pdc->Rectangle(rWnd);
	pdc->SelectObject(pOldBrush);*/

	// 배경을 컬러로 채운다
	pdc->FillRect(rWnd, &CBrush(crBack));
	//pdc->FillRect(rWnd, &CBrush(RGB(0,0,0)));

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

// 텍스트 문자열 설정
void CThemeEdit::SetWindowText(CString strText)
{
	// 에디트박스에 Text 설정
	m_pEditbox->SetWindowText(strText);
}

// 컨트롤에 입력된 문자열 반환
void CThemeEdit::GetWindowText(CString &strText)
{
	m_pEditbox->GetWindowText(strText);
}

void CThemeEdit::ModifyEditStyle(DWORD remove, DWORD add)
{
	m_pEditbox->ModifyStyle( remove, add );
}

void CThemeEdit::SetSel(int nStart, int nEnd)
{
	m_pEditbox->SetSel( nStart, nEnd );
}

void CThemeEdit::SetFocus(BOOL f)
{
	if( f ) m_pEditbox->SetFocus();
}

void CThemeEdit::SetMinMaxValue(double minValue, double maxValue, BOOL valueIsInteger)
{
	m_stThemeInfo.nMinValue = minValue;
	m_stThemeInfo.nMaxValue = maxValue;
	m_stThemeInfo.bIntValue = valueIsInteger;
}

// 클래스 종료시에 실행되는 함수
void CThemeEdit::OnDestroy() 
{
	CWnd::OnDestroy();
	 
	//컨트롤 삭제
	if(m_pEditbox) 
	{ 
		m_pEditbox->DestroyWindow(); 
		//delete m_pEditbox;
		m_pEditbox = NULL;
	}
	
	// 툴팁 삭제
	DeleteToolTip();
	
	// 에디트박스 배경 컬러 브러쉬 삭제
	DeleteObject(m_brushBk);	
}

// 폰트 크기 설정
void CThemeEdit::SetFontSize(int nFontSize)
{
	m_stThemeInfo.nFontSize = nFontSize;

	// 에디트박스 컨트롤에 폰트 설정
	SetFontToEditbox(m_stThemeInfo.nFontSize, m_stThemeInfo.bFontBold);
}

// 폰트 두께 설정
void CThemeEdit::SetFontBold(BOOL bFontBold)
{
	m_stThemeInfo.bFontBold = bFontBold;
	
	// 에디트박스 컨트롤에 폰트 설정
	SetFontToEditbox(m_stThemeInfo.nFontSize, m_stThemeInfo.bFontBold);
}

// 폰트 컬러 설정
void CThemeEdit::SetFontColor(COLORREF crFont)
{
	m_stThemeInfo.crFont = crFont;

	// 에디트박스 컨트롤에 폰트 컬러를 설정
	SetFontColorToEditbox(m_stThemeInfo.crFont);
}

// 배경 컬러 설정
void CThemeEdit::SetBackColor(COLORREF crBack)
{
	m_stThemeInfo.crBackground = crBack;

	// 에디트박스 컨트롤에 배경 컬러를 설정
	SetBackColorToEditbox(m_stThemeInfo.crBackground);
}

// 에디트박스 컨트롤에 배경 컬러를 설정
void CThemeEdit::SetBackColorToEditbox(COLORREF crBack)
{
	// 에디트박스 컨트롤에 배경색 지정
	//m_pEditbox->SetBackgroundColor(FALSE, crBack);
	if( m_pEditbox == NULL )
		return;

	// 에디트박스 컨트롤에 배경색 지정
	//m_pEditbox->SetBackgroundColor(FALSE, crBack);

	m_stThemeInfo.crBackground = crBack;
	
	// 에디트박스 배경 컬러 브러쉬를 삭제하고 새로 생성
	if(m_brushBk != NULL) 
		DeleteObject(m_brushBk);

	m_brushBk = CreateSolidBrush( crBack );
}

// 에디트박스 컨트롤에 폰트 컬러를 설정
void CThemeEdit::SetFontColorToEditbox(COLORREF crFont)
{

}

// 에디트박스 컨트롤에 폰트 설정
void CThemeEdit::SetFontToEditbox(int nFontSize, BOOL bFontBold)
{
	CString m_sName = TEXT("돋움");

	if( m_ftEdit.GetSafeHandle() )
		m_ftEdit.DeleteObject();
	
	// 폰트 생성
	int nFontHeavy = ( bFontBold ) ? FW_BOLD : FW_THIN;
	m_ftEdit.CreateFont(nFontSize, 0,0,0, nFontHeavy, 0,0,0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, 
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, m_sName);

	// 컨트롤에 폰트를 설정
	//m_pEditbox->SetFont(&m_ftEdit);
	SetFont(&m_ftEdit);
	
}

// 클래스 생성시에 실행되는 함수
int CThemeEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//AfxInitRichEdit();
	//LoadLibrary(_T("Riched32.dll"));
	
	DWORD dwStyle = lpCreateStruct->style;

	//에디트박스 컨트롤 생성
	//m_pEditbox = new CRichEditCtrl();
	//m_pEditbox = new CEdit();
	m_pEditbox = new CBaseEdit();
	m_pEditbox->Create(WS_TABSTOP|WS_VISIBLE|dwStyle , CRect(0,0,40,20), this, dfIDC_EDITBOX_CTRL);
	
	// 에디트박스 컨트롤에 폰트 설정
	SetFontToEditbox(m_stThemeInfo.nFontSize, m_stThemeInfo.bFontBold);
	// 에디트박스 컨트롤에 배경 컬러를 설정
	SetBackColorToEditbox(m_stThemeInfo.crBackground);

	//m_brushBk = CreateSolidBrush( m_stThemeInfo.crBackground );

	//m_msg.hwnd = m_hWnd;
	//m_oldEditProc = (WNDPROC)SetWindowLong(m_pEditbox->GetSafeHwnd(), GWL_WNDPROC, (LONG)EditProc);
	
	return 0;
}

// 위치가 변경될때 실행되는 함수
void CThemeEdit::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	//현재 클래스 사이즈가 너무 작으면 함수 탈출
	if( cx < 10 || cy < 10 )
		return;
	
	//컨트롤을 재배치하는 함수
	RepositionControl();
	
	/*// 툴팁 변경
	if( m_ToolTip.GetSafeHwnd() )
	{
		CString strToolTip;
		// 현재 툴팁 캡션을 구한다
		strToolTip = m_ToolTip.GetToolTipText();
		// 툴팁 캡션 변경함수를 호출
		ChangeToolTip(strToolTip);
	}*/
}

// 에디트박스 컨트롤에 폰트를 설정
void CThemeEdit::SetFont(CFont *pFont)
{
	m_pEditbox->SetFont(pFont, FALSE);
}

// 툴팁을 생성한다
void CThemeEdit::MakeToolTip(CString strToolTip)
{
	// 에디트박스가 생성되지 않았다면 함수 탈출
	if( !m_pEditbox )
		return;

	// 에디트박스에 툴팁 설정
	m_pEditbox->MakeToolTip(strToolTip);
}

// 툴팁 텍스트 변경
void CThemeEdit::ChangeToolTip(CString strToolTip)
{
	// 에디트박스가 생성되지 않았다면 함수 탈출
	if( !m_pEditbox )
		return;

	// 에디트박스에 툴팁 텍스트 변경
	m_pEditbox->ChangeToolTip(strToolTip);
}

// 툴팁 삭제
void CThemeEdit::DeleteToolTip()
{
	// 에디트박스가 생성되지 않았다면 함수 탈출
	if( !m_pEditbox )
		return;

	// 에디트박스에 툴팁 삭제
	m_pEditbox->DeleteToolTip();
}

// 툴팁 문자열 설정
void CThemeEdit::SetToolTip(CString strText)
{
	m_pEditbox->SetToolTip(strText);
}

// 툴팁을 위한 마우스 이벤트를 받는 함수
LRESULT CThemeEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	{
		/*if( m_ToolTip.GetSafeHwnd() )
		{
			//MSG msg;
			//msg.hwnd = m_hWnd;
			m_msg.message = message;
			m_msg.wParam = wParam;
			m_msg.lParam = lParam;

			m_ToolTip.RelayEvent(&m_msg);
		}*/
	}
		break;
	default:
		break;
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

// 컨트롤 컬러 변경시 실행되는 함수
HBRUSH CThemeEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// 에디트박스 컨트롤의 폰트 컬러 & 배경 컬러를 설정
	if( m_pEditbox == pWnd )
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor( m_stThemeInfo.crFont );
		pDC->SetBkColor( m_stThemeInfo.crBackground );

		if( m_brushBk == NULL )
			m_brushBk = CreateSolidBrush(m_stThemeInfo.crBackground);
		return m_brushBk;
		//HBRUSH Br = CreateSolidBrush(RGB(225,225,0)); 
		//return Br;
	}

	return hbr;
}

// 클래스 생성
BOOL CThemeEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, pContext);
}

/*BOOL CThemeEdit::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}*/

// 에디트박스 내용 변경시 실행되는 함수
void CThemeEdit::OnChangeEditbox()
{
	//TRACE("Freq Editbox - Change!\n");
	// 부모 윈도우로 메세지 전달
	CWnd * pParent;
	pParent = this->GetParent();
	pParent->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), EN_CHANGE), (LPARAM)m_hWnd);
}

// 에디트박스에서 'EN_ERRSPACE' 메시지 발생시 실행되는 함수
void CThemeEdit::OnErrspaceEditbox()
{
	// 부모 윈도우로 메세지 전달
	CWnd * pParent;
	pParent = this->GetParent();
	pParent->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), EN_ERRSPACE), (LPARAM)m_hWnd);
}

// 에디트박스에서 'EN_HSCROLL' 메시지 발생시 실행되는 함수
void CThemeEdit::OnHscrollEditbox()
{
	// 부모 윈도우로 메세지 전달
	CWnd * pParent;
	pParent = this->GetParent();
	pParent->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), EN_HSCROLL), (LPARAM)m_hWnd);
}

// 에디트박스에 포커스가 해제되었을때 실행되는 함수
void CThemeEdit::OnKillfocusEditbox()
{
	// 부모 윈도우로 메세지 전달
	CWnd * pParent;
	pParent = this->GetParent();
	pParent->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), EN_KILLFOCUS), (LPARAM)m_hWnd);

	CString v;
	m_pEditbox->GetWindowText( v );

	if( m_stThemeInfo.nMinValue == 0 && m_stThemeInfo.nMaxValue == 0 ) return;

	if( m_stThemeInfo.bIntValue == FALSE ){
		if( _wtof(v) < m_stThemeInfo.nMinValue || _wtof(v) > m_stThemeInfo.nMaxValue )
		{
			CString msg;
			msg.Format(TEXT("입력범위가 올바르지 않습니다. 입력범위 : [ %.6f ~ %.6f ] ")
				, m_stThemeInfo.nMinValue, m_stThemeInfo.nMaxValue);
			AfxMessageBox( msg );
			
			msg.Format(TEXT("%.6f"), m_stThemeInfo.nMinValue );
			m_pEditbox->SetWindowText( msg );
			m_pEditbox->SetFocus();
		}
	}
	else
	{
		if(_wtof(v) < m_stThemeInfo.nMinValue || _wtof(v) > m_stThemeInfo.nMaxValue )
		{
			CString msg;
			msg.Format(TEXT("입력범위가 올바르지 않습니다. 입력범위 : [ %d ~ %d ] ")
				, (int)(m_stThemeInfo.nMinValue), (int)(m_stThemeInfo.nMaxValue));
			AfxMessageBox( msg );

			msg.Format(TEXT("%d"),(int) m_stThemeInfo.nMinValue );
			m_pEditbox->SetWindowText( msg );
			m_pEditbox->SetFocus();
		}
	}
	
}

// 에디트박스에서 'EN_MAXTEXT' 메시지 발생시 실행되는 함수
void CThemeEdit::OnMaxtextEditbox()
{
	// 부모 윈도우로 메세지 전달
	CWnd * pParent;
	pParent = this->GetParent();
	pParent->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), EN_MAXTEXT), (LPARAM)m_hWnd);
}

// 에디트박스에서 'EN_SETFOCUS' 메시지 발생시 실행되는 함수
void CThemeEdit::OnSetfocusEditbox()
{
	// 부모 윈도우로 메세지 전달
	CWnd * pParent;
	pParent = this->GetParent();
	pParent->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), EN_SETFOCUS), (LPARAM)m_hWnd);
}

// 에디트박스에서 'EN_UPDATE' 메시지 발생시 실행되는 함수
void CThemeEdit::OnUpdateEditbox()
{
	// 부모 윈도우로 메세지 전달
	CWnd * pParent;
	pParent = this->GetParent();
	pParent->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), EN_UPDATE), (LPARAM)m_hWnd);
}

// 에디트박스에서 'EN_VSCROLL' 메시지 발생시 실행되는 함수
void CThemeEdit::OnVscrollEditbox()
{
	// 부모 윈도우로 메세지 전달
	CWnd * pParent;
	pParent = this->GetParent();
	pParent->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), EN_VSCROLL), (LPARAM)m_hWnd);
}

// 외곽선 두께 설정
void CThemeEdit::SetOutlineThick(int nThick)
{
	m_stThemeInfo.nOutlineThick = nThick;
}

// 외곽선(Left-Top) 컬러 설정
void CThemeEdit::SetOutlineLTColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineLT = crOutline;
}

// 외곽선(Right-Bottom) 컬러 설정
void CThemeEdit::SetOutlineRBColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineRB = crOutline;
}

// 외곽선 컬러 설정
void CThemeEdit::SetOutlineColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineLT = crOutline;
	m_stThemeInfo.crOutlineRB = crOutline;
}

void CThemeEdit::Save()
{
	CString text;		
	CString temp(mIdString.c_str());
	CString str;
	GetWindowText(str);
	mNumericValue = _wtof(str);

	str.Format(TEXT("%.3f"), mNumericValue);

	WritePrivateProfileString(TEXT("a"), temp, str, TEXT("D:\\ac.txt"));
}

void CThemeEdit::Load()
{
	CString strValue;
	TCHAR szBuffer[MAX_PATH] = { 0, };
	int id = GetDlgCtrlID();
	CString temp(mIdString.c_str());
	GetPrivateProfileString(TEXT("a"), temp, TEXT(""), szBuffer, MAX_PATH, TEXT("D:\\ac.txt"));
	strValue.Format(TEXT("%s"), szBuffer);
	SetWindowText(strValue);
	
	mNumericValue = _wtof(strValue);
}

double CThemeEdit::GetDouble()
{	
	return mNumericValue;
}

int CThemeEdit::GetInt()
{
	return 2;
}

CString CThemeEdit::GetString()
{
	return TEXT("히히");
}

void CThemeEdit::setId2String(const std::string& str)
{
	mIdString = str;
}