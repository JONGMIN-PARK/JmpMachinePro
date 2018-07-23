// ThemeProgress.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "ThemeProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib,"msimg32.lib")

/////////////////////////////////////////////////////////////////////////////
// CThemeProgress

CThemeProgress::CThemeProgress()
{
	m_stThemeInfo.strIniCtrlName = "";						// INI 파일 조회용 컨트롤명
	m_stThemeInfo.fBarColorChangePercent = 90.0;			// 막대 색깔 변경 위치
	m_stThemeInfo.nMin = 0;									// 최소값
	m_stThemeInfo.nMax = 100;								// 최대값
	m_stThemeInfo.nPos = 70;								// 현재 진행율값
	m_stThemeInfo.strTextUnit = "";							// 문자열 단위
	
	// 테마 관련 변수 초기화
	InitThemeVariable();
	
}

// 테마 변수 초기화
void CThemeProgress::InitThemeVariable()
{

	// 환경변수 초기화
	m_stThemeInfo.nOutlineThick = 1;						// 경계선 두께
	m_stThemeInfo.crOutlineLT = RGB(0,0,0);					// 경계선 색깔 - 왼쪽 & 위쪽
	m_stThemeInfo.crOutlineRB = RGB(0,0,0);					// 경계선 색깔 - 오른쪽 & 아래쪽
	m_stThemeInfo.crBack = RGB(127,127,127);				// 배경 색깔
	m_stThemeInfo.bGradient = FALSE;						// 그라데이션 여부

	m_stThemeInfo.crBarColor1_Up = RGB(85,189,66);			// 막대 색깔-1 위쪽 부분
	m_stThemeInfo.crBarColor1_Down = RGB(42,173,19);		// 막대 색깔-1 아래쪽 부분
	m_stThemeInfo.crBarColor1_Outline = RGB(100,219,19);	// 막대 색깔-1 테두리
	m_stThemeInfo.crBarColor2_Up = RGB(222,183,54);			// 막대 색깔-2 위쪽 부분
	m_stThemeInfo.crBarColor2_Down = RGB(213,165,3);		// 막대 색깔-2 아래쪽 부분
	m_stThemeInfo.crBarColor2_Outline = RGB(255,198,0);		// 막대 색깔-2 테두리

	m_stThemeInfo.bDrawText = FALSE;						// 문자열 표시 여부
	m_stThemeInfo.nFontSize = 12;							// 폰트 크기
	m_stThemeInfo.bFontBold = FALSE;						// 폰트 두께
	m_stThemeInfo.crFont = RGB(255,255,255);				// 폰트 컬러
	m_stThemeInfo.crShadow = dfCOLOR_TRANSPARENT;			// 그림자 컬러
	m_stThemeInfo.rInitPos = CRect(-1,-1,-1,-1);			// 초기 위치 정보

}

CThemeProgress::~CThemeProgress()
{
}


BEGIN_MESSAGE_MAP(CThemeProgress, CWnd)
	//{{AFX_MSG_MAP(CThemeProgress)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CThemeProgress message handlers

// 외곽선을 그리는 함수
void CThemeProgress::DrawOutline(CPaintDC *pdc)
{
	CRect rWnd;
	
	//클래스의 크기를 구한다
	this->GetClientRect(&rWnd);

	rWnd.right --;
	rWnd.bottom --;

	for(int i=0 ; i < m_stThemeInfo.nOutlineThick ; i++)
	{
		// 테두리 선을 그린다
		gf_DrawRectLine(rWnd, m_stThemeInfo.crOutlineLT, m_stThemeInfo.crOutlineLT, 
					m_stThemeInfo.crOutlineRB, m_stThemeInfo.crOutlineRB, pdc);
		//사각형 좌표 변경
		rWnd.left ++;
		rWnd.top ++;
		rWnd.right --;
		rWnd.bottom --;
	}
}

// 컨트롤 경계선 정보를 받아들이는 함수
void CThemeProgress::SetOutline(COLORREF crOutlineLT, COLORREF crOutlineRB, int nOutlineThick, COLORREF crBack)
{
	m_stThemeInfo.crOutlineLT = crOutlineLT;
	m_stThemeInfo.crOutlineRB = crOutlineRB;
	m_stThemeInfo.nOutlineThick = nOutlineThick;
	m_stThemeInfo.crBack = crBack;
}

// 범위를 지정하는 함수
void CThemeProgress::SetRange(int nMin, int nMax)
{
	m_stThemeInfo.nMin = nMin;
	m_stThemeInfo.nMax = nMax;
}

// 위치를 지정하는 함수
void CThemeProgress::SetPos(int nPos)
{
	m_stThemeInfo.nPos = nPos;
	Invalidate(FALSE);
}

// 위치 값을 반환하는 함수
int CThemeProgress::GetPos()
{
	return m_stThemeInfo.nPos;
}

// 범위를 반환하는 함수
void CThemeProgress::GetRange(int &nMin, int &nMax)
{
	nMin = m_stThemeInfo.nMin;
	nMax = m_stThemeInfo.nMax;
}

// 클래스 생성
BOOL CThemeProgress::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, pContext);
}

// 막대바 색깔 설정 - 색깔 2개
void CThemeProgress::SetBarColor(COLORREF crBar1, COLORREF crBar2, double fColorChangePercent)
{
	m_stThemeInfo.crBarColor1_Up = crBar1;
	m_stThemeInfo.crBarColor1_Down = crBar1;
	m_stThemeInfo.crBarColor1_Outline = crBar1;
	m_stThemeInfo.crBarColor2_Up = crBar2;
	m_stThemeInfo.crBarColor2_Down = crBar2;
	m_stThemeInfo.crBarColor2_Outline = crBar2;
	m_stThemeInfo.fBarColorChangePercent = fColorChangePercent;
}

// 막대바 색깔 설정 - 색깔 6개
void CThemeProgress::SetBarColor(COLORREF crBar1Up, COLORREF crBar1Down, COLORREF crBar1Outline, COLORREF crBar2Up, COLORREF crBar2Down, COLORREF crBar2Outline, double fColorChangePercent)
{
	m_stThemeInfo.crBarColor1_Up = crBar1Up;
	m_stThemeInfo.crBarColor1_Down = crBar1Down;
	m_stThemeInfo.crBarColor1_Outline = crBar1Outline;
	m_stThemeInfo.crBarColor2_Up = crBar2Up;
	m_stThemeInfo.crBarColor2_Down = crBar2Down;
	m_stThemeInfo.crBarColor2_Outline = crBar2Outline;
	m_stThemeInfo.fBarColorChangePercent = fColorChangePercent;
}

// 막대바 색깔 변경 위치(%) 반환
double CThemeProgress::GetBarColorChangePercent()
{
	return m_stThemeInfo.fBarColorChangePercent;
}

// 막대바 색깔 변경 위치(%) 설정
void CThemeProgress::SetColorChangePercent(double fColorChangePercent)
{
	m_stThemeInfo.fBarColorChangePercent = fColorChangePercent;
	// 화면을 새로 그린다
	Invalidate(FALSE);
}

// Paint 이벤트 발생시 실행되는 함수
void CThemeProgress::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if( m_stThemeInfo.bGradient )
		// 막대바를 그리는 함수(그라데이션 효과)
		DrawBarGradient(&dc);
	else
		// 막대를 그리는 함수
		DrawBar(&dc);

	// 외곽선을 그리는 함수
	DrawOutline(&dc);
	
	if( m_stThemeInfo.bDrawText )
		// 현재 값을 문자열로 화면에 표시
		DrawPosText(&dc);

}

// 막대바를 그리는 함수
void CThemeProgress::DrawBar(CPaintDC *pdc)
{
	CRect rWnd, rBarUp, rBarDown, rBackGround;
	
	// 클래스의 크기를 구한다
	this->GetClientRect(&rWnd);

	// 클래스 영역에서 테두리 두께를 제외한다
	rWnd.left += m_stThemeInfo.nOutlineThick;
	rWnd.right -= m_stThemeInfo.nOutlineThick;
	rWnd.top += m_stThemeInfo.nOutlineThick;
	rWnd.bottom -= m_stThemeInfo.nOutlineThick;

	double fPercent=0.0;
	COLORREF crBarUp, crBarDown, crBarOutline;
	int nPos=m_stThemeInfo.nPos;

	if( nPos < m_stThemeInfo.nMin )
		nPos = m_stThemeInfo.nMin;
	if( nPos > m_stThemeInfo.nMax )
		nPos = m_stThemeInfo.nMax;
	
	// 현재 퍼센트
	fPercent = (double)(nPos - m_stThemeInfo.nMin) / (double)(m_stThemeInfo.nMax - m_stThemeInfo.nMin);
	
	// 막대바의 영역좌표를 구한다
	rBarUp = rWnd;
	rBarUp.right = rBarUp.left + (int)((double)rWnd.Width() * fPercent);
	rBarDown = rBarUp;
	// 막대바 위쪽 영역
	rBarUp.bottom = rBarUp.top + (rBarUp.Height() / 2);
	// 막대바 아래쪽 영역
	rBarDown.top = rBarUp.bottom;

	// 막대바 색깔 - 색깔 변경 범위를 초과하지 않았다면
	if( (fPercent*100.) <= m_stThemeInfo.fBarColorChangePercent )
	{
		crBarUp =  m_stThemeInfo.crBarColor1_Up;
		crBarDown = m_stThemeInfo.crBarColor1_Down;
		crBarOutline = m_stThemeInfo.crBarColor1_Outline;
	}
	// 막대바 색깔 - 색깔 변경 범위를 초과했다면
	else
	{
		crBarUp =  m_stThemeInfo.crBarColor2_Up;
		crBarDown = m_stThemeInfo.crBarColor2_Down;
		crBarOutline = m_stThemeInfo.crBarColor2_Outline;
	}
	
	// 화면에 막대바를 그린다
	pdc->FillRect(rBarUp, &CBrush(crBarUp));
	pdc->FillRect(rBarDown, &CBrush(crBarDown));

	// 막대바 테두리 선을 그린다
	rBarUp.bottom = rBarDown.bottom-1;
	rBarUp.right --;
	gf_DrawRectLine(rBarUp, crBarOutline, crBarOutline, crBarOutline, crBarOutline, pdc);

	// 배경 위영역좌표를 구한다
	rBackGround = rWnd;
	rBackGround.left = rBarUp.right+1;
	// 화면에 배경을 그린다
	if( rBackGround.right > rBackGround.left)
		pdc->FillRect(rBackGround, &CBrush(m_stThemeInfo.crBack));

}

// 막대바를 그리는 함수(그라데이션 효과)
void CThemeProgress::DrawBarGradient(CPaintDC *pdc)
{
	CRect rWnd;
	
	// 클래스의 크기를 구한다
	this->GetClientRect(&rWnd);

	// 클래스 영역에서 테두리 두께를 제외한다
	rWnd.left += m_stThemeInfo.nOutlineThick;
	rWnd.right -= m_stThemeInfo.nOutlineThick;
	rWnd.top += m_stThemeInfo.nOutlineThick;
	rWnd.bottom -= m_stThemeInfo.nOutlineThick;

	// 사각형 그라디에이션을 그리는 함수
	DrawGradientRect(rWnd, m_stThemeInfo.crBarColor1_Down, m_stThemeInfo.crBarColor2_Down, 0, pdc);

	double fPercent=0.0;
	int nPos=m_stThemeInfo.nPos;

	if( nPos < m_stThemeInfo.nMin )
		nPos = m_stThemeInfo.nMin;
	if( nPos > m_stThemeInfo.nMax )
		nPos = m_stThemeInfo.nMax;
	
	// 현재 퍼센트
	fPercent = (double)(nPos - m_stThemeInfo.nMin) / (double)(m_stThemeInfo.nMax - m_stThemeInfo.nMin);
	
	// 막대바 이외의 배경 영역
	rWnd.left = rWnd.left + (int)((double)rWnd.Width() * fPercent);
	// 화면에 배경을 그린다
	if( rWnd.right > rWnd.left)
		pdc->FillRect(rWnd, &CBrush(m_stThemeInfo.crBack));
}

// 사각형 그라디에이션을 그리는 함수
void CThemeProgress::DrawGradientRect(CRect rArea, COLORREF crBegin, COLORREF crEnd, int nDirection, CPaintDC *pdc)
{
	HMODULE hinst_msimg32 = LoadLibrary( TEXT("msimg32.dll") );

	if(hinst_msimg32)
	{
		TRIVERTEX data[2];
		GRADIENT_RECT draw_rect;
		data[0].x = rArea.left;
		data[0].y = rArea.top;

		data[0].Red = GetRValue(crBegin) << 8;
		data[0].Green = GetGValue(crBegin) << 8;
		data[0].Blue = GetBValue(crBegin) << 8;
		data[0].Alpha = 0x0000;

		data[1].x = rArea.right;
		data[1].y = rArea.bottom;

		data[1].Red = GetRValue(crEnd) << 8;
		data[1].Green = GetGValue(crEnd) << 8;
		data[1].Blue = GetBValue(crEnd) << 8;
		data[1].Alpha = 0x0000;

		draw_rect.UpperLeft  = 0;
		draw_rect.LowerRight = 1;
		// 그라데이션 방향 결정 - 0이면 수평 / 0이 아니면 수직
		ULONG nGradientDirection = (nDirection == 0 ) ? GRADIENT_FILL_RECT_H : GRADIENT_FILL_RECT_V;
		BOOL bFlag = GradientFill(pdc->m_hDC, data, 2, &draw_rect, 1, nGradientDirection);
	}

	FreeLibrary( hinst_msimg32 );

}

// 그라데이션 정보를 설정하는 함수
void CThemeProgress::SetGradientInfo(BOOL bGradient, COLORREF crBegin, COLORREF crEnd)
{
	m_stThemeInfo.bGradient = bGradient;				// 그라데이션 여부
	m_stThemeInfo.crBarColor1_Up = crBegin;
	m_stThemeInfo.crBarColor1_Down = crBegin;
	m_stThemeInfo.crBarColor1_Outline = crBegin;
	m_stThemeInfo.crBarColor2_Up = crEnd;
	m_stThemeInfo.crBarColor2_Down = crEnd;
	m_stThemeInfo.crBarColor2_Outline = crEnd;
}

// 툴팁을 생성한다
void CThemeProgress::MakeToolTip(CString strToolTip)
{
	// 현 컨트롤 ID를 구한다
	UINT unWindowID = (UINT)this->GetSafeHwnd();

	// 툴팁 생성
	m_ToolTip.CreateToolTip(this, unWindowID, strToolTip);
}

// 툴팁 텍스트 변경
void CThemeProgress::ChangeToolTip(CString strToolTip)
{
	// 툴팁 텍스트 변경
	m_ToolTip.ChangeToolTip(strToolTip);
}

// 컨트롤 화면 크기 변경시 실행되는 함수
void CThemeProgress::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
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

// 툴팁 삭제
void CThemeProgress::DeleteToolTip()
{
	if( !m_ToolTip.GetSafeHwnd() )
		return;

	m_ToolTip.DelTool(this);
}

// 클래스 종료시에 실행되는 함수
void CThemeProgress::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// 툴팁 삭제
	DeleteToolTip();	
}

// 툴팁을 위한 마우스 이벤트를 받는 함수
LRESULT CThemeProgress::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
	
	return CWnd::WindowProc(message, wParam, lParam);
}

// 'Bar1-윗부분' 컬러 설정
void CThemeProgress::SetBar1UpColor(COLORREF crBar1Up)
{
	m_stThemeInfo.crBarColor1_Up = crBar1Up;
}

// 'Bar1-아래부분' 컬러 설정
void CThemeProgress::SetBar1DownColor(COLORREF crBar1Down)
{
	m_stThemeInfo.crBarColor1_Down = crBar1Down;
}

// 'Bar1-Outline' 컬러 설정
void CThemeProgress::SetBar1OutlineColor(COLORREF crBar1Outline)
{
	m_stThemeInfo.crBarColor1_Outline = crBar1Outline;
}

// 'Bar2-윗부분' 컬러 설정
void CThemeProgress::SetBar2UpColor(COLORREF crBar2Up)
{
	m_stThemeInfo.crBarColor2_Up = crBar2Up;
}

// 'Bar2-아래부분' 컬러 설정
void CThemeProgress::SetBar2DownColor(COLORREF crBar2Down)
{
	m_stThemeInfo.crBarColor2_Down = crBar2Down;
}

// 'Bar2-Outline' 컬러 설정
void CThemeProgress::SetBar2OutlineColor(COLORREF crBar2Outline)
{
	m_stThemeInfo.crBarColor2_Outline = crBar2Outline;
}

// 막대바 컬러 설정 - 색깔 1개
void CThemeProgress::SetBarColor(COLORREF crBar)
{
	m_stThemeInfo.crBarColor1_Up = crBar;
	m_stThemeInfo.crBarColor1_Down = crBar;
	m_stThemeInfo.crBarColor1_Outline = crBar;
	m_stThemeInfo.crBarColor2_Up = crBar;
	m_stThemeInfo.crBarColor2_Down = crBar;
	m_stThemeInfo.crBarColor2_Outline = crBar;
}

// 막대바-1 컬러 설정
void CThemeProgress::SetBar1Color(COLORREF crBar1)
{
	m_stThemeInfo.crBarColor1_Up = crBar1;
	m_stThemeInfo.crBarColor1_Down = crBar1;
	m_stThemeInfo.crBarColor1_Outline = crBar1;
}

// 막대바-2 컬러 설정
void CThemeProgress::SetBar2Color(COLORREF crBar2)
{
	m_stThemeInfo.crBarColor2_Up = crBar2;
	m_stThemeInfo.crBarColor2_Down = crBar2;
	m_stThemeInfo.crBarColor2_Outline = crBar2;
}

// 외곽선 두께 설정
void CThemeProgress::SetOutlineThick(int nOutlineThick)
{
	m_stThemeInfo.nOutlineThick = nOutlineThick;
}

// 외곽선 컬러 설정
void CThemeProgress::SetOutlineColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineLT = crOutline;
	m_stThemeInfo.crOutlineRB = crOutline;
}

// 외곽선(Left-Top) 컬러 설정
void CThemeProgress::SetOutlineLTColor(COLORREF crOutlineLT)
{
	m_stThemeInfo.crOutlineLT = crOutlineLT;
}

// 외곽선(Right-Bottom) 컬러 설정
void CThemeProgress::SetOutlineRBColor(COLORREF crOutlineRB)
{
	m_stThemeInfo.crOutlineRB = crOutlineRB;
}

// 배경 컬러 설정
void CThemeProgress::SetBackColor(COLORREF crBack)
{
	m_stThemeInfo.crBack = crBack;
}

// 범위-최소값 설정
void CThemeProgress::SetRangeMin(int nMin)
{
	m_stThemeInfo.nMin = nMin;
}

// 범위-최대값 설정
void CThemeProgress::SetRangeMax(int nMax)
{
	m_stThemeInfo.nMax = nMax;
}

// 그라데이션 여부 설정
void CThemeProgress::SetGradient(BOOL bGradient)
{
	m_stThemeInfo.bGradient = bGradient;
}

// 현재 값을 문자열로 화면에 표시
void CThemeProgress::DrawPosText(CPaintDC *pdc)
{
	CRect rValueText, rShadow;

	CString strText[50], strTemp;
	int nLineEnd=0, nLineCount=0, i=0;
	CRect rText[50];

	// 표시할 문자열을 구한다
	strTemp.Format(TEXT("%d"), m_stThemeInfo.nPos);
	if( m_stThemeInfo.strTextUnit.GetLength() > 0 )
		strTemp += m_stThemeInfo.strTextUnit;
	
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
			strTemp = TEXT("");
		}

		nLineCount ++;
		
	} while( strTemp.GetLength() > 0 );

	if( nLineCount < 1 )
		return;

	double fTextAreaHeight=0.;

	// 화면 영역좌표를 구한다
	this->GetClientRect(&rValueText);
	
	// 문자열 한줄의 높이
	fTextAreaHeight = (double)rValueText.Height() / (double)nLineCount;
	
	// 각 문자열 라인을 표시할 영역좌표 계산
	rText[0] = rValueText;
	rText[0].bottom = rText[0].top + (int)fTextAreaHeight;

	for(i=1 ; i < nLineCount ; i++)
	{
		rText[i] = rValueText;
		rText[i].top = rText[0].top + (int)(fTextAreaHeight * (double)i);
		rText[i].bottom = rText[i].top + (int)fTextAreaHeight;
	}

	// Text 문자를 화면에 표시
	for(i=0 ; i < nLineCount ; i++)
	{
		// Text 그림자 컬러가 투명이 아니라면
		if( m_stThemeInfo.crShadow != dfCOLOR_TRANSPARENT )
		{			
			// 그림자 영역좌표를 구한다
			rShadow = rText[i];
			rShadow.left ++;
			rShadow.top ++;
			rShadow.right ++;
			rShadow.bottom ++;

			// Text 그림자를 화면에 표시
			gf_DrawText(pdc, strText[i], m_stThemeInfo.nFontSize, 
				m_stThemeInfo.crShadow, rShadow, 
				DT_CENTER|DT_VCENTER|DT_SINGLELINE, m_stThemeInfo.bFontBold);
		}

		// Text 텍스트를 화면에 표시
		gf_DrawText(pdc, strText[i], m_stThemeInfo.nFontSize, 
				m_stThemeInfo.crFont, rText[i], 
				DT_CENTER|DT_VCENTER|DT_SINGLELINE, m_stThemeInfo.bFontBold);
	}
}

// 텍스트 표시 여부 설정
void CThemeProgress::SetDrawText(BOOL bDrawText)
{
	m_stThemeInfo.bDrawText = bDrawText;
}

// 텍스트 단위를 설정
void CThemeProgress::SetTextUnit(CString strTextUnit)
{
	m_stThemeInfo.strTextUnit = strTextUnit;
}

// 텍스트 폰트 크기를 설정
void CThemeProgress::SetFontSize(int nFontSize)
{
	m_stThemeInfo.nFontSize = nFontSize;
}

// 텍스트 폰트 두껍기 설정
void CThemeProgress::SetFontBold(BOOL bFontBold)
{
	m_stThemeInfo.bFontBold = bFontBold;
}

// 텍스트 폰트 컬러 설정
void CThemeProgress::SetFontColor(COLORREF crFont)
{
	m_stThemeInfo.crFont = crFont;
}

// 텍스트 그림자 컬러 설정
void CThemeProgress::SetShadowColor(COLORREF crShadow)
{
	m_stThemeInfo.crShadow = crShadow;
}

// 툴팁 문자열 설정
void CThemeProgress::SetToolTip(CString strText)
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
