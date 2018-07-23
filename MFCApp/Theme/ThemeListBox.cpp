// ThemeListBox.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "ThemeListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

WNDPROC				m_oldScrollProc;
LRESULT CALLBACK ScrollProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
CWnd*				m_wndThis;

/////////////////////////////////////////////////////////////////////////////
// CThemeListBox

CThemeListBox::CThemeListBox()
{

	m_bStylePopUp = FALSE;	

	m_stThemeInfo.strIniCtrlName = "";							// INI 파일 조회용 컨트롤명
	m_stThemeInfo.nSelItemIndex = -1;							// 선택 항목 번호
	
	// 테마 관련 변수 초기화
	InitThemeVariable();

}

// 테마 관련 변수 초기화
void CThemeListBox::InitThemeVariable()
{
	m_stThemeInfo.nFontSize = 12;								// 폰트 크기
	m_stThemeInfo.nAlignFormat = DT_CENTER|DT_VCENTER;			// 정렬 방식
	m_stThemeInfo.bFontBold = FALSE;							// 폰트 두께
	m_stThemeInfo.crFontState1 = ::GetSysColor(COLOR_WINDOWTEXT);		// 폰트 색깔 - 상태1
	m_stThemeInfo.crFontState2 = RGB(254,246,0);				// 폰트 색깔 - 상태2
	m_stThemeInfo.crFontState3 = RGB(207,13,13);				// 폰트 색깔 - 상태3
	m_stThemeInfo.crNormalItemBack[0] = RGB(255,255,255);		// 일반 항목 배경색
	m_stThemeInfo.crNormalItemBack[1] = RGB(210,230,255);		// 일반 항목 배경색
	m_stThemeInfo.crSelItemFont = ::GetSysColor(COLOR_HIGHLIGHTTEXT);	// 선택 항목 폰트 컬러
	m_stThemeInfo.crSelItemBack = RGB(51, 153, 204);			// 선택 항목 배경색
	m_stThemeInfo.crFrame = RGB(200,200,200);					// 항목 구분선 색깔
	m_stThemeInfo.nItemHeight = 16;								// 항목 높이
	m_stThemeInfo.nLeftGap = 2;									// 왼쪽 여백
	m_stThemeInfo.nRightGap = 2;								// 오른쪽 여백
	m_stThemeInfo.nOutlineThick = 1;							// 외곽선 두께
	m_stThemeInfo.crOutlineLT = RGB(78,123,164);				// 외곽선 컬러(Left-Top)
	m_stThemeInfo.crOutlineRB = RGB(192,195,200);				// 외곽선 컬러(Right-Bottom)
	m_stThemeInfo.crBackGround = RGB(231,234,225);				// 배경화면 색깔
	m_stThemeInfo.bVScroll = TRUE;								// 수직 스크롤바 사용 여부
	m_stThemeInfo.rInitPos = CRect(-1,-1,-1,-1);				// 초기 위치 정보
	
}

CThemeListBox::~CThemeListBox()
{
}


BEGIN_MESSAGE_MAP(CThemeListBox, CWnd)
	//{{AFX_MSG_MAP(CThemeListBox)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_LBN_ERRSPACE(dfIDC_LISTBOX_CTRL, OnErrspaceListBox)
	ON_LBN_SELCANCEL(dfIDC_LISTBOX_CTRL, OnSelcancelListBox)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CThemeListBox message handlers

// 클래스 생성시 실행되는 함수
int CThemeListBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// 컨트롤 생성 함수
	CreateControl();

	// 수직 스크롤이 선택되지 않았다면
	/*DWORD dwStyle = lpCreateStruct->style;
	if( (dwStyle & WS_VSCROLL) != WS_VSCROLL )
		m_stThemeInfo.bVScroll = FALSE;*/

	// 스크롤 리스트박스 컨트롤에 콜백 함수 선언
	m_oldScrollProc = (WNDPROC)SetWindowLongPtr(m_pScrollListBox->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)ScrollProc);
	//m_oldScrollProc = (WNDPROC)SetWindowLong(m_pScrollListBox->GetSafeHwnd(), GWL_WNDPROC, (LONG)ScrollProc); // x86

	m_wndThis = this;
	
	return 0;
}

// 클래스 종료시 실행되는 함수
void CThemeListBox::OnDestroy() 
{
	CWnd::OnDestroy();

	// 스크롤바 대용 리스트박스 삭제
	if( m_pScrollListBox != NULL )
	{
		m_pScrollListBox->DestroyWindow();
		delete m_pScrollListBox;
		m_pScrollListBox = NULL;
	}
	
	// 툴팁 삭제
	DeleteToolTip();
}

// 컨트롤 생성 함수
void CThemeListBox::CreateControl()
{
	// 스크롤바 대용 리스트박스 생성
	m_pScrollListBox = new CListBox;
	m_pScrollListBox->Create(LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_VISIBLE | WS_TABSTOP, CRect(10,10,30,30), this, dfIDC_LISTBOX_CTRL);
	m_pScrollListBox->ShowWindow(FALSE);
}

// 컨트롤 위치 변경 함수
void CThemeListBox::RepositionControl()
{
	CRect rWnd, rCtrl;

	// 현 클래스의 크기를 구한다
	this->GetClientRect(&rWnd);
	// 테두리 영역을 제외
	rWnd.left += m_stThemeInfo.nOutlineThick;
	rWnd.top += m_stThemeInfo.nOutlineThick;
	rWnd.right -= m_stThemeInfo.nOutlineThick;
	rWnd.bottom -= m_stThemeInfo.nOutlineThick;

	// 수직 스크롤바의 위치정보를 구한다
	rCtrl = rWnd;
	rCtrl.left = rCtrl.right - dfSCROLLBAR_WIDTH;
	// 스크롤바 대용 리스트박스 위치 변경
	m_pScrollListBox->MoveWindow(rCtrl);

}

// 현 윈도우 크기 변경시 실행되는 함수
void CThemeListBox::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// 윈도우 크기가 너무 작으면 무시
	if( cx <= 20 || cy <= 20 )
		return;
	
	// 컨트롤 위치 변경 함수
	RepositionControl();
	
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

// 외곽선 선을 그리는 함수
void CThemeListBox::DrawOutline(CPaintDC *pdc)
{
	CRect rWnd;
	
	//클래스의 크기를 구한다
	this->GetClientRect(&rWnd);

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

// WM_PAINT 이벤트 발생시 실행되는 함수
void CThemeListBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// 화면에 항목 내용을 출력하는 함수
	DrawItemList(&dc);

	// 외곽선을 그리는 함수
	DrawOutline(&dc);

	/*// 화면 영역좌표 구하기
	CRect rScreen;
	this->GetClientRect(&rScreen);
	// 메모리 버퍼 구하기
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	// CBitmap 을 생성하고 메모리 DC에 할당
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rScreen.Width(), rScreen.Height());
	MemDC.SelectObject(&bmp);

	// 화면에 항목 내용을 출력하는 함수
	DrawItemList((CPaintDC*)&MemDC);

	// 외곽선을 그리는 함수
	DrawOutline((CPaintDC*)&MemDC);

	// CPaintDC에 메모리 버퍼를 복사
	dc.BitBlt(rScreen.left,rScreen.top,rScreen.Width(),rScreen.Height(), &MemDC, 0,0,SRCCOPY);*/

}

// 윈도우에 이벤트 발생시 실행되는 함수
LRESULT CThemeListBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	int i=0;
	
    switch(message)
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
	case WM_VSCROLL :						// 수직 스크롤 동작시
		// 이 윈도우에 포커스를 지정
		this->SetFocus();
	
		// 화면을 새로 그린다
		Invalidate(FALSE);
		break;
    }
	
	return CWnd::WindowProc(message, wParam, lParam);
}

// 특정 항목을 선택표시하는 함수
int CThemeListBox::SetCurSel(int nIndex)
{
	if( nIndex < 0 )
		return -1;
	if( nIndex >= m_pScrollListBox->GetCount() )
		return -1;

	m_stThemeInfo.nSelItemIndex = nIndex;
	Invalidate(FALSE);
	
	int nResult = m_pScrollListBox->SetCurSel(nIndex);
	
	return nResult;
}

// 항목 개수를 반환하는 함수
int CThemeListBox::GetCount()
{
	int nItemCount = m_pScrollListBox->GetCount();
	
	return nItemCount;
}

// 특정 항목의 내용을 가져오는 함수
void CThemeListBox::GetText(int nIndex, LPTSTR lpszBuffer)
{
	m_pScrollListBox->GetText(nIndex, lpszBuffer);
}

// 특정 항목의 내용을 가져오는 함수
void CThemeListBox::GetText(int nIndex, CString &rString)
{
	m_pScrollListBox->GetText(nIndex, rString);
}

// 특정 항목을 검색
int CThemeListBox::SelectString(int nStart, LPCTSTR lpszBuffer)
{
	int nIndex=-1;

	nIndex = m_pScrollListBox->SelectString(nStart, lpszBuffer);
	
	return nIndex;
}

// 현재 선택된 항목 번호를 반환
int CThemeListBox::GetCurSel()
{
	return m_stThemeInfo.nSelItemIndex;
}

// 사용자가 마우스왼쪽 버튼을 더블클릭하면 실행되는 함수
void CThemeListBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int nSelItemNo=-1;

	// 마우스 포인터로 사용자가 선택한 항목을 판단하는 함수
	if( (nSelItemNo=SelectItemByMousePos( point )) >= 0 )
	{
		// 항목을 더블클릭하면 실행되는 함수
		OnDblClickItem();
	}
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

// 마우스 왼쪽 버튼 클릭시 실행되는 함수
void CThemeListBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// 이 윈도우에 포커스를 지정
	this->SetFocus();
	
	CWnd::OnLButtonDown(nFlags, point);
}

// 항목 폰트 크기를 받아서 설정하는 함수
void CThemeListBox::SetFontSize(int nFontSize)
{
	m_stThemeInfo.nFontSize = nFontSize;
}

// 일반 항목 폰트 색깔 설정 함수
void CThemeListBox::SetFontColorState1(COLORREF crFontState1)
{
	m_stThemeInfo.crFontState1 = crFontState1;
}

// 텍스트 정렬 방식 설정 함수
void CThemeListBox::SetAlignFormat(int nAlignFormat)
{
	m_stThemeInfo.nAlignFormat = nAlignFormat;
}

// 항목을 삭제하는 함수
int CThemeListBox::DeleteString(UINT nIndex)
{
	int nResult=0;

	nResult = m_pScrollListBox->DeleteString( nIndex );

	// 스크롤바를 표시해야할 상황인지 판단하여 처리하는 함수
	SetCheckScrollBarShow();

	// 화면을 새로 그린다
	Invalidate(FALSE);

	// 남은 항목 개수를 반환
	return nResult;
}

// 리스트 중간에 항목을 삽입하는 함수
int CThemeListBox::InsertString(int nIndex, LPCTSTR lpszItem)
{
	int nResult=0;

	nResult = m_pScrollListBox->InsertString( nIndex, lpszItem );

	// 스크롤바를 표시해야할 상황인지 판단하여 처리하는 함수
	SetCheckScrollBarShow();
	
	// 화면을 새로 그린다
	Invalidate(FALSE);

	return nResult;
}

// 스크롤바를 표시해야할 상황인지 판단하여 처리하는 함수
void CThemeListBox::SetCheckScrollBarShow()
{
	BOOL bIsPageOver;

	// 항목 리스트가 한 페이지를 넘어가는지 체크하는 함수
	bIsPageOver = (IsItemListOverPage() && m_stThemeInfo.bVScroll);

	if( m_pScrollListBox->IsWindowVisible() != bIsPageOver )
		m_pScrollListBox->ShowWindow(bIsPageOver);
}

// 항목 리스트가 한 페이지를 넘어가는지 체크하는 함수
BOOL CThemeListBox::IsItemListOverPage()
{
	int nItemHeight=20, nItemCount=0, nTotItemHeight=0;
	CRect rWnd;

	// 화면 영역
	this->GetClientRect(&rWnd);

	// 테두리 영역을 제외
	rWnd.left += m_stThemeInfo.nOutlineThick;
	rWnd.top += m_stThemeInfo.nOutlineThick;
	rWnd.right -= m_stThemeInfo.nOutlineThick;
	rWnd.bottom -= m_stThemeInfo.nOutlineThick;

	// 항목 1개의 높이
	nItemHeight = m_pScrollListBox->GetItemHeight(0);
	// 항목 개수
	nItemCount = m_pScrollListBox->GetCount();
	// 전체 항목의 높이
	nTotItemHeight = nItemCount * nItemHeight;

	if( rWnd.Height() >= nTotItemHeight )
		return FALSE;
	else
		return TRUE;
}

// 항목 리스트의 화면 영역을 반환하는 함수
CRect CThemeListBox::GetItemListArea()
{
	CRect rWnd, rArea;

	// 화면 영역
	this->GetClientRect(&rWnd);

	// 테두리 영역을 제외
	rWnd.left += m_stThemeInfo.nOutlineThick;
	rWnd.top += m_stThemeInfo.nOutlineThick;
	rWnd.right -= m_stThemeInfo.nOutlineThick;
	rWnd.bottom -= m_stThemeInfo.nOutlineThick;

	// 항목 영역의 위치좌표를 구한다
	rArea = rWnd;

	// 항목 리스트가 한 페이지를 넘어가는지 체크하는 함수
	if( IsItemListOverPage() && m_stThemeInfo.bVScroll )
		// 리스트 영역에서 스크롤바 영역을 뺀다
		rArea.right = rWnd.right - dfSCROLLBAR_WIDTH;
	
	return rArea;
}

// 이벤트 메시지를 부모 윈도우에 알려주는 함수
LRESULT CThemeListBox::SendMessageToParent(long nRow, long nCol, int nMessage)
{
	if (!IsWindow(m_hWnd))
        return 0;

    NM_LISTBOXVIEW nmgv;
    nmgv.iRow         = nRow;
    nmgv.iColumn      = nCol;
    nmgv.hdr.hwndFrom = m_hWnd;
    nmgv.hdr.idFrom   = m_unCtrlID;
    nmgv.hdr.code     = nMessage;

    CWnd *pOwner = m_pParent;
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
    else
        return 0;
}

// 스크롤바에 이벤트가 발생하면 실행되는 함수
LRESULT CALLBACK ScrollProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	int i=0;
	
    switch(iMessage)
    {   
	case WM_MOUSEMOVE:
		break;
    case WM_KEYDOWN:
		break;
    case WM_KEYUP:
		break;
    case WM_CHAR:
		break;
    case WM_PAINT:
		break;
    case WM_DESTROY:
		break;
	case WM_LBUTTONDOWN :
		//m_wndThis->PostMessage(WM_VSCROLL, wParam, lParam);
		break;
	case WM_VSCROLL :			// 수직 스크롤바 이벤트 라면
		// 부모 윈도우로 메시지를 보낸다
		CWnd* pWnd = CWnd::FromHandle(hWnd);
		pWnd->GetParent()->PostMessage(iMessage, wParam, lParam);
		break;
    }

    return CallWindowProc(m_oldScrollProc, hWnd, iMessage,wParam,lParam);
}

// 화면에 항목 내용을 출력하는 함수
void CThemeListBox::DrawItemList(CPaintDC *pdc)
{
	CFont *pftText, *pftOld;
	CString strItem = TEXT("");
	CRect rArea, rItem;
	int nFontHeavy=0, nItemHeight=20, nTopItemIndex=0, nNowItemIndex=0, nItemCount=0;
	CPen *pOldPen, penDivideLine(PS_SOLID, 1, COLORREF(m_stThemeInfo.crFrame));

	nFontHeavy = ( m_stThemeInfo.bFontBold ) ? FW_BOLD : FW_THIN;
	//폰트 생성
	pftText = new CFont;
	pftText->CreateFont(m_stThemeInfo.nFontSize, 0,0,0, nFontHeavy, 0,0,0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, 
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("돋움"));
	//폰트 등록
	pftOld = pdc->SelectObject(pftText);
	pdc->SetBkMode(TRANSPARENT);
	//pdc->SetTextColor(m_stCtrlInfo.crNormalItemText);
	
	// 항목 구분선에 사용할 펜 등록
	pOldPen = pdc->SelectObject(&penDivideLine);

	// 항목 1개의 높이
	nItemHeight = m_pScrollListBox->GetItemHeight(0);
	// 항목 리스트의 화면 영역을 반환하는 함수
	rArea = GetItemListArea();
	rItem = rArea;
	// 왼쪽/오른쪽 여백 설정
	rItem.left += m_stThemeInfo.nLeftGap;
	rItem.right -= m_stThemeInfo.nRightGap;
	// 첫번째 항목의 영역
	rItem.bottom = rItem.top + nItemHeight;
	
	// 상단에 표시될 항목의 번호를 구한다
	nTopItemIndex = m_pScrollListBox->GetScrollPos(1);
	nNowItemIndex = nTopItemIndex;

	// 전체 항목 개수를 구한다
	nItemCount = m_pScrollListBox->GetCount();

	do {
		// 표시할 항목 번호가 범위를 벗어나면 루프 탈출
		if( nNowItemIndex >= nItemCount )
			break;

		// 현재 표시할 항목이 선택된 항목 일때
		if( nNowItemIndex == m_stThemeInfo.nSelItemIndex )
		{
			// 선택 항목 배경색으로 항목 영역을 채운다
			pdc->FillRect(rItem, &CBrush(m_stThemeInfo.crSelItemBack));
			// 선택 항목 글자색을 설정
			pdc->SetTextColor(m_stThemeInfo.crSelItemFont);
		}
		// 현재 표시할 항목이 선택된 항목이 아닐때
		else
		{
			// 배경색으로 항목 영역을 채운다
			pdc->FillRect(rItem, &CBrush(m_stThemeInfo.crNormalItemBack[nNowItemIndex % 2]));

			// 항목 상태가 '정상' 일때
			if( m_pScrollListBox->GetItemData(nNowItemIndex) == 0 )
				// 텍스트 색깔 설정
				pdc->SetTextColor( m_stThemeInfo.crFontState1);
			// 항목 상태가 '부분정상' 일때
			else if( m_pScrollListBox->GetItemData(nNowItemIndex) == 1 )
				// 텍스트 색깔 설정
				pdc->SetTextColor( m_stThemeInfo.crFontState2);
			// 항목 상태가 '비정상' 일때
			else
				// 텍스트 색깔 설정
				pdc->SetTextColor( m_stThemeInfo.crFontState3);
		}

		// 항목 구분선을 그린다
		pdc->MoveTo(rItem.left, rItem.bottom-1);
		pdc->LineTo(rItem.right, rItem.bottom-1);
		
		// 항목 내용을 구한다
		m_pScrollListBox->GetText(nNowItemIndex, strItem);

		// 항목 내용을 화면에 출력
		pdc->DrawText(strItem, rItem, m_stThemeInfo.nAlignFormat|DT_SINGLELINE);
		
		// 현재 표시될 항목 번호를 증가
		nNowItemIndex ++;
		// 항목 표시위치를 변경
		rItem.top += nItemHeight;
		rItem.bottom += nItemHeight;
	}while( rItem.top < rArea.bottom );

	// 배경을 그린다
	if( rItem.top < rArea.bottom )
	{
		rItem.bottom = rArea.bottom;
		pdc->FillRect(rItem, &CBrush(m_stThemeInfo.crBackGround));
	}
	CRect rGap;
	rGap = rArea;
	if( rItem.left > rArea.left )
	{
		rGap.left = rArea.left;
		rGap.right = rItem.left;
		pdc->FillRect(rGap, &CBrush(m_stThemeInfo.crBackGround));
	}
	if( rItem.right < rArea.right )
	{
		rGap.left = rItem.right;
		rGap.right = rArea.right;
		pdc->FillRect(rGap, &CBrush(m_stThemeInfo.crBackGround));
	}

	// 폰트를 예전 형식으로 되돌린다
	pdc->SelectObject(pftOld);
	// 펜을 예전 형식으로 되돌린다
	pdc->SelectObject(pOldPen);

	delete pftText;
}

// 리스트박스에 항목을 추가
int CThemeListBox::AddString(CString strItem)
{
	int nIndex=-1;

	// 항목 추가
	nIndex = m_pScrollListBox->AddString(strItem);
	// 항목 데이터 추가
	m_pScrollListBox->SetItemData(nIndex, 0);
	// 항목 높이 설정
	m_pScrollListBox->SetItemHeight(nIndex, m_stThemeInfo.nItemHeight);

	// 스크롤바를 표시해야할 상황인지 판단하여 처리하는 함수
	SetCheckScrollBarShow();

	// 화면을 새로 그린다
	Invalidate(FALSE);
	
	return nIndex;
}

// 항목 구분석 색깔을 지정하는 함수
void CThemeListBox::SetItemFrameColor(COLORREF crFrame)
{
	m_stThemeInfo.crFrame = crFrame;					// 항목 구분석 색깔
}

// 배경 색깔을 지정하는 함수
void CThemeListBox::SetBackColor(COLORREF crBack)
{
	m_stThemeInfo.crBackGround = crBack;						// 배경 색깔
}

// 컨트롤 경계선 정보를 받아들이는 함수
void CThemeListBox::SetOutline(long crEdgeLeftTop, long crEdgeRightBottom, int nLineThick)
{
	//전역변수 설정값 변경
	m_stThemeInfo.nOutlineThick = nLineThick;
	m_stThemeInfo.crOutlineLT = crEdgeLeftTop;
	m_stThemeInfo.crOutlineRB = crEdgeRightBottom;

	//컨트롤을 재배치하는 함수
	RepositionControl();
}

// 선택 항목 색깔을 지정하는 함수
void CThemeListBox::SetSelectItemColor(COLORREF crSelItemFont, COLORREF crSelItemBack)
{
	m_stThemeInfo.crSelItemFont = crSelItemFont;						// 선택된 항목 배경색
	m_stThemeInfo.crSelItemBack = crSelItemBack;						// 선택된 항목 글자색
}

// 경계선 정보, 항목 구분선, 배경색을 받아들이는 함수
void CThemeListBox::SetOutline(COLORREF crEdgeLeftTop, COLORREF crEdgeRightBottom, int nLineThick, COLORREF crFrame, COLORREF crBackGround)
{
	//전역변수 설정값 변경
	m_stThemeInfo.nOutlineThick = nLineThick;
	m_stThemeInfo.crOutlineLT = crEdgeLeftTop;
	m_stThemeInfo.crOutlineRB = crEdgeRightBottom;

	m_stThemeInfo.crFrame = crFrame;							// 항목 구분석 색깔
	m_stThemeInfo.crBackGround = crBackGround;					// 배경 색깔

	//컨트롤을 재배치하는 함수
	RepositionControl();
}

// 일반 항목 배경색을 지정하는 함수
void CThemeListBox::SetNormalItemBackColor(COLORREF crItem1, COLORREF crItem2)
{
	m_stThemeInfo.crNormalItemBack[0] = crItem1;					// 일반 항목 배경색-1
	m_stThemeInfo.crNormalItemBack[1] = crItem2;					// 일반 항목 배경색-2
}

// 항목의 화면상 영역 좌표를 구해서 반환하는 함수
CRect CThemeListBox::GetItemArea(int nIndex)
{
	CRect rArea=CRect(0,0,0,0), rItem=CRect(0,0,0,0);
	int nItemCount=0, nSelImageNo=-1, nScrollVal=-1, nItemHeight=0;
	CString strFileName;

	//스크롤바 현재값 얻기
	nScrollVal = m_pScrollListBox->GetScrollPos(1);
	
	//스크롤바 값이 존재하지 않으면 함수 탈출
	if( nScrollVal < 0 )
		return rItem;
	// 해당 항목이 화면에 표시되어있지 않다면 함수 탈출
	if( nIndex < nScrollVal )
		return rItem;

	//리스트의 화면 영역을 반환하는 함수
	rArea = GetItemListArea();

	// 개별 항목 영역 좌우 초기화
	rItem.left = rArea.left;
	rItem.right = rArea.right;
	// 항목 1개의 높이
	nItemHeight = m_pScrollListBox->GetItemHeight(0);

	//항목 리스트 개수를 구한다
	nItemCount = m_pScrollListBox->GetCount();

	// 해당 항목이 범위를 초과한다면 함수 탈출
	if( nIndex < 0 )
		return rItem;
	if( nIndex+1 > nItemCount )
		return rItem;

	//항목이 표시될 위치정보를 계산
	rItem.top = rArea.top + ((nIndex - nScrollVal) * nItemHeight);
	rItem.bottom = rItem.top + nItemHeight - 1;

	return rItem;
}

// 일반 항목 글자 색깔 설정(정상, 부분정상, 비정상)
void CThemeListBox::SetFontColor(COLORREF crFontState1, COLORREF crFontState2, COLORREF crFontState3)
{
	m_stThemeInfo.crFontState1 = crFontState1;					// 정상
	m_stThemeInfo.crFontState2 = crFontState2;					// 부분정상
	m_stThemeInfo.crFontState3 = crFontState3;					// 비정상
}

// 일반 항목 색깔을 지정하는 함수
void CThemeListBox::SetNormalItemColor(COLORREF crFontState1, COLORREF crFontState2, COLORREF crFontState3, COLORREF crItemBack1, COLORREF crItemBack2)
{
	m_stThemeInfo.crFontState1 = crFontState1;					// 정상
	m_stThemeInfo.crFontState2 = crFontState2;					// 부분정상
	m_stThemeInfo.crFontState3 = crFontState3;					// 비정상
	m_stThemeInfo.crNormalItemBack[0] = crItemBack1;					// 일반 항목 배경색-1
	m_stThemeInfo.crNormalItemBack[1] = crItemBack2;					// 일반 항목 배경색-2
}

//사용자가 마우스왼쪽 버튼을 클릭했다 떼면 실행되는 함수
void CThemeListBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int nSelItemNo=-1, nSelItemOld;
	CRect rItem;

	// 현재 선택 항목 번호를 저장해 둔다
	nSelItemOld = m_stThemeInfo.nSelItemIndex;

	// 마우스 포인터로 사용자가 선택한 항목을 판단하는 함수
	if( (nSelItemNo=SelectItemByMousePos( point )) >= 0 )
	{
		// 선택된 항목 번호를 전역변수에 저장
		m_stThemeInfo.nSelItemIndex = nSelItemNo;

		// 항목의 화면상 영역 좌표를 구해서 반환하는 함수
		rItem = GetItemArea(nSelItemOld);
		// 화면을 새로 그린다
		this->InvalidateRect(rItem, FALSE);

		// 항목의 화면상 영역 좌표를 구해서 반환하는 함수
		rItem = GetItemArea(nSelItemNo);
		// 화면을 새로 그린다
		this->InvalidateRect(rItem, FALSE);

		// 선택 항목이 변경되었다면
		//if( nSelItemOld != nSelItemNo )
			// 선택 항목이 변경되었음을 부모 윈도우에 알려주는 함수
		//	OnSelItemChange();
	}
	//else
	
	// 선택 항목이 변경되었음을 부모 윈도우에 알려주는 함수
	OnSelItemChange();
	
	CWnd::OnLButtonUp(nFlags, point);
}

// 마우스 포인터로 사용자가 선택한 항목을 판단하는 함수
int CThemeListBox::SelectItemByMousePos(CPoint poMouse)
{
	CRect rArea, rItem;
	int i=0, nItemCount=0, nSelImageNo=-1, nScrollVal=-1, nItemHeight=0;
	CString strFileName;

	//스크롤바 현재값 얻기
	nScrollVal = m_pScrollListBox->GetScrollPos(1);
	
	//스크롤바 값이 존재하지 않으면 함수 탈출
	if( nScrollVal < 0 )
		return -1;

	//리스트의 화면 영역을 반환하는 함수
	rArea = GetItemListArea();

	// 개별 항목 영역 좌우 초기화
	rItem.left = rArea.left;
	rItem.right = rArea.right;
	// 항목 1개의 높이
	nItemHeight = m_pScrollListBox->GetItemHeight(0);

	//항목 리스트 개수를 구한다
	nItemCount = m_pScrollListBox->GetCount();
	//항목 리스트 개수 만큼 루프를 돌린다
	for(i=0 ; i < nItemCount ; i++)
	{
		//항목이 표시될 위치정보를 계산
		rItem.top = rArea.top + (i * nItemHeight);
		rItem.bottom = rItem.top + nItemHeight - 1;

		//if( rItem.bottom > rArea.bottom )
		if( rItem.top > rArea.bottom )
			break;
		//사용자가 선택한 항목을 찾았다면
		if( poMouse.x >= rItem.left && poMouse.x <= rItem.right
			&& poMouse.y >= rItem.top && poMouse.y <= rItem.bottom )
		{
			nSelImageNo = i + nScrollVal;
			break;
		}
	}

	//선택한 항목이 없다면 함수 탈출
	if( nSelImageNo < 0 )
		return -1;
	// 선택한 항목이 범위를 초과한다면 함수 탈출
	if( nSelImageNo >= nItemCount)
		return -1;

	return nSelImageNo;
}

// 왼쪽/오른쪽 여백을 설정하는 함수
void CThemeListBox::SetLeftRightGap(int nLeft, int nRight)
{
	// 전역변수에 저장
	m_stThemeInfo.nLeftGap = nLeft;
	m_stThemeInfo.nRightGap = nRight;

	// 화면을 새로 그린다
	Invalidate(FALSE);
}

// 리스트박스에 항목을 추가
int CThemeListBox::AddString(CString strItem, int nState)
{
	int nIndex=-1;

	// 항목 추가
	nIndex = m_pScrollListBox->AddString(strItem);
	// 항목 데이터 추가
	m_pScrollListBox->SetItemData(nIndex, nState);
	// 항목 높이 설정
	m_pScrollListBox->SetItemHeight(nIndex, m_stThemeInfo.nItemHeight);

	// 스크롤바를 표시해야할 상황인지 판단하여 처리하는 함수
	SetCheckScrollBarShow();

	// 화면을 새로 그린다
	Invalidate(FALSE);
	
	return nIndex;
}

// 선택 항목이 변경되었음을 부모 윈도우에 알려주는 함수
void CThemeListBox::OnSelItemChange()
{
	// 이벤트 메시지를 부모 윈도우에 전달
	CWnd *pOwner = m_pParent;
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->SendMessage(WM_COMMAND, MAKELONG(m_unCtrlID, LBN_SELCHANGE), (LPARAM)m_hWnd);

	/*********************************************************************/
	// 부모 윈도우쪽 코딩
	// BEGIN_MESSAGE_MAP에 다음 코딩 추가
	//		ON_LBN_SELCHANGE(IDC_LIST_CONTURY, OnSelchangeListContury)
	// 이벤트 발생시 실행되는 함수 생성 - void OnSelchangeListCountry()
	/*********************************************************************/
}

// 항목을 더블클릭하면 실행되는 함수
void CThemeListBox::OnDblClickItem()
{
	// 이벤트 메시지를 부모 윈도우에 전달
	//SendMessageToParent(0, 0, LBN_DBLCLK);
	CWnd *pOwner = m_pParent;
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->SendMessage(WM_COMMAND, MAKELONG(m_unCtrlID, LBN_DBLCLK), (LPARAM)m_hWnd);

	/*********************************************************************/
	// 부모 윈도우쪽 코딩
	// BEGIN_MESSAGE_MAP에 다음 코딩 추가
	//		ON_LBN_DBLCLK(IDC_LIST_CONTURY, OnDblclkListContury)
	// 이벤트 발생시 실행되는 함수 생성 - void OnDblclkListCountry()
	/*********************************************************************/
}

// 리스트박스에서 'LBN_ERRSPACE' 이벤트 발생시 실행되는 함수
void CThemeListBox::OnErrspaceListBox()
{
	// 이벤트 메시지를 부모 윈도우에 전달
	CWnd *pOwner = m_pParent;
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->SendMessage(WM_COMMAND, MAKELONG(m_unCtrlID, LBN_ERRSPACE), (LPARAM)m_hWnd);

	/*********************************************************************/
	// 부모 윈도우쪽 코딩
	// BEGIN_MESSAGE_MAP에 다음 코딩 추가
	//		ON_LBN_ERRSPACE(IDC_LIST_CONTURY, OnErrspaceListContury)
	// 이벤트 발생시 실행되는 함수 생성 - void OnErrspaceListContury()
	/*********************************************************************/
}

// 윈도우에 포커스가 떠나면 실행되는 함수
void CThemeListBox::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);

	long nWnd = (long)pNewWnd;
	long nHwnd = (long)pNewWnd->GetSafeHwnd();
	
	// 이벤트 메시지를 부모 윈도우에 전달
	CWnd *pOwner = m_pParent;
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->SendMessage(WM_COMMAND, MAKELONG(m_unCtrlID, LBN_KILLFOCUS), (LPARAM)nHwnd);
	// 이벤트 메시지를 부모 윈도우에 알려주는 함수
	//SendMessageToParent(nHwnd, nWnd, LBN_KILLFOCUS);

	/*********************************************************************/
	// 부모 윈도우쪽 코딩
	// BEGIN_MESSAGE_MAP에 다음 코딩 추가
	//		ON_LBN_KILLFOCUS(IDC_LIST_CONTURY, OnKillfocusListContury)
	// 이벤트 발생시 실행되는 함수 생성 - void OnKillfocusListContury()
	/*********************************************************************/	
}

// 리스트박스에 'LBN_SELCANCEL' 이벤트 발생시 실행되는 함수
void CThemeListBox::OnSelcancelListBox()
{
	// 이벤트 메시지를 부모 윈도우에 전달
	CWnd *pOwner = m_pParent;
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->SendMessage(WM_COMMAND, MAKELONG(m_unCtrlID, LBN_SELCANCEL), (LPARAM)m_hWnd);

	/*********************************************************************/
	// 부모 윈도우쪽 코딩
	// BEGIN_MESSAGE_MAP에 다음 코딩 추가
	//		ON_LBN_SELCANCEL(IDC_LIST_CONTURY, OnSelcancelListContury)
	// 이벤트 발생시 실행되는 함수 생성 - void OnSelcancelListContury()
	/*********************************************************************/
}

// 윈도우에 포커스 지정되면 실행되는 함수
void CThemeListBox::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	long nWnd = (long)pOldWnd;
	long nHwnd = (long)pOldWnd->GetSafeHwnd();
	
	// 이벤트 메시지를 부모 윈도우에 전달
	CWnd *pOwner = m_pParent;
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->SendMessage(WM_COMMAND, MAKELONG(m_unCtrlID, LBN_SETFOCUS), (LPARAM)nHwnd);
	
	/*********************************************************************/
	// 부모 윈도우쪽 코딩
	// BEGIN_MESSAGE_MAP에 다음 코딩 추가
	//		ON_LBN_SETFOCUS(IDC_LIST_CONTURY, OnSetfocusListContury)
	// 이벤트 발생시 실행되는 함수 생성 - void OnSetfocusListContury()
	/*********************************************************************/	
}

// 외곽선 컬러 설정
void CThemeListBox::SetOutlineColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineLT = crOutline;
	m_stThemeInfo.crOutlineRB = crOutline;
}

// 외곽선(Left-Top) 컬러 설정
void CThemeListBox::SetOutlineLTColor(COLORREF crOutlineLT)
{
	m_stThemeInfo.crOutlineLT = crOutlineLT;
}

// 외곽선(Right-Bottom) 컬러 설정
void CThemeListBox::SetOutlineRBColor(COLORREF crOutlineRB)
{
	m_stThemeInfo.crOutlineRB = crOutlineRB;
}

// 폰트 컬러 설정
void CThemeListBox::SetFontColor(COLORREF crFont)
{
	m_stThemeInfo.crFontState1 = crFont;
	m_stThemeInfo.crFontState2 = crFont;
	m_stThemeInfo.crFontState3 = crFont;
}

// '상태-1' 폰트 컬러 설정
void CThemeListBox::SetFontState1Color(COLORREF crFontState1)
{
	m_stThemeInfo.crFontState1 = crFontState1;
}

// '상태-2' 폰트 컬러 설정
void CThemeListBox::SetFontState2Color(COLORREF crFontState2)
{
	m_stThemeInfo.crFontState2 = crFontState2;
}

// '상태-3' 폰트 컬러 설정
void CThemeListBox::SetFontState3Color(COLORREF crFontState3)
{
	m_stThemeInfo.crFontState3 = crFontState3;
}

// 폰트 두껍기 설정
void CThemeListBox::SetFontBold(BOOL bBold)
{
	m_stThemeInfo.bFontBold = bBold;
}

// 선택된 항목 폰트 컬러 설정
void CThemeListBox::SetSelectItemFontColor(COLORREF crFont)
{
	m_stThemeInfo.crSelItemFont = crFont;
}

// 선택된 항목 배경 컬러 설정
void CThemeListBox::SetSelectItemBackColor(COLORREF crBack)
{
	m_stThemeInfo.crSelItemBack = crBack;
}

// 일반 항목 배경 컬러 설정
void CThemeListBox::SetNormalItemBackColor(COLORREF crItemBack)
{
	m_stThemeInfo.crNormalItemBack[0] = crItemBack;
	m_stThemeInfo.crNormalItemBack[1] = crItemBack;
}

// 일반 항목(짝수번째) 배경 컬러 설정
void CThemeListBox::SetNormalItem1BackColor(COLORREF crItem1Back)
{
	m_stThemeInfo.crNormalItemBack[0] = crItem1Back;
}

// 일반 항목(홀수번째) 배경 컬러 설정
void CThemeListBox::SetNormalItem2BackColor(COLORREF crItem2Back)
{
	m_stThemeInfo.crNormalItemBack[1] = crItem2Back;
}

// 왼쪽 여백 설정
void CThemeListBox::SetLeftGap(int nLeftGap)
{
	m_stThemeInfo.nLeftGap = nLeftGap;
}

// 오른쪽 여백 설정
void CThemeListBox::SetRightGap(int nRightGap)
{
	m_stThemeInfo.nRightGap = nRightGap;
}

// 외곽선 두께 설정
void CThemeListBox::SetOutlineThick(int nOutlineThick)
{
	m_stThemeInfo.nOutlineThick = nOutlineThick;
}

// 툴팁을 생성한다
void CThemeListBox::MakeToolTip(CString strToolTip)
{
	// 현 컨트롤 ID를 구한다
	UINT unWindowID = (UINT)this->GetSafeHwnd();

	// 툴팁 생성
	m_ToolTip.CreateToolTip(this, unWindowID, strToolTip);
}

// 툴팁 텍스트 변경
void CThemeListBox::ChangeToolTip(CString strToolTip)
{
	// 툴팁 텍스트 변경
	m_ToolTip.ChangeToolTip(strToolTip);
}

// 툴팁 삭제
void CThemeListBox::DeleteToolTip()
{
	if( !m_ToolTip.GetSafeHwnd() )
		return;

	m_ToolTip.DelTool(this);
}

// 클래스 생성
BOOL CThemeListBox::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// 부모 윈도우 핸들 저장
	m_pParent = pParentWnd;
	// 자신의 ID값 저장
	m_unCtrlID = nID;
	
	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CThemeListBox::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	// Popup 스타일 설정
	if (m_bStylePopUp)
	{
		cs.style |= WS_POPUP;
		cs.hMenu =  NULL;
	}
	
	return TRUE;
}

void CThemeListBox::SetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

// 툴팁 문자열 설정
void CThemeListBox::SetToolTip(CString strText)
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

// '수직' 스크롤바 사용 여부 설정
void CThemeListBox::SetVScroll(BOOL bVScroll)
{
	m_stThemeInfo.bVScroll = bVScroll;

	// 스크롤바를 표시해야할 상황인지 판단하여 처리하는 함수
	SetCheckScrollBarShow();
}

// 항목 높이 설정 함수
void CThemeListBox::SetItemHeight(int nItemHeight)
{
	int i=0, nItemCount=0;

	// 전역변수에 저장
	m_stThemeInfo.nItemHeight = nItemHeight;
	
	// 리스트컨트롤에 현재 입력된 항목 개수를 구한다
	nItemCount = m_pScrollListBox->GetCount();

	// 현재 입력된 모든 항목의 높이를 재설정
	for(i=0 ; i < nItemCount ; i++)
		m_pScrollListBox->SetItemHeight(i, m_stThemeInfo.nItemHeight);
}

// 항목 높이를 반환
int CThemeListBox::GetItemHeight(int nIndex)
{
	return m_pScrollListBox->GetItemHeight(nIndex);
}

// 최상단 항목 번호 반환
int CThemeListBox::GetTopIndex()
{
	return m_pScrollListBox->GetTopIndex();
}

// 마우스 휠 동작시 실행되는 함수
BOOL CThemeListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	int nTopIndex=0;

	switch( zDelta )
	{
	case 120 :				// Wheel Up 일때
		// 최상단 항목 번호 반환
		nTopIndex = GetTopIndex();
		nTopIndex --;
		if( nTopIndex >= 0 )
			// 최상단 항목 번호 설정
			SetTopIndex(nTopIndex);
		break;
	case -120 :				// WHeel Down 일때
		// 최상단 항목 번호 반환
		nTopIndex = GetTopIndex();
		nTopIndex ++;
		if( nTopIndex >= 0 )
			// 최상단 항목 번호 설정
			SetTopIndex(nTopIndex);
		break;
	}
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

// 최상단 항목 번호 설정
int CThemeListBox::SetTopIndex(int nIndex)
{
	static int nOldIndex=0, nResult=0;

	nResult = m_pScrollListBox->SetTopIndex(nIndex);

	// 선택 번호가 변경되었다면
	if( nOldIndex != nIndex )
		// 화면을 새로 그린다
		Invalidate(FALSE);

	nOldIndex = nIndex;

	return nResult;
}

// 항목 상태 설정
void CThemeListBox::SetItemState(int nIndex, int nState)
{
	// 항목 데이터 변경
	m_pScrollListBox->SetItemData(nIndex, (DWORD)nState);

	// 화면을 새로 그린다
	Invalidate(FALSE);
}

// 항목 상태 반환
int CThemeListBox::GetItemState(int nIndex)
{
	int nState=0;

	// 항목 데이터를 구한다
	nState = (int)m_pScrollListBox->GetItemData(nIndex);

	return nState;
}

// 외곽선 컬러를 설정
void CThemeListBox::SetOutlineColor(COLORREF crOutlineLT, COLORREF crOutlineRB)
{
	m_stThemeInfo.crOutlineLT = crOutlineLT;
	m_stThemeInfo.crOutlineRB = crOutlineRB;
}

// 배경을 지울때 실행되는 함수
BOOL CThemeListBox::OnEraseBkgnd(CDC* pDC) 
{
	// 화면 번쩍거림을 막기 위해서 함수 탈출
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

// 반투명도 설정
BOOL CThemeListBox::SetTransparent(BYTE byteDegree)
{
	if (byteDegree <= 0)
		return FALSE;

	HINSTANCE hUserLib32 = ::GetModuleHandle(TEXT("USER32.DLL"));

	if (!hUserLib32)
		return FALSE;

	SLWA pSetLWA = (SLWA)GetProcAddress(hUserLib32, "SetLayeredWindowAttributes");

	if (!pSetLWA)
		return FALSE;

	HWND hWnd;
	hWnd = this->GetSafeHwnd();
	::SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	pSetLWA(hWnd, 0, byteDegree, LWA_ALPHA);

	return TRUE;
}
