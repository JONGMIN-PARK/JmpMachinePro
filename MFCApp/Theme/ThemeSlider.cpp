// ThemeSlider.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "ThemeSlider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThemeSlider

CThemeSlider::CThemeSlider()
{

	m_pBtnBar = NULL;											// Bar 버튼 컨트롤 초기화

	m_stThemeInfo.strIniCtrlName = "";							// INI 파일 조회용 컨트롤명
	m_stThemeInfo.nMin = 0;										// 범위-최소값
	m_stThemeInfo.nMax = 100;									// 범위-최대값
	m_stThemeInfo.nPos = 0;										// 현재 설정값
	m_stThemeInfo.nPageUnit = 0;								// 스크롤 페이지 단위
	m_stThemeInfo.bAutoToolTip = FALSE;							// 자동 툴팁
	m_stThemeInfo.bVertical = FALSE;							// 방향-수직 여부

	// 테마 관련 변수 초기화
	InitThemeVariable();

}

// 테마 관련 변수 초기화
void CThemeSlider::InitThemeVariable()
{
	if( m_pBtnBar )
		m_pBtnBar->InitThemeVariable();							// Bar 버튼 컨트롤 전역변수 초기화

	// 배경 그리기 방식이 이미지라면 기존 이미지를 삭제
	if( m_stThemeInfo.nBackDrawType == dfBACK_DRAW_TYPE_IMAGE )
		m_stThemeInfo.bmpBack.DeleteObject();

	// 테마 컨트롤 옵션 초기화
	m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_COLOR;		// 배경 그리기 방식
	m_stThemeInfo.nBackImageFillType = dfBACK_IMAGE_FILL_TYPE_BLOCK;	// 배경 이미지 채우기 방식
	m_stThemeInfo.nBackImageAlignFormat = DT_CENTER|DT_VCENTER;	// 배경 이미지 정렬방식
	m_stThemeInfo.crBackground = RGB(255,255,255);				// 배경 컬러
	m_stThemeInfo.crOutlineLT = RGB(200,200,200);				// 테두리 컬러(Left-Top)
	m_stThemeInfo.crOutlineRB = RGB(200,200,200);				// 테두리 컬러(Right-Bottom)
	m_stThemeInfo.nOutlineThick = 1;							// 테두리 두께
	m_stThemeInfo.nEdgeRoundValue = 0;							// 모서리 둥근 정보
	m_stThemeInfo.rInitPos = CRect(-1,-1,-1,-1);				// 초기 위치 정보

	// Bar 버튼 컨트롤 설정값 초기화
	m_stBtnBarInfo.uIDImage = 0;
	m_stBtnBarInfo.strImageFile = "";
	m_stBtnBarInfo.nImageCount = 4;
	m_stBtnBarInfo.bMakeRgn = FALSE;
	m_stBtnBarInfo.nWidth = 20;									// Bar 넓이
	m_stBtnBarInfo.nHeight = 30;								// Bar 높이
	m_stBtnBarInfo.bLButtonDown = FALSE;						// 왼쪽 버튼 클릭 상태
	
}

CThemeSlider::~CThemeSlider()
{
}


BEGIN_MESSAGE_MAP(CThemeSlider, CWnd)
	//{{AFX_MSG_MAP(CThemeSlider)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(dfWM_BTN_LBUTTON_DOWN, OnBtnLButtonDown)
	ON_MESSAGE(dfWM_BTN_LBUTTON_UP, OnBtnLButtonUp)
	ON_MESSAGE(dfWM_BTN_MOUSE_MOVE, OnBtnMouseMove)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CThemeSlider message handlers

// 클래스 생성
BOOL CThemeSlider::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	
	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, pContext);
}

// 클래스 생성시에 실행되는 함수
int CThemeSlider::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// '수직 방향' 옵션이라면
	if( (lpCreateStruct->style & TBS_VERT) == TBS_VERT )
		m_stThemeInfo.bVertical = TRUE;

	// 컨트롤 생성 함수
	CreateControl();
	
	return 0;
}

void CThemeSlider::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// 화면 영역좌표 구하기
	CRect rScreen;
	this->GetClientRect(&rScreen);

	/*// 메모리 버퍼 구하기
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	// CBitmap 을 생성하고 메모리 DC에 할당
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rScreen.Width(), rScreen.Height());

	MemDC.SelectObject(&bmp);*/

	// 배경 그리기 방식에 따라서
	switch( m_stThemeInfo.nBackDrawType )
	{
	// 컬러로 배경을 그린다면
	case dfBACK_DRAW_TYPE_COLOR :
		// 컬러로 배경을 그린다
		DrawBackByColor(&dc, m_stThemeInfo.crBackground, rScreen);
		break;
	// 이미지로 배경을 그린다면
	case dfBACK_DRAW_TYPE_IMAGE :
		// 비트맵 이미지(포커스 설정용)로 배경을 그린다
		DrawBackByImage(&dc, &m_stThemeInfo.bmpBack, rScreen);
		break;
	// 배경을 그리지 않는다면
	default :
		break;
	}
	
	// CPaintDC에 메모리 버퍼를 복사
	//dc.BitBlt(rScreen.left,rScreen.top,rScreen.Width(),rScreen.Height(), &MemDC, 0,0,SRCCOPY);
}

// 컬러로 배경을 그린다
void CThemeSlider::DrawBackByColor(CPaintDC *pdc, COLORREF crBack, CRect rScreen)
{
	// 모서리가 둥글다면
	if( m_stThemeInfo.nEdgeRoundValue > 0 )
	{
		// 모서리가 둥근 사각형을 그리는 함수
		gf_DrawRoundEdgeRect(pdc, m_stThemeInfo.crBackground, m_stThemeInfo.crOutlineLT, m_stThemeInfo.nOutlineThick, m_stThemeInfo.nEdgeRoundValue, rScreen);
	}
	// 모서리가 직각이라면
	else
	{
		// 테두리를 가지는 사각형을 그리는 함수
		gf_DrawRectWithOutline(pdc, m_stThemeInfo.crBackground, m_stThemeInfo.crOutlineLT, m_stThemeInfo.crOutlineRB, m_stThemeInfo.nOutlineThick, rScreen);
	}
}

// 비트맵 이미지로 배경을 그린다
void CThemeSlider::DrawBackByImage(CPaintDC *pdc, CBitmap *pBitmap, CRect rScreen)
{
	// '배경 이미지 채우기 방식'에 따라서
	switch( m_stThemeInfo.nBackImageFillType )
	{
	case dfBACK_IMAGE_FILL_TYPE_BLOCK :				// 블럭
		// 이미지를 9조각으로 분할, 크기 변경하여 화면에 표시하는 함수(투명 지원)
		gf_DrawImageTransparentResizeBlock(pdc, pBitmap, rScreen);
		break;
	case dfBACK_IMAGE_FILL_TYPE_RESIZE :			// 늘이기
		// 이미지 크기 변경하여 그리는 함수(투명 지원)
		gf_DrawImageResizeTransparent(pdc, pBitmap, rScreen);
		break;
	case dfBACK_IMAGE_FILL_TYPE_PATTERN :			// 바둑판식
		// 이미지를 Pattern 브러쉬 형식으로 화면 영역에 그리는 함수(바둑판 형식)
		DrawImageByPatternBrush(pdc, pBitmap, rScreen);
		break;
	case dfBACK_IMAGE_FILL_TYPE_ONCE :				// 1회
		// 이미지를 화면에 그리는 함수(원본 그대로 1회)
		gf_DrawImageTransparent(pdc, pBitmap, rScreen, m_stThemeInfo.nBackImageAlignFormat);
		break;
	default :
		break;
	}
}

// 이미지를 Pattern 브러쉬 형식으로 화면 영역에 그리는 함수(바둑판 형식)
void CThemeSlider::DrawImageByPatternBrush(CPaintDC *pdc, CBitmap *pBitmap, CRect rArea)
{
	CBrush brImage, *pOldBrush;

	// 이미지 Pattern 브러쉬 생성
    brImage.CreatePatternBrush(pBitmap);
    pOldBrush = pdc->SelectObject(&brImage);

	// 이미지 Pattern 브러쉬로 사각형 영역을 채운다
	pdc->FillRect(rArea, &brImage);

	// 브러쉬 초기화
	pdc->SelectObject(pOldBrush);
}

// 컨트롤 크기 변경시 실행되는 함수
void CThemeSlider::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	int nLimit=8;
	// 크기가 너무 작다면 무시하고 함수 탈출
	if( cx < 8 || cy < 8 )
		return;
	
	// 배경 그리기 방식이 '이미지'가 아니라면
	if( m_stThemeInfo.nBackDrawType != dfBACK_DRAW_TYPE_IMAGE )
	{
		// 슬라이더 방향이 수직이라면
		if( m_stThemeInfo.bVertical )
		{
			// Bar 버튼 크기 변경
			m_stBtnBarInfo.nWidth = cx;
			m_stBtnBarInfo.nHeight = 20;
		}
		// 슬라이더 방향이 수평이라면
		else
		{
			// Bar 버튼 크기 변경
			m_stBtnBarInfo.nWidth = 20;
			m_stBtnBarInfo.nHeight = cy;
		}
	}

	// 컨트롤을 재배치하는 함수
	RepositionControl();

	// 컨트롤 화면을 새로 그린다
	RedrawControl();
	
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

// 컨트롤 화면을 새로 그린다
void CThemeSlider::RedrawControl()
{
	Invalidate(FALSE);
}

// 배경 컬러 설정
void CThemeSlider::SetBackColor(COLORREF crBack)
{
	m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_COLOR;		// 배경 그리기 방식
	m_stThemeInfo.crBackground = crBack;
}

// 배경 그리기 방식 설정
void CThemeSlider::SetBackDrawType(int nDrawType)
{
	m_stThemeInfo.nBackDrawType = nDrawType;
}

// 배경 이미지를 여는 함수(Resource ID)
BOOL CThemeSlider::SetBackImage(UINT nImageID)
{
	// 배경 그리기 방식이 이미지라면 기존 이미지를 삭제
	if( m_stThemeInfo.nBackDrawType == dfBACK_DRAW_TYPE_IMAGE )
		m_stThemeInfo.bmpBack.DeleteObject();

	// 이미지 파일 로딩에 성공했다면
	if( m_stThemeInfo.bmpBack.LoadBitmap(nImageID) )
	{
		// 배경 그리기 방식을 이미지로 설정
		m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_IMAGE;
		return TRUE;
	}
	// 이미지 파일 로딩에 실패했다면
	else
	{
		// 배경 그리기 방식을 Default로 설정
		m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_COLOR;
	}

	return FALSE;
}

// 배경 이미지를 여는 함수(파일)
BOOL CThemeSlider::SetBackImage(CString strFileName)
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
		m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_COLOR;
	}

	return bBmpLoad;
}

// 배경 이미지 정렬방식 설정(이미지 1회 일때만 적용)
void CThemeSlider::SetBackImageAlignFormat(UINT nAlignFormat)
{
	m_stThemeInfo.nBackImageAlignFormat = nAlignFormat;
}

// 배경 이미지 채우기 방식 설정
void CThemeSlider::SetBackImageFillType(int nImageFillType)
{
	m_stThemeInfo.nBackImageFillType = nImageFillType;
}

// 배경 모서리 둥근 정도 설정
void CThemeSlider::SetEdgeRoundValue(int nRoundValue)
{
	m_stThemeInfo.nEdgeRoundValue = nRoundValue;
}

// 외곽선 컬러 설정
void CThemeSlider::SetOutlineColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineLT = crOutline;
	m_stThemeInfo.crOutlineRB = crOutline;
}

// 외곽선(Left-Top) 컬러 설정
void CThemeSlider::SetOutlineLTColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineLT = crOutline;
}

// 외곽선(Right-Bottom) 컬러 설정
void CThemeSlider::SetOutlineRBColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineRB = crOutline;
}

// 외곽선 두께 설정
void CThemeSlider::SetOutlineThick(int nThick)
{
	m_stThemeInfo.nOutlineThick = nThick;
}

// 클래스 삭제시에 실행되는 함수
void CThemeSlider::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// Bar 버튼 컨트롤 삭제
	if( m_pBtnBar )
	{
		m_pBtnBar->DestroyWindow();
		delete m_pBtnBar;
		m_pBtnBar = NULL;
	}
	
	// 툴팁 삭제
	DeleteToolTip();
}

// 버튼 컨트롤 배경 이미지 정보를 계산
void CThemeSlider::CalculateBtnImageInfo()
{
	CBitmap bmpBitmap;
	BITMAP bm;
	BOOL bLoad=FALSE;

	if( m_stBtnBarInfo.uIDImage > 0 )
	{
		// 버튼 이미지 로딩
		bLoad = bmpBitmap.LoadBitmap(m_stBtnBarInfo.uIDImage);
		if( !bLoad )
			return;
	}
	else if( m_stBtnBarInfo.strImageFile.GetLength() > 3 )
	{		
		// 이미지 파일을 읽어서 CBitmap으로 반환
		bLoad = gf_LoadPictureFile(m_stBtnBarInfo.strImageFile, &bmpBitmap);
		if( !bLoad )
			return;
	}
	else
		return;

	bmpBitmap.GetBitmap(&bm);

	// Bar 버튼 크기를 구한다
	m_stBtnBarInfo.nWidth = bm.bmWidth / m_stBtnBarInfo.nImageCount;
	m_stBtnBarInfo.nHeight = bm.bmHeight;
}

// 버튼 컨트롤 배경 이미지 설정(파일)
BOOL CThemeSlider::SetBarImage(CString strFileName)
{
	BOOL bLoad=FALSE;

	// 버튼 컨트롤에 이미지 파일 경로 전달
	bLoad = m_pBtnBar->SetBackImage(strFileName);
	if( !bLoad )
		return bLoad;

	// 버튼 Caption 텍스트를 삭제
	m_pBtnBar->SetCaptionText(TEXT(""));

	// 파일 경로를 전역변수에 저장
	m_stBtnBarInfo.strImageFile = strFileName;
	m_stBtnBarInfo.uIDImage = 0;

	// 버튼 컨트롤 배경 이미지 정보를 계산
	CalculateBtnImageInfo();

	//컨트롤을 재배치하는 함수
	RepositionControl();

	return bLoad;
}

// 버튼 컨트롤 배경 이미지 설정(Resource ID)
BOOL CThemeSlider::SetBarImage(UINT nImageID)
{
	BOOL bLoad=FALSE;

	// 버튼 컨트롤에 이미지 Resource ID 전달
	bLoad = m_pBtnBar->SetBackImage(nImageID);
	if( !bLoad )
		return bLoad;

	// 버튼 Caption 텍스트를 삭제
	m_pBtnBar->SetCaptionText(TEXT(""));

	// 파일 경로를 전역변수에 저장
	m_stBtnBarInfo.strImageFile = "";
	m_stBtnBarInfo.uIDImage = nImageID;

	// 버튼 컨트롤 배경 이미지 정보를 계산
	CalculateBtnImageInfo();

	//컨트롤을 재배치하는 함수
	RepositionControl();

	return bLoad;
}

// 버튼 컨트롤 배경 이미지 블럭 개수 설정
void CThemeSlider::SetBarImageBlockCount(int nBlockCount)
{
	// 버튼 컨트롤 배경 이미지 블럭 개수 설정
	m_pBtnBar->SetBackImageBlockCount(nBlockCount);

	m_stBtnBarInfo.nImageCount = nBlockCount;

	// 버튼 컨트롤 배경 이미지 정보를 계산
	CalculateBtnImageInfo();

	// 컨트롤을 재배치하는 함수
	RepositionControl();
}

// 컨트롤을 재배치하는 함수
void CThemeSlider::RepositionControl()
{
	CRect rBar;

	// 슬라이더 방향이 수직일때
	if( m_stThemeInfo.bVertical )
	{
		// Bar의 영역좌표를 구해서 반환(수직)
		rBar = GetBarRectV(m_stThemeInfo.nPos);
	}
	// 슬라이더 방향이 수평일때
	else
	{
		// Bar의 영역좌표를 구해서 반환(수평)
		rBar = GetBarRectH(m_stThemeInfo.nPos);
	}

	// Bar 버튼 컨트롤 위치 변경
	m_pBtnBar->MoveWindow(rBar);
}

// 컨트롤 생성 함수
void CThemeSlider::CreateControl()
{
	CRect rCtrl;

	// 'Bar' 버튼 컨트롤 생성
	rCtrl = CRect(0,0, m_stBtnBarInfo.nWidth, m_stBtnBarInfo.nHeight);
	m_pBtnBar = new CThemeButton();
	m_pBtnBar->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP, rCtrl, this, dfIDC_BUTTON_CTRL);
	m_pBtnBar->SetBackColor(RGB(193,211,251));
	
}

// 마우스 왼쪽 버튼 더블 클릭시 실행되는 함수
void CThemeSlider::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// 마우스 왼쪽 버튼 클릭 다운시 처리하는 함수
	ProcessLButtonDown(point);
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

// 마우스 왼쪽 버튼 클릭 다운시 실행되는 함수
void CThemeSlider::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// 마우스 왼쪽 버튼 클릭 다운시 처리하는 함수
	ProcessLButtonDown(point);
	
	CWnd::OnLButtonDown(nFlags, point);
}

// 이벤트 메세지를 부모 윈도우로 전송하는 함수
LRESULT CThemeSlider::SendMessageToParent(int nPos, int nMessage)
{

    NM_LISTVIEW nmgv;
	nmgv.iItem        = nPos;
    nmgv.iSubItem     = nPos;
    nmgv.hdr.hwndFrom = m_hWnd;
    nmgv.hdr.idFrom   = GetDlgCtrlID();
    nmgv.hdr.code     = nMessage;

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
    else
        return 0;
}

// 현재 설정값을 반환
int CThemeSlider::GetPos()
{
	return m_stThemeInfo.nPos;
}

// 설정값 지정
void CThemeSlider::SetPos(int nPos)
{
	m_stThemeInfo.nPos = nPos;
	
	// 컨트롤을 재배치하는 함수
	RepositionControl();
}

// 범위 지정
void CThemeSlider::SetRange(int nMin, int nMax)
{
	m_stThemeInfo.nMin = nMin;
	m_stThemeInfo.nMax = nMax;
	
	// 컨트롤을 재배치하는 함수
	RepositionControl();
}

// 범위-최소값 지정
void CThemeSlider::SetRangeMin(int nMin)
{
	m_stThemeInfo.nMin = nMin;
	
	// 컨트롤을 재배치하는 함수
	RepositionControl();
}

// 범위-최대값 지정
void CThemeSlider::SetRangeMax(int nMax)
{
	m_stThemeInfo.nMax = nMax;
	
	// 컨트롤을 재배치하는 함수
	RepositionControl();
}

// 범위를 반환
void CThemeSlider::GetRange(int &nMin, int &nMax)
{
	nMin = m_stThemeInfo.nMin;
	nMax = m_stThemeInfo.nMax;
}

// 범위-최소값 반환
void CThemeSlider::GetRangeMin(int &nMin)
{
	nMin = m_stThemeInfo.nMin;
}

// 범위-최대값 반환
void CThemeSlider::GetRangeMax(int &nMax)
{
	nMax = m_stThemeInfo.nMax;
}

// 마우스 왼쪽 버튼 클릭 해제시 실행되는 함수
void CThemeSlider::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// 마우스 왼쪽 버튼 클릭 해제시 처리하는 함수
	ProcessLButtonUp(point);
	
	CWnd::OnLButtonUp(nFlags, point);
}

// 마우스 왼쪽 버튼 클릭 해제시 처리하는 함수
void CThemeSlider::ProcessLButtonUp(CPoint point)
{
	// 이벤트 메세지를 부모 윈도우로 전송하는 함수
	SendMessageToParent(m_stThemeInfo.nPos, NM_RELEASEDCAPTURE);
}

// 툴팁을 생성한다
void CThemeSlider::MakeToolTip(CString strToolTip)
{
	// 현 컨트롤 ID를 구한다
	UINT unWindowID = (UINT)this->GetSafeHwnd();

	// 툴팁 생성
	m_ToolTip.CreateToolTip(this, unWindowID, strToolTip);
}

// 툴팁 텍스트 변경
void CThemeSlider::ChangeToolTip(CString strToolTip)
{
	// 툴팁 텍스트 변경
	m_ToolTip.ChangeToolTip(strToolTip);
}

// 툴팁 삭제
void CThemeSlider::DeleteToolTip()
{
	if( !m_ToolTip.GetSafeHwnd() )
		return;

	m_ToolTip.DelTool(this);
}

// 툴팁 문자열 설정
void CThemeSlider::SetToolTip(CString strText)
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

// 툴팁을 위한 마우스 이벤트를 받는 함수
LRESULT CThemeSlider::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
			if( m_ToolTip.GetSafeHwnd() )
			{
				MSG msg;
				msg.hwnd = m_hWnd;
				msg.message = message;
				msg.wParam = wParam;
				msg.lParam = lParam;

				m_ToolTip.RelayEvent(&msg);
			}
			break;
		default:
			break;
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

// '자동 툴팁' 여부를 설정
void CThemeSlider::SetAutoToolTip(BOOL bAutoToolTip)
{
	m_stThemeInfo.bAutoToolTip = bAutoToolTip;

	// 자동으로 툴팁을 표시한다면
	if( bAutoToolTip )
	{
		// 아직 툴팁이 생성되지 않았다면 툴팁 생성
		if( !m_ToolTip.GetSafeHwnd() )
			MakeToolTip(TEXT(""));
	}
}

// 마우스 포인터 위치에 따라서 설정값을 반환
int CThemeSlider::GetPosValueByMouse(CPoint point)
{
	int nPosValue=0;
	
	// 슬라이드 방향이 수직일때
	if( m_stThemeInfo.bVertical )
	{
		// 마우스 포인터 위치에 따라서 설정값을 반환(수직)
		nPosValue = GetPosValueByMouseV(point);
	}
	// 슬라이드 방향이 수평일때
	else
	{
		// 마우스 포인터 위치에 따라서 설정값을 반환(수평)
		nPosValue = GetPosValueByMouseH(point);
	}

	return nPosValue;
}

// 마우스 포인터 이동시 실행되는 함수
void CThemeSlider::OnMouseMove(UINT nFlags, CPoint point) 
{
	// 자동 툴팁 이라면
	if( m_stThemeInfo.bAutoToolTip )
		// 마우스 위치에 따라서 툴팁을 설정한다
		SetToolTipByMouse(point);

	CWnd::OnMouseMove(nFlags, point);
	
	// 부모 윈도우로 마우스 포인트 이동 이벤트 전달
	GetParent()->PostMessage(dfWM_SLIDER_MOUSE_MOVE, WPARAM(this->GetDlgCtrlID()), MAKELPARAM(point.x, point.y));

	// 부모 윈도우쪽 코딩
	// BEGIN_MESSAGE_MAP(C~, C~)
	//		ON_MESSAGE(dfWM_SLIDER_MOUSE_MOVE, OnSliderMouseMove)
	// END_MESSAGE_MAP()
	//
	// // 슬라이더 컨트롤에서 마우스 이동시 실행되는 함수
	// LRESULT C~::OnSliderMouseMove(WPARAM wParam, LPARAM lParam)
	// {
	//		CPoint point;
	//		point = (CPoint)lParam;
	//	
	//		switch( (UINT)wParam )
	//		{
	//		case IDC_SLIDER_CTRL :
	//			~
	//			break;
	//		default :
	//			break;
	//		}
	//	
	//		return TRUE;
	//	}
}

// 마우스 위치에 따라서 툴팁을 설정한다
void CThemeSlider::SetToolTipByMouse(CPoint point)
{
	int nMousePos=0;
	CString strText;
	
	// 슬라이드 방향이 수직일때
	if( m_stThemeInfo.bVertical )
	{
		// 마우스 포인터 위치에 따라서 설정값을 반환(수직)
		nMousePos = GetPosValueByMouseV(point);
	}
	// 슬라이드 방향이 수평일때
	else
	{
		// 마우스 포인터 위치에 따라서 설정값을 반환(수평)
		nMousePos = GetPosValueByMouseH(point);
	}

	// 툴팁 문자열 생성
	strText.Format(TEXT("%d"), nMousePos);
	// 툴팁 문자열 변경
	ChangeToolTip(strText);

}

// 방향이 수직인지 여부를 설정
void CThemeSlider::SetVertical(BOOL bVertical)
{
	m_stThemeInfo.bVertical = bVertical;
}

// Bar의 영역좌표를 구해서 반환(수평)
CRect CThemeSlider::GetBarRectH(int nPosValue)
{
	int nBarPosition=0;
	CRect rBar, rScreen;
	
	// Bar의 위치를 구해서 반환(수평)
	nBarPosition = GetBarPositionH(nPosValue);

	// 화면 영역좌표를 구한다
	this->GetClientRect(&rScreen);

	// Bar Left 위치좌표
	rBar.left = nBarPosition -= (m_stBtnBarInfo.nWidth / 2);
	rBar.right = rBar.left + m_stBtnBarInfo.nWidth;

	// Bar Top 위치좌표
	rBar.top = rScreen.CenterPoint().y - (m_stBtnBarInfo.nHeight / 2);
	rBar.bottom = rBar.top + m_stBtnBarInfo.nHeight;

	return rBar;
}

// Bar의 영역좌표를 구해서 반환(수직)
CRect CThemeSlider::GetBarRectV(int nPosValue)
{
	int nBarPosition=0;
	CRect rBar, rScreen;
	
	// Bar의 위치를 구해서 반환(수직)
	nBarPosition = GetBarPositionV(nPosValue);

	// 화면 영역좌표를 구한다
	this->GetClientRect(&rScreen);

	// Bar Top 위치좌표
	rBar.top = nBarPosition -= (m_stBtnBarInfo.nHeight / 2);
	rBar.bottom = rBar.top + m_stBtnBarInfo.nHeight;

	// Bar left 위치좌표
	rBar.left = rScreen.CenterPoint().x - (m_stBtnBarInfo.nWidth / 2);
	rBar.right = rBar.left + m_stBtnBarInfo.nWidth;

	return rBar;
}

// Bar의 위치를 구해서 반환(수평)
int CThemeSlider::GetBarPositionH(int nPosValue)
{
	CRect rScreen;
	int nBarMoveLength=0, nBarPosition=0, nRange=0;

	// 화면 영역좌표를 구한다
	this->GetClientRect(&rScreen);

	// 화면 넓이 보다 Bar의 넓이가 더 크다면 함수 탈출
	if( rScreen.Width() < m_stBtnBarInfo.nWidth )
		return 0;

	// Bar가 이동할수 있는 거리
	nBarMoveLength = rScreen.Width() - m_stBtnBarInfo.nWidth;

	// 범위
	nRange = m_stThemeInfo.nMax - m_stThemeInfo.nMin;
	// 범위가 0 보다 작다면 함수 탈출
	if( nRange <= 0 )
		return 0;
	// 설정값이 범위를 벗어난다면 함수 탈출
	if( nPosValue < m_stThemeInfo.nMin || nPosValue > m_stThemeInfo.nMax )
		return 0;

	// Bar의 위치
	nBarPosition = (int)((double)nBarMoveLength * (double)(nPosValue-m_stThemeInfo.nMin) / (double)nRange);

	// Bar 위치에 왼쪽 여백을 더한다
	nBarPosition += m_stBtnBarInfo.nWidth / 2;

	return nBarPosition;
}

// Bar의 위치를 구해서 반환(수직)
int CThemeSlider::GetBarPositionV(int nPosValue)
{
	CRect rScreen;
	int nBarMoveLength=0, nBarPosition=0, nRange=0;

	// 화면 영역좌표를 구한다
	this->GetClientRect(&rScreen);

	// 화면 넓이 보다 Bar의 높이가 더 크다면 함수 탈출
	if( rScreen.Height() < m_stBtnBarInfo.nHeight )
		return 0;

	// Bar가 이동할수 있는 거리
	nBarMoveLength = rScreen.Height() - m_stBtnBarInfo.nHeight;

	// 범위
	nRange = m_stThemeInfo.nMax - m_stThemeInfo.nMin;
	// 범위가 0 보다 작다면 함수 탈출
	if( nRange <= 0 )
		return 0;
	// 설정값이 범위를 벗어난다면 함수 탈출
	if( nPosValue < m_stThemeInfo.nMin || nPosValue > m_stThemeInfo.nMax )
		return 0;

	// Bar의 위치
	nBarPosition = (int)((double)nBarMoveLength * (double)nPosValue / (double)nRange);

	// Bar 위치에 왼쪽 여백을 더한다
	nBarPosition += m_stBtnBarInfo.nHeight / 2;

	return nBarPosition;
}

// 마우스 포인터 위치에 따라서 Bar의 위치를 반환(수평)
CRect CThemeSlider::GetBarPositionByMouseH(CPoint point)
{
	CRect rBar, rScreen;

	// 화면 영역좌표를 구한다
	this->GetClientRect(&rScreen);

	// Bar Left 위치좌표
	rBar.left = point.x - (m_stBtnBarInfo.nWidth / 2);
	if( rBar.left < rScreen.left )
		rBar.left = rScreen.left;
	if( rBar.left > (rScreen.right - m_stBtnBarInfo.nWidth) )
		rBar.left = rScreen.right - m_stBtnBarInfo.nWidth;
	rBar.right = rBar.left + m_stBtnBarInfo.nWidth;

	// Bar Top 위치좌표
	rBar.top = rScreen.CenterPoint().y - (m_stBtnBarInfo.nHeight / 2);
	rBar.bottom = rBar.top + m_stBtnBarInfo.nHeight;

	return rBar;
}

// 마우스 포인터 위치에 따라서 Bar의 위치를 반환(수직)
CRect CThemeSlider::GetBarPositionByMouseV(CPoint point)
{
	CRect rBar, rScreen;

	// 화면 영역좌표를 구한다
	this->GetClientRect(&rScreen);

	// Bar Top 위치좌표
	rBar.top = point.y - (m_stBtnBarInfo.nHeight / 2);
	if( rBar.top < rScreen.top )
		rBar.top = rScreen.top;
	if( rBar.top > (rScreen.bottom - m_stBtnBarInfo.nHeight) )
		rBar.top = rScreen.bottom - m_stBtnBarInfo.nHeight;
	rBar.bottom = rBar.top + m_stBtnBarInfo.nHeight;

	// Bar Left 위치좌표
	rBar.left = rScreen.CenterPoint().x - (m_stBtnBarInfo.nWidth / 2);
	rBar.right = rBar.left + m_stBtnBarInfo.nWidth;

	return rBar;
}

// 마우스 포인터 위치에 따라서 설정값을 반환(수평)
int CThemeSlider::GetPosValueByMouseH(CPoint point)
{
	CRect rScreen;
	int nBarMoveLength=0, nRange, nPosValue=0;

	// 화면 영역좌표를 구한다
	this->GetClientRect(&rScreen);

	// 화면 넓이 보다 Bar의 넓이가 더 크다면 함수 탈출
	if( rScreen.Width() < m_stBtnBarInfo.nWidth )
		return 0;

	// Bar가 이동할수 있는 거리
	nBarMoveLength = rScreen.Width() - m_stBtnBarInfo.nWidth;

	// 범위
	nRange = m_stThemeInfo.nMax - m_stThemeInfo.nMin;
	// 범위가 0 보다 작다면 함수 탈출
	if( nRange <= 0 )
		return 0;
	
	// 설정값을 구한다
	nPosValue = (int)((double)(point.x - (m_stBtnBarInfo.nWidth / 2)) / (double)nBarMoveLength * (double)nRange);
	nPosValue += m_stThemeInfo.nMin;

	// 설정값이 범위를 벗어나면
	if( nPosValue > m_stThemeInfo.nMax )
		nPosValue = m_stThemeInfo.nMax;
	if( nPosValue < m_stThemeInfo.nMin )
		nPosValue = m_stThemeInfo.nMin;

	return nPosValue;
}

// 마우스 포인터 위치에 따라서 설정값을 반환(수직)
int CThemeSlider::GetPosValueByMouseV(CPoint point)
{
	CRect rScreen;
	int nBarMoveLength=0, nRange, nPosValue=0;

	// 화면 영역좌표를 구한다
	this->GetClientRect(&rScreen);

	// 화면 넓이 보다 Bar의 높이가 더 크다면 함수 탈출
	if( rScreen.Height() < m_stBtnBarInfo.nHeight )
		return 0;

	// Bar가 이동할수 있는 거리
	nBarMoveLength = rScreen.Height() - m_stBtnBarInfo.nHeight;

	// 범위
	nRange = m_stThemeInfo.nMax - m_stThemeInfo.nMin;
	// 범위가 0 보다 작다면 함수 탈출
	if( nRange <= 0 )
		return 0;
	
	// 설정값을 구한다
	nPosValue = (int)((double)(point.y - (m_stBtnBarInfo.nHeight / 2)) / (double)nBarMoveLength * (double)nRange);
	nPosValue += m_stThemeInfo.nMin;

	// 설정값이 범위를 벗어나면
	if( nPosValue > m_stThemeInfo.nMax )
		nPosValue = m_stThemeInfo.nMax;
	if( nPosValue < m_stThemeInfo.nMin )
		nPosValue = m_stThemeInfo.nMin;

	return nPosValue;
}

// 버튼 컨트롤에 마우스 클릭 다운시 실행되는 함수
LRESULT CThemeSlider::OnBtnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	CRect rBtn;

	switch( (UINT)wParam )
	{
	// Bar 버튼 컨트롤 일때
	case dfIDC_BUTTON_CTRL :
		// 마우스 왼쪽 버튼 클릭 상태 설정
		m_stBtnBarInfo.bLButtonDown = TRUE;

		// 마우스 포인터 위치를 구한다
		point = (CPoint)lParam;

		// Bar 버튼 컨트롤 위치좌표를 구한다
		m_pBtnBar->GetWindowRect(&rBtn);
		ScreenToClient(&rBtn);
		
		// 마우스 포인터 위치를 슬라이더 컨트롤에 대한 상대좌표로 변환
		point.x += rBtn.left;
		point.y += rBtn.top;

		// 마우스 포인터 이동시 처리하는 함수
		ProcessMouseMove(point);
		break;
	default :
		break;
	}

	return TRUE;
}

// 버튼 컨트롤에 마우스 클릭 해제시 실행되는 함수
LRESULT CThemeSlider::OnBtnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	CRect rBtn;
	
	switch( (UINT)wParam )
	{
	// Bar 버튼 컨트롤 일때
	case dfIDC_BUTTON_CTRL :
		// 마우스 왼쪽 버튼 클릭 상태 해제
		m_stBtnBarInfo.bLButtonDown = FALSE;

		// 마우스 포인터 위치를 구한다
		point = (CPoint)lParam;

		// Bar 버튼 컨트롤 위치좌표를 구한다
		m_pBtnBar->GetWindowRect(&rBtn);
		ScreenToClient(&rBtn);
		
		// 마우스 포인터 위치를 슬라이더 컨트롤에 대한 상대좌표로 변환
		point.x += rBtn.left;
		point.y += rBtn.top;

		// 마우스 왼쪽 버튼 클릭 해제시 처리하는 함수
		//ProcessLButtonUp(point);
		// 마우스 왼쪽 버튼 클릭 해제 이벤트 발생
		this->PostMessage(WM_LBUTTONUP, this->GetDlgCtrlID(), MAKELONG(point.x, point.y));
		break;
	default :
		break;
	}

	return TRUE;
}

// 버튼 컨트롤에 마우스 포인트 이동시 실행되는 함수
LRESULT CThemeSlider::OnBtnMouseMove(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	CRect rBtn;
	
	switch( (UINT)wParam )
	{
	// Bar 버튼 컨트롤 일때
	case dfIDC_BUTTON_CTRL :
		// 마우스 왼쪽 버튼 클릭 상태가 아니라면 무시
		if( !m_stBtnBarInfo.bLButtonDown )
			break;

		// 마우스 포인터 위치를 구한다
		point = (CPoint)lParam;

		// Bar 버튼 컨트롤 위치좌표를 구한다
		m_pBtnBar->GetWindowRect(&rBtn);
		ScreenToClient(&rBtn);
		
		// 마우스 포인터 위치를 슬라이더 컨트롤에 대한 상대좌표로 변환
		point.x += rBtn.left;
		point.y += rBtn.top;

		// 마우스 포인터 이동시 처리하는 함수
		ProcessMouseMove(point);
		break;
	default :
		break;
	}

	return TRUE;
}

// 마우스 왼쪽 버튼 클릭 다운시 처리하는 함수
void CThemeSlider::ProcessLButtonDown(CPoint point)
{
	// 페이지 단위가 존재하지 않을때
	if( m_stThemeInfo.nPageUnit <= 0 )
	{
		// 마우스 포인터 이동시 처리하는 함수
		ProcessMouseMove(point);
		// 함수 탈출
		return;
	}
	
	CRect rBar;
	int nPosValue=0;

	// 슬라이드 방향이 수직일때
	if( m_stThemeInfo.bVertical )
	{
		// 마우스 포인터 위치에 따라서 설정값을 반환(수직)
		nPosValue = GetPosValueByMouseV(point);
	}
	// 슬라이드 방향이 수평일때
	else
	{
		// 마우스 포인터 위치에 따라서 설정값을 반환(수평)
		nPosValue = GetPosValueByMouseH(point);
	}

	// 사용자가 Bar의 왼쪽을 클릭했다면
	if( nPosValue < m_stThemeInfo.nPos )
	{
		// 설정값을 페이지 단위 만큼 줄인다
		m_stThemeInfo.nPos -= m_stThemeInfo.nPageUnit;
		if( m_stThemeInfo.nPos < m_stThemeInfo.nMin )
			m_stThemeInfo.nPos = m_stThemeInfo.nMin;
	}
	// 사용자가 Bar의 오른쪽을 클릭했다면
	else
	{
		// 설정값을 페이지 단위 만큼 추가한다
		m_stThemeInfo.nPos += m_stThemeInfo.nPageUnit;
		if( m_stThemeInfo.nPos > m_stThemeInfo.nMax )
			m_stThemeInfo.nPos = m_stThemeInfo.nMax;
	}

	// 컨트롤 위치 변경
	RepositionControl();

	// 이벤트 메세지를 부모 윈도우로 전송하는 함수
	SendMessageToParent(m_stThemeInfo.nPos, NM_CUSTOMDRAW);
}

// 마우스 포인터 이동시 처리하는 함수
void CThemeSlider::ProcessMouseMove(CPoint point)
{
	CRect rBar;

	// 슬라이드 방향이 수직일때
	if( m_stThemeInfo.bVertical )
	{
		// 마우스 포인터 위치에 따라서 설정값을 반환(수직)
		m_stThemeInfo.nPos = GetPosValueByMouseV(point);

		// 마우스 포인터 위치에 따라서 Bar의 위치를 변경(수직)
		rBar = GetBarPositionByMouseV(point);
	}
	// 슬라이드 방향이 수평일때
	else
	{
		// 마우스 포인터 위치에 따라서 설정값을 반환(수평)
		m_stThemeInfo.nPos = GetPosValueByMouseH(point);

		// 마우스 포인터 위치에 따라서 Bar의 위치를 변경(수평)
		rBar = GetBarPositionByMouseH(point);
	}

	// Bar 버튼 컨트롤 위치 이동
	m_pBtnBar->MoveWindow(rBar);

	// 이벤트 메세지를 부모 윈도우로 전송하는 함수
	SendMessageToParent(m_stThemeInfo.nPos, NM_CUSTOMDRAW);
}
