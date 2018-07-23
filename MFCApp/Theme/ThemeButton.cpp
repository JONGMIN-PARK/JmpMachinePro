// ThemeButton.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "ThemeButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThemeButton

CThemeButton::CThemeButton()
{
	
	// 메뉴 포인터 초기화
	m_pMenu = NULL;

	m_stThemeInfo.strIniCtrlName = "";							// INI 파일 조회용 컨트롤명
	m_stThemeInfo.strCaption = "";								// Caption 문자열

	// 테마 관련 변수 초기화
	InitThemeVariable();

}

// 테마 관련 변수 초기화
void CThemeButton::InitThemeVariable()
{

	// 배경 그리기 방식이 이미지라면 기존 이미지를 삭제
	if( m_stThemeInfo.nBackDrawType == dfBACK_DRAW_TYPE_IMAGE )
		m_stThemeInfo.bmpBack.DeleteObject();

	// 테마 옵션 초기화
	m_stThemeInfo.rCaption = CRect(0,0,0,0);					// Caption 영역좌표
	m_stThemeInfo.nFontSize = 12;								// 폰트 크기
	m_stThemeInfo.bFontBold = FALSE;							// 폰트 두께
	m_stThemeInfo.crFont[ST_NORMAL] = RGB(8,3,69);				// 폰트 컬러(Normal)
	m_stThemeInfo.crFont[ST_HOVER] = RGB(8,3,69);				// 폰트 컬러(Hover)
	m_stThemeInfo.crFont[ST_PRESSED] = RGB(8,3,69);				// 폰트 컬러(Pressed)
	m_stThemeInfo.crFont[ST_DISABLED] = RGB(128,128,128);		// 폰트 컬러(Disabled)
	m_stThemeInfo.crShadow[ST_NORMAL] = dfCOLOR_TRANSPARENT;	// 그림자 컬러
	m_stThemeInfo.crShadow[ST_HOVER] = dfCOLOR_TRANSPARENT;		// 그림자 컬러
	m_stThemeInfo.crShadow[ST_PRESSED] = dfCOLOR_TRANSPARENT;	// 그림자 컬러
	m_stThemeInfo.crShadow[ST_DISABLED] = dfCOLOR_TRANSPARENT;	// 그림자 컬러
	m_stThemeInfo.nAlignFormat = DT_CENTER|DT_VCENTER;			// Caption 정렬방식
	m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_COLOR;				// 배경 그리기 방식
	m_stThemeInfo.nBackImageFillType = dfBACK_IMAGE_FILL_TYPE_BLOCK;	// 배경 이미지 채우기 방식
	m_stThemeInfo.nBackImageAlignFormat = DT_CENTER|DT_VCENTER;			// 배경 이미지 정렬방식
	m_stThemeInfo.nBackImageBlockCount = NUM_OF_STATES;					// 배경 이미지 블럭 개수
	m_stThemeInfo.nBackImageStateBlock[ST_NORMAL] = 0;			// 배경 이미지 상태별 블럭 번호 - Normal
	m_stThemeInfo.nBackImageStateBlock[ST_HOVER] = 1;			// 배경 이미지 상태별 블럭 번호 - Hover
	m_stThemeInfo.nBackImageStateBlock[ST_PRESSED] = 2;			// 배경 이미지 상태별 블럭 번호 - Pressed
	m_stThemeInfo.nBackImageStateBlock[ST_DISABLED] = 3;		// 배경 이미지 상태별 블럭 번호 - Disabled
	m_stThemeInfo.crBackground[ST_NORMAL] = RGB(64,192,255);	// 배경 컬러
	m_stThemeInfo.crBackground[ST_HOVER] = RGB(0,128,255);		// 배경 컬러
	m_stThemeInfo.crBackground[ST_PRESSED] = RGB(255,192,64);	// 배경 컬러
	m_stThemeInfo.crBackground[ST_DISABLED] = RGB(192,192,192);	// 배경 컬러
	m_stThemeInfo.crOutlineLT = RGB(192,192,192);				// 테두리 컬러(Left-Top)
	m_stThemeInfo.crOutlineRB = RGB(0,0,0);						// 테두리 컬러(Right-Bottom)
	m_stThemeInfo.nOutlineThick = 2;							// 테두리 두께
	m_stThemeInfo.nEdgeRoundValue = 0;							// 모서리 둥근 정보
	m_stThemeInfo.nBtnState = ST_NORMAL;						// 버튼 상태(Normal, Pressed, Hover, Disabled)
	m_stThemeInfo.bPressed = FALSE;								// 마우스 클릭 상태
	m_stThemeInfo.bHover = FALSE;								// 마우스 Hover 상태
	m_stThemeInfo.rInitPos = CRect(-1,-1,-1,-1);				// 초기 위치 정보
	
}

CThemeButton::~CThemeButton()
{
}


BEGIN_MESSAGE_MAP(CThemeButton, CButton)
	//{{AFX_MSG_MAP(CThemeButton)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThemeButton message handlers

// 클래스 생성
BOOL CThemeButton::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CButton::Create(NULL, dwStyle, rect, pParentWnd, nID);
}

// 클래스 생성
/*BOOL CThemeButton::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, pContext);
}*/

// 클래스 생성시 실행되는 함수
int CThemeButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	/*// Caption 영역좌표 설정
	CRect rScreen;
	this->GetClientRect(&rScreen);
	m_stThemeInfo.rCaption.right = rScreen.right - m_stThemeInfo.rCaption.left;
	m_stThemeInfo.rCaption.bottom = rScreen.bottom - m_stThemeInfo.rCaption.top;*/
	
	return 0;
}

// 컨트롤 화면 크기 변경시 실행되는 함수
void CThemeButton::OnSize(UINT nType, int cx, int cy) 
{
	CButton::OnSize(nType, cx, cy);
	
	int nLimitSize=8;			// 최소 크기

	if( cx < nLimitSize || cy < nLimitSize )
		return;
	
	// 화면을 새로 그린다
	Invalidate(TRUE);

	// 툴팁 변경
	if( m_ToolTip.GetSafeHwnd() )
	{
		CString strToolTip;
		// 현재 툴팁 캡션을 구한다
		strToolTip = m_ToolTip.GetToolTipText();
		// 툴팁 캡션 변경함수를 호출
		ChangeToolTip(strToolTip);
	}		
}

// 배경을 지울때 실행되는 함수
BOOL CThemeButton::OnEraseBkgnd(CDC* pDC) 
{
	// 화면 번쩍거림을 방지
	return true;
	//return CButton::OnEraseBkgnd(pDC);
}

void CThemeButton::PreSubclassWindow() 
{
	// 기본 윈도우 지원 스킨을 방지
	ModifyStyle(0, BS_OWNERDRAW | BS_PUSHBUTTON);
	
	CButton::PreSubclassWindow();
}

void CThemeButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	
}

// WM_PAINT 이벤트 발생시 실행되는 함수
void CThemeButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// 컨트롤 전체를 그리는 함수
	DrawControlTotal(&dc);
}

// 컨트롤 전체를 그리는 함수
void CThemeButton::DrawControlTotal(CPaintDC *pdc)
{
	// 화면 영역좌표 구하기
	CRect rScreen;
	this->GetClientRect(&rScreen);

	// 현재 컨트롤이 비활성 상태라면
	if( !this->IsWindowEnabled() && m_stThemeInfo.nBtnState != ST_DISABLED )
		m_stThemeInfo.nBtnState = ST_DISABLED;
	// 현재 컨트롤이 활성 상태라면
	if( this->IsWindowEnabled() && m_stThemeInfo.nBtnState == ST_DISABLED )
		m_stThemeInfo.nBtnState = ST_NORMAL;

	// 배경 그리기 방식에 따라서
	switch( m_stThemeInfo.nBackDrawType )
	{
	// 컬러로 배경을 그린다면
	case dfBACK_DRAW_TYPE_COLOR :
		// 컬러로 배경을 그린다
		DrawBackByColor(pdc, m_stThemeInfo.crBackground[m_stThemeInfo.nBtnState], rScreen);
		break;
	// 이미지로 배경을 그린다면
	case dfBACK_DRAW_TYPE_IMAGE :
		// 비트맵 이미지(포커스 설정용)로 배경을 그린다
		DrawBackByImage(pdc, &m_stThemeInfo.bmpBack, rScreen);
		break;
	// 배경을 그리지 않는다면
	default :
		break;
	}
	
	// Caption 문자열을 화면에 표시한다
	DrawCaptionText(pdc, rScreen);

}

// 컬러로 배경을 그린다
void CThemeButton::DrawBackByColor(CPaintDC *pdc, COLORREF crBack, CRect rScreen)
{
	COLORREF crOutlineLT, crOutlineRB;

	// 모서리가 둥글다면
	if( m_stThemeInfo.nEdgeRoundValue > 0 )
	{
		// 모서리가 둥근 사각형을 그리는 함수
		gf_DrawRoundEdgeRect(pdc, m_stThemeInfo.crBackground[m_stThemeInfo.nBtnState], m_stThemeInfo.crOutlineLT, m_stThemeInfo.nOutlineThick, m_stThemeInfo.nEdgeRoundValue, rScreen);
	}
	// 모서리가 직각이라면
	else
	{
		if( m_stThemeInfo.nBtnState == ST_PRESSED )
		{
			crOutlineLT = m_stThemeInfo.crOutlineRB;
			crOutlineRB = m_stThemeInfo.crOutlineLT;
		}
		else
		{
			crOutlineLT = m_stThemeInfo.crOutlineLT;
			crOutlineRB = m_stThemeInfo.crOutlineRB;
		}
		//rScreen.InflateRect(-1,-1,-1,-1);
		// 테두리를 가지는 사각형을 그리는 함수
		gf_DrawRectWithOutline(pdc, m_stThemeInfo.crBackground[m_stThemeInfo.nBtnState], crOutlineLT, crOutlineRB, m_stThemeInfo.nOutlineThick, rScreen);
	}
}

// Caption 문자열을 화면에 표시한다
void CThemeButton::DrawCaptionText(CPaintDC *pdc, CRect rScreen)
{
	CRect rCaption, rShadow;

	// Caption 영역좌표를 절대 영역좌표로 변환
	rCaption = gf_RelativeToAbsolute(m_stThemeInfo.rCaption, this);
	// Caption 영역좌표가 설정되지 않았다면 화면 영역으로 대체한다
	if( rCaption.left < 0 || rCaption.top < -1 )
		rCaption = rScreen;

	// 버튼 Pressed 상태라면
	if( m_stThemeInfo.nBtnState == ST_PRESSED )
	{
		rCaption.left ++;
		rCaption.top ++;
		rCaption.right ++;
		rCaption.bottom ++;
	}
	
	CString strText[50], strTemp;
	int nLineEnd=0, nLineCount=0, i=0;
	CRect rText[50];

	// Caption 문자열을 구한다
	strTemp = m_stThemeInfo.strCaption;
	
	// 캡션 텍스트가 여러줄 일경우에 각 라인을 분할
	do {
		if( (nLineEnd = strTemp.Find(TEXT("\n"))) >= 0 )
		{
			strText[nLineCount] = strTemp.Left(nLineEnd);
			strTemp = strTemp.Mid(nLineEnd+1);
		}
		else if( (nLineEnd = strTemp.Find(TEXT("\\n"))) >= 0 )
		{
			strText[nLineCount] = strTemp.Left(nLineEnd);
			strTemp = strTemp.Mid(nLineEnd+2);
		}
		else
		{
			strText[nLineCount] = strTemp;
			strTemp = "";
		}

		nLineCount ++;
		
	} while( strTemp.GetLength() > 0 );

	if( nLineCount < 1 )
		return;

	double fTextAreaHeight=0.;

	// 문자열 한줄의 높이
	fTextAreaHeight = (double)rCaption.Height() / (double)nLineCount;
	
	// 각 문자열 라인을 표시할 영역좌표 계산
	rText[0] = rCaption;
	//rText[0].top = rCaption.top + ((rCaption.Height() - (nTextAreaHeight * nLineCount)) / 2);
	rText[0].bottom = rText[0].top + (int)fTextAreaHeight;

	for(i=1 ; i < nLineCount ; i++)
	{
		rText[i] = rCaption;
		rText[i].top = rText[0].top + (int)(fTextAreaHeight * (double)i);
		rText[i].bottom = rText[i].top + (int)fTextAreaHeight;
	}

	// Caption 문자를 화면에 표시
	for(i=0 ; i < nLineCount ; i++)
	{
		// Caption 그림자 컬러가 투명이 아니라면
		if( m_stThemeInfo.crShadow[m_stThemeInfo.nBtnState] != dfCOLOR_TRANSPARENT )
		{
			// Caption 그림자 영역좌표를 구한다
			rShadow = rText[i];
			rShadow.left ++;
			rShadow.top ++;
			rShadow.right ++;
			rShadow.bottom ++;
			
			// Caption 그림자를 화면에 표시
			gf_DrawText(pdc, strText[i], m_stThemeInfo.nFontSize, 
				m_stThemeInfo.crShadow[m_stThemeInfo.nBtnState], rShadow, 
				m_stThemeInfo.nAlignFormat|DT_SINGLELINE, m_stThemeInfo.bFontBold);
		}

		// Caption 텍스트를 화면에 표시
		gf_DrawText(pdc, strText[i], m_stThemeInfo.nFontSize, 
				m_stThemeInfo.crFont[m_stThemeInfo.nBtnState], rText[i], 
				m_stThemeInfo.nAlignFormat|DT_SINGLELINE, m_stThemeInfo.bFontBold);
	}
}

// Caption 문자열 설정
void CThemeButton::SetCaptionText(CString strCaption)
{
	// 전역변수에 저장
	m_stThemeInfo.strCaption = strCaption;

	// 화면을 새로 그린다
	RedrawCtrl();
	// 윈도우 Text로 설정
	//this->SetWindowText(strCaption);
}

// Caption 문자열 설정
void CThemeButton::SetWindowText(CString strText)
{
	SetCaptionText(strText);
}

// Caption 영역좌표 설정
void CThemeButton::SetCaptionArea(CRect rCaption)
{
	// 전역변수에 저장
	m_stThemeInfo.rCaption = rCaption;
}

// Caption 정렬 방식 설정
void CThemeButton::SetAlignFormat(UINT nAlignFormat)
{
	// 전역변수에 저장
	m_stThemeInfo.nAlignFormat = nAlignFormat;
}

// 폰트 크기 설정
void CThemeButton::SetFontSize(int nFontSize)
{
	m_stThemeInfo.nFontSize = nFontSize;
}

// 폰트 두께 설정
void CThemeButton::SetFontBold(BOOL bBold)
{
	m_stThemeInfo.bFontBold = bBold;
}

// 배경 모서리 둥근 정도 설정
void CThemeButton::SetEdgeRoundValue(int nRoundValue)
{
	m_stThemeInfo.nEdgeRoundValue = nRoundValue;
}

// 외곽선 두께 설정
void CThemeButton::SetOutlineThick(int nThick)
{
	m_stThemeInfo.nOutlineThick = nThick;
}

// 외곽선(Left-Top) 컬러 설정
void CThemeButton::SetOutlineLTColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineLT = crOutline;
}

// 외곽선(Right-Bottom) 컬러 설정
void CThemeButton::SetOutlineRBColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineRB = crOutline;
}

// 외곽선 컬러 설정
void CThemeButton::SetOutlineColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineLT = crOutline;
	m_stThemeInfo.crOutlineRB = crOutline;
}

// 배경 그리기 방식 설정
void CThemeButton::SetBackDrawType(int nDrawType)
{
	m_stThemeInfo.nBackDrawType = nDrawType;
}

// 배경 이미지 채우기 방식 설정
void CThemeButton::SetBackImageFillType(int nImageFillType)
{
	m_stThemeInfo.nBackImageFillType = nImageFillType;
}

// 배경 이미지 정렬방식 설정
void CThemeButton::SetBackImageAlignFormat(UINT nAlignFormat)
{
	m_stThemeInfo.nBackImageAlignFormat = nAlignFormat;
}

// 툴팁을 생성한다
void CThemeButton::MakeToolTip(CString strToolTip)
{
	// 현 컨트롤 ID를 구한다
	UINT unWindowID = (UINT)this->GetSafeHwnd();

	// 툴팁 생성
	m_ToolTip.CreateToolTip(this, unWindowID, strToolTip);
}

// 툴팁 텍스트 변경
void CThemeButton::ChangeToolTip(CString strToolTip)
{
	// 툴팁 텍스트 변경
	m_ToolTip.ChangeToolTip(strToolTip);
}

// 툴팁 삭제
void CThemeButton::DeleteToolTip()
{
	if( !m_ToolTip.GetSafeHwnd() )
		return;

	m_ToolTip.DelTool(this);
}

// 클래스 종료시에 실행되는 함수
void CThemeButton::OnDestroy() 
{
	CButton::OnDestroy();

	// 툴팁 삭제
	DeleteToolTip();	
}

// 툴팁을 위한 이벤트를 받는 함수
LRESULT CThemeButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
				if( m_ToolTip.GetSafeHwnd() )
				{
					MSG msg;
					msg.hwnd = m_hWnd;
					msg.message = message;
					msg.wParam = wParam;
					msg.lParam = lParam;

					m_ToolTip.RelayEvent(&msg);
				}
			}
			break;
		default:
			break;
	}
	
	return CButton::WindowProc(message, wParam, lParam);
}

// 폰트 컬러 설정
void CThemeButton::SetFontAndShadowColor(COLORREF crFont, COLORREF crShadow)
{
	// State Normal
	m_stThemeInfo.crFont[ST_NORMAL] = crFont;
	m_stThemeInfo.crShadow[ST_NORMAL] = crShadow;
	
	// State Hover
	m_stThemeInfo.crFont[ST_HOVER] = crFont;
	m_stThemeInfo.crShadow[ST_HOVER] = crShadow;

	// State Pressed
	m_stThemeInfo.crFont[ST_PRESSED] = crFont;
	m_stThemeInfo.crShadow[ST_PRESSED] = crShadow;

	// State Disabled
	m_stThemeInfo.crFont[ST_DISABLED] = crFont;
	m_stThemeInfo.crShadow[ST_DISABLED] = crShadow;

}

// 폰트 특성 설정
void CThemeButton::SetFont(int nFontSize, BOOL bFontBold, COLORREF crFont, COLORREF crShadow, UINT nAlignFormat)
{
	// 폰트 특성을 전역변수에 저장
	m_stThemeInfo.nFontSize = nFontSize;
	m_stThemeInfo.bFontBold = bFontBold;
	m_stThemeInfo.nAlignFormat = nAlignFormat;
	// 폰트&그림자 컬러 설정
	SetFontAndShadowColor(crFont, crShadow);
	
}

// 배경 컬러 설정
void CThemeButton::SetBackColor(COLORREF crBackNormal, COLORREF crBackHover, COLORREF crBackPressed, COLORREF crBackDisabled)
{
	m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_COLOR;		// 배경 그리기 방식

	// State Normal
	m_stThemeInfo.crBackground[ST_NORMAL] = crBackNormal;
	
	// State Hover
	if( crBackHover == dfCOLOR_TRANSPARENT )
		m_stThemeInfo.crBackground[ST_HOVER] = crBackNormal;
	else
		m_stThemeInfo.crBackground[ST_HOVER] = crBackHover;

	// State Pressed
	if( crBackPressed == dfCOLOR_TRANSPARENT )
		m_stThemeInfo.crBackground[ST_PRESSED] = crBackNormal;
	else
		m_stThemeInfo.crBackground[ST_PRESSED] = crBackPressed;

	// State Disabled
	if( crBackDisabled == dfCOLOR_TRANSPARENT )
		m_stThemeInfo.crBackground[ST_DISABLED] = crBackNormal;
	else
		m_stThemeInfo.crBackground[ST_DISABLED] = crBackDisabled;

}

// 마우스 포인터가 Hover 상태인지 체크
void CThemeButton::CheckHover(CPoint point)
{
	if (!::IsWindow(GetSafeHwnd()))
		return;

	// hover상태에서 Disable시 Click가능 현상을 없애준다.
	if (!IsWindowEnabled())
	{
		//m_bHover = false;
		return;
	}

	TRACKMOUSEEVENT EventTrack1, EventTrack2;
	EventTrack1.cbSize = sizeof(TRACKMOUSEEVENT);
	EventTrack1.dwFlags = TME_HOVER;
	EventTrack1.hwndTrack = m_hWnd;
	EventTrack1.dwHoverTime = 10;

	_TrackMouseEvent(&EventTrack1);
	
	EventTrack2.cbSize = sizeof(TRACKMOUSEEVENT);
	EventTrack2.dwFlags = TME_LEAVE;
	EventTrack2.hwndTrack = m_hWnd;
	EventTrack2.dwHoverTime = 0;

	_TrackMouseEvent(&EventTrack2);
}

// 마우스 Hover시 실행되는 함수
LRESULT CThemeButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	// 마우스 Hover가 아닌 상태로 설정
	if( !m_stThemeInfo.bHover )
		m_stThemeInfo.bHover = TRUE;

	// 마우스 Pressed 상태라면
	if( m_stThemeInfo.bPressed )
	{
		if( m_stThemeInfo.nBtnState != ST_PRESSED )
			m_stThemeInfo.nBtnState = ST_PRESSED;
	}
	// 마우스 Pressed 상태가 아니라면
	// 버튼 상태를 Hover로 설정
	else if( m_stThemeInfo.nBtnState != ST_HOVER )
	{
		m_stThemeInfo.nBtnState = ST_HOVER;
		// 화면을 새로 그린다
		//RedrawCtrl();
		// 배경을 지우지않고 화면을 새로 그린다
		Invalidate(FALSE);
	}

	return 0;
}

// 마우스 Leave시 실행되는 함수
LRESULT CThemeButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	// 마우스 Hover 상태로 설정
	if( m_stThemeInfo.bHover )
		m_stThemeInfo.bHover = FALSE;

	// 마우스 Pressed 상태라면
	if( m_stThemeInfo.bPressed )
		m_stThemeInfo.bPressed = FALSE;

	// 버튼 상태를 Normal로 설정
	if( m_stThemeInfo.nBtnState != ST_NORMAL )
		m_stThemeInfo.nBtnState = ST_NORMAL;
		
	// 화면을 새로 그린다
	//RedrawCtrl();
	// 배경을 지우지않고 화면을 새로 그린다
	Invalidate(FALSE);

	return 0;
}

// 화면을 새로 그린다
void CThemeButton::RedrawCtrl()
{
	CRect rCtrl;

	// 배경을 그리지 않는다면
	if( m_stThemeInfo.nBackDrawType == dfBACK_DRAW_TYPE_NONE )
	{
		// 컨트롤 위치좌표를 부모 윈도우에 대한 상대좌표로 반환
		rCtrl = GetCtrlRectByParent();
		// 컨트롤의 배경 영역에 해당하는 부모 윈도우를 새로 그린다
		this->GetParent()->InvalidateRect(rCtrl, TRUE);
	}

	// 배경을 지우지않고 화면을 새로 그린다
	Invalidate(FALSE);
}

// 마우스 왼쪽 버튼 클릭 해제시 실행되는 함수
void CThemeButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// 마우스 클릭 상태 해제
	m_stThemeInfo.bPressed = FALSE;

	// 버튼 상태 변경
	if( m_stThemeInfo.bHover )
		m_stThemeInfo.nBtnState = ST_HOVER;
	else
		m_stThemeInfo.nBtnState = ST_NORMAL;

	// 화면을 새로 그린다
	RedrawCtrl();

	CButton::OnLButtonUp(nFlags, point);

	// 부모 윈도우로 마우스 클릭 해제 이벤트 전달
	GetParent()->PostMessage(dfWM_BTN_LBUTTON_UP, WPARAM(this->GetDlgCtrlID()), MAKELPARAM(point.x, point.y));
}

// 마우스 왼쪽 버튼 더블 클릭시 실행되는 함수
void CThemeButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CButton::OnLButtonDblClk(nFlags, point);

	// 부모 윈도우로 마우스 더블 클릭 이벤트 전달
	GetParent()->PostMessage(dfWM_BTN_LBUTTON_DBLCLK, WPARAM(this->GetDlgCtrlID()), MAKELPARAM(point.x, point.y));
}

// 마우스 이동시 실행되는 함수
void CThemeButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// 마우스 클릭 상태라면
	if((nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		// 마우스 Pressed 상태 설정
		if( m_stThemeInfo.bPressed != TRUE )
			m_stThemeInfo.bPressed = TRUE;
	}
	// 마우스 클릭 해제 상태라면
	else
	{
		// 마우스 Pressed 해제 상태로 설정
		if( m_stThemeInfo.bPressed == TRUE )
			m_stThemeInfo.bPressed = FALSE;
	}

	// 마우스 Hover인지 체크
	CheckHover(point);
	
	CButton::OnMouseMove(nFlags, point);
	
	// 부모 윈도우로 마우스 포인트 이동 이벤트 전달
	GetParent()->PostMessage(dfWM_BTN_MOUSE_MOVE, WPARAM(this->GetDlgCtrlID()), MAKELPARAM(point.x, point.y));
	
	// 부모 윈도우쪽 코딩
	// BEGIN_MESSAGE_MAP(C~, C~)
	//		ON_MESSAGE(dfWM_BTN_MOUSE_MOVE, OnBtnMouseMove)
	// END_MESSAGE_MAP()
	//
	// LRESULT C~::OnBtnMouseMove(WPARAM wParam, LPARAM lParam)
	// {
	//		CPoint point;
	//		point = (CPoint)lParam;
	//	
	//		switch( (UINT)wParam )
	//		{
	//		case IDC_BUTTON_CTRL :
	//			~
	//			break;
	//		default :
	//			break;
	//		}
	//	
	//		return TRUE;
	//	}
}

// 배경 이미지를 여는 함수(파일)
BOOL CThemeButton::SetBackImage(CString strFileName)
{
	CString strFilePath;
	BOOL bBmpLoad=FALSE;

	// 배경 그리기 방식이 이미지라면 기존 이미지를 삭제
	if( m_stThemeInfo.nBackDrawType == dfBACK_DRAW_TYPE_IMAGE )
		m_stThemeInfo.bmpBack.DeleteObject();

	// 이미지 파일 전체 경로를 구한다
	//strFilePath = FilteringImageFilePath(strFileName);
	strFilePath = strFileName;

	// 이미지 파일을 읽어서 CBitmap으로 반환
	bBmpLoad = gf_LoadPictureFile(strFilePath, &m_stThemeInfo.bmpBack);

	// 이미지 파일 로딩에 성공했다면
	if( bBmpLoad )
	{
		// 배경 그리기 방식을 이미지로 설정
		m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_IMAGE;
	}
	// 이미지 파일 로딩에 실패했다면
	else
	{
		// 배경 그리기 방식을 Default로 설정
		m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_NONE;
	}

	// 화면을 새로 그린다
	Invalidate(FALSE);

	return bBmpLoad;
}

// 배경 이미지를 여는 함수(Resource ID)
BOOL CThemeButton::SetBackImage(UINT nImageID)
{
	BOOL bBmpLoad=FALSE;

	// 배경 그리기 방식이 이미지라면 기존 이미지를 삭제
	if( m_stThemeInfo.nBackDrawType == dfBACK_DRAW_TYPE_IMAGE )
		m_stThemeInfo.bmpBack.DeleteObject();

	// 이미지를 로딩
	bBmpLoad = m_stThemeInfo.bmpBack.LoadBitmap(nImageID);

	// 이미지 파일 로딩에 성공했다면
	if( bBmpLoad )
	{
		// 배경 그리기 방식을 이미지로 설정
		m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_IMAGE;
	}
	// 이미지 파일 로딩에 실패했다면
	else
	{
		// 배경 그리기 방식을 Default로 설정
		m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_NONE;
	}

	// 화면을 새로 그린다
	Invalidate(FALSE);

	return bBmpLoad;
}

// 배경 이미지 블럭 개수 설정
void CThemeButton::SetBackImageBlockCount(int nBlock)
{
	// 블럭 개수가 범위를 벗어나면 함수 탈출
	if( nBlock < 1 || nBlock > NUM_OF_STATES )
		return;

	m_stThemeInfo.nBackImageBlockCount = nBlock;

	switch( nBlock )
	{
	case 1 :
		m_stThemeInfo.nBackImageStateBlock[ST_NORMAL] = 0;
		m_stThemeInfo.nBackImageStateBlock[ST_HOVER] = 0;
		m_stThemeInfo.nBackImageStateBlock[ST_PRESSED] = 0;
		m_stThemeInfo.nBackImageStateBlock[ST_DISABLED] = 0;
		break;
	case 2 :
		m_stThemeInfo.nBackImageStateBlock[ST_NORMAL] = 0;
		m_stThemeInfo.nBackImageStateBlock[ST_HOVER] = 0;
		m_stThemeInfo.nBackImageStateBlock[ST_PRESSED] = 1;
		m_stThemeInfo.nBackImageStateBlock[ST_DISABLED] = 0;
		break;
	case 3 :
		m_stThemeInfo.nBackImageStateBlock[ST_NORMAL] = 0;
		m_stThemeInfo.nBackImageStateBlock[ST_HOVER] = 1;
		m_stThemeInfo.nBackImageStateBlock[ST_PRESSED] = 2;
		m_stThemeInfo.nBackImageStateBlock[ST_DISABLED] = 0;
		break;
	case 4 :
		m_stThemeInfo.nBackImageStateBlock[ST_NORMAL] = 0;
		m_stThemeInfo.nBackImageStateBlock[ST_HOVER] = 1;
		m_stThemeInfo.nBackImageStateBlock[ST_PRESSED] = 2;
		m_stThemeInfo.nBackImageStateBlock[ST_DISABLED] = 3;
		break;
	default :
		break;
	}
}

// 현 상황에 맞는 이미지 부분 영역좌표를 반환
CRect CThemeButton::GetImagePartByState(int nState)
{
	int nBlockWidth=0, nBlockNo=0;
	CRect rBlock;
	BITMAP bm;
	m_stThemeInfo.bmpBack.GetBitmap(&bm);
	
	// 배경 이미지 블럭 넓이를 구한다
	nBlockWidth = bm.bmWidth / m_stThemeInfo.nBackImageBlockCount;

	// 현재 상태에 맞는 블럭 번호
	nBlockNo = m_stThemeInfo.nBackImageStateBlock[nState];

	// 블럭 이미지 영역좌표를 구한다
	rBlock.top = 0;
	rBlock.bottom = bm.bmHeight;
	rBlock.left = nBlockWidth * nBlockNo;
	rBlock.right = rBlock.left + nBlockWidth;

	return rBlock;
}

// 비트맵 이미지로 배경을 그린다
void CThemeButton::DrawBackByImage(CPaintDC *pdc, CBitmap *pBitmap, CRect rScreen)
{
	// 현 상태에 맞는 이미지의 영역을 구해서 반환하는 함수
	CRect rImgPart = GetImagePartByState(m_stThemeInfo.nBtnState);

	// '배경 이미지 채우기 방식'에 따라서
	switch( m_stThemeInfo.nBackImageFillType )
	{
	case dfBACK_IMAGE_FILL_TYPE_BLOCK :				// 블럭
		// 이미지를 9조각으로 분할하여 화면에 표시하는 함수(투명 적용)
		gf_DrawImagePartSkin(pdc, pBitmap, rImgPart, rScreen);
		break;
	case dfBACK_IMAGE_FILL_TYPE_RESIZE :			// 늘이기
		// 이미지 일부를 크기 변경하여 그리는 함수(투명 지원)
		gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgPart, rScreen);
		break;
	case dfBACK_IMAGE_FILL_TYPE_PATTERN :			// 바둑판식
		// 이미지를 Pattern 브러쉬 형식으로 화면 영역에 그리는 함수(바둑판 형식)
		gf_DrawImageByPatternBrush(pdc, pBitmap, rImgPart, rScreen);
		break;
	case dfBACK_IMAGE_FILL_TYPE_ONCE :				// 1회
		// 이미지를 화면에 그리는 함수(원본 그대로 1회)
		gf_DrawImagePartTransparent(pdc, pBitmap, rImgPart, rScreen, m_stThemeInfo.nBackImageAlignFormat);
		break;
	default :
		break;
	}
}

// 폰트 컬러 설정
void CThemeButton::SetFontColor(COLORREF crFontNormal, COLORREF crFontHover, COLORREF crFontPressed, COLORREF crFontDisabled)
{
	// State Normal
	m_stThemeInfo.crFont[ST_NORMAL] = crFontNormal;

	// State Hover
	if( crFontHover == dfCOLOR_TRANSPARENT )
		m_stThemeInfo.crFont[ST_HOVER] = crFontNormal;
	else
		m_stThemeInfo.crFont[ST_HOVER] = crFontHover;

	// State Pressed
	if( crFontPressed == dfCOLOR_TRANSPARENT )
		m_stThemeInfo.crFont[ST_PRESSED] = crFontNormal;
	else
		m_stThemeInfo.crFont[ST_PRESSED] = crFontPressed;

	// State Disabled
	if( crFontDisabled == dfCOLOR_TRANSPARENT )
		m_stThemeInfo.crFont[ST_DISABLED] = crFontNormal;
	else
		m_stThemeInfo.crFont[ST_DISABLED] = crFontDisabled;

}

// 그림자 컬러 설정
void CThemeButton::SetShadowColor(COLORREF crShadowNormal, COLORREF crShadowHover, COLORREF crShadowPressed, COLORREF crShadowDisabled)
{
	// State Normal
	m_stThemeInfo.crShadow[ST_NORMAL] = crShadowNormal;

	// State Hover
	if( crShadowHover == dfCOLOR_TRANSPARENT )
		m_stThemeInfo.crShadow[ST_HOVER] = crShadowNormal;
	else
		m_stThemeInfo.crShadow[ST_HOVER] = crShadowHover;

	// State Pressed
	if( crShadowPressed == dfCOLOR_TRANSPARENT )
		m_stThemeInfo.crShadow[ST_PRESSED] = crShadowNormal;
	else
		m_stThemeInfo.crShadow[ST_PRESSED] = crShadowPressed;

	// State Disabled
	if( crShadowDisabled == dfCOLOR_TRANSPARENT )
		m_stThemeInfo.crShadow[ST_DISABLED] = crShadowNormal;
	else
		m_stThemeInfo.crShadow[ST_DISABLED] = crShadowDisabled;

}

// 폰트 Hover 컬러 설정
void CThemeButton::SetFontHoverColor(COLORREF crFontHover)
{
	m_stThemeInfo.crFont[ST_HOVER] = crFontHover;
}

// 폰트 Pressed 컬러 설정
void CThemeButton::SetFontPressedColor(COLORREF crFontPressed)
{
	m_stThemeInfo.crFont[ST_PRESSED] = crFontPressed;
}

// 폰트 Disabled 컬러 설정
void CThemeButton::SetFontDisabledColor(COLORREF crFontDisabled)
{
	m_stThemeInfo.crFont[ST_DISABLED] = crFontDisabled;
}

// 그림자 Hover 컬러 설정
void CThemeButton::SetShadowHoverColor(COLORREF crShadowHover)
{
	m_stThemeInfo.crShadow[ST_HOVER] = crShadowHover;
}

// 그림자 Pressed 컬러 설정
void CThemeButton::SetShadowPressedColor(COLORREF crShadowPressed)
{
	m_stThemeInfo.crShadow[ST_PRESSED] = crShadowPressed;
}

// 그림자 Disabled 컬러 설정
void CThemeButton::SetShadowDisabledColor(COLORREF crShadowDisabled)
{
	m_stThemeInfo.crShadow[ST_DISABLED] = crShadowDisabled;
}

// 컨트롤 위치좌표를 부모 윈도우에 대한 상대좌표로 반환
CRect CThemeButton::GetCtrlRectByParent()
{
	CRect rCtrl, rParent;

	// 부모 윈도우의 위치좌표
	this->GetParent()->GetWindowRect(&rParent);

	// 컨트롤의 위치좌표
	this->GetWindowRect(&rCtrl);

	// 부모 윈도우에 대한 상대좌표를 구한다
	rCtrl.left -= rParent.left;
	rCtrl.right -= rParent.left;
	rCtrl.top -= rParent.top;
	rCtrl.bottom -= rParent.top;

	return rCtrl;
}

// 현재 마우스 Hover 상태인지 여부를 반환
BOOL CThemeButton::IsMouseHover()
{
	return m_stThemeInfo.bHover;
}

// 툴팁 문자열 설정
void CThemeButton::SetToolTip(CString strText)
{
	// 툴팁이 이미 생성되어 있다면
	if( m_ToolTip.GetSafeHwnd() )
		// 툴팁 문자열 변경
		ChangeToolTip(strText);
	// 툴팁이 아직 생성되지 않았다면
	else
		// 툴팁 생성
		MakeToolTip(strText);
}

// 마우스 왼쪽 버튼 클릭 다운시 실행되는 함수
void CThemeButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// 마우스 클릭 상태 설정
	m_stThemeInfo.bPressed = TRUE;

	// 버튼 상태 변경
	if( m_stThemeInfo.bHover )
		m_stThemeInfo.nBtnState = ST_PRESSED;
	else
		m_stThemeInfo.nBtnState = ST_NORMAL;

	// 화면을 새로 그린다
	RedrawCtrl();

	// 부모 윈도우로 마우스 클릭 다운 이벤트 전달
	GetParent()->PostMessage(dfWM_BTN_LBUTTON_DOWN, WPARAM(this->GetDlgCtrlID()), MAKELPARAM(point.x, point.y));

	CButton::OnLButtonDown(nFlags, point);
	
	// 메뉴를 화면에 보여주는 함수
	ShowMenu();

}

// 메뉴 설정
void CThemeButton::SetMenu(CThemeMenu *pMenu)
{
	m_pMenu = pMenu;
}

// 메뉴를 화면에 보여주는 함수
void CThemeButton::ShowMenu()
{
	// 메뉴가 존재하지 않는다면 함수 탈출
	if( m_pMenu == NULL )
		return;

	CRect rScreen;
	CPoint poMenu;

	// 버튼 컨트롤 위치좌표를 구한다
	this->GetWindowRect(&rScreen);
	// 절대좌표를 부모 윈도우에 대한 상대좌표로 변경
	GetParent()->ScreenToClient(&rScreen);

	// 메뉴를 표시할 위치좌표를 구한다
	poMenu.x = rScreen.left;
	poMenu.y = rScreen.bottom;

	// 메뉴를 화면에 표시
	m_pMenu->ShowSubMenu(poMenu);
}

// 활성/비활성 이벤트 발생시 실행되는 함수
void CThemeButton::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
	// 화면을 새로 그린다
	RedrawCtrl();
	
}
