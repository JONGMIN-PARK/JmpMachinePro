// DlgResizeWnd.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "DlgResizeWnd.h"
#include "ThemeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgResizeWnd

CDlgResizeWnd::CDlgResizeWnd()
{
	m_stThemeInfo.nEdgeRange = 7;								// 가장자리 범위
	m_stThemeInfo.nResizingDirection = dfRESIZE_DIRECTION_NONE;	// Resize 방향
	m_stThemeInfo.poMouse = CPoint(0,0);						// 마우스 포인터 위치좌표
	
}

CDlgResizeWnd::~CDlgResizeWnd()
{
}


BEGIN_MESSAGE_MAP(CDlgResizeWnd, CWnd)
	//{{AFX_MSG_MAP(CDlgResizeWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDlgResizeWnd message handlers

// 화면에 그림을 그린다
void CDlgResizeWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Resize 테두리를 그린다
	DrawResizeFrame(&dc);
}

// Resize 테두리를 그린다
void CDlgResizeWnd::DrawResizeFrame(CPaintDC *pdc)
{
	CRect rScreen;

	// 화면 영역좌표를 구한다
	this->GetClientRect(&rScreen);
	rScreen.right --;
	rScreen.bottom --;

	// 배경을 투명하게 칠한다
	pdc->FillRect(rScreen, &CBrush(RGB(255,0,255)));

	//CPen pnNew, *pOldPen;
	COLORREF crDark=RGB(50,50,50), crBright=RGB(255,255,255);

	//pnNew.CreatePen(PS_DASH, 1, RGB(220,220,220));
	// 사각형 선을 그리는 함수
	DrawRectLine(rScreen, crBright, crBright, crBright, crBright, pdc);
	DrawRectLine(rScreen, crDark, crDark, crDark, crDark, pdc, PS_DOT);
}

// 사각형 선을 그리는 함수
void CDlgResizeWnd::DrawRectLine(CRect rLine, COLORREF crLeft, COLORREF crTop, COLORREF crRight, 
								 COLORREF crBottom, CPaintDC *pdc, int nPenStyle/*=PS_SOLID*/)
{
	CPen *pOldPen, *pPenLeft, *pPenTop, *pPenRight, *pPenBottom;

	pPenLeft = new CPen(nPenStyle, 1, crLeft);
	pPenTop = new CPen(nPenStyle, 1, crTop);
	pPenRight = new CPen(nPenStyle, 1, crRight);
	pPenBottom = new CPen(nPenStyle, 1, crBottom);
	
	pdc->MoveTo(rLine.left, rLine.bottom);
	pOldPen = pdc->SelectObject(pPenLeft);
	pdc->LineTo(rLine.left, rLine.top);
	pdc->SelectObject(pPenTop);
	pdc->LineTo(rLine.right, rLine.top);
	pdc->SelectObject(pPenRight);
	pdc->LineTo(rLine.right, rLine.bottom);
	pdc->SelectObject(pPenBottom);
	pdc->LineTo(rLine.left, rLine.bottom);

	pdc->SelectObject(pOldPen);
	delete pPenLeft;
	delete pPenTop;
	delete pPenRight;
	delete pPenBottom;

}

// 특정 윈도우에 맞게 자신의 크기를 변경
void CDlgResizeWnd::SetSizeToWnd(CWnd *pWnd)
{
	CRect rScreen;

	// 특정 윈도우의 위치좌표를 구한다
	pWnd->GetWindowRect(&rScreen);

	// 자신의 크기를 변경
	this->MoveWindow(rScreen);

	// 화면을 새로 그린다
	Invalidate(TRUE);
}

// 부모 윈도우에 맞게 자신의 크기를 변경
void CDlgResizeWnd::SetSizeToParent()
{
	// 특정 윈도우에 맞게 자신의 크기를 변경
	SetSizeToWnd(this->GetParent());
}

BOOL CDlgResizeWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	// TODO: Add your specialized code here and/or call the base class
	cs.style |= WS_POPUP;
	cs.hMenu     =  NULL;

	return TRUE;
}

// 배경을 투명으로 설정(투명 컬러 지정)
BOOL CDlgResizeWnd::SetTransparentBack(COLORREF crTrans)
{
	HINSTANCE hUserLib32 = ::GetModuleHandle(TEXT("USER32.DLL"));

	if (!hUserLib32)
		return false;

	SLWA pSetLWA = (SLWA)GetProcAddress(hUserLib32, "SetLayeredWindowAttributes");

	if (!pSetLWA)
		return FALSE;

	HWND hWnd;
	hWnd = this->GetSafeHwnd();
	::SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	pSetLWA(hWnd, crTrans, 0, LWA_COLORKEY);

	return true;
}

// 클래스 생성시 실행되는 함수
int CDlgResizeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// 배경을 투명으로 설정(투명 컬러 지정)
	SetTransparentBack(RGB(255,0,255));
	
	return 0;
}

// 마우스 포인터가 가장자리 영역에 위치하는지 판단하는 함수
int CDlgResizeWnd::IsMouseInEdgeRange(CWnd *pWnd, CPoint poMouse)
{
	CRect rWnd;

	// 해당 윈도우 크기를 구한다
	pWnd->GetWindowRect(&rWnd);

	// 왼쪽위
	if( poMouse.x >= rWnd.left && poMouse.x <= rWnd.left+m_stThemeInfo.nEdgeRange && poMouse.y >= rWnd.top && poMouse.y <= rWnd.top+m_stThemeInfo.nEdgeRange )
		return dfRESIZE_DIRECTION_LEFT_TOP;
	// 오른쪽위
	if( poMouse.x >= rWnd.right-m_stThemeInfo.nEdgeRange && poMouse.x <= rWnd.right && poMouse.y >= rWnd.top && poMouse.y <= rWnd.top+m_stThemeInfo.nEdgeRange )
		return dfRESIZE_DIRECTION_RIGHT_TOP;
	// 왼쪽아래
	if( poMouse.x >= rWnd.left && poMouse.x <= rWnd.left+m_stThemeInfo.nEdgeRange && poMouse.y >= rWnd.bottom-m_stThemeInfo.nEdgeRange && poMouse.y <= rWnd.bottom )
		return dfRESIZE_DIRECTION_LEFT_BOTTOM;
	// 오른쪽아래
	if( poMouse.x >= rWnd.right-m_stThemeInfo.nEdgeRange && poMouse.x <= rWnd.right && poMouse.y >= rWnd.bottom-m_stThemeInfo.nEdgeRange && poMouse.y <= rWnd.bottom )
		return dfRESIZE_DIRECTION_RIGHT_BOTTOM;
	// 왼쪽
	if( poMouse.x >= rWnd.left && poMouse.x <= rWnd.left+m_stThemeInfo.nEdgeRange )
		return dfRESIZE_DIRECTION_LEFT;
	// 위쪽
	if( poMouse.y >= rWnd.top && poMouse.y <= rWnd.top+m_stThemeInfo.nEdgeRange )
		return dfRESIZE_DIRECTION_TOP;
	// 오른쪽
	if( poMouse.x >= rWnd.right-m_stThemeInfo.nEdgeRange && poMouse.x <= rWnd.right )
		return dfRESIZE_DIRECTION_RIGHT;
	// 아래쪽
	if( poMouse.y >= rWnd.bottom-m_stThemeInfo.nEdgeRange && poMouse.y <= rWnd.bottom )
		return dfRESIZE_DIRECTION_BOTTOM;

	return dfRESIZE_DIRECTION_NONE;
}

// 특정 윈도우에서 마우스 드래그시 실행되는 함수
void CDlgResizeWnd::OnTargetWindowMouseDrag(CWnd *pWnd, CPoint point)
{
	CRect rScreen;

	// 마우스 포인터 위치좌표로 새로운 윈도우 영역좌표를 구한다
	rScreen = GetNewAreaByPoint(pWnd, point);

	// 윈도우를 새로운 위치로 이동
	this->MoveWindow(rScreen);
	
	// 화면을 새로 그린다
	Invalidate(TRUE);
}

// 특정 윈도우에서 마우스 왼쪽 버튼 클릭시 실행되는 함수
void CDlgResizeWnd::OnTargetWindowLButtonDown(CWnd *pWnd, CPoint point)
{
	// 마우스 포인터 위치를 저장
	m_stThemeInfo.poMouse = point;

	// 마우스 포인터가 가장자리 영역에 위치하는지 판단하는 함수
	m_stThemeInfo.nResizingDirection = IsMouseInEdgeRange(pWnd, point);
	
	// Resize 테두리 윈도우를 화면에 표시
	ShowWindow(SW_SHOW);

	// 특정 윈도우에 맞게 자신의 크기를 변경
	SetSizeToWnd(pWnd);
}

// 마우스 포인터 위치좌표로 새로운 윈도우 영역좌표를 구한다
CRect CDlgResizeWnd::GetNewAreaByPoint(CWnd *pWnd, CPoint point)
{
	CRect rScreen;
	int nMinWidth=0, nMinHeight=0, nMaxWidth=0, nMaxHeight=0;
	CThemeDialog* pParent;
	pParent = (CThemeDialog*)pWnd;

	// 대화상자 최대/최소 크기를 구한다
	nMinWidth = pParent->m_stThemeInfo.nMinWidth;
	nMinHeight = pParent->m_stThemeInfo.nMinHeight;
	nMaxWidth = pParent->m_stThemeInfo.nMaxWidth;
	nMaxHeight = pParent->m_stThemeInfo.nMaxHeight;

	// 해당 윈도우 영역좌표를 구한다
	pWnd->GetWindowRect(&rScreen);

	// Left 영역 계산
	if( m_stThemeInfo.nResizingDirection == dfRESIZE_DIRECTION_LEFT 
		|| m_stThemeInfo.nResizingDirection == dfRESIZE_DIRECTION_LEFT_TOP
		|| m_stThemeInfo.nResizingDirection == dfRESIZE_DIRECTION_LEFT_BOTTOM)
	{
		rScreen.left += (point.x - m_stThemeInfo.poMouse.x);

		// 화면 넓이가 최고값 보다 작다면
		if( rScreen.Width() < nMinWidth )
			rScreen.left = rScreen.right - nMinWidth;
		// 화면 넓이가 최고값 보다 크다면
		if( rScreen.Width() > nMaxWidth )
			rScreen.left = rScreen.right - nMaxWidth;
	}

	// Top 영역 계산
	if( m_stThemeInfo.nResizingDirection == dfRESIZE_DIRECTION_TOP 
		|| m_stThemeInfo.nResizingDirection == dfRESIZE_DIRECTION_LEFT_TOP
		|| m_stThemeInfo.nResizingDirection == dfRESIZE_DIRECTION_RIGHT_TOP)
	{
		rScreen.top += (point.y - m_stThemeInfo.poMouse.y);

		// 화면 높이가 최고값 보다 작다면
		if( rScreen.Height() < nMinHeight )
			rScreen.top = rScreen.bottom - nMinHeight;
		// 화면 높이가 최고값 보다 크다면
		if( rScreen.Height() > nMaxHeight )
			rScreen.top = rScreen.bottom - nMaxHeight;
	}

	// Right 영역 계산
	if( m_stThemeInfo.nResizingDirection == dfRESIZE_DIRECTION_RIGHT 
		|| m_stThemeInfo.nResizingDirection == dfRESIZE_DIRECTION_RIGHT_TOP
		|| m_stThemeInfo.nResizingDirection == dfRESIZE_DIRECTION_RIGHT_BOTTOM)
	{
		rScreen.right += (point.x - m_stThemeInfo.poMouse.x);

		// 화면 넓이가 최고값 보다 작다면
		if( rScreen.Width() < nMinWidth )
			rScreen.right = rScreen.left + nMinWidth;
		// 화면 넓이가 최고값 보다 크다면
		if( rScreen.Width() > nMaxWidth )
			rScreen.right = rScreen.left + nMaxWidth;
	}

	// Bottom 영역 계산
	if( m_stThemeInfo.nResizingDirection == dfRESIZE_DIRECTION_BOTTOM 
		|| m_stThemeInfo.nResizingDirection == dfRESIZE_DIRECTION_LEFT_BOTTOM
		|| m_stThemeInfo.nResizingDirection == dfRESIZE_DIRECTION_RIGHT_BOTTOM)
	{
		rScreen.bottom += (point.y - m_stThemeInfo.poMouse.y);

		// 화면 높이가 최고값 보다 작다면
		if( rScreen.Height() < nMinHeight )
			rScreen.bottom = rScreen.top + nMinHeight;
		// 화면 높이가 최고값 보다 크다면
		if( rScreen.Height() > nMaxHeight )
			rScreen.bottom = rScreen.top + nMaxHeight;
	}

	return rScreen;
}

// 특정 윈도우 마우스 왼쪽 버튼 클릭 해제 되었을때 실행되는 함수
CRect CDlgResizeWnd::OnTargetWindowLButtonUp(CWnd *pWnd, CPoint point)
{
	CRect rScreen;

	// 마우스 포인터 위치좌표로 새로운 윈도우 영역좌표를 구한다
	rScreen = GetNewAreaByPoint(pWnd, point);

	// 윈도우를 감춘다
	this->ShowWindow(SW_HIDE);

	return rScreen;
}
