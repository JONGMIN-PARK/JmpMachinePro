// ThemeListCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "ThemeListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThemeListCtrl

CThemeListCtrl::CThemeListCtrl()
{

	// 체크박스 어레이
	m_UIntArrCheckBox.RemoveAll();
	
	// 에디트박스 컨트롤 초기화
	m_pEditItem = NULL;
	// 콤보박스 컨트롤 초기화
	m_pComboItem = NULL;

	m_stThemeInfo.strIniCtrlName = "";							// INI 파일 조회용 컨트롤명

	// 체크박스 관련 변수
	m_stThemeInfo.bCheckBoxOn		= FALSE;					// 체크박스 사용 여부
	m_stThemeInfo.bCheckBoxImageSettled = FALSE;				// 체크박스 이미지 존재 여부
	m_stThemeInfo.bHeadCheckBoxOn = FALSE;						// 헤더 체크박스 사용 여부

	m_stThemeInfo.nSelectItem = -1;								// 선택 항목 번호
	m_stThemeInfo.nSelectCol = -1;								// 선택 컬럼 번호

	// 마우스 더블클릭으로 편집 여부 초기화
	m_stThemeInfo.bEditDblClk = FALSE;
	
	// 에디트박스 배경 컬러 브러쉬 초기화
	m_stThemeInfo.brushBk = NULL;

	// 전역 변수 초기화
	InitThemeVariable();
	
}

// 전역 변수 초기화
void CThemeListCtrl::InitThemeVariable()
{
	// 기존 체크박스 이미지가 존재한다면 기존 이미지를 삭제
	if( m_stThemeInfo.bCheckBoxImageSettled )
		m_stThemeInfo.bmpCheckBox.DeleteObject();

	m_stThemeInfo.crSelectItemBack  = RGB(51, 153, 204)/*::GetSysColor(COLOR_HIGHLIGHT)*/;
	m_stThemeInfo.crSelectItemText  = RGB(0,0,0);

	m_stThemeInfo.crNormalItemBack[0] = RGB(255,255,255);
	m_stThemeInfo.crNormalItemText[0] = RGB(0,0,0);

	m_stThemeInfo.crNormalItemBack[1] = RGB(210,230,255);
	m_stThemeInfo.crNormalItemText[1] = RGB(0,0,0);

	// 셀 테두리 색깔
	m_stThemeInfo.crFrame = RGB(0,0,0);
	// 배경 색
	m_stThemeInfo.crBackGround = RGB(202,213,219);
	
	m_stThemeInfo.nFontSize = 12;								// 폰트 크기 초기화
	m_stThemeInfo.nItemHeight = 17;								// 항목 높이
	m_stThemeInfo.rInitPos = CRect(-1,-1,-1,-1);				// 초기 위치 정보

	// 현재 클릭된 버튼 위치 초기화
	m_poClickBtn = CPoint(-1,-1);

}

CThemeListCtrl::~CThemeListCtrl()
{
}


BEGIN_MESSAGE_MAP(CThemeListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CThemeListCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_EN_KILLFOCUS( dfIDC_EDIT_INPUT, OnKillfocusEditInput )
	ON_CBN_SELCHANGE(dfIDC_COB_INPUT, OnSelchangeCobInput)
	ON_CBN_KILLFOCUS(dfIDC_COB_INPUT, OnKillfocusCobInput)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CThemeListCtrl message handlers

int CThemeListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// 헤더에 컬러 설정
	SetHeadColor(RGB(28,62,110), RGB(0,12,26), RGB(255,255,255));

	// 리스트컨트롤 항목 높이 설정용 폰트 생성
	CreateItemHeightFont(m_stThemeInfo.nItemHeight);
	this->SetFont(&m_stThemeInfo.ftItemHeight);

	// 셀 편집용 컨트롤 생성
	CreateCtl();

	return 0;
}

// 클래스 종료시 실행되는 함수
void CThemeListCtrl::OnDestroy() 
{
	CListCtrl::OnDestroy();

	// 툴팁 삭제 함수
	DeleteToolTip();

	int i=0;
	ST_COLUMN_COLOR_INFO* pColumnColorInfo = NULL;
	for (i = 0; i < m_PtrArrColumnColor.GetSize(); i++)
	{
		pColumnColorInfo = (ST_COLUMN_COLOR_INFO*)m_PtrArrColumnColor.GetAt(i);
		if (!pColumnColorInfo)
			continue;

		delete pColumnColorInfo;
		pColumnColorInfo = NULL;
	}
	m_PtrArrColumnColor.RemoveAll();

	//Item Color
	ST_ITEM_COLOR_INFO* pItemColorInfo = NULL;
	for (i = 0; i < m_PtrArrItemColor.GetSize(); i++)
	{
		pItemColorInfo = (ST_ITEM_COLOR_INFO*)m_PtrArrItemColor.GetAt(i);
		if (!pItemColorInfo)
			continue;

		delete pItemColorInfo;
		pItemColorInfo = NULL;
	}
	m_PtrArrItemColor.RemoveAll();


    // 컬럼이 존재한다면
	if( this->GetHeaderCtrl()->GetItemCount() > 0 )
		m_HeaderCtrl.UnsubclassWindow();

	m_UIntArrCheckBox.RemoveAll();
	
	// 체크박스 이미지 삭제
	if( m_stThemeInfo.bCheckBoxImageSettled )
		m_stThemeInfo.bmpCheckBox.DeleteObject();
	
	// 에디트박스 컨트롤 삭제
	if( m_pEditItem )
	{
		m_pEditItem->DestroyWindow();
		delete m_pEditItem;
		m_pEditItem = NULL;
	}

	// 콤보박스 컨트롤 삭제
	if( m_pComboItem )
	{
		m_pComboItem->DestroyWindow();
		delete m_pComboItem;
		m_pComboItem = NULL;
	}
	
	// 컬럼 정보 어레이 삭제
	m_arrColInfo.RemoveAll();
	
	// 에디트박스 배경 컬러 브러쉬 삭제
	DeleteObject(m_stThemeInfo.brushBk);
}

// 특정 항목 컬러(텍스트, 배경) 반환
BOOL CThemeListCtrl::GetSubItemColor(int nRow, int nColumn, COLORREF &rcrColorText, COLORREF &rcrColorBack)
{
	ST_COLUMN_COLOR_INFO* pColumnColorInfo = NULL;
	for (int i = 0; i < m_PtrArrColumnColor.GetSize(); i++)
	{
		pColumnColorInfo = (ST_COLUMN_COLOR_INFO*)m_PtrArrColumnColor.GetAt(i);
		if (!pColumnColorInfo)
			continue;

		if (pColumnColorInfo->nRow == nRow && pColumnColorInfo->nColumn == nColumn)
		{
			rcrColorText = pColumnColorInfo->crColorText;
			rcrColorBack = pColumnColorInfo->crColorBack;
			return TRUE;
		}
	}

	return FALSE;
}

// 특정 항목 컬러(텍스트, 배경) 설정
void CThemeListCtrl::SetColumnColor(int nRow, int nColumn, COLORREF crColorText, COLORREF crColorBack)
{
	ST_COLUMN_COLOR_INFO* pColumnColorInfo = NULL;
	for (int i = 0; i < m_PtrArrColumnColor.GetSize(); i++)
	{
		pColumnColorInfo = (ST_COLUMN_COLOR_INFO*)m_PtrArrColumnColor.GetAt(i);
		if (!pColumnColorInfo)
			continue;

		if (pColumnColorInfo->nRow == nRow && pColumnColorInfo->nColumn == nColumn)
		{
			pColumnColorInfo->crColorText = crColorText;
			pColumnColorInfo->crColorBack = crColorBack;
			RedrawItems(nRow, nRow);
			return;
		}
	}

	pColumnColorInfo = new ST_COLUMN_COLOR_INFO;
	pColumnColorInfo->nRow = nRow;
	pColumnColorInfo->nColumn = nColumn;
	pColumnColorInfo->crColorText = crColorText;
	pColumnColorInfo->crColorBack = crColorBack;

	m_PtrArrColumnColor.Add(pColumnColorInfo);
	RedrawItems(nRow, nRow);
}

// 항목 컬러 설정
void CThemeListCtrl::SetItemColor(int nRow, COLORREF crColor)
{
	ST_ITEM_COLOR_INFO* pItemColorInfo = NULL;
	for (int i = 0; i < m_PtrArrItemColor.GetSize(); i++)
	{
		pItemColorInfo = (ST_ITEM_COLOR_INFO*)m_PtrArrItemColor.GetAt(i);
		if (!pItemColorInfo)
			continue;

		if (pItemColorInfo->nRow == nRow)
		{
			pItemColorInfo->crColor = crColor;
			RedrawItems(nRow, nRow);
			return;
		}
	}

	pItemColorInfo = new ST_ITEM_COLOR_INFO;
	pItemColorInfo->nRow = nRow;
	pItemColorInfo->crColor = crColor;

	m_PtrArrItemColor.Add(pItemColorInfo);
	RedrawItems(nRow, nRow);
}

// 항목 컬러 반환
BOOL CThemeListCtrl::GetItemColor(int nRow, COLORREF &rcrColor)
{
	ST_ITEM_COLOR_INFO* pItemColorInfo = NULL;
	for (int i = 0; i < m_PtrArrItemColor.GetSize(); i++)
	{
		pItemColorInfo = (ST_ITEM_COLOR_INFO*)m_PtrArrItemColor.GetAt(i);
		if (!pItemColorInfo)
			continue;

		if (pItemColorInfo->nRow == nRow)
		{
			rcrColor = pItemColorInfo->crColor;
			return TRUE;
		}
	}

	return FALSE;
}

// 컬러 개수 설정
void CThemeListCtrl::SetHeadItemCount(int nCount)
{
	if(!GetSafeHead())
		return;

	m_HeaderCtrl.SetCount(nCount, GetFont());

	Update(0);
	Invalidate();
}

// 선택된 항목 번호 반환
int CThemeListCtrl::FindRowIdx()
{
	POSITION	pos;
	int		nRowIdx;

	pos = GetFirstSelectedItemPosition();
	if ( pos == NULL )
		return -1;
	
	nRowIdx	= GetNextSelectedItem(pos);		
	
	return nRowIdx;
}

// 해당 항목이 체크박스 어레이 몇번째 위치에 있는지를 반환
int CThemeListCtrl::FindArrIdx(int nItem)
{
	if ( nItem < 0 )
		return -1;
	
	for ( int i = 0; i < m_UIntArrCheckBox.GetSize(); i++)
	{		
		int nTmp = m_UIntArrCheckBox.GetAt(i);

		if ( nTmp == nItem )
		{
			return i;
		}
	}
	
	return -1;
}

// 배경을 지울때 실행되는 함수
// 컬러로 배경을 그린다
BOOL CThemeListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	CRect RectClient;
	GetClientRect(&RectClient);

	pDC->FillRect(RectClient, &CBrush(m_stThemeInfo.crBackGround));

	return TRUE;	
	//return CListCtrl::OnEraseBkgnd(pDC);
}

BOOL CThemeListCtrl::GetSafeHead()
{
	if((GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
	{
		if(!m_HeaderCtrl.GetSafeHwnd())	
			m_HeaderCtrl.SubclassWindow(GetHeaderCtrl()->m_hWnd);

		return TRUE;
	}

	return FALSE;
}

// 헤더 컬러 텍스트 설정
void CThemeListCtrl::SetHeadItemText(int nItem, int nSubItem, LPCSTR lpstTxt)
{
	if(!GetSafeHead()) 
		return;

	m_HeaderCtrl.SetItemText(nItem, nSubItem, lpstTxt);
}

void CThemeListCtrl::SetHeadItemMerge(int nsItem, int nsSubItem, int neItem, int neSubItem)
{
	if(!GetSafeHead()) 
		return;

	m_HeaderCtrl.SetItemMerge(nsItem, nsSubItem, neItem, neSubItem);
}

BOOL CThemeListCtrl::DeleteColumn(int nCol)
{
	if(GetSafeHead()) 
		m_HeaderCtrl.DeleteColumn(nCol);

	return CListCtrl::DeleteColumn(nCol);
}

// 헤더 컬럼 컬러 설정
void CThemeListCtrl::SetHeadColor(COLORREF crFrame, COLORREF crBack, COLORREF crText)
{
	if ( m_HeaderCtrl.GetSafeHwnd() != NULL)
		m_HeaderCtrl.SetColor(crFrame, crBack, crText);
}

// '선택 항목 폰트 컬러'를 설정하는 함수
void CThemeListCtrl::SetSelectItemFontColor(COLORREF crFont)
{
	m_stThemeInfo.crSelectItemText = crFont;
}

// '선택 항목 배경 컬러'를 설정하는 함수
void CThemeListCtrl::SetSelectItemBackColor(COLORREF crBack)
{
	m_stThemeInfo.crSelectItemBack = crBack;
}

// '일반 항목-1 폰트 컬러'를 설정하는 함수
void CThemeListCtrl::SetNormalItem1FontColor(COLORREF crFont)
{
	m_stThemeInfo.crNormalItemText[0] = crFont;
}

// '일반 항목-1 배경 컬러'를 설정하는 함수
void CThemeListCtrl::SetNormalItem1BackColor(COLORREF crBack)
{
	m_stThemeInfo.crNormalItemBack[0] = crBack;

	// 에디트박스 배경 컬러 브러쉬를 삭제하고 새로 생성
	if(m_stThemeInfo.brushBk != NULL) DeleteObject(m_stThemeInfo.brushBk);

	m_stThemeInfo.brushBk = CreateSolidBrush( m_stThemeInfo.crNormalItemBack[0] );
}

// '일반 항목-2 폰트 컬러'를 설정하는 함수
void CThemeListCtrl::SetNormalItem2FontColor(COLORREF crFont)
{
	m_stThemeInfo.crNormalItemText[1] = crFont;
}

// '일반 항목-2 배경 컬러'를 설정하는 함수
void CThemeListCtrl::SetNormalItem2BackColor(COLORREF crBack)
{
	m_stThemeInfo.crNormalItemBack[1] = crBack;
}

// '일반 항목 폰트 컬러'를 설정하는 함수
void CThemeListCtrl::SetNormalItemFontColor(COLORREF crFont)
{
	m_stThemeInfo.crNormalItemText[0] = crFont;
	m_stThemeInfo.crNormalItemText[1] = crFont;
}

// '일반 항목 폰트 컬러'를 설정하는 함수
void CThemeListCtrl::SetFontColor(COLORREF crFont)
{
	// '일반 항목 폰트 컬러'를 설정하는 함수
	SetNormalItemFontColor(crFont);
}

// '일반 항목 배경 컬러'를 설정하는 함수
void CThemeListCtrl::SetNormalItemBackColor(COLORREF crBack)
{
	m_stThemeInfo.crNormalItemBack[0] = crBack;
	m_stThemeInfo.crNormalItemBack[1] = crBack;

	// 에디트박스 배경 컬러 브러쉬를 삭제하고 새로 생성
	if(m_stThemeInfo.brushBk != NULL) DeleteObject(m_stThemeInfo.brushBk);

	m_stThemeInfo.brushBk = CreateSolidBrush( m_stThemeInfo.crNormalItemBack[0] );
}

// '항목 셀 테두리 컬러'를 설정하는 함수
void CThemeListCtrl::SetFrameColor(COLORREF crFrame)
{
	m_stThemeInfo.crFrame = crFrame;
}

// '항목 셀 테두리 컬러'를 설정하는 함수
void CThemeListCtrl::SetItemFrameColor(COLORREF crFrame)
{
	SetFrameColor(crFrame);
}

// '배경 컬러'를 설정하는 함수
void CThemeListCtrl::SetBackColor(COLORREF crBack)
{
	m_stThemeInfo.crBackGround = crBack;
}

// '컬럼 테두리 컬러'를 변경하는 함수
void CThemeListCtrl::SetHeadFrameColor(COLORREF crFrame)
{
	m_HeaderCtrl.SetFrameColor(crFrame);
}

// '컬럼 배경 컬러'를 변경하는 함수
void CThemeListCtrl::SetHeadBackColor(COLORREF crBack)
{
	m_HeaderCtrl.SetBackColor(crBack);
}

// '컬럼 폰트 컬러'를 변경하는 함수
void CThemeListCtrl::SetHeadFontColor(COLORREF crFont)
{
	m_HeaderCtrl.SetTextColor(crFont);
}

// '폰트 크기'를 설정하는 함수
void CThemeListCtrl::SetFontSize(int nFontSize)
{
	m_stThemeInfo.nFontSize = nFontSize;
	
	// 헤더 컨트롤에 폰트 크기 설정
	m_HeaderCtrl.SetFontSize(nFontSize);

	// 자체 컨트롤용 폰트를 생성하는 함수
	CreateCtrlFont(m_stThemeInfo.nFontSize);

	// 에디트박스에 새로운 폰트 설정
	m_pEditItem->SetFont(&m_stThemeInfo.fntEdit);
	// 콤보박스에 새로운 폰트 설정
	m_pComboItem->SetFont(&m_stThemeInfo.fntEdit);
}

// 해당 항목이 체크 상태인지 여부를 반환하는 함수
BOOL CThemeListCtrl::GetCheck(int nItem)
{
	int nArrIdx=-1;

	// 체크 어레이 리스트를 검색
	nArrIdx = FindArrIdx(nItem);

	// 어레이에 없다면
	if( nArrIdx < 0 )
		return FALSE;
	// 어레이에 있다면
	else
		return TRUE;
}

// 해당 항목 체크 설정
BOOL CThemeListCtrl::SetCheck(int nItem, BOOL bCheck)
{
	int nArrIdx=-1;

	// 체크 어레이 리스트를 검색
	nArrIdx = FindArrIdx(nItem);

	if( nArrIdx < 0 && bCheck )								// not found
	{
		m_UIntArrCheckBox.Add(nItem);
	}
	else if( nArrIdx >= 0 && !bCheck ) 						// found
	{			
		m_UIntArrCheckBox.RemoveAt(nArrIdx, 1);
	}

	RedrawItems(nItem, nItem);
	
	return TRUE;
}

// 컨트롤 크기 변경시 실행되는 함수
void CThemeListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);

	int nLimitSize=8;			// 최소 크기

	if( cx < nLimitSize || cy < nLimitSize )
		return;

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

LRESULT CThemeListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
	
	return CListCtrl::WindowProc(message, wParam, lParam);
}

// 클래스 생성 전에 실행되는 함수
BOOL CThemeListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	// 리스트컨트롤 스타일 Report 형식으로 설정
	cs.style |= LVS_ALIGNTOP | LVS_OWNERDRAWFIXED | LVS_REPORT/* | LVS_SINGLESEL|WS_BORDER*/;
	//cs.style |= LVS_ALIGNTOP | LVS_REPORT;
	
	return CListCtrl::PreCreateWindow(cs);
}

// 컨트롤 생성 함수
void CThemeListCtrl::CreateCtl()
{
	// 자체 컨트롤용 폰트를 생성하는 함수
	CreateCtrlFont(m_stThemeInfo.nFontSize);

	// 에디트박스 컨트롤 생성
	if (!m_pEditItem)
	{
		CRect rcBound(0, 0, 10, 10);
		// 에디트박스 컨트롤 생성 함수
		CreateListEdit(&rcBound, this);
	}

	// 콤보박스 컨트롤 생성
	if (!m_pComboItem)
	{
		CRect rcBound(0, 0, 10, 10);
		CreateListCombo(&rcBound, this);
	}

}

// 에디트박스 컨트롤 위치 이동
void CThemeListCtrl::MoveListEdit(int nItem, int nSubItem)
{
//	if (!m_editItem.m_hWnd)
	if (!m_pEditItem)
		return;

	CRect rcBound, rList;
	if (!GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rcBound))
		return;

	// 첫번째 컬럼이라면 컬럼 폭을 조정
	if( nSubItem == 0 )
		rcBound.right = rcBound.left + GetColumnWidth(0);
	
	m_stThemeInfo.nSelectItem	= nItem;
	m_stThemeInfo.nSelectCol	= nSubItem;

	//---------------------------------------------

	m_pEditItem->MoveWindow(rcBound);
	m_pEditItem->SetWindowText(GetItemText(nItem, nSubItem));
	m_pEditItem->ShowWindow(SW_SHOW);
	
	m_pEditItem->SetSel(0, -1);
	m_pEditItem->SetFocus(); 

	m_pComboItem->ShowWindow(SW_HIDE);

}

// 콤보박스에서 포커스가 떠났을때 실행되는 함수
void CThemeListCtrl::OnKillfocusCobInput()
{
	OnSelchangeCobInput();
}

// 마우스 오른쪽 버튼 클릭 다운시 실행되는 함수
void CThemeListCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	
	CListCtrl::OnRButtonDown(nFlags, point);
}

LRESULT CThemeListCtrl::OnSetFont(WPARAM wParam, LPARAM)
{
	LRESULT res =  Default();

	CRect rc;
	GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );

	return res;
}

void CThemeListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	LOGFONT lf;
	GetFont()->GetLogFont( &lf );

	if( lf.lfHeight < 0 )
		lpMeasureItemStruct->itemHeight = -lf.lfHeight;
	else
		lpMeasureItemStruct->itemHeight = lf.lfHeight;

}

// 정렬 방식 설정 함수
void CThemeListCtrl::SetSortType(UINT nSortType)
{
	m_HeaderCtrl.SetSortType( nSortType );
}

// 자체 컨트롤용 폰트를 생성하는 함수
void CThemeListCtrl::CreateCtrlFont(int nFontSize)
{
	// 폰트를 생성하기 전에 제거
	m_stThemeInfo.fntEdit.DeleteObject();
	
	// 자체 컨트롤용 폰트
	m_stThemeInfo.fntEdit.CreateFont(nFontSize, 0,0,0, FW_THIN, 0,0,0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, 
				CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, dfFONT_NAME_COMMON);
}

// 배경색을 칠할때 실행되는 함수
HBRUSH CThemeListCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CListCtrl::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// 에디트박스 폰트,배경색 변경
	if( m_pEditItem == pWnd )
	{
		pDC->SetTextColor( m_stThemeInfo.crNormalItemText[0] );
		pDC->SetBkColor( m_stThemeInfo.crNormalItemBack[0] );

		if( m_stThemeInfo.brushBk == NULL )
			m_stThemeInfo.brushBk = CreateSolidBrush(m_stThemeInfo.crNormalItemBack[0]);
		return m_stThemeInfo.brushBk;
	}
	// 콤보박스 폰트,배경색 변경
	else if( m_pComboItem == pWnd )
	{
		pDC->SetTextColor( m_stThemeInfo.crNormalItemText[0] );
		pDC->SetBkColor( m_stThemeInfo.crNormalItemBack[0] );

		if( m_stThemeInfo.brushBk == NULL )
			m_stThemeInfo.brushBk = CreateSolidBrush(m_stThemeInfo.crNormalItemBack[0]);
		return m_stThemeInfo.brushBk;
	}

	return hbr;
}

// 컬럼 폭을 구해서 반환
int CThemeListCtrl::GetColumnWidth(int nCol)
{
	//
	LV_COLUMN stListColumn ;
	stListColumn.mask = LVCF_FMT | LVCF_WIDTH;

	UINT unJustify		= DT_LEFT ;
	UINT unLabelFormat  = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS;

	if( !GetColumn(nCol, &stListColumn) )
		return -1;

	return stListColumn.cx;
}

// 특정 셀에 자체 컨트롤을 보여주는 함수
void CThemeListCtrl::ShowSelfControlAtCell(int nRow, int nCol)
{
	int nColSize=0, nRowSize=0;

	// 컬럼 갯수를 구한다
	nColSize = m_arrColInfo.GetSize();
	// 컬럼 번호가 범위를 벗어나면 함수 탈출
	if( nCol < 0 || nCol >= nColSize )
		return;

	// 항목 갯수를 구한다
	nRowSize = GetItemCount();
	// 항목 번호가 범위를 벗어나면 함수 탈출
	if( nRow < 0 || nRow >= nRowSize )
		return;

	// 해당 컬럼의 컨트롤 종류에 따라서
	switch( m_arrColInfo[nCol].nControlType )
	{
	case dfCONTROL_TYPE_NONE :				// 컨트롤 종류가 '없음'이라면 함수 탈출
		return;
		break;
	case dfCONTROL_TYPE_EDITBOX :			// 컨트롤 종류가 '에디트박스'라면
		// 에디트박스 컨트롤 위치 이동
		MoveListEdit(nRow, nCol);
		break;
	case dfCONTROL_TYPE_COMBOBOX :			// 컨트롤 종류가 '콤보박스'라면
		// 콤보박스 항목 리스트 설정 함수
		SetComboItemList(m_arrColInfo[nCol].strComboItemList);
		// 콤보박스 컨트롤 위치 이동
		MoveListCombo(nRow, nCol);
		break;
	}
}

// 콤보박스 컨트롤 위치 이동
void CThemeListCtrl::MoveListCombo(int nItem, int nSubItem)
{
	if (!m_pComboItem)
		return;

	CRect rcBound;
	if (!GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rcBound))
		return;

	// 현재 선택된 항목을 설정
	
	CString strCellText = TEXT("");
	int nItemNo=-1;

	// 현재 셀의 텍스트를 구한다
	strCellText = GetItemText(nItem, nSubItem);

	// 현재 텍스트와 일치하는 콤보박스 항목을 찾는다
	nItemNo = m_pComboItem->FindString(0, strCellText);
	if( nItemNo >= 0 )
		m_pComboItem->SetCurSel(nItemNo);
	else
		m_pComboItem->SetWindowText(strCellText);
	
	m_stThemeInfo.nSelectItem	= nItem;
	m_stThemeInfo.nSelectCol	= nSubItem;

	m_pComboItem->ShowWindow(SW_SHOW);

	rcBound.bottom += 200;

	m_pComboItem->MoveWindow(rcBound);

	m_pComboItem->SetFocus();

}

// 콤보박스 항목 리스트 설정 함수
void CThemeListCtrl::SetComboItemList(CString strItemList)
{
	CString strNextItem = TEXT("\n"), strItem = TEXT("");
	int nPos=-1;

	if( !m_pComboItem )
		return;

	// 콤보박스에 저장되어있는 항목을 모두 삭제
	while( m_pComboItem->GetCount() > 0 )
		m_pComboItem->DeleteString(0);

	// 항목 리스트에서 문자열을 한씩 추출하여 콤보박스에 추가
	while( strItemList.GetLength() > 0 )
	{
		// 항목 구분자 위치를 찾는다
		nPos = strItemList.Find(strNextItem);
		if( nPos > 0 )
		{
			strItem = strItemList.Left(nPos);
			strItemList = strItemList.Mid(nPos+1);
		}
		else
		{
			strItem = strItemList;
			strItemList = "";
		}

		// 항목 문자열에서 좌우 여백 제거
		strItem.TrimLeft();
		strItem.TrimRight();

		// 콤보박스에 문자열을 추가
		m_pComboItem->AddString(strItem);
	}
}

// 에디트박스 컨트롤 생성 함수
void CThemeListCtrl::CreateListEdit(CRect *rcBound, CWnd *pParent, LPCTSTR pszDefault)
{

	DWORD dwStyle = WS_CHILD | ES_CENTER | WS_VISIBLE;

	m_pEditItem = new CEdit();
	m_pEditItem->CreateEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(""),
		dwStyle,
		*rcBound, 
		pParent, 
		dfIDC_EDIT_INPUT,
		NULL);

	// 폰트 설정
	m_pEditItem->SetFont(&m_stThemeInfo.fntEdit);
	m_pEditItem->ShowWindow(SW_HIDE);
	
}

// 콤보박스 컨트롤 생성
void CThemeListCtrl::CreateListCombo(CRect *rcBound, CWnd *pParent, LPCTSTR pszDefault)
{
	if (m_pComboItem)
		return;

	m_pComboItem = new CComboBox();
	m_pComboItem->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN, *rcBound, pParent, dfIDC_COB_INPUT);
	
	// 폰트 설정
	m_pComboItem->SetFont(&m_stThemeInfo.fntEdit);
	m_pComboItem->ShowWindow(SW_HIDE);
}

// 포인트 변수로 해당 컬럼 번호를 찾아서 반환하는 함수
int CThemeListCtrl::FindColIdx(CPoint point)
{
	if( point.x < 0 )
		return -1;

	//
	LV_COLUMN stListColumn ;
	stListColumn.mask = LVCF_FMT | LVCF_WIDTH;

	UINT unJustify		= DT_LEFT ;
	UINT unLabelFormat  = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS;

	int nColWidth=0;

	// 컬럼의 폭을 하나씩 구한다
	for(int nColumn = 0; GetColumn(nColumn, &stListColumn); nColumn++)
	{
		nColWidth += stListColumn.cx ;

		if( point.x < nColWidth )
			return nColumn;
	}

	return -1;
}

// 키보드 입력시 실행되는 함수
BOOL CThemeListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
    {
        switch( pMsg->wParam )
        {
        case VK_RETURN :    // Enter 키를 눌렀을때
			// 자체 컨트롤을 감추는 함수
			HideSelfControl();
			return TRUE;
			break;
		}
	}
	
	return CListCtrl::PreTranslateMessage(pMsg);
}

// 자체 컨트롤을 감추는 함수
void CThemeListCtrl::HideSelfControl()
{
	// 에디트박스 활성화 상태라면
	if( m_pEditItem->IsWindowVisible() )
	{
		// 에디트박스에서 포커스가 떠났을때 실행되는 함수
		OnKillfocusEditInput();
	}
	// 콤보박스 활성화 상태라면
	else if( m_pComboItem->IsWindowVisible() )
	{
		// 콤보박스에서 포커스가 떠났을때 실행되는 함수
		OnKillfocusCobInput();
	}
}

// 마우스 왼쪽버튼 더블클릭 일때 실행되는 함수
void CThemeListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// 마우스 더블클릭으로 자체 컨트롤을 표시한다면
	if( m_stThemeInfo.bEditDblClk )
		// 자체 컨트롤을 보여주는 함수
		ShowSelfControl( point );
	
	CListCtrl::OnLButtonDblClk(nFlags, point);
}

// 자체 컨트롤을 보여주는 함수
void CThemeListCtrl::ShowSelfControl(CPoint point)
{
	// 포인트 변수로 해당 항목 번호를 찾아서 반환하는 함수
	int nRow = FindRowIdx(point);

	// 포인트 변수로 해당 컬럼 번호를 찾아서 반환하는 함수
	int nCol = FindColIdx(point);

	// 항목이 선택되지 않았다면
	if( nRow < 0 )
		// 자체 컨트롤을 감추는 함수
		HideSelfControl();
	// 항목이 선택되었다면
	else
		// 특정 셀에 자체 컨트롤을 보여주는 함수
		ShowSelfControlAtCell(nRow, nCol);

}

// 마우스 더블클릭으로 편집 여부 설정
void CThemeListCtrl::SetEditDblClk(BOOL bDblClk)
{
	m_stThemeInfo.bEditDblClk = bDblClk;
}

// 화면에 항목을 표시
void CThemeListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	LV_ITEM	stListItem;
	stListItem.mask		 = LVIF_STATE | LVIF_IMAGE;
	stListItem.iItem	 = lpDrawItemStruct->itemID;
	stListItem.iSubItem  = 0;
	stListItem.stateMask = 0xFFFF;
	GetItem(&stListItem);
	
	//
	CRect RectBounds;
	GetItemRect(lpDrawItemStruct->itemID, RectBounds, LVIR_BOUNDS);

	CRect RectLabel;
	GetItemRect(lpDrawItemStruct->itemID, RectLabel, LVIR_LABEL);

	CRect RectSubItem;
	GetSubItemRect(lpDrawItemStruct->itemID,0,LVIR_BOUNDS,RectSubItem);

	CRect RectColumn(RectBounds);

	CRect RectClient;
	GetClientRect(&RectClient);

	CRect RectHighlight;

	RectHighlight.SetRect(RectBounds.left, RectBounds.top, RectBounds.right, RectBounds.bottom);

	//
	CDC* pDC   = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nDCOld = pDC->SaveDC();

	//
	int	nOffset  = 0;

	COLORREF crItem;
	BOOL bItemColor = GetItemColor(lpDrawItemStruct->itemID, crItem);

	if ( stListItem.state & LVIS_SELECTED )
	{	
		if (bItemColor)
			pDC->FillRect(RectHighlight, &CBrush(crItem));
		else
			pDC->FillRect(RectHighlight, &CBrush(m_stThemeInfo.crSelectItemBack));

		pDC->SetTextColor(m_stThemeInfo.crSelectItemText);	
		pDC->SetBkColor(m_stThemeInfo.crSelectItemBack);
	}	
	else
	{
		if (bItemColor)
			pDC->FillRect(RectHighlight, &CBrush(crItem));
		else
			pDC->FillRect(RectHighlight, &CBrush(m_stThemeInfo.crNormalItemBack[lpDrawItemStruct->itemID % 2]));

		pDC->SetTextColor(m_stThemeInfo.crNormalItemText[lpDrawItemStruct->itemID % 2]);
	}	

	// 
	RectColumn.right = RectColumn.left/*  + GetColumnWidth(0)*/;

	//
	LV_COLUMN stListColumn ;
	stListColumn.mask = LVCF_FMT | LVCF_WIDTH;

	UINT unJustify		= DT_LEFT ;
	UINT unLabelFormat  = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS;

	COLORREF crColorText;
	COLORREF crColorBack;
	CString strLabel;

	for(int nColumn = 0; GetColumn(nColumn, &stListColumn); nColumn++)
	{
		RectColumn.left	 = RectColumn.right ;
		RectColumn.right += stListColumn.cx ;
		
		strLabel = GetItemText(lpDrawItemStruct->itemID, nColumn);	
		
		//	
		switch( stListColumn.fmt & LVCFMT_JUSTIFYMASK )	
		{		
			case LVCFMT_RIGHT:
				unJustify = DT_RIGHT;
				break;

			case LVCFMT_CENTER:			
				unJustify = DT_CENTER;
				break;

			default:			
	            unJustify = DT_LEFT;
				break;		
		}		

		if (nColumn == 0)
            unJustify = DT_CENTER;
		
		// 셀 테두리 그리기
		CRect RectFrame(RectColumn);
		RectFrame.right += 1;
		RectFrame.top -= 1;
		// 마지막 셀은 오른쪽 테두리를 그린다
		//if( RectFrame.right >= RectClient.right )
		//	RectFrame.right = RectClient.right;
		pDC->FrameRect(&RectFrame, &CBrush(m_stThemeInfo.crFrame));

		RectLabel = RectColumn ;
		RectLabel.left  += nOffset;
		RectLabel.right -= nOffset;

		if (GetSubItemColor(lpDrawItemStruct->itemID, nColumn, crColorText, crColorBack))
		{
			pDC->SetTextColor(crColorText);
			if (crColorBack != RGB(0,0,0))
				pDC->FillRect(RectLabel, &CBrush(crColorBack));
		}

		
		CFont *pftText, *pftOld;
		int nFontSize=m_stThemeInfo.nFontSize;
		//폰트 생성
		pftText = new CFont;
		pftText->CreateFont(nFontSize, 0,0,0, FW_THIN, 0,0,0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, 
			CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("돋움"));
		//폰트 등록
		pftOld = pDC->SelectObject(pftText);
		pDC->SetBkMode(TRANSPARENT);
	
		// 셀 텍스트를 표시
		pDC->DrawText(strLabel, strLabel.GetLength(), RectLabel, unJustify | unLabelFormat);

		// 폰트를 예전 형식으로 되돌린다
		pDC->SelectObject(pftOld);
		delete pftText;

		RectLabel.right -= 1;
		RectLabel.bottom -= 2;
		// 해당 셀의 버튼 테두리 사각형을 그리는 함수
		DrawButtonEdgeRect((CPaintDC*)pDC, lpDrawItemStruct->itemID, nColumn, RectLabel);
	}

	//Check Box
	if ( m_stThemeInfo.bCheckBoxOn )
	{
		CRect RectCheck(RectSubItem.left + 5, RectColumn.top + 2, RectSubItem.left + 17, RectColumn.top + 13);
		pDC->Rectangle(&RectCheck);
		
		DrawCheckBox(pDC, RectCheck, lpDrawItemStruct->itemID);
	}
	
	pDC->RestoreDC(nDCOld);
}

// 해당 셀의 버튼 테두리 사각형을 그리는 함수
void CThemeListCtrl::DrawButtonEdgeRect(CPaintDC *pdc, int nItem, int nSubItem, CRect rCell)
{
	// 컬럼 번호가 범위를 벗어난다면 함수 탈출
	if( nSubItem < 0 && nSubItem >= GetHeaderCtrl()->GetItemCount() )
		return;
	// 해당 컬럼이 버튼 형식이 아니라면 함수 탈출
	if( m_arrColInfo[nSubItem].nControlType != dfCONTROL_TYPE_BUTTON )
		return;

	int i=0, nEdgeGap=3;
	COLORREF crSideUp=RGB(192,192,192), crSideDown=RGB(128,128,128);
	
	// 테두리 선을 그린다
	for(i=0 ; i < nEdgeGap ; i++)
	{
		if( m_poClickBtn.x == nSubItem && m_poClickBtn.y == nItem )
			// 버튼 Down 상태의 테두리 사각형 선을 그린다
			gf_DrawRectLine(rCell, crSideDown, crSideDown, crSideUp, crSideUp, pdc);
		else
			// 버튼 Up 상태의 테두리 사각형 선을 그린다
			gf_DrawRectLine(rCell, crSideUp, crSideUp, crSideDown, crSideDown, pdc);
		//사각형 좌표 변경
		rCell.DeflateRect(1,1);
	}

}

// 마우스 왼쪽버튼 클릭-Down 일때 실행되는 함수
void CThemeListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{

	// 선택된 버튼을 클릭 상태로 보여주는 함수
	ShowButtonClicked(point);

	CListCtrl::OnLButtonDown(nFlags, point);
	
	// 마우스 클릭으로 셀 편집 한다면
	if( !m_stThemeInfo.bEditDblClk )
		// 자체 컨트롤을 보여주는 함수
		ShowSelfControl( point );

	int nRowIdx, nArrIdx;

	// 선택된 행번호를 구한다
	nRowIdx = FindRowIdx();					// nRowIdx is selected row index
	nArrIdx = FindArrIdx(nRowIdx);		// nSelItem is Array index of 

	// 체크박스를 사용하지 않는다면 함수 탈출
	if ( !m_stThemeInfo.bCheckBoxOn )
		return;
	
	if ( nRowIdx >= 0 )
	{
		if ( nArrIdx < 0 )								// not found
		{
			m_UIntArrCheckBox.Add(nRowIdx);
		}
		else												// found
		{
			m_UIntArrCheckBox.RemoveAt(nArrIdx, 1);
		}
	}

	RedrawItems(nRowIdx, nRowIdx);

}

// 선택된 버튼을 클릭 상태로 보여주는 함수
void CThemeListCtrl::ShowButtonClicked(CPoint point)
{
	// 포인트 변수로 해당 항목 번호를 찾아서 반환하는 함수
	int nRow = FindRowIdx(point);

	// 포인트 변수로 해당 컬럼 번호를 찾아서 반환하는 함수
	int nCol = FindColIdx(point);

	// 항목이 선택되지 않았다면 함수 탈출
	if( nRow < 0 )
		return;

	if( nCol < 0 )
		return;

	// 해당 컬럼이 버튼 컨트롤이 아니라면 함수 탈출
	if( m_arrColInfo[nCol].nControlType != dfCONTROL_TYPE_BUTTON )
		return;

	// 클릭된 버튼 컨트롤 위치를 저장
	m_poClickBtn = CPoint(nCol, nRow);

	// 해당 항목을 새로 그린다
	RedrawItems(nRow, nRow);
}

// 선택된 버튼을 클릭 해제 상태로 보여주는 함수
void CThemeListCtrl::ShowButtonUnclicked()
{
	
	// 버튼 컨트롤이 클릭된 상태라면
	if( m_poClickBtn.y >= 0 )
	{
		int nRow=m_poClickBtn.y;

		// 클릭된 버튼 컨트롤 위치 초기화
		m_poClickBtn = CPoint(-1,-1);

		// 해당 항목을 새로 그린다
		RedrawItems(nRow, nRow);
	}
}

// 마우스 포인터 이동시 실행되는 함수
void CThemeListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// 버튼 컨트롤이 클릭된 상태로 움직인다면
	if((nFlags & MK_LBUTTON) == MK_LBUTTON && m_poClickBtn.y >= 0 )
	{
		// 포인트 변수로 해당 항목 번호를 찾아서 반환하는 함수
		int nRow = FindRowIdx(point);

		// 포인트 변수로 해당 컬럼 번호를 찾아서 반환하는 함수
		int nCol = FindColIdx(point);

		// 마우스 포인터가 셀 영역을 벗어났다면
		if( m_poClickBtn.y != nRow || m_poClickBtn.x != nCol )
		{
			nRow = m_poClickBtn.y;

			// 클릭된 버튼 컨트롤 위치 초기화
			m_poClickBtn = CPoint(-1,-1);

			// 클릭 상태의  항목을 새로 그린다
			RedrawItems(nRow, nRow);
		}
	}


	CRect RectClient;
	GetClientRect(&RectClient);

	if ((point.x > RectClient.right) || (point.y > RectClient.bottom))
	{
		GetWindowRect(&RectClient);
		
		GetParent()->ScreenToClient(&RectClient);
		GetParent()->InvalidateRect(RectClient);
	}
	
	CListCtrl::OnMouseMove(nFlags, point);
}

// 마우스 왼쪽버튼 클릭-Up 일때 실행되는 함수
void CThemeListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//MessageBox("Click Up");
	
	CListCtrl::OnLButtonUp(nFlags, point);
}

// 마우스 왼쪽버튼 클릭 일때 실행되는 함수
void CThemeListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	pNMListView->hdr.hwndFrom = m_hWnd;
	pNMListView->hdr.idFrom = GetDlgCtrlID();

	// 부모 윈도우로 버튼 클릭 이벤트를 전달
	if( m_poClickBtn.y >= 0 && m_poClickBtn.x >= 0 )
	{
		pNMListView->iItem = m_poClickBtn.y;
		pNMListView->iSubItem = m_poClickBtn.x;
		pNMListView->hdr.code = WM_LIST_BTN_CLICK;
	
		GetParent()->SendMessage(WM_NOTIFY, pNMListView->hdr.idFrom, (LPARAM)pNMListView);
		
	/* 부모 윈도우 코딩
	BEGIN_MESSAGE_MAP(CListCtrlSkinExDlg, CDialog)
		ON_NOTIFY(WM_LIST_BTN_CLICK, IDC_LIST_CTRL, OnClickListBtn)
	END_MESSAGE_MAP()

	void CListCtrlSkinExDlg::OnClickListBtn(NMHDR *pNMHDR, LRESULT *pResult)
	{
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

		int nRow = pNMListView->iItem;
		int nCol = pNMListView->iSubItem;
		
		*pResult = 0;
		
	}*/
	}
	else
	{
		pNMListView->hdr.code = WM_LIST_ITEM_CLICK;
		
		GetParent()->SendMessage(WM_NOTIFY, pNMListView->hdr.idFrom, (LPARAM)pNMListView);

	/* 부모 윈도우 코딩
	BEGIN_MESSAGE_MAP(CListCtrlSkinExDlg, CDialog)
		ON_NOTIFY(WM_LIST_ITEM_CLICK, IDC_LIST_CTRL, OnClickListItem)
	END_MESSAGE_MAP()

	void CListCtrlSkinExDlg::OnClickListItem(NMHDR *pNMHDR, LRESULT *pResult)
	{
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

		int nRow = pNMListView->iItem;
		int nCol = pNMListView->iSubItem;
		
		*pResult = 0;

	}*/
	}

	// 선택된 버튼을 클릭 해제 상태로 보여주는 함수
	ShowButtonUnclicked();

	*pResult = 0;
}


// 특정 컬럼의 자체 컨트롤 종류를 반환하는 함수
int CThemeListCtrl::GetSelfControlType(int nCol)
{
	return m_arrColInfo[nCol].nControlType;
}

// 특정 컬럼의 콤보박스 항목 리스트를 반환하는 함수
CString CThemeListCtrl::GetSelfControlComboItemList(int nCol)
{
	return m_arrColInfo[nCol].strComboItemList;
}

// 컬럼 갯수를 반환하는 함수
int CThemeListCtrl::GetColumnCount()
{
	return this->GetHeaderCtrl()->GetItemCount();
}

// 콤보박스 선택항목 변경시 실행되는 함수
void CThemeListCtrl::OnSelchangeCobInput()
{
	if (m_pComboItem)
	{
		CString sValue;

		if( m_pComboItem->GetCurSel() >= 0 )
			m_pComboItem->GetLBText(m_pComboItem->GetCurSel(), sValue);
		else
			m_pComboItem->GetWindowText(sValue);
		SetItemText(m_stThemeInfo.nSelectItem, m_stThemeInfo.nSelectCol, sValue);
		m_pComboItem->ShowWindow(SW_HIDE);

		int nItem=0, nSubItem=0;
		nItem = m_stThemeInfo.nSelectItem;
		nSubItem = m_stThemeInfo.nSelectCol;

		// 부모 윈도우로 메시지를 전송하는 함수
		SendMessageToParent(LVN_ITEMCHANGED, nItem, nSubItem);

		m_stThemeInfo.nSelectItem	= -1;
		m_stThemeInfo.nSelectCol	= -1;
	}
}

// 에디트박스에서 포커스가 떠났을때 실행되는 함수
void CThemeListCtrl::OnKillfocusEditInput()
{
	if (m_pEditItem)
	{
		BOOL bValid = TRUE;

		if (bValid)
		{
			CString sValue;

			m_pEditItem->GetWindowText(sValue);
			SetItemText(m_stThemeInfo.nSelectItem, m_stThemeInfo.nSelectCol, sValue);
			m_pEditItem->ShowWindow(SW_HIDE);

			int nItem=0, nSubItem=0;
			nItem = m_stThemeInfo.nSelectItem;
			nSubItem = m_stThemeInfo.nSelectCol;

			// 부모 윈도우로 메시지를 전송하는 함수
			SendMessageToParent(LVN_ITEMCHANGED, nItem, nSubItem);
		}
	}
}

// 부모 윈도우로 메시지를 전송하는 함수
LRESULT CThemeListCtrl::SendMessageToParent(UINT nMsgId, int nItem, int nSubItem)
{
	NM_LISTVIEW NMListView;
	NMListView.hdr.hwndFrom = m_hWnd;
	NMListView.hdr.idFrom = GetDlgCtrlID();

	NMListView.iItem = nItem;
	NMListView.iSubItem = nSubItem;
	NMListView.hdr.code = nMsgId;
	
	return GetParent()->SendMessage(WM_NOTIFY, NMListView.hdr.idFrom, (LPARAM)&NMListView);
}

// 헤더 컬럼 마우스 클릭시 실행되는 함수
void CThemeListCtrl::OnHeaderColumnClick(CPoint point)
{
	// 포인트 변수로 해당 컬럼 번호를 찾아서 반환하는 함수
	int nCol = FindColIdx(point);

	// 부모 윈도우로 메시지를 전송하는 함수
	SendMessageToParent(WM_HEADER_CLICK, -1, nCol);
	
}

// 체크박스 사용 여부를 설정
void CThemeListCtrl::SetCheckBoxOn(BOOL bFlag)
{
	// 체크박스 사용 여부를 전역변수에 저장
	m_stThemeInfo.bCheckBoxOn = bFlag;
}

// 체크박스 이미지를 설정(Resource ID)
BOOL CThemeListCtrl::SetCheckBoxImage(UINT nImageID)
{
	// 기존 이미지가 존재한다면 기존 이미지를 삭제
	if( m_stThemeInfo.bCheckBoxImageSettled )
		m_stThemeInfo.bmpCheckBox.DeleteObject();

	// 이미지를 로딩
	m_stThemeInfo.bCheckBoxImageSettled = m_stThemeInfo.bmpCheckBox.LoadBitmap(nImageID);

	return m_stThemeInfo.bCheckBoxImageSettled;
}

// 체크박스 이미지를 설정(파일)
BOOL CThemeListCtrl::SetCheckBoxImage(CString strFileName)
{
	// 기존 이미지가 존재한다면 기존 이미지를 삭제
	if( m_stThemeInfo.bCheckBoxImageSettled )
		m_stThemeInfo.bmpCheckBox.DeleteObject();

	// 이미지 파일을 읽어서 CBitmap으로 반환
	m_stThemeInfo.bCheckBoxImageSettled = gf_LoadPictureFile(strFileName, &m_stThemeInfo.bmpCheckBox);

	return m_stThemeInfo.bCheckBoxImageSettled;
}

// 체크박스를 그린다
void CThemeListCtrl::DrawCheckBox(CDC *pDC, CRect RectCheck, int nItem)
{
	int i=0, nBmpIndex = 0;
	UINT nChk=0;
	
	for ( i = 0; i < m_UIntArrCheckBox.GetSize(); i++ )
	{
		if ( (nChk = m_UIntArrCheckBox.GetAt(i)) == (UINT)nItem )
		{
			nBmpIndex = 1;
			break;
		}
	}

	// 체크박스 이미지가 존재한다면
	if( m_stThemeInfo.bCheckBoxImageSettled )
		// 체크박스를 이미지로 그린다
		DrawCheckBoxByImage(pDC, RectCheck, nBmpIndex);
	else 
		// 체크박스를 컬러로 그린다
		DrawCheckBoxByColor(pDC, RectCheck, nBmpIndex);
}

// 체크박스를 이미지로 그린다
void CThemeListCtrl::DrawCheckBoxByImage(CDC *pDC, CRect RectCheck, BOOL bCheck)
{

	CClientDC WinDC(this);
	CDC MemDC;

	MemDC.CreateCompatibleDC(&WinDC);

	BITMAP bm;
	m_stThemeInfo.bmpCheckBox.GetBitmap(&bm);
	CBitmap*	pOldBitmap = (CBitmap*)MemDC.SelectObject(&m_stThemeInfo.bmpCheckBox);
	
	if ( bCheck == 0 )
		WinDC.BitBlt(RectCheck.left+1, RectCheck.top+1, bm.bmWidth/2, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
	else
		WinDC.BitBlt(RectCheck.left+1, RectCheck.top+1, bm.bmWidth/2, bm.bmHeight, &MemDC, bm.bmWidth/2, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap);

}

// 체크박스를 컬러로 그린다
void CThemeListCtrl::DrawCheckBoxByColor(CDC *pDC, CRect RectCheck, BOOL bCheck)
{
	CRect rMark;
	CPoint po1, po2;
	//CClientDC WinDC(this);

	// 체크 마크 영역좌표
	rMark = CRect(RectCheck.left+1, RectCheck.top+1, RectCheck.left+12, RectCheck.top+12);

	// 마크 배경을 그린다
	pDC->FillRect(rMark, &CBrush(RGB(255,255,255)));
	// 테두리 사각형을 그린다
	gf_DrawRectLine(rMark, RGB(0,0,0), RGB(0,0,0), RGB(192,192,192), RGB(192,192,192), (CPaintDC*)pDC);

	// 현재 Check 상태라면
	if( bCheck )
	{
		// 라인 위치좌표를 구한다
		po1.x = rMark.left + 3;
		po2.x = rMark.right - 3;
		po1.y = rMark.top + 3;
		po2.y = rMark.bottom - 3;

		// 라인(선)을 그리는 함수
		gf_DrawLine(po1, po2, RGB(0,0,0), 2, (CPaintDC*)pDC);

		po1.y = rMark.bottom - 3;
		po2.y = rMark.top + 3;

		// 라인(선)을 그리는 함수
		gf_DrawLine(po1, po2, RGB(0,0,0), 2, (CPaintDC*)pDC);
	}
}

// 항목 높이 설정 함수
void CThemeListCtrl::SetItemHeight(int nItemHeight)
{
	m_stThemeInfo.nItemHeight = nItemHeight;

	// 리스트컨트롤 항목 높이 설정용 폰트 생성
	CreateItemHeightFont(m_stThemeInfo.nItemHeight);

	SetFont(&m_stThemeInfo.ftItemHeight);
}

// 리스트컨트롤 항목 높이 설정용 폰트 생성
void CThemeListCtrl::CreateItemHeightFont(int nItemHeight)
{
	// 폰트를 생성하기 전에 제거
	m_stThemeInfo.ftItemHeight.DeleteObject();
	
	// 리스트컨트롤 항목 높이 설정용 폰트
	m_stThemeInfo.ftItemHeight.CreateFont(nItemHeight, 0,0,0, FW_THIN, 0,0,0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, 
				CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, dfFONT_NAME_COMMON);
		
}

// 포인트 변수로 해당 항목 번호를 찾아서 반환하는 함수
int CThemeListCtrl::FindRowIdx(CPoint point)
{
	int nHeaderHeight=0, nItemHeight=0, nItemNo=0;

	// 항목 높이
	nItemHeight = m_stThemeInfo.nItemHeight;
	// 헤더 높이
	nHeaderHeight = nItemHeight + 3;

	if( point.y < nHeaderHeight )
		return -1;

	// 해당 포인트의 항목 번호를 계산
	nItemNo = (point.y - nHeaderHeight) / nItemHeight;
	nItemNo += GetTopIndex();

	// 범위를 초과하면 함수 탈출
	if( nItemNo >= GetItemCount() )
		return -1;

	return nItemNo;
}

// 헤더에 체크박스 사용 여부를 설정
void CThemeListCtrl::SetHeadCheckBoxOn(BOOL bFlag)
{
	// 헤더 체크박스 사용 여부를 전역변수에 설정
	m_stThemeInfo.bHeadCheckBoxOn = bFlag;

	// 헤더가 존재하지 않는다면
	if( GetHeaderCtrl()->GetItemCount() < 1 )
		// 함수 탈출
		return;

	// 헤더에 체크박스를 설정한다
	SetHeadCheckBox();
}

// 컬럼 삽입 함수
int CThemeListCtrl::InsertColumn(int nCol, LPCWSTR lpstTxt, int nFormat /*= LVCFMT_LEFT*/, int nWidth /*= -1*/, int nSubItem /*= -1*/)
{
	int nResult = CListCtrl::InsertColumn(nCol, lpstTxt, nFormat, nWidth, nSubItem);

	if(GetSafeHead())
	{
		m_HeaderCtrl.InsertColumn(nCol);		
		m_HeaderCtrl.SetItemText(0, nCol, (LPCSTR)lpstTxt);
	}

	ST_COLUMN_INFO stColInfo;

	stColInfo.nControlType = dfCONTROL_TYPE_NONE;
	stColInfo.strComboItemList = "";

	// 새로운 컬럼 정보를 어레이에 추가
	m_arrColInfo.InsertAt(nCol, stColInfo);

	// 첫번째 컬럼 이고 헤더 체크박스를 사용한다면
	if( GetHeaderCtrl()->GetItemCount() == 1 && m_stThemeInfo.bHeadCheckBoxOn )
	{
		// 헤더에 체크박스를 설정한다
		SetHeadCheckBox();
	}

	return nResult;
}

// 헤더에 체크박스를 설정한다
void CThemeListCtrl::SetHeadCheckBox()
{
	// 헤더가 존재하지 않는다면
	if( GetHeaderCtrl()->GetItemCount() < 1 )
		// 함수 탈출
		return;

	int nCheckPosX = 5;
	// 헤더 체크박스 왼쪽 여백 설정
	m_HeaderCtrl.SetCheckGapLeft(nCheckPosX);

	// 헤더 체크박스 사용 여부를 설정
	m_HeaderCtrl.SetCheckBoxOn(m_stThemeInfo.bHeadCheckBoxOn);

	// 체크박스 이미지가 존재한다면
	if( m_stThemeInfo.bCheckBoxImageSettled )
		// 헤더 체크박스 이미지 설정
		m_HeaderCtrl.SetCheckBoxImage(&m_stThemeInfo.bmpCheckBox);
}

// 수직 스크롤바 이동시 실행되는 함수
void CThemeListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//상위 클래스에 스크롤이 이동했음을 알린다
	this->GetParent()->SendMessage( WM_VSCROLL, nSBCode, LPARAM(pScrollBar));
	
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

// 수평 스크롤바 이동시 실행되는 함수
void CThemeListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//상위 클래스에 스크롤이 이동했음을 알린다
	this->GetParent()->SendMessage( WM_HSCROLL, nSBCode, LPARAM(pScrollBar));
	
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

// 툴팁을 생성한다
void CThemeListCtrl::MakeToolTip(CString strToolTip)
{
	// 현 컨트롤 ID를 구한다
	UINT unWindowID = (UINT)this->GetSafeHwnd();

	// 툴팁 생성
	m_ToolTip.CreateToolTip(this, unWindowID, strToolTip);
}

// 툴팁 텍스트 변경
void CThemeListCtrl::ChangeToolTip(CString strToolTip)
{
	// 툴팁 텍스트 변경
	m_ToolTip.ChangeToolTip(strToolTip);
}

// 툴팁 문자열 설정
void CThemeListCtrl::SetToolTip(CString strText)
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

// 툴팁 삭제
void CThemeListCtrl::DeleteToolTip()
{
	if( !m_ToolTip.GetSafeHwnd() )
		return;

	m_ToolTip.DelTool(this);
}

// 클래스 생성
BOOL CThemeListCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwStyle/*=WS_TABSTOP|WS_CHILD|WS_VISIBLE|WS_BORDER*/) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(TEXT("SysListView32"), NULL, dwStyle, rect, pParentWnd, nID, NULL);
}

// 클래스 생성
BOOL CThemeListCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(TEXT("SysListView32"), NULL, dwStyle, rect, pParentWnd, nID, pContext);
}

// 자체 컨트롤 종류를 설정하는 함수
void CThemeListCtrl::SetSelfControlType(int nCol, int nControlType, CString strComboItemList/*=""*/)
{
	int nArrSize=0;

	// 현재 컬럼 갯수
	nArrSize = m_arrColInfo.GetSize();

	// 설정 변경할 컬럼이 범위를 초과한다면 함수 탈출
	if( nCol < 0 || nCol >= nArrSize )
		return;

	// 해당 컬럼에 자체 컨트롤 종류를 설정
	m_arrColInfo[nCol].nControlType = nControlType;

	// 콤보박스 항목 내용 저장
	m_arrColInfo[nCol].strComboItemList = strComboItemList;
}

// 해당 컬럼을 에디트박스 컨트롤로 설정
void CThemeListCtrl::SetColumnEditBox(int nCol)
{
	// 자체 컨트롤 종류를 설정하는 함수
	SetSelfControlType(nCol, dfCONTROL_TYPE_EDITBOX);
}

// 해당 컬럼을 버튼 컨트롤로 설정
void CThemeListCtrl::SetColumnButton(int nCol)
{
	// 자체 컨트롤 종류를 설정하는 함수
	SetSelfControlType(nCol, dfCONTROL_TYPE_BUTTON);
}

// 해당 컬럼을 콤보박스 컨트롤로 설정
void CThemeListCtrl::SetColumnComboBox(int nCol, CString strComboItemList)
{
	// 자체 컨트롤 종류를 설정하는 함수
	SetSelfControlType(nCol, dfCONTROL_TYPE_COMBOBOX, strComboItemList);
}
