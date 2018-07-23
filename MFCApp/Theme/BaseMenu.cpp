// BaseMenu.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "BaseMenu.h"
#include "ThemeMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseMenu

CBaseMenu::CBaseMenu(CWnd* pParent)
{
	// 부모 윈도우 포인터
	m_pParent = pParent;

	// 메뉴 레벨 초기화
	m_nMenuLevel = 0;

	// 컬러 설정 초기화
	m_crNormalItemBack = RGB(220,220,220);		// 정상 항목 배경 컬러
	m_crHoverItemBack = RGB(178,212,237);		// 마우스 오버 항목 배경 컬러
	m_crPressedItemBack = RGB(255,128,0);		// 선택 항목 배경 컬러
	m_crFontEnable = RGB(0,0,0);				// 폰트 컬러(사용 가능)
	m_crFontDisable = RGB(128,128,128);			// 폰트 컬러(사용 불가능)
	m_crBack = RGB(220,220,220);				// 배경 컬러
	//m_crBack = RGB(220,0,220);				// 배경 컬러
	m_crOutLineLT = RGB(128,128,128);			// 테두리(Left-Top) 컬러
	m_crOutLineRB = RGB(64,64,64);				// 테두리(Right-Bottom) 컬러
	m_crSeperateUp = RGB(128,128,128);			// 구분선 위쪽 컬러
	m_crSeperateDown = RGB(0,0,0);				// 구분선 아래쪽 컬러

	m_nItemHeight = 23;							// 항목 높이
	m_nItemLeftGap = 17;						// 항목 왼쪽 여백
	m_nItemRightGap = 13;						// 항목 오른쪽 여백
	m_nSeperateHeight = 8;						// 구분자 높이
	m_nBackTick = 2;							// 배경 두께
	m_nOutLineTick = 2;							// 외곽선 두께
	m_nFontSize = 12;							// 폰트 크기

	// 마우스 위치한 항목 번호 초기화
	m_nItemNumberMouseOver = -1;
	// 선택된 항목 번호 초기화
	m_nItemNumberSelected = -1;
	// 서브메뉴를 표시할 항목 번호 초기화
	m_nItemNumberViewSub = -1;

	// 이미지 존재여부 초기화
	m_bItemImageSettled = FALSE;
	m_bSeperateImageSettled = FALSE;

	// 항목 이미지 갯수
	m_nItemImageCount = 3;

	// 메뉴 포인터
	m_pMenuPointer = NULL;

}

CBaseMenu::~CBaseMenu()
{
}


BEGIN_MESSAGE_MAP(CBaseMenu, CWnd)
	//{{AFX_MSG_MAP(CBaseMenu)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBaseMenu message handlers

// 클래스 생성 전에 실행되는 함수
BOOL CBaseMenu::PreCreateWindow(CREATESTRUCT& cs) 
{
	// 팝업 형식으로 생성
	cs.style |= WS_POPUP;
	cs.hMenu     =  NULL;
	
	return CWnd::PreCreateWindow(cs);
}

// WM_PAINT 이벤트 발생시 실행되는 함수
void CBaseMenu::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rWnd;

	this->GetClientRect(&rWnd);
	// 화면 폭이 너무 작다면
	if( rWnd.Width() <= 50 )
		// 메뉴 컨트롤 크기를 자동 조정하는 함수
		ResizeMenuCtrl(&dc);

	// 배경을 그리는 함수
	DrawBackground(&dc);

	// 외곽선 사각형을 그리는 함수
	DrawOutLine(&dc);

	// 모든 메뉴 항목을 그리는 함수
	DrawAllMenuItem(&dc);

	//dc.TextOut(3,3, m_strMessage);
	
}

// 배경을 그리는 함수
void CBaseMenu::DrawBackground(CPaintDC *pdc)
{
	CRect rWnd;

	// 화면 크기를 구한다
	this->GetClientRect(&rWnd);

	// 배경색으로 화면을 채운다
	pdc->FillRect(rWnd, &CBrush(m_crBack));
}

// 테두리 사각형을 그리는 함수
void CBaseMenu::DrawOutLine(CPaintDC *pdc)
{
	CRect rWnd;

	// 외곽선을 그리지 않는다면 함수 탈출
	if( m_nOutLineTick < 1 )
		return;
	
	// 화면 크기를 구한다
	this->GetClientRect(&rWnd);
	rWnd.right --;
	rWnd.bottom --;
	
	for(int i=0 ; i < m_nOutLineTick ; i++)
	{
		// 테두리 선을 그린다
		gf_DrawRectLine(rWnd, m_crOutLineLT, m_crOutLineLT, 
					m_crOutLineRB, m_crOutLineRB, pdc);
		//사각형 좌표 변경
		rWnd.DeflateRect(1,1);
	}
}

// 모든 메뉴 항목을 그리는 함수
void CBaseMenu::DrawAllMenuItem(CPaintDC *pdc)
{
	///*** 일반 항목 컬러로 배경을 칠한다
	CRect rAllItem;
	int nGap=0;

	nGap = m_nBackTick + m_nOutLineTick;
	// 항목 영역좌표를 구한다
	this->GetClientRect(&rAllItem);
	rAllItem.DeflateRect(nGap,nGap);

	// 일반 항목 컬러로 항목 배경을 칠한다
	pdc->FillRect(rAllItem, &CBrush(m_crNormalItemBack));

	///*** 개별 항목을 화면에 그린다
	int i=0, nItemCount=0, nItemStartY=0;
	UINT nMenuID=0;
	CRect rItem, rText;
	CString strText = TEXT("");

	// 항목 Y축 시작위치 초기화
	nItemStartY = m_nBackTick + m_nOutLineTick;

	// 항목 영역좌표 초기화
	rItem = rAllItem;
	rItem.top = nItemStartY;

	// 메뉴 항목 갯수를 구한다
	nItemCount = GetMenuPointer()->GetMenuItemCount();
	//nItemCount = m_pSubMenu->GetMenuItemCount();

	// 메뉴 항목 갯수 만큼 루프를 돌린다
	for(i=0 ; i < nItemCount ; i++)
	{
		// 메뉴 ID 구하기
		nMenuID = GetMenuPointer()->GetMenuItemID(i);
		//nMenuID = m_pSubMenu->GetMenuItemID(i);

		UINT nState=0;
		// 메뉴 항목 상태를 구한다
		nState = GetMenuPointer()->GetMenuState(i, MF_BYPOSITION);
		//nState = m_pSubMenu->GetMenuState(i, MF_BYPOSITION);

		// 메뉴 항목이 구분자 일때
		if( nMenuID == 0 )
		{
			// 항목 영역좌표 Bottom값 설정
			rItem.bottom = rItem.top + m_nSeperateHeight;

			// 구분선 이미지가 존재한다면
			if( m_bSeperateImageSettled )
				// 이미지를 사용하여 항목 구분선을 그리는 함수
				DrawSeperateByImage(pdc, rItem);
			// 구분선 이미지가 존재하지 않는다면
			else
				// 항목 구분선 그리는 함수
				DrawSeperate(pdc, rItem);

			// 항목 Y축 시작위치 변경
			nItemStartY += m_nSeperateHeight;
			rItem.top = nItemStartY;
		}
		// 메뉴 항목이 일반항목 일때
		else
		{
			// 항목 영역좌표 Bottom값 설정
			rItem.bottom = rItem.top + m_nItemHeight;

			int nItemState=dfITEM_STATE_NORMAL;
			
			// 항목 이미지가 존재한다면
			if( m_bItemImageSettled )
			{
				// 선택된 항목번호이라면
				if( m_nItemNumberSelected == i )
					nItemState = dfITEM_STATE_SELECT;
				// 마우스 위치한 항목이라면
				else if( m_nItemNumberMouseOver == i )
					nItemState = dfITEM_STATE_MOUSE_OVER;

				// 이미지를 사용하여 항목 배경을 그리는 함수
				DrawItemByImage(pdc, rItem, nItemState);
			}
			// 항목 이미지가 존재하지 않는다면
			else
			{
				// 선택된 항목번호와 일치한다면 항목 배경색을 칠한다
				if( m_nItemNumberSelected == i )
					pdc->FillRect(rItem, &CBrush(m_crPressedItemBack));
				// 마우스 위치한 항목번호와 일치한다면 항목 배경색을 칠한다
				else if( m_nItemNumberMouseOver == i )
					pdc->FillRect(rItem, &CBrush(m_crHoverItemBack));
			}

			// 항목 텍스트 영역좌표 계산
			rText = rItem;
			rText.left += m_nItemLeftGap;
			rText.right -= m_nItemRightGap;

			//메뉴 텍스트 구하기
			GetMenuPointer()->GetMenuString(i, strText, MF_BYPOSITION);
			//m_pSubMenu->GetMenuString(i, strText, MF_BYPOSITION);

			COLORREF crFont;
			crFont = m_crFontEnable;

			// 항목이 비활성화 상태라면
			if( nState & MF_GRAYED )
				crFont = m_crFontDisable;

			// Text를 화면에 표시하는 함수
			DrawText(pdc, rText, m_nFontSize, crFont, strText);

			// 팝업 메뉴 항목이라면 화살표를 표시한다
			if( nMenuID == dfPOPUP_MENU_ITEM_ID )
			{
				rText.left = rText.right;
				rText.right = rItem.right;
				strText = "▶";
				// Text를 화면에 표시하는 함수
				DrawText(pdc, rText, m_nFontSize-2, m_crFontEnable, strText);
			}
			else
			{
				// 체크 상태라면
				if (nState & MF_CHECKED)
				{

					rText.left = rItem.left;
					rText.right = rText.left + m_nItemLeftGap;
					strText = "∨";
					BOOL bBold = TRUE;
					int nFontSize=m_nFontSize;

					CThemeMenu* pParent = (CThemeMenu*)m_pParent;
					int nPosition = pParent->FindPositionInArrayMenuItem(nMenuID);
					// 라디오 버튼 이라면
					//if( nState & MF_USECHECKBITMAPS )
					if( nPosition >= 0 )
						if( pParent->m_arMenuItemProperty[nPosition].nControlType == dfCONTROL_TYPE_RADIO )
						{
							//strText = "ㆍ";
							strText = " ●";
							bBold = FALSE;
							nFontSize=m_nFontSize-4;
						}

					// 체크 마크 표시
					DrawText(pdc, rText, nFontSize, m_crFontEnable, strText, bBold);
				}
			}

			// 항목 Y축 시작위치 변경
			nItemStartY += m_nItemHeight;
			rItem.top = nItemStartY;
		}
	}

}

// 화면상 위치에 따른 항목 번호를 구해서 반환하는 함수
int CBaseMenu::CalcItemNumberByPosition(CPoint point)
{
	int i=0, nItemCount=0;
	UINT nMenuID;
	CRect rItem;

	if( m_nMenuLevel < 0 )
		return -1;

	CMenu* pMenu;
	pMenu = GetMenuPointer();
	if( pMenu == NULL )
		return -1;

	// 메뉴 항목 갯수를 구한다
	nItemCount = GetMenuPointer()->GetMenuItemCount();
	//nItemCount = m_pSubMenu->GetMenuItemCount();

	// 메뉴 항목 갯수 만큼 루프를 돌린다
	for(i=0 ; i < nItemCount ; i++)
	{
		// 메뉴 ID 구하기
		nMenuID = GetMenuPointer()->GetMenuItemID(i);
		//nMenuID = m_pSubMenu->GetMenuItemID(i);

		// 해당 항목 영역좌표를 구해서 반환하는 함수
		rItem = GetItemRect(i);

		if( rItem.left > point.x || rItem.right < point.x )
			continue;
		if( rItem.top > point.y || rItem.bottom < point.y )
			continue;

		// 포인터에 해당하는 항목을 찾았다면 함수 탈출
		return i;
	}

	// 포인터에 해당하는 항목을 찾지 못했다면 함수 탈출
	return -1;
}

// 해당 항목 영역좌표를 구해서 반환하는 함수
CRect CBaseMenu::GetItemRect(int nItemNumber)
{
	CRect rItem;
	int nGap=0, nItemStartY=0;

	///*** 항목 영역 좌표를 구한다
	nGap = m_nBackTick + m_nOutLineTick;
	// 항목 영역좌표를 구한다
	this->GetClientRect(&rItem);
	rItem.DeflateRect(nGap,nGap);
	// 항목 영역좌표 Y축 시작위치
	nItemStartY = rItem.top;

	int i=0, nItemCount=0;
	UINT nMenuID;

	// 메뉴 항목 갯수를 구한다
	nItemCount = GetMenuPointer()->GetMenuItemCount();
	//nItemCount = m_pSubMenu->GetMenuItemCount();

	// 메뉴 항목 갯수 만큼 루프를 돌린다
	for(i=0 ; i < nItemCount ; i++)
	{
		// 메뉴 ID 구하기
		nMenuID = GetMenuPointer()->GetMenuItemID(i);
		//nMenuID = m_pSubMenu->GetMenuItemID(i);

		// 메뉴 항목이 구분자 일때
		if( nMenuID == 0 )
		{
			// 항목 영역좌표 Bottom값 설정
			rItem.bottom = rItem.top + m_nSeperateHeight;

			// 항목 Y축 시작위치 변경
			nItemStartY += m_nSeperateHeight;
		}
		// 메뉴 항목이 일반항목 일때
		else
		{
			// 항목 영역좌표 Bottom값 설정
			rItem.bottom = rItem.top + m_nItemHeight;

			// 항목 Y축 시작위치 변경
			nItemStartY += m_nItemHeight;
		}

		// 해당 항목 번호라면 영역좌표를 반환
		if( nItemNumber == i )
			return rItem;

		rItem.top = nItemStartY;
	}

	return CRect(0,0,10,10);
}

// 마우스 왼쪽버튼 Down일때 실행되는 함수
void CBaseMenu::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// 선택된 항목을 보여주는 함수
	ShowSelectItem(point);
	
	//m_strMessage.Format("Click %d / %d", point.x, point.y);
	//Invalidate();

	CWnd::OnLButtonDown(nFlags, point);
	//CWndSkin::OnLButtonDown(nFlags, point);
}

// 선택된 항목을 보여주는 함수
void CBaseMenu::ShowSelectItem(CPoint point)
{
	// 마우스 위치한 항목이 존재하지 않는다면 함수 탈출
	if( m_nItemNumberMouseOver < 0 )
		return;

	// 항목 번호가 한계를 벗어나면 함수 탈출
	int nMenuItemCount = GetMenuPointer()->GetMenuItemCount();
	if( nMenuItemCount <= m_nItemNumberMouseOver )
		return;

	int nMenuID=0;
	// 메뉴 ID 구하기
	nMenuID = GetMenuPointer()->GetMenuItemID(m_nItemNumberMouseOver);
	//nMenuID = m_pSubMenu->GetMenuItemID(m_nItemNumberMouseOver);

	// 메뉴 항목이 구분자 라면 함수 탈출
	if( nMenuID == 0 )
		return;
	// 팝업 메뉴 항목이라면 함수 탈출
	if( nMenuID == dfPOPUP_MENU_ITEM_ID )
		return;

	UINT nState=0;
	// 메뉴 항목 상태를 구한다
	nState = GetMenuPointer()->GetMenuState(m_nItemNumberMouseOver, MF_BYPOSITION);
	//nState = m_pSubMenu->GetMenuState(m_nItemNumberMouseOver, MF_BYPOSITION);
	
	// 항목이 비활성화 상태라면
	if( nState & MF_GRAYED )
		return;

	// 선택된 항목 번호를 전역변수에 저장
	m_nItemNumberSelected = m_nItemNumberMouseOver;

	CRect rItem;
	// 선택된 항목 영역좌표를 구한다
	rItem = GetItemRect(m_nItemNumberSelected);

	// 화면을 새로 그린다
	InvalidateRect(rItem);
}

// 마우스 왼쪽 버튼 Up일때 실행되는 함수
void CBaseMenu::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);
	//CWndSkin::OnLButtonUp(nFlags, point);

	//m_strMessage.Format("Up %d / %d", point.x, point.y);
	//Invalidate();

	// 항목이 선택되었다는 메시지를 부모 윈도우로 전달
	SendSelectItemMessage(point);
	
}

// 항목이 선택되었다는 메시지를 부모 윈도우로 전달
void CBaseMenu::SendSelectItemMessage(CPoint point)
{
	// 선택된 항목이 없다면 함수 탈출
	if( m_nItemNumberSelected < 0 )
		return;
	// 선택 항목과 마우스 오버 항목과 일치하지 않는다면
	if( m_nItemNumberSelected != m_nItemNumberMouseOver )
		return;

	// 항목 번호가 한계를 벗어나면 함수 탈출
	int nMenuItemCount = GetMenuPointer()->GetMenuItemCount();
	if( nMenuItemCount <= m_nItemNumberMouseOver )
		return;

	int nMenuID=0;
	// 메뉴 ID 구하기
	nMenuID = GetMenuPointer()->GetMenuItemID(m_nItemNumberMouseOver);
	//nMenuID = m_pSubMenu->GetMenuItemID(m_nItemNumberMouseOver);

	CThemeMenu* pParent = (CThemeMenu*)m_pParent;
	
	// 메뉴 항목 컨트롤 상태를 설정하는 함수
	pParent->SetItemControlState(GetMenuPointer(), nMenuID);

	// 메뉴 항목 선택시 실행되는 함수
	pParent->OnMenuItemSelect(m_nMenuLevel, nMenuID, m_nItemNumberSelected);

}

// 윈도우에 포커스가 떠나면 실행되는 함수
void CBaseMenu::OnKillFocus(CWnd* pNewWnd) 
{
	CThemeMenu* pParent = (CThemeMenu*)m_pParent;
	
	// 부모 윈도우에 메시지 전달
	pParent->OnKillFocusSubMenu(m_nMenuLevel);
}

// 포커스 지정시 실행되는 함수
void CBaseMenu::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	CThemeMenu* pParent = (CThemeMenu*)m_pParent;
	
	// 서브 메뉴에 포커스 지정될때 실행되는 함수
	pParent->OnSetFocusSubMenu(m_nMenuLevel);
}

// 메뉴 컨트롤 크기를 계산해서 반환하는 함수
CRect CBaseMenu::CalcMenuCtrlSize(CPaintDC *pdc)
{
	int i=0, nItemCount=0, nMenuCtrlHeight=0, nMenuCtrlWidth=0;
	UINT nMenuID=0;

	///*** 메뉴 컨트롤 높이를 계산

	// 메뉴 항목 갯수를 구한다
	nItemCount = GetMenuPointer()->GetMenuItemCount();
	//nItemCount = m_pSubMenu->GetMenuItemCount();

	// 메뉴 항목 갯수 만큼 루프를 돌린다
	for(i=0 ; i < nItemCount ; i++)
	{
		//메뉴 ID 구하기
		nMenuID = GetMenuPointer()->GetMenuItemID(i);
		//nMenuID = m_pSubMenu->GetMenuItemID(i);

		// 메뉴 항목이 구분자 일때
		if( nMenuID == 0 )
		{
			nMenuCtrlHeight += m_nSeperateHeight;
		}
		// 메뉴 항목이 일반항목 일때
		else
		{
			nMenuCtrlHeight += m_nItemHeight;
		}
	}

	// 배경 두께와 외곽선 두께를 합쳐서 메뉴 컨트롤 높이 계산을 완료
	nMenuCtrlHeight = nMenuCtrlHeight + m_nBackTick*2 + m_nOutLineTick*2;

	///*** 메뉴 컨트롤 넓이 계산

	// 항목 텍스트 중에서 가장 폭이 긴 항목의 길이를 구해서 반환
	nMenuCtrlWidth = GetMaxItemTextWidth(pdc);

	// 메뉴 컨트롤 폭이 너무 작다면
	if( nMenuCtrlWidth < 70 )
		nMenuCtrlWidth = 70;

	// 배경 두께와 외곽선 두께를 합쳐서 메뉴 컨트롤 넓이 계산을 완료
	nMenuCtrlWidth = nMenuCtrlWidth + m_nBackTick*2 + m_nOutLineTick*2 + 10;

	CRect rMenuCtrl;
	rMenuCtrl = CRect(0,0, nMenuCtrlWidth, nMenuCtrlHeight);

	return rMenuCtrl;
}

// 항목 텍스트 중에서 가장 폭이 항목의 길이를 구해서 반환
int CBaseMenu::GetMaxItemTextWidth(CPaintDC *pdc)
{
	int i=0, j=0, nItemCount=0, nCharLen=0, nWidthChar=0, nMenuItemWidth=0, nMaxMenuItemWidth=0;
	CString strText = TEXT("");
	wchar_t sLine[255];
	
	CFont ftText, *pftOld;

	//폰트 생성
	ftText.CreateFont(m_nFontSize, 0,0,0, FW_THIN, 0,0,0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, 
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("돋움"));
	//폰트 지정
	pftOld = pdc->SelectObject(&ftText);

	// 메뉴 항목 갯수를 구한다
	nItemCount = GetMenuPointer()->GetMenuItemCount();
	//nItemCount = m_pSubMenu->GetMenuItemCount();

	// 메뉴 항목 갯수 만큼 루프를 돌린다
	for(i=0 ; i < nItemCount ; i++)
	{
		// 항목 문자열을 구한다
		GetMenuPointer()->GetMenuString(i, strText, MF_BYPOSITION);
		//m_pSubMenu->GetMenuString(i, strText, MF_BYPOSITION);
		
		//라인 전체 단어 개수를 구한다
		nCharLen = strText.GetLength();

		//_tcscpy(sLine, strText);
		_tcscpy_s(sLine, _countof(sLine), strText);

		//전체 단어 개수 만큼 루프를 돌린다.
		for(j=0 ; j < nCharLen ; j++)
		{
			// 한글 이라면
			if( sLine[j] < 0 )
			{
				nWidthChar = m_nFontSize+3;
				j ++;
			}
			// 영문 이라면
			else
				//개별 단어의 폭 크기를 구한다
				pdc->GetOutputCharWidth(sLine[j], sLine[j], &nWidthChar);

			//개별 단어의 폭 크기를 누적
			nMenuItemWidth += (nWidthChar+2);
		}

		if( nMaxMenuItemWidth < nMenuItemWidth )
			nMaxMenuItemWidth = nMenuItemWidth;
		
		nMenuItemWidth = 0;
	}

	pdc->SelectObject(pftOld);

	return nMaxMenuItemWidth;
}

// 메뉴 설정 함수
void CBaseMenu::SetMenu(int nLevel, CMenu *pSubMenu)
{
	// 메뉴 레벨 저장
	m_nMenuLevel = nLevel;

	// 서브 메뉴를 전역변수에 저장
	//m_pSubMenu = pSubMenu;

	//ShowMenuItemCount();

	// 화면을 새로 그린다
	Invalidate();
}

// 메뉴 컨트롤 크기를 자동 조정하는 함수
void CBaseMenu::ResizeMenuCtrl(CPaintDC *pdc)
{
	CRect rWndSize, rWndNow;
	// 메뉴 컨트롤 크기를 계산해서 반환하는 함수
	rWndSize = CalcMenuCtrlSize(pdc);

	// 메뉴 컨트롤 위치 변경
	this->GetWindowRect(&rWndNow);
	rWndNow.right = rWndNow.left + rWndSize.Width();
	rWndNow.bottom = rWndNow.top + rWndSize.Height();
	this->MoveWindow(rWndNow);
}

// 항목 구분선 그리는 함수
void CBaseMenu::DrawSeperate(CPaintDC *pdc, CRect rArea)
{
	CPen *pnOld, pnNew1(PS_SOLID, 1, m_crSeperateUp), pnNew2(PS_SOLID, 1, m_crSeperateDown);
	int nLineY=0;

	// 위치 계산
	nLineY = (rArea.top + rArea.bottom) / 2 - 1;

	// 첫번째 펜 등록
	pnOld = pdc->SelectObject(&pnNew1);

	// 항목 구분선을 그린다
	pdc->MoveTo(rArea.left+2, nLineY);
	pdc->LineTo(rArea.right-2, nLineY);

	// 위치 변경
	nLineY ++;

	// 두번째 펜 등록
	pdc->SelectObject(&pnNew2);

	// 항목 구분선을 그린다
	pdc->MoveTo(rArea.left+2, nLineY);
	pdc->LineTo(rArea.right-2, nLineY);

	// 펜을 예전을 되돌린다
	pdc->SelectObject(pnOld);
}

// 이미지를 사용하여 항목 구분선을 그리는 함수
void CBaseMenu::DrawSeperateByImage(CPaintDC *pdc, CRect rArea)
{
	// 구분선 이미지가 존재하지 않는다면 함수 탈출
	if( !m_bSeperateImageSettled )
		return;
	if( rArea.left >= rArea.right || rArea.top >= rArea.bottom )
		return;

	// 이미지 크기를 구한다
	BITMAP bm;
	//m_bmpSeperate.GetBitmap(&bm);
	m_pBmpSeperate->GetBitmap(&bm);
	CRect rImage;
	rImage.left = 0;
	rImage.right = bm.bmWidth;
	rImage.top = 0;
	rImage.bottom = bm.bmHeight;
	
	// 이미지의 일부를 9조각으로 분할, 크기 변경하여 화면에 표시하는 함수(투명 지원)
	gf_DrawImagePartTransparentResizeBlock(pdc, m_pBmpSeperate, rImage, rArea);
}

// 이미지를 사용하여 항목 배경을 그리는 함수
void CBaseMenu::DrawItemByImage(CPaintDC *pdc, CRect rArea, int nState)
{
	// 항목 이미지가 존재하지 않는다면 함수 탈출
	if( !m_bItemImageSettled )
		return;
	if( m_nItemImageCount < 1 )
		return;

	// 이미지 크기를 구한다
	BITMAP bm;
	//m_bmpItem.GetBitmap(&bm);
	m_pBmpItem->GetBitmap(&bm);
	
	// 부분 이미지의 크기
	int nImageWidth=0;
	nImageWidth = (bm.bmWidth / m_nItemImageCount);

	// 항목 이미지 번호를 반환
	int nImageNumber = GetItemImageNumberByState(nState);

	// 부분 이미지의 영역좌표
	CRect rImage;
	rImage.left = nImageWidth * nImageNumber;
	rImage.right = rImage.left + nImageWidth;
	rImage.top = 0;
	rImage.bottom = bm.bmHeight;

	// 이미지의 일부를 9조각으로 분할, 크기 변경하여 화면에 표시하는 함수(투명 지원)
	gf_DrawImagePartTransparentResizeBlock(pdc, m_pBmpItem, rImage, rArea);
}

// 항목 이미지 갯수를 설정하는 함수
void CBaseMenu::SetItemImageCount(int nImageCount)
{
	m_nItemImageCount = nImageCount;
}

// 항목 이미지 번호를 반환
int CBaseMenu::GetItemImageNumberByState(int nState)
{
	if( nState < 0 || nState > 2 )
		return 0;
	
	int nImageNumber[3];
	
	switch( m_nItemImageCount )
	{
	case 1 :
		nImageNumber[dfITEM_STATE_NORMAL] = 0;
		nImageNumber[dfITEM_STATE_MOUSE_OVER] = 0;
		nImageNumber[dfITEM_STATE_SELECT] = 0;
		break;
	case 2 :
		nImageNumber[dfITEM_STATE_NORMAL] = 0;
		nImageNumber[dfITEM_STATE_MOUSE_OVER] = 1;
		nImageNumber[dfITEM_STATE_SELECT] = 1;
		break;
	case 3 :
		nImageNumber[dfITEM_STATE_NORMAL] = 0;
		nImageNumber[dfITEM_STATE_MOUSE_OVER] = 1;
		nImageNumber[dfITEM_STATE_SELECT] = 2;
		break;
	case 4 :
		nImageNumber[dfITEM_STATE_NORMAL] = 0;
		nImageNumber[dfITEM_STATE_MOUSE_OVER] = 1;
		nImageNumber[dfITEM_STATE_SELECT] = 2;
		break;
	default :
		return 0;
		break;
	}

	return nImageNumber[nState];
}

void CBaseMenu::ShowMenuItemCount()
{
	int nItemCount=-1;

	// 메뉴 항목 갯수를 구한다
	nItemCount = GetMenuPointer()->GetMenuItemCount();
	
	m_strMessage.Format(TEXT("Item:%d"), nItemCount);
	Invalidate();

}

// 서브 메뉴 포인터 반환
CMenu* CBaseMenu::GetMenuPointer()
{
	// 메뉴 포인터가 존재하지 않는다면 구한다
	if( m_pMenuPointer == NULL )
	{
		CThemeMenu* pParent = (CThemeMenu*)m_pParent;
		
		m_pMenuPointer = pParent->GetSubMenuPointer(m_nMenuLevel);

		return m_pMenuPointer;
	}
	
	return m_pMenuPointer;
}

// 마우스 포인터 이동시 실행되는 함수
void CBaseMenu::OnMouseMove(UINT nFlags, CPoint point) 
{
	// 마우스 위치 항목을 보여주는 함수
	ShowMouseOverItem(point);

	CWnd::OnMouseMove(nFlags, point);
}

// 마우스 위치 항목을 보여주는 함수
void CBaseMenu::ShowMouseOverItem(CPoint point)
{
	int nItemNumber=-1;

	// 화면상 위치에 따른 항목 번호를 구해서 반환하는 함수
	nItemNumber = CalcItemNumberByPosition(point);

	if( nItemNumber < 0 )
		return;
	// 항목 번호가 한계를 벗어나면 함수 탈출
	int nMenuItemCount = GetMenuPointer()->GetMenuItemCount();
	if( nMenuItemCount <= nItemNumber )
		return;
	
	// 기존 마우스 위치한 항목 번호와 일치한다면
	if( m_nItemNumberMouseOver == nItemNumber )
		return;

	CRect rItemNew, rItemOld;

	// 기존 마우스 위치한 항목의 영역좌표
	rItemOld = GetItemRect(m_nItemNumberMouseOver);

	// 새로운 마우스 위치 항목의 영역좌표
	rItemNew = GetItemRect(nItemNumber);

	// 새로운 마우스 위치한 항목 번호를 전역변수에 저장
	m_nItemNumberMouseOver = nItemNumber;
		
	//화면을 새로 그린다
	if( rItemOld.left < rItemOld.right && rItemOld.top < rItemOld.bottom )
		InvalidateRect(rItemOld);
	if( rItemNew.left < rItemNew.right && rItemNew.top < rItemNew.bottom )
		InvalidateRect(rItemNew);

	// 서브메뉴를 표시할 항목 번호와 일치한다면
	if( m_nItemNumberViewSub == nItemNumber )
		return;

	// 서브메뉴를 표시할 항목 번호를 저장
	m_nItemNumberViewSub = nItemNumber;

	// 서브메뉴를 표시할 타이머 종료
	KillTimer(dfID_TIMER_VIEW_SUB_MENU);

	if( m_nItemNumberViewSub >= 0 )
		// 서브메뉴를 표시할 타이머 발생
		SetTimer(dfID_TIMER_VIEW_SUB_MENU, 200, NULL);

	// 마우스 위치 항목을 보여주는 함수
	//ShowMouseOverItem(m_nItemNumberViewSub);

}

// 마우스 위치한 서브메뉴를 보여주는 함수
void CBaseMenu::ShowItemSubMenu(int nItemNumber)
{

	if( m_nItemNumberMouseOver < 0 )
		return;
	// 항목 번호가 한계를 벗어나면 함수 탈출
	int nMenuItemCount = GetMenuPointer()->GetMenuItemCount();
	if( nMenuItemCount <= m_nItemNumberMouseOver )
		return;

	CRect rItemNew, rItemAbsolute;

	// 새로운 마우스 위치 항목의 영역좌표
	rItemNew = GetItemRect(nItemNumber);

	this->GetWindowRect(&rItemAbsolute);
	rItemNew.left += rItemAbsolute.left;
	rItemNew.right += rItemAbsolute.left;
	rItemNew.top += rItemAbsolute.top;
	rItemNew.bottom += rItemAbsolute.top;

	///*** 마우스 Over 메시지를 부모 윈도우로 전달
	int nMenuID=0;
	// 메뉴 ID 구하기
	nMenuID = GetMenuPointer()->GetMenuItemID(m_nItemNumberMouseOver);

	CThemeMenu* pParent = (CThemeMenu*)m_pParent;
	//CSkinMenuCtrl* pParent = (CSkinMenuCtrl*)m_pParent;

	// 메뉴 항목 선택시 실행되는 함수
	pParent->OnMenuItemMouseOver(m_nMenuLevel, nMenuID, m_nItemNumberMouseOver, rItemNew);

}

// 클래스 종료시 실행되는 함수
void CBaseMenu::OnDestroy() 
{
	CWnd::OnDestroy();
	//CWndSkin::OnDestroy();
	
	// 서브메뉴를 표시할 타이머 종료
	KillTimer(dfID_TIMER_VIEW_SUB_MENU);
}

// 타이머 이벤트 발생시 실행되는 함수
void CBaseMenu::OnTimer(UINT_PTR nIDEvent) 
{
	// 서브메뉴를 표시할 타이머 일때
	if( nIDEvent == dfID_TIMER_VIEW_SUB_MENU )
	{
		// 타이머 종료
		KillTimer(dfID_TIMER_VIEW_SUB_MENU);

		// 마우스 위치한 서브 메뉴 보여주는 함수
		ShowItemSubMenu(m_nItemNumberViewSub);
	}
	
	CWnd::OnTimer(nIDEvent);
	//CWndSkin::OnTimer(nIDEvent);
}

// 반투명 설정
BOOL CBaseMenu::SetTransParent(BYTE byteDegree)
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

// 항목 이미지 설정 함수
void CBaseMenu::SetItemImage(CBitmap *pBitmap)
{
	// 항목 높이 변경
	BITMAP bm;
	BOOL bLoad=FALSE;

	bLoad = pBitmap->GetBitmap(&bm);

	// 항목 이미지가 올바르지 않다면
	if( bLoad == FALSE && bm.bmHeight < 1 )
	{
		m_bItemImageSettled = FALSE;
		return;
	}

	m_bItemImageSettled = TRUE;
	m_nItemHeight = bm.bmHeight;
	m_pBmpItem = pBitmap;
}

// 구분선 이미지 설정 함수
void CBaseMenu::SetSeperateImage(CBitmap *pBitmap)
{
	// 구분선 높이 변경
	BITMAP bm;
	BOOL bLoad=FALSE;

	bLoad = pBitmap->GetBitmap(&bm);

	// 구분선 이미지가 올바르지 않다면
	if( bLoad == FALSE && bm.bmHeight < 1 )
	{
		m_bSeperateImageSettled = FALSE;
		return;
	}

	m_bSeperateImageSettled = TRUE;
	m_nSeperateHeight = bm.bmHeight;
	m_pBmpSeperate = pBitmap;
}

// Text를 화면에 표시하는 함수
void CBaseMenu::DrawText(CPaintDC *pdc, CRect &rArea, int nFontSize, 
						COLORREF crFontColor, CString strItem, BOOL bBold/*=FALSE*/)
{
	CFont ftText, *pftOld;
	CRect rWnd/*, rText[50], rMark*/;
	CString strText[50], strTemp = TEXT("");	
	int nFontHeavy=0, nLineEnd=0, nLineCount=0, i=0;
	
	rWnd = rArea;

	nFontHeavy = ( bBold ) ? FW_BOLD : FW_THIN;
	//폰트 생성
	ftText.CreateFont(nFontSize, 0,0,0, nFontHeavy, 0,0,0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, 
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("돋움"));
	//폰트와 색상 지정
	pftOld = pdc->SelectObject(&ftText);
	pdc->SetBkMode(TRANSPARENT);
	pdc->SetTextColor(crFontColor);

	pdc->DrawText(strItem, &rWnd, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	//초기화
	pdc->SelectObject(pftOld);

}
