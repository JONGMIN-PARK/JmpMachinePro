// ThemeMenu.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "ThemeMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThemeMenu

CThemeMenu::CThemeMenu()
{
	int i=0;

	// 하위메뉴 초기화
	for(i=0 ; i < dfMAX_SUB_MENU_LEVEL ; i++)
		m_pSubMenu[i] = NULL;

	m_stThemeInfo.strIniCtrlName = "";							// INI 파일 조회용 컨트롤명

	// 전역 변수 초기화
	InitThemeVariable();

}

// 전역 변수 초기화
void CThemeMenu::InitThemeVariable()
{
	
	// 항목 이미지가 존재한다면 기존 이미지를 삭제
	if( m_stThemeInfo.bItemImageSettled == TRUE )
		m_stThemeInfo.bmpItem.DeleteObject();

	// 구분선 이미지가 존재한다면 기존 이미지를 삭제
	if( m_stThemeInfo.bSeperateImageSettled == TRUE )
		m_stThemeInfo.bmpSeperate.DeleteObject();

	// 포커스 지정된 메뉴 레벨
	m_stThemeInfo.nFocusMenuLevel = -1;
	
	// 컬러 설정 초기화
	m_stThemeInfo.crNormalItemBack = RGB(220,220,220);			// 일반 항목 배경 컬러
	m_stThemeInfo.crHoverItemBack = RGB(178,212,237);			// 마우스 오버 항목 배경 컬러
	m_stThemeInfo.crPressedItemBack = RGB(255,128,0);			// 선택 항목 배경 컬러
	m_stThemeInfo.crFontEnable = RGB(0,0,0);					// 폰트 컬러(사용 가능)
	m_stThemeInfo.crFontDisable = RGB(128,128,128);				// 폰트 컬러(사용 불가능)
	m_stThemeInfo.crBack = RGB(220,220,220);					// 배경 컬러
	m_stThemeInfo.crOutlineLT = RGB(128,128,128);				// 테두리(Left-Top) 컬러
	m_stThemeInfo.crOutlineRB = RGB(64,64,64);					// 테두리(Right-Bottom) 컬러
	m_stThemeInfo.crSeperateUp = RGB(128,128,128);				// 구분선 위쪽 컬러
	m_stThemeInfo.crSeperateDown = RGB(0,0,0);					// 구분선 아래쪽 컬러

	m_stThemeInfo.nItemHeight = 23;								// 항목 높이
	m_stThemeInfo.nItemLeftGap = 17;							// 항목 왼쪽 여백
	m_stThemeInfo.nItemRightGap = 13;							// 항목 오른쪽 여백
	m_stThemeInfo.nSeperateHeight = 8;							// 구분자 높이
	m_stThemeInfo.nBackThick = 2;								// 배경 두께
	m_stThemeInfo.nOutlineThick = 2;							// 외곽선 두께
	m_stThemeInfo.nFontSize = 12;								// 폰트 크기	
	// 서브 메뉴 번호
	m_stThemeInfo.nSubMenuNumber = 0;

	m_stThemeInfo.bItemImageSettled = FALSE;					// 항목 이미지 존재 여부
	m_stThemeInfo.bSeperateImageSettled = FALSE;				// 구분선 이미지 존재 여부
	// 항목 이미지 갯수
	m_stThemeInfo.nItemImageCount = 3;

	m_stThemeInfo.nTransparent = 255;							// 투명도
	m_stThemeInfo.rInitPos = CRect(-1,-1,-1,-1);				// 초기 위치 정보

}

CThemeMenu::~CThemeMenu()
{
}


BEGIN_MESSAGE_MAP(CThemeMenu, CWnd)
	//{{AFX_MSG_MAP(CThemeMenu)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CThemeMenu message handlers

// 메뉴 로딩
void CThemeMenu::LoadMenu(UINT nIDResource)
{
	m_menuMain.LoadMenu(nIDResource);
}

// 서브 메뉴 컨트롤을 삭제하는 함수
void CThemeMenu::DeleteSubMenu()
{
	int i=0;

	for(i=0 ; i < dfMAX_SUB_MENU_LEVEL ; i++)
		if( m_pSubMenu[i] )
		{
			m_pSubMenu[i]->DestroyWindow();
			delete m_pSubMenu[i];
			m_pSubMenu[i] = NULL;
		}
		//M_W_DELETE(m_pSubMenu[i]);
}

// 타이머 이벤트 발생시 실행되는 함수
void CThemeMenu::OnTimer(UINT_PTR nIDEvent) 
{
	// 서브 메뉴 컨트롤 삭제 타임 일때
	if( dfID_TIMER_DELETE_SUB_MENU == nIDEvent )
	{
		KillTimer(dfID_TIMER_DELETE_SUB_MENU);

		// 새로 포커스를 얻은 메뉴가 없다면
		if( m_stThemeInfo.nFocusMenuLevel < 0 )
			// 서브 메뉴 컨트롤을 삭제하는 함수
			DeleteSubMenu();
	}
	
	CWnd::OnTimer(nIDEvent);
}

// 클래스 종료시 실행되는 함수
void CThemeMenu::OnDestroy() 
{
	CWnd::OnDestroy();

	// 서브 메뉴 컨트롤을 삭제하는 함수
	DeleteSubMenu();

	// 메뉴 항목 Array 모두 삭제
	m_arMenuItemProperty.RemoveAll();
	
	// 서브 메뉴 삭제 타이머 종료
	KillTimer(dfID_TIMER_DELETE_SUB_MENU);
}

// 메뉴 항목에 마우스 Over 일때 실행되는 함수
void CThemeMenu::OnMenuItemMouseOver(int nLevel, UINT nMenuID, int nItemNumber, CRect rItem)
{
	// 팝업 메뉴 항목이라면
	if( dfPOPUP_MENU_ITEM_ID == nMenuID )
	{
		///*** 기존 팝업 메뉴 컨트롤을 삭제
		int i=0;

		for(i=nLevel+1 ; i < dfMAX_SUB_MENU_LEVEL ; i++)
		{
			if( m_pSubMenu[i] )
			{
				m_pSubMenu[i]->DestroyWindow();
				delete m_pSubMenu[i];
				m_pSubMenu[i] = NULL;
			}
			//M_W_DELETE(m_pSubMenu[i]);
		}

		// 하위 Popup 메뉴 포인터 가져오기
		CMenu *pSubMenu2=NULL;
		//pSubMenu2 = m_pSubMenu[nLevel]->m_pSubMenu->GetSubMenu(nItemNumber);

		// 하위 Popup 메뉴의 위치 계산
		CPoint poMenu;
		poMenu.x = rItem.right-2;
		poMenu.y = rItem.top;

		// 스킨 적용된 서브메뉴를 화면에 표시하는 함수
		ShowSubMenu(nLevel+1, pSubMenu2, poMenu);
	}
}

// 스킨 적용된 서브메뉴를 화면에 표시하는 함수
void CThemeMenu::ShowSubMenu(int nIndex, CPoint poMenu)
{
	//CRect rParent;

	// 서브메뉴 번호
	m_stThemeInfo.nSubMenuNumber = nIndex;

	// 스킨 적용된 서브메뉴를 화면에 표시하는 함수
	ShowSubMenu(poMenu);
	
	/*// 메뉴 위치좌표를 구한다
	GetParent()->GetWindowRect(&rParent);
	//ClientToScreen(&rParent);
	//ScreenToClient(&rParent);
	poMenu.x += rParent.left;
	poMenu.y += rParent.top;

	CMenu *pSubMenu = m_menuMain.GetSubMenu(m_stThemeInfo.nSubMenuNumber);
	// 스킨 적용된 서브메뉴를 화면에 표시하는 함수
	ShowSubMenu(0, pSubMenu, poMenu);*/
}

// 스킨 적용된 서브메뉴를 화면에 표시하는 함수
void CThemeMenu::ShowSubMenu(CPoint poMenu)
{
	CRect rParent;
	
	// 메뉴 위치좌표를 구한다
	GetParent()->GetWindowRect(&rParent);
	//ClientToScreen(&rParent);
	//ScreenToClient(&rParent);
	poMenu.x += rParent.left;
	poMenu.y += rParent.top;

	// 서브 메뉴 포인터를 구한다
	CMenu *pSubMenu = m_menuMain.GetSubMenu(m_stThemeInfo.nSubMenuNumber);

	// 스킨 적용된 서브메뉴를 화면에 표시하는 함수
	ShowSubMenu(0, pSubMenu, poMenu);
}

// 스킨 적용된 서브메뉴를 화면에 표시하는 함수
void CThemeMenu::ShowSubMenu(int nLevel, CMenu *pSubMenu, CPoint poMenu)
{
	CRect rSubMenu;

	rSubMenu.left = poMenu.x;
	rSubMenu.top = poMenu.y;
	rSubMenu.right = rSubMenu.left+10;
	rSubMenu.bottom = rSubMenu.top+10;

	// 기존 서브 메뉴 삭제
	if( m_pSubMenu[nLevel] )
	{
		m_pSubMenu[nLevel]->DestroyWindow();
		delete m_pSubMenu[nLevel];
		m_pSubMenu[nLevel] = NULL;
		//M_W_DELETE(m_pSubMenu[nLevel]);
	}
	// 서브 메뉴 생성
	m_pSubMenu[nLevel] = new CBaseMenu(this);
	m_pSubMenu[nLevel]->m_nMenuLevel = nLevel;
	if( !m_pSubMenu[nLevel]->Create(NULL, NULL, WS_CHILD/*|WS_VISIBLE*/|WS_TABSTOP, rSubMenu, this, dfIDC_SUB_MENU1+nLevel) )
	{
		m_pSubMenu[nLevel]->DestroyWindow();
		delete m_pSubMenu[nLevel];
		m_pSubMenu[nLevel] = NULL;
		return;
	}

	// 컬러 설정
	m_pSubMenu[nLevel]->m_crNormalItemBack = m_stThemeInfo.crNormalItemBack;	// 정상 항목 배경 컬러
	m_pSubMenu[nLevel]->m_crHoverItemBack = m_stThemeInfo.crHoverItemBack;		// 마우스 오버 항목 배경 컬러
	m_pSubMenu[nLevel]->m_crPressedItemBack = m_stThemeInfo.crPressedItemBack;	// 선택 항목 배경 컬러
	m_pSubMenu[nLevel]->m_crFontEnable = m_stThemeInfo.crFontEnable;			// 폰트 컬러(사용 가능)
	m_pSubMenu[nLevel]->m_crFontDisable = m_stThemeInfo.crFontDisable;			// 폰트 컬러(사용 불가능)
	m_pSubMenu[nLevel]->m_crBack = m_stThemeInfo.crBack;						// 배경 컬러
	m_pSubMenu[nLevel]->m_crOutLineLT = m_stThemeInfo.crOutlineLT;				// 테두리(Left-Top) 컬러
	m_pSubMenu[nLevel]->m_crOutLineRB = m_stThemeInfo.crOutlineRB;				// 테두리(Right-Bottom) 컬러
	m_pSubMenu[nLevel]->m_crSeperateUp = m_stThemeInfo.crSeperateUp;			// 구분선 위쪽 컬러
	m_pSubMenu[nLevel]->m_crSeperateDown = m_stThemeInfo.crSeperateDown;		// 구분선 아래쪽 컬러

	// 수치값 설정
	m_pSubMenu[nLevel]->m_nItemHeight = m_stThemeInfo.nItemHeight;				// 항목 높이
	m_pSubMenu[nLevel]->m_nItemLeftGap = m_stThemeInfo.nItemLeftGap;			// 항목 왼쪽 여백
	m_pSubMenu[nLevel]->m_nItemRightGap = m_stThemeInfo.nItemRightGap;			// 항목 오른쪽 여백
	m_pSubMenu[nLevel]->m_nSeperateHeight = m_stThemeInfo.nSeperateHeight;			// 구분자 높이
	m_pSubMenu[nLevel]->m_nBackTick = m_stThemeInfo.nBackThick;					// 배경 두께
	m_pSubMenu[nLevel]->m_nOutLineTick = m_stThemeInfo.nOutlineThick;			// 외곽선 두께
	m_pSubMenu[nLevel]->m_nFontSize = m_stThemeInfo.nFontSize;					// 폰트 크기

	// 항목 이미지가 존재한다면
	if( m_stThemeInfo.bItemImageSettled )
	{
		// 항목 이미지 설정 함수
		m_pSubMenu[nLevel]->SetItemImage(&m_stThemeInfo.bmpItem);
		// 항목 이미지 갯수를 설정하는 함수
		m_pSubMenu[nLevel]->SetItemImageCount(m_stThemeInfo.nItemImageCount);
	}
	
	// 구분선 이미지가 존재한다면
	if( m_stThemeInfo.bSeperateImageSettled )
		// 구분선 이미지 설정 함수
		m_pSubMenu[nLevel]->SetSeperateImage(&m_stThemeInfo.bmpSeperate);

	// 다른 윈도우에 가려지지 않는 최상위 윈도우로 설정
	m_pSubMenu[nLevel]->SetWindowPos(&wndTopMost, rSubMenu.left,rSubMenu.top,10,10, SWP_NOSIZE);

	// 메뉴 설정 함수
	m_pSubMenu[nLevel]->SetMenu(nLevel, pSubMenu);
	
	// 투명도 설정
	m_pSubMenu[nLevel]->SetTransParent(m_stThemeInfo.nTransparent);
	// 메뉴를 화면에 표시
	m_pSubMenu[nLevel]->ShowWindow(SW_SHOW);
}

// 서브 메뉴에 포커스가 떠났을때 실행되는 함수
void CThemeMenu::OnKillFocusSubMenu(int nLevel)
{
	// 포커스 지정된 메뉴 레벨 초기화
	m_stThemeInfo.nFocusMenuLevel = -1;

	// 서브 메뉴 컨트롤 삭제 타이머 발생
	SetTimer(dfID_TIMER_DELETE_SUB_MENU, 50, NULL);
}

// 서브 메뉴에 포커스 지정될때 실행되는 함수
void CThemeMenu::OnSetFocusSubMenu(int nLevel)
{
	// 포커스 지정된 메뉴 레벨 저장
	m_stThemeInfo.nFocusMenuLevel = nLevel;
}

// 메인 메뉴 포인터를 반환
CMenu* CThemeMenu::GetMenu()
{
	return &m_menuMain;
}

// 메뉴 항목 Array에서 해당 메뉴 ID의 위치를 찾는다
int CThemeMenu::FindPositionInArrayMenuItem(UINT nMenuID)
{
	int i=0, nArrSize=0;

	nArrSize = m_arMenuItemProperty.GetSize();

	for(i=0 ; i < nArrSize ; i++)
	{
		// 컨트롤 종류가 체크박스 일때
		if( m_arMenuItemProperty[i].nControlType == dfCONTROL_TYPE_CHECK )
		{
			if( m_arMenuItemProperty[i].nMenuID1 == nMenuID )
				return i;
		}
		// 컨트롤 종류가 라디오버튼 일때
		else if( m_arMenuItemProperty[i].nControlType == dfCONTROL_TYPE_RADIO )
		{
			if( m_arMenuItemProperty[i].nMenuID1 <= nMenuID && m_arMenuItemProperty[i].nMenuID2 >= nMenuID )
				return i;
		}
	}

	return -1;
}

// 메뉴 항목을 체크박스 컨트롤로 설정하는 함수
void CThemeMenu::SetCheckMenuItem(UINT nMenuID)
{	
	int nPosition=-1;

	// 메뉴 항목 Array에서 해당 메뉴 ID의 위치를 찾는다
	nPosition = FindPositionInArrayMenuItem(nMenuID);
	if( nPosition >= 0 )
		m_arMenuItemProperty.RemoveAt(nPosition);

	ST_MENU_ITEM_PROPERTY stMenuItemProperty; 

	stMenuItemProperty.nMenuID1 = nMenuID;
	stMenuItemProperty.nControlType = dfCONTROL_TYPE_CHECK;
	
	m_arMenuItemProperty.Add(stMenuItemProperty);
}

// 메뉴 항목을 라디오버튼 컨트롤로 설정하는 함수
void CThemeMenu::SetRadioMenuItem(UINT nIdFirst, UINT nIdLast)
{
	int nPosition=-1;

	// 메뉴 항목 Array에서 해당 메뉴 ID의 위치를 찾는다
	nPosition = FindPositionInArrayMenuItem(nIdFirst);
	if( nPosition >= 0 )
		m_arMenuItemProperty.RemoveAt(nPosition);

	ST_MENU_ITEM_PROPERTY stMenuItemProperty; 

	stMenuItemProperty.nMenuID1 = nIdFirst;
	stMenuItemProperty.nMenuID2 = nIdLast;
	stMenuItemProperty.nControlType = dfCONTROL_TYPE_RADIO;

	m_arMenuItemProperty.Add(stMenuItemProperty);

}

// 정상 항목 컬러 설정 함수
void CThemeMenu::SetNormalBackColor(COLORREF crNormalBack)
{
	m_stThemeInfo.crNormalItemBack = crNormalBack;
}

// 마우스 오버 항목 컬러 설정 함수
void CThemeMenu::SetHoverBackColor(COLORREF crHoverBack)
{
	m_stThemeInfo.crHoverItemBack = crHoverBack;
}

// 선택 항목 컬러 설정 함수
void CThemeMenu::SetPressedBackColor(COLORREF crPressedBack)
{
	m_stThemeInfo.crPressedItemBack = crPressedBack;
}

// 폰트 컬러(사용 가능) 설정 함수
void CThemeMenu::SetFontColor(COLORREF crFont)
{
	// 폰트 컬러(사용 가능) 설정 함수
	SetFontEnableColor(crFont);
}

// 폰트 컬러(사용 가능) 설정 함수
void CThemeMenu::SetFontEnableColor(COLORREF crFontEnable)
{
	m_stThemeInfo.crFontEnable = crFontEnable;
}

// 폰트 컬러(사용 불가능) 설정 함수
void CThemeMenu::SetFontDisableColor(COLORREF crFontDisable)
{
	m_stThemeInfo.crFontDisable = crFontDisable;
}

// 배경 컬러 설정 함수
void CThemeMenu::SetBackColor(COLORREF crBack)
{
	m_stThemeInfo.crBack = crBack;
}

// 구분선 위쪽 컬러
void CThemeMenu::SetSeperateUpColor(COLORREF crSeperate)
{
	m_stThemeInfo.crSeperateUp = crSeperate;
}

// 구분선 아래쪽 컬러
void CThemeMenu::SetSeperateDownColor(COLORREF crSeperate)
{
	m_stThemeInfo.crSeperateDown = crSeperate;
}

// 항목 높이 설정 함수
void CThemeMenu::SetItemHeight(int nItemHeight)
{
	m_stThemeInfo.nItemHeight = nItemHeight;
}

// 항목 왼쪽 여백 설정 함수
void CThemeMenu::SetItemLeftGap(int nItemLeftGap)
{
	m_stThemeInfo.nItemLeftGap = nItemLeftGap;
}

// 항목 오른쪽 여백 설정 함수
void CThemeMenu::SetItemRightGap(int nItemRightGap)
{
	m_stThemeInfo.nItemRightGap = nItemRightGap;
}

// 구분자 높이 설정 함수
void CThemeMenu::SetSeperateHeight(int nSeperateHeight)
{
	m_stThemeInfo.nSeperateHeight = nSeperateHeight;
}

// 배경 두께 설정 함수
void CThemeMenu::SetBackThick(int nBackThick)
{
	m_stThemeInfo.nBackThick = nBackThick;
}

// 외곽선 두께 설정 함수
void CThemeMenu::SetOutlineThick(int nOutlineThick)
{
	m_stThemeInfo.nOutlineThick = nOutlineThick;
}

// 폰트 크기 설정 함수
void CThemeMenu::SetFontSize(int nFontSize)
{
	m_stThemeInfo.nFontSize = nFontSize;
}

// 항목 이미지 갯수를 설정하는 함수
void CThemeMenu::SetItemImageCount(int nImageCount)
{
	m_stThemeInfo.nItemImageCount = nImageCount;
}

// 해당 레벨의 메뉴 포인터를 반환하는 함수
CMenu* CThemeMenu::GetSubMenuPointer(int nMenuLevel)
{
	
	// 메뉴 레벨이 0일때
	if( nMenuLevel == 0 )
		return m_menuMain.GetSubMenu(m_stThemeInfo.nSubMenuNumber);
	
	// 메뉴 레벨이 0이 아니라면
	CMenu *pHighMenu=NULL, *pSubMenu=NULL;
	// 자신을 다시 호출해서 이전 레벨의 메뉴 포인터를 구한다
	pHighMenu = GetSubMenuPointer(nMenuLevel-1);

	// 해당 레벨의 메뉴 포인터를 구한다
	pSubMenu = pHighMenu->GetSubMenu(m_pSubMenu[nMenuLevel-1]->m_nItemNumberMouseOver);

	return pSubMenu;
}

// 서브 메뉴 번호를 설정
void CThemeMenu::SetSubMenuNumber(int nSubMenuNumber)
{
	m_stThemeInfo.nSubMenuNumber = nSubMenuNumber;
}

// 항목 이미지 ID 설정 함수
//DEL void CThemeMenu::SetBmpItem(UINT uIDBmpItem)
//DEL {
//DEL 	m_uIDBmpItem = uIDBmpItem;
//DEL }

// 구분선 이미지 ID 설정 함수
//DEL void CThemeMenu::SetBmpSeperate(UINT uIDBmpSeperate)
//DEL {
//DEL 	m_uIDBmpSeperate = uIDBmpSeperate;
//DEL }

// 항목 이미지를 여는 함수(Resource ID)
BOOL CThemeMenu::SetItemImage(UINT nImageID)
{
	//BOOL bBmpLoad=FALSE;

	// 항목 이미지가 존재한다면 기존 이미지를 삭제
	if( m_stThemeInfo.bItemImageSettled == TRUE )
		m_stThemeInfo.bmpItem.DeleteObject();

	// 이미지를 로딩
	m_stThemeInfo.bItemImageSettled = m_stThemeInfo.bmpItem.LoadBitmap(nImageID);

	/*// 이미지 파일 로딩에 성공했다면
	if( bBmpLoad )
	{
		// 항목 이미지 존재상태로 설정
		m_stThemeInfo.bItemImageSettled = TRUE;
	}*/

	return m_stThemeInfo.bItemImageSettled;
}

// 구분선 이미지를 여는 함수(Resource ID)
BOOL CThemeMenu::SetSeperateImage(UINT nImageID)
{
	//BOOL bBmpLoad=FALSE;

	// 구분선 이미지가 존재한다면 기존 이미지를 삭제
	if( m_stThemeInfo.bSeperateImageSettled == TRUE )
		m_stThemeInfo.bmpSeperate.DeleteObject();

	// 이미지를 로딩
	m_stThemeInfo.bItemImageSettled = m_stThemeInfo.bmpSeperate.LoadBitmap(nImageID);

	/*// 이미지 파일 로딩에 성공했다면
	if( bBmpLoad )
	{
		// 구분선 이미지 존재상태로 설정
		m_stThemeInfo.bSeperateImageSettled = TRUE;
	}*/

	return m_stThemeInfo.bItemImageSettled;
}

// 항목 이미지를 여는 함수(파일)
BOOL CThemeMenu::SetItemImage(CString strFileName)
{
	//BOOL bBmpLoad=FALSE;

	// 항목 이미지가 존재한다면 기존 이미지를 삭제
	if( m_stThemeInfo.bItemImageSettled == TRUE )
		m_stThemeInfo.bmpItem.DeleteObject();

	// 이미지 파일을 읽어서 CBitmap으로 반환
	m_stThemeInfo.bItemImageSettled = gf_LoadPictureFile(strFileName, &m_stThemeInfo.bmpItem);

	/*// 이미지 파일 로딩에 성공했다면
	if( bBmpLoad )
	{
		// 항목 이미지 존재상태로 설정
		m_stThemeInfo.bItemImageSettled = TRUE;
	}*/

	return m_stThemeInfo.bItemImageSettled;
}

// 구분선 이미지를 여는 함수(파일)
BOOL CThemeMenu::SetSeperateImage(CString strFileName)
{
	//BOOL bBmpLoad=FALSE;

	// 구분선 이미지가 존재한다면 기존 이미지를 삭제
	if( m_stThemeInfo.bSeperateImageSettled == TRUE )
		m_stThemeInfo.bmpSeperate.DeleteObject();

	// 이미지 파일을 읽어서 CBitmap으로 반환
	m_stThemeInfo.bSeperateImageSettled = gf_LoadPictureFile(strFileName, &m_stThemeInfo.bmpSeperate);

	/*// 이미지 파일 로딩에 성공했다면
	if( bBmpLoad )
	{
		// 구분선 이미지 존재상태로 설정
		m_stThemeInfo.bSeperateImageSettled = TRUE;
	}*/

	return m_stThemeInfo.bSeperateImageSettled;
}

// 클래스 생성
BOOL CThemeMenu::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, pContext);
}

// 메뉴 항목 선택시 실행되는 함수
void CThemeMenu::OnMenuItemSelect(int nLevel, UINT nMenuID, int nItemNumber)
{
	// 팝업 메뉴 항목이라면
	if( dfPOPUP_MENU_ITEM_ID == nMenuID )
	{
		return;
	}
	// 일반 메뉴 항목이라면
	else
	{
		//CMenu *pSubMenu;
		//pSubMenu = GetSubMenuPointer(nLevel);

		// 메뉴 항목 컨트롤 상태를 설정하는 함수
		//SetItemControlState(pSubMenu, nMenuID);
		
		// 서브 메뉴 컨트롤을 삭제하는 함수
		DeleteSubMenu();

		// 부모 윈도우로 메뉴 항목 클릭 메시지 전송
		this->GetParent()->SendMessage(WM_COMMAND, (WPARAM)nMenuID, 0);
	}
}

// 투명도 설정
void CThemeMenu::SetTransparent(BYTE byteDegree)
{
	m_stThemeInfo.nTransparent = (int)byteDegree;
}

// 메뉴 항목 컨트롤 상태를 설정하는 함수
void CThemeMenu::SetItemControlState(CMenu *pSubMenu, UINT nMenuID)
{
	int nPosition=-1;

	// 메뉴 항목 Array에서 해당 메뉴 ID의 위치를 찾는다
	nPosition = FindPositionInArrayMenuItem(nMenuID);

	// 컨트롤로 지정된 항목이 아니라면 함수 탈출
	if( nPosition < 0 )
		return;

	// 컨트롤 종류가 'Check'라면
	if( m_arMenuItemProperty[nPosition].nControlType == dfCONTROL_TYPE_CHECK )
	{
		UINT nState = pSubMenu->GetMenuState(nMenuID, MF_BYCOMMAND);
		if (nState & MF_CHECKED)
		{
			pSubMenu->CheckMenuItem(nMenuID, MF_UNCHECKED | MF_BYCOMMAND);

			//m_arMenuItemProperty[nPosition].bCheck = FALSE;
		}
		else
		{
			pSubMenu->CheckMenuItem(nMenuID, MF_CHECKED | MF_BYCOMMAND);

			//m_arMenuItemProperty[nPosition].bCheck = TRUE;
		}
	}
	// 컨트롤 종류가 'Radio'라면
	else if( m_arMenuItemProperty[nPosition].nControlType == dfCONTROL_TYPE_RADIO )
	{
		UINT nIdFirst=0, nIdLast=0;
		nIdFirst = m_arMenuItemProperty[nPosition].nMenuID1;
		nIdLast = m_arMenuItemProperty[nPosition].nMenuID2;
		pSubMenu->CheckMenuRadioItem(nIdFirst, nIdLast, nMenuID, MF_BYCOMMAND);
	}
}

// 메뉴 항목을 체크/언체크 설정
BOOL CThemeMenu::SetCheck(UINT nMenuID, BOOL bCheck)
{
	int nPosition=-1;

	// 메뉴 항목 Array에서 해당 메뉴 ID의 위치를 찾는다
	nPosition = FindPositionInArrayMenuItem(nMenuID);

	if( nPosition < 0 )
		return FALSE;

	// 체크 해제라면
	if( !bCheck )
	{
		// 해당 항목 체크 해제
		m_menuMain.CheckMenuItem(nMenuID, MF_UNCHECKED | MF_BYCOMMAND);
	}
	// 체크 설정이라면
	else
	{
		// 컨트롤 종류가 'Check'라면
		if( m_arMenuItemProperty[nPosition].nControlType == dfCONTROL_TYPE_CHECK )
			m_menuMain.CheckMenuItem(nMenuID, MF_CHECKED | MF_BYCOMMAND);
		// 컨트롤 종류가 'Radio'라면
		else if( m_arMenuItemProperty[nPosition].nControlType == dfCONTROL_TYPE_RADIO )
		{
			UINT nIdFirst=0, nIdLast=0;
			nIdFirst = m_arMenuItemProperty[nPosition].nMenuID1;
			nIdLast = m_arMenuItemProperty[nPosition].nMenuID2;
			m_menuMain.CheckMenuRadioItem(nIdFirst, nIdLast, nMenuID, MF_BYCOMMAND);
			
		}
		// Check나 Radio 컨트롤이 아니라면
		else
			return FALSE;
	}
	
	return TRUE;
}

// 메뉴 항목 체크 여부 반환
BOOL CThemeMenu::GetCheck(UINT nMenuID)
{
	UINT nState=0;

	nState = m_menuMain.GetMenuState(nMenuID, MF_BYCOMMAND);

	if( (nState&MF_CHECKED) == MF_CHECKED )
		return TRUE;

	return FALSE;
}

// 외곽선(Left-Top) 컬러 설정 함수
void CThemeMenu::SetOutlineLTColor(COLORREF crOutlineLT)
{
	m_stThemeInfo.crOutlineLT = crOutlineLT;
}

// 외곽선(Right-Bottom) 컬러 설정 함수
void CThemeMenu::SetOutlineRBColor(COLORREF crOutlineRB)
{
	m_stThemeInfo.crOutlineRB = crOutlineRB;
}

// 외곽선 컬러 설정 함수
void CThemeMenu::SetOutlineColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineLT = crOutline;
	m_stThemeInfo.crOutlineRB = crOutline;
}
