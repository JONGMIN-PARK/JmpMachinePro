// ThemeDialog.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "ThemeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThemeDialog dialog


//CThemeDialog::CThemeDialog(CWnd* pParent /*=NULL*/)
//	: CDialog(CThemeDialog::IDD, pParent)
CThemeDialog::CThemeDialog(UINT uIDTemplate, CWnd* pParent/*=NULL*/)
	: CDialog(uIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CThemeDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// 다이얼로그 ID 저장
	m_unDlgID = uIDTemplate;

	// 다이얼로그 크기 변경 윈도우 초기화
	m_pDlgResizeWnd = NULL;

	m_stThemeInfo.strThemeGroupName = "";						// 테마 그룹명
	m_stThemeInfo.strIniCtrlName = "";							// INI 파일 조회용 컨트롤명
	m_stThemeInfo.strImageFilePath = "Image\\";					// 이미지 파일 폴더 경로
	m_stThemeInfo.strConfigFilePath = "Config\\";				// Config 파일 폴더 경로
	m_stThemeInfo.rInitPos = CRect(-1,-1,-1,-1);				// 초기 위치 정보

	// 테마 관련 변수 초기화
	InitThemeVariable();

}

// 테마 관련 변수 초기화
void CThemeDialog::InitThemeVariable()
{

	// 배경 그리기 방식이 이미지라면 기존 이미지를 삭제
	if( m_stThemeInfo.nBackDrawType == dfBACK_DRAW_TYPE_IMAGE )
		m_stThemeInfo.bmpBackOnFocus.DeleteObject();

	// 포커스 해제용 배경 이미지가 있다면 기존 이미지를 삭제
	if( m_stThemeInfo.bSettledImageKillFocus )
		m_stThemeInfo.bmpBackKillFocus.DeleteObject();

	// 테마 옵션 초기화
	m_stThemeInfo.strCaption = "";								// Caption 문자열
	m_stThemeInfo.rCaption = CRect(27,8,170,24);				// Caption 영역좌표
	m_stThemeInfo.nFontSize = 12;								// 폰트 크기
	m_stThemeInfo.bFontBold = TRUE;								// 폰트 두께
	m_stThemeInfo.crFont = RGB(255,255,255);					// 폰트 컬러
	m_stThemeInfo.crShadow = RGB(0,0,0);						// 그림자 컬러
	m_stThemeInfo.nAlignFormat = DT_LEFT|DT_VCENTER;			// Caption 정렬방식
	m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_COLOR;		// 배경 그리기 방식
	m_stThemeInfo.nBackImageAlignFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;		// 배경 이미지 정렬방식
	m_stThemeInfo.crBackground = RGB(207,224,240);				// 배경 컬러
	m_stThemeInfo.nBackImageFillType = dfBACK_IMAGE_FILL_TYPE_BLOCK;	// 배경 이미지 채우기 방식
	m_stThemeInfo.bSettledImageKillFocus = FALSE;				// 배경 이미지(포커스 해제) 존재여부
	m_stThemeInfo.bFocus = TRUE;								// 포커스 지정 여부
	m_stThemeInfo.bStyleResize = FALSE;							// 스타일 - Resize
	m_stThemeInfo.bMouseDragMove = FALSE;						// 마우스 드래그로 이동 여부
	m_stThemeInfo.nEdgeRange = 7;								// 가장자리 범위
	m_stThemeInfo.nResizingDirection = dfRESIZE_DIRECTION_NONE;	// Resize 방향
	m_stThemeInfo.poMouse = CPoint(0,0);						// 마우스 포인터 위치좌표
	//m_stThemeInfo.bResizeState = FALSE;						// Resize 상태
	m_stThemeInfo.nMinWidth = 50;								// 최소 넓이
	m_stThemeInfo.nMinHeight = 50;								// 최소 높이
	m_stThemeInfo.nMaxWidth = 4000;								// 최대 넓이
	m_stThemeInfo.nMaxHeight = 4000;							// 최대 높이
	m_stThemeInfo.bTransparentByColor = FALSE;					// 컬러로 투명 처리 여부
	m_stThemeInfo.crTransparent = dfCOLOR_TRANSPARENT;			// 투명으로 처리할 컬러
	m_stThemeInfo.byteTransparent = 255;						// 반투명도
	m_stThemeInfo.bChild = FALSE;								// 차일드 형식 여부

}


void CThemeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThemeDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThemeDialog, CDialog)
	//{{AFX_MSG_MAP(CThemeDialog)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//ON_WM_NCPAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_ACTIVATE()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThemeDialog message handlers

// 'Resize' 스타일 설정/해제
void CThemeDialog::SetStyleResize(BOOL bStyleResize)
{
	// 전역변수에 'Resize' 옵션 설정
	m_stThemeInfo.bStyleResize = bStyleResize;
}

// 마우스 드래그로 이동 여부 설정
void CThemeDialog::SetMouseDragMove(BOOL bMouseDragMove)
{
	m_stThemeInfo.bMouseDragMove = bMouseDragMove;
}

// Caption 문자열 설정
void CThemeDialog::SetCaptionText(CString strCaption)
{
	CString strTemp;
	int nPos=0;
	
	// "\n"을 줄바꿈 기호로 변경
	while( (nPos=strCaption.Find(TEXT("\\n"))) >= 0 )
	{
		strTemp = strCaption.Left(nPos) + '\n';
		strTemp += strCaption.Mid(nPos+2);
		strCaption = strTemp;
	}

	// 전역변수에 저장
	m_stThemeInfo.strCaption = strCaption;

	// 줄바꿈 기호를 제거
	while( (nPos=strCaption.Find('\n')) >= 0 )
	{
		strTemp = strCaption.Left(nPos);
		strTemp += strCaption.Mid(nPos+1);
		strCaption = strTemp;
	}

	// 윈도우 Text로 설정
	this->SetWindowText(strCaption);
}

// Caption 영역좌표 설정
void CThemeDialog::SetCaptionArea(CRect rCaption)
{
	// 전역변수에 저장
	m_stThemeInfo.rCaption = rCaption;
}

// Caption 정렬 방식 설정
void CThemeDialog::SetAlignFormat(UINT nAlignFormat)
{
	// 전역변수에 저장
	m_stThemeInfo.nAlignFormat = nAlignFormat;
}

// 폰트 컬러 설정
void CThemeDialog::SetFontColor(COLORREF crFont)
{
	m_stThemeInfo.crFont = crFont;
}

// 그림자 컬러 설정
void CThemeDialog::SetShadowColor(COLORREF crShadow)
{
	m_stThemeInfo.crShadow = crShadow;
}

// 폰트 컬러 설정
void CThemeDialog::SetFontColor(COLORREF crFont, COLORREF crShadow)
{
	m_stThemeInfo.crFont = crFont;
	m_stThemeInfo.crShadow = crShadow;
}

// 폰트 크기 설정
void CThemeDialog::SetFontSize(int nFontSize)
{
	m_stThemeInfo.nFontSize = nFontSize;
}

// 폰트 두께 설정
void CThemeDialog::SetFontBold(BOOL bBold)
{
	m_stThemeInfo.bFontBold = bBold;
}

// 폰트 특성 설정
void CThemeDialog::SetFont(int nFontSize, BOOL bFontBold, COLORREF crFont, COLORREF crShadow, UINT nAlignFormat)
{
	// 폰트 특성을 전역변수에 저장
	m_stThemeInfo.nFontSize = nFontSize;
	m_stThemeInfo.bFontBold = bFontBold;
	m_stThemeInfo.crFont = crFont;
	m_stThemeInfo.crShadow = crShadow;
	m_stThemeInfo.nAlignFormat = nAlignFormat;
}

// 배경 이미지를 여는 함수(포커스 설정-파일)
BOOL CThemeDialog::SetBackImageOnFocus(CString strFileName)
{
	CString strFilePath;
	BOOL bBmpLoad=FALSE;

	// 이미지 파일 전체 경로를 구한다
	strFilePath = FilteringImageFilePath(strFileName);

	// 이미지 파일을 읽어서 CBitmap으로 반환
	bBmpLoad = gf_LoadPictureFile(strFilePath, &m_stThemeInfo.bmpBackOnFocus);

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

// 배경 이미지를 여는 함수(포커스 해제-파일)
BOOL CThemeDialog::SetBackImageKillFocus(CString strFileName)
{
	CString strFilePath;
	BOOL bBmpLoad=FALSE;

	// 이미지 파일 전체 경로를 구한다
	strFilePath = FilteringImageFilePath(strFileName);

	// 이미지 파일을 읽어서 CBitmap으로 반환
	m_stThemeInfo.bSettledImageKillFocus = gf_LoadPictureFile(strFilePath, &m_stThemeInfo.bmpBackKillFocus);

	return m_stThemeInfo.bSettledImageKillFocus;
}

// 배경 이미지를 여는 함수(포커스 설정-Resource ID)
BOOL CThemeDialog::SetBackImageOnFocus(UINT nImageID)
{
	// 이미지 파일 로딩에 성공했다면
	if( m_stThemeInfo.bmpBackOnFocus.LoadBitmap(nImageID) )
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

// 배경 이미지를 여는 함수(포커스 해제-Resource ID)
BOOL CThemeDialog::SetBackImageKillFocus(UINT nImageID)
{
	BOOL bLoad=FALSE;

	bLoad = m_stThemeInfo.bmpBackKillFocus.LoadBitmap(nImageID);
	// 이미지 파일 로딩
	m_stThemeInfo.bSettledImageKillFocus = (bLoad) ? TRUE : FALSE;

	return m_stThemeInfo.bSettledImageKillFocus;
}

// 배경 컬러 설정
void CThemeDialog::SetBackColor(COLORREF crBack)
{
	m_stThemeInfo.crBackground = crBack;
}

// 배경 그리기 방식 설정
void CThemeDialog::SetBackDrawType(int nDrawType)
{
	m_stThemeInfo.nBackDrawType = nDrawType;
}

// 배경 이미지 채우기 방식 설정
void CThemeDialog::SetBackImageFillType(int nImageFillType)
{
	m_stThemeInfo.nBackImageFillType = nImageFillType;
}

// 컬러로 배경을 그린다
void CThemeDialog::DrawBackByColor(CPaintDC *pdc, COLORREF crBack, CRect rScreen)
{
	// 화면을 컬러로 채운다
	pdc->FillRect(rScreen, &CBrush(crBack));

}

// 이미지 파일 전체 경로를 구한다
CString CThemeDialog::FilteringImageFilePath(CString strFileName)
{
	CString strFilePath;

	if( strFileName.Find(TEXT(":")) > 0 )
		return strFileName;
	else if( m_stThemeInfo.strImageFilePath.Find(TEXT(":")) > 0 )
	{
		strFilePath = m_stThemeInfo.strImageFilePath + strFileName;
		return strFilePath;
	}

	//실행파일이 존재하는 폴더 경로 반환
	strFilePath = gf_GetModulePath();
	strFilePath = strFilePath + m_stThemeInfo.strImageFilePath + strFileName;
	
	return strFilePath;
}

//실행파일이 존재하는 폴더 경로 반환
//DEL CString CThemeDialog::GetModulePath()
//DEL {
//DEL 	char szPath[MAX_PATH];
//DEL 	memset(szPath, 0x00, MAX_PATH);
//DEL 
//DEL 	::GetModuleFileName(NULL, szPath, MAX_PATH);
//DEL 
//DEL 	CString sTempPath = szPath;
//DEL 	int iLength = sTempPath.GetLength();
//DEL 	int iPos = sTempPath.ReverseFind(TCHAR('\\'));
//DEL 
//DEL 	CString sModulePath = sTempPath.Left(iPos);
//DEL 	sModulePath += "\\";
//DEL 
//DEL 	return sModulePath;
//DEL 
//DEL }

// 비트맵 이미지로 배경을 그린다
void CThemeDialog::DrawBackByImage(CPaintDC *pdc, CBitmap *pBitmap, CRect rScreen)
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
void CThemeDialog::DrawImageByPatternBrush(CPaintDC *pdc, CBitmap *pBitmap, CRect rArea)
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

// 마우스 포인터가 가장자리 영역에 위치하는지 판단하는 함수
int CThemeDialog::IsMouseInEdgeRange(CPoint poMouse)
{
	CRect rWnd;

	// 자신의 윈도우 크기를 구한다
	this->GetClientRect(&rWnd);

	// 왼쪽위
	if( poMouse.x >= 0 && poMouse.x <= m_stThemeInfo.nEdgeRange && poMouse.y >= 0 && poMouse.y <= m_stThemeInfo.nEdgeRange )
		return dfRESIZE_DIRECTION_LEFT_TOP;
	// 오른쪽위
	if( poMouse.x >= rWnd.right-m_stThemeInfo.nEdgeRange && poMouse.x <= rWnd.right && poMouse.y >= 0 && poMouse.y <= m_stThemeInfo.nEdgeRange )
		return dfRESIZE_DIRECTION_RIGHT_TOP;
	// 왼쪽아래
	if( poMouse.x >= 0 && poMouse.x <= m_stThemeInfo.nEdgeRange && poMouse.y >= rWnd.bottom-m_stThemeInfo.nEdgeRange && poMouse.y <= rWnd.bottom )
		return dfRESIZE_DIRECTION_LEFT_BOTTOM;
	// 오른쪽아래
	if( poMouse.x >= rWnd.right-m_stThemeInfo.nEdgeRange && poMouse.x <= rWnd.right && poMouse.y >= rWnd.bottom-m_stThemeInfo.nEdgeRange && poMouse.y <= rWnd.bottom )
		return dfRESIZE_DIRECTION_RIGHT_BOTTOM;
	// 왼쪽
	if( poMouse.x >= 0 && poMouse.x <= m_stThemeInfo.nEdgeRange )
		return dfRESIZE_DIRECTION_LEFT;
	// 위쪽
	if( poMouse.y >= 0 && poMouse.y <= m_stThemeInfo.nEdgeRange )
		return dfRESIZE_DIRECTION_TOP;
	// 오른쪽
	if( poMouse.x >= rWnd.right-m_stThemeInfo.nEdgeRange && poMouse.x <= rWnd.right )
		return dfRESIZE_DIRECTION_RIGHT;
	// 아래쪽
	if( poMouse.y >= rWnd.bottom-m_stThemeInfo.nEdgeRange && poMouse.y <= rWnd.bottom )
		return dfRESIZE_DIRECTION_BOTTOM;

	return dfRESIZE_DIRECTION_NONE;
}

// 마우스 포인터 모양을 방향에 맞추어 변경하는 함수
void CThemeDialog::ChangeMousePointImage(int nDirection)
{
	LPCTSTR strMousePointer;

	switch(nDirection)
	{
	case dfRESIZE_DIRECTION_LEFT :				// 왼쪽 가장자리 일때
	case dfRESIZE_DIRECTION_RIGHT :				// 오른쪽 가장자리 일때
		strMousePointer = IDC_SIZEWE;
		break;
	case dfRESIZE_DIRECTION_TOP :				// 위쪽 가장자리 일때
	case dfRESIZE_DIRECTION_BOTTOM :			// 아래쪽 가장자리 일때
		strMousePointer = IDC_SIZENS;
		break;
	case dfRESIZE_DIRECTION_LEFT_TOP :			// 왼쪽위 가장자리 일때
	case dfRESIZE_DIRECTION_RIGHT_BOTTOM :		// 오른쪽아래 가장자리 일때
		strMousePointer = IDC_SIZENWSE;
		break;
	case dfRESIZE_DIRECTION_RIGHT_TOP :			// 오른쪽위 가장자리 일때
	case dfRESIZE_DIRECTION_LEFT_BOTTOM :		// 왼쪽아래 가장자리 일때
		strMousePointer = IDC_SIZENESW;
		break;
	default :
		strMousePointer = IDC_ARROW;
		break;
	}
		
	// 마우스 포인터 모양을 변경
	HCURSOR lCur;
	lCur = AfxGetApp()->LoadStandardCursor(strMousePointer);
	SetCursor(lCur);

}

// 상대 좌표를 절대좌표로 변경하는 함수
CPoint CThemeDialog::Point2Absolute(CPoint poPoint)
{
	CPoint poNew;
	CRect rWnd, rParent;

	// 자신의 윈도우 크기를 구한다
	this->GetWindowRect(&rWnd);
		
	// 마우스 포인터 위치를 절대좌표로 변경해서 저장
	poNew.x = poPoint.x + rWnd.left;
	poNew.y = poPoint.y + rWnd.top;

	return poNew;
}

// 방향에 따라서 자신의 크기를 변경하는 함수
void CThemeDialog::ResizeWndByDirection(int nDirection, CPoint poMouseOld, CPoint poMouseNew)
{
	CPoint poMove;
	CRect rResize=CRect(0,0,0,0);

	// Resize 크기를 구한다
	poMove.x = poMouseNew.x - poMouseOld.x;
	poMove.y = poMouseNew.y - poMouseOld.y;

	// 변경 크기가 0이라면 함수 탈출
	if( poMove == CPoint(0,0) )
		return;

	switch(nDirection)
	{
	case dfRESIZE_DIRECTION_LEFT :				// 왼쪽 가장자리 일때
		rResize.left = poMove.x;
		break;
	case dfRESIZE_DIRECTION_TOP :				// 위쪽 가장자리 일때
		rResize.top = poMove.y;
		break;
	case dfRESIZE_DIRECTION_RIGHT :				// 오른쪽 가장자리 일때
		rResize.right = poMove.x;
		break;
	case dfRESIZE_DIRECTION_BOTTOM :			// 아래쪽 가장자리 일때
		rResize.bottom = poMove.y;
		break;
	case dfRESIZE_DIRECTION_LEFT_TOP :			// 왼쪽위 가장자리 일때
		rResize.left = poMove.x;
		rResize.top = poMove.y;
		break;
	case dfRESIZE_DIRECTION_RIGHT_TOP :			// 오른쪽위 가장자리 일때
		rResize.right = poMove.x;
		rResize.top = poMove.y;
		break;
	case dfRESIZE_DIRECTION_LEFT_BOTTOM :		// 왼쪽아래 가장자리 일때
		rResize.left = poMove.x;
		rResize.bottom = poMove.y;
		break;
	case dfRESIZE_DIRECTION_RIGHT_BOTTOM :		// 오른쪽아래 가장자리 일때
		rResize.right = poMove.x;
		rResize.bottom = poMove.y;
		break;
	default :									// 이동하지 않는다면
		// 함수 탈출
		return;
		break;
	}
	
	CRect rWnd;
	// 윈도우 위치좌표를 구한다
	this->GetWindowRect(&rWnd);

	// 새로운 위치좌표를 구한다
	rWnd.left += rResize.left;
	rWnd.top += rResize.top;
	rWnd.right += rResize.right;
	rWnd.bottom += rResize.bottom;

	// 윈도우 크기가 범위를 벗어난다면 함수 탈출
	if( rWnd.Width() < m_stThemeInfo.nMinWidth )
		return;
	if( rWnd.Height() < m_stThemeInfo.nMinHeight )
		return;
	if( rWnd.Width() > m_stThemeInfo.nMaxWidth )
		rWnd.right = rWnd.left + m_stThemeInfo.nMaxWidth;
	if( rWnd.Height() > m_stThemeInfo.nMaxHeight )
		rWnd.bottom = rWnd.top + m_stThemeInfo.nMaxHeight;

	// 윈도우의 크기를 변경
	this->MoveWindow(rWnd);
}

// Resize 하기전에 방향에 따라 윈도우 크기를 변경
void CThemeDialog::ResizeWndInit(int nDirection)
{
	CRect rResize=CRect(0,0,0,0);
	int nInitResizeGap=15;

	switch(nDirection)
	{
	case dfRESIZE_DIRECTION_LEFT :				// 왼쪽 가장자리 일때
		rResize.left -= nInitResizeGap;
		break;
	case dfRESIZE_DIRECTION_TOP :				// 위쪽 가장자리 일때
		rResize.top -= nInitResizeGap;
		break;
	case dfRESIZE_DIRECTION_RIGHT :				// 오른쪽 가장자리 일때
		rResize.right += nInitResizeGap;
		break;
	case dfRESIZE_DIRECTION_BOTTOM :			// 아래쪽 가장자리 일때
		rResize.bottom += nInitResizeGap;
		break;
	case dfRESIZE_DIRECTION_LEFT_TOP :			// 왼쪽위 가장자리 일때
		rResize.left -= nInitResizeGap;
		rResize.top -= nInitResizeGap;
		break;
	case dfRESIZE_DIRECTION_RIGHT_TOP :			// 오른쪽위 가장자리 일때
		rResize.right += nInitResizeGap;
		rResize.top -= nInitResizeGap;
		break;
	case dfRESIZE_DIRECTION_LEFT_BOTTOM :		// 왼쪽아래 가장자리 일때
		rResize.left -= nInitResizeGap;
		rResize.bottom += nInitResizeGap;
		break;
	case dfRESIZE_DIRECTION_RIGHT_BOTTOM :		// 오른쪽아래 가장자리 일때
		rResize.right += nInitResizeGap;
		rResize.bottom += nInitResizeGap;
		break;
	default :									// 이동하지 않는다면
		// 함수 탈출
		return;
		break;
	}
	
	CRect rWnd;
	// 윈도우 위치좌표를 구한다
	this->GetWindowRect(&rWnd);

	// 새로운 위치좌표를 구한다
	rWnd.left += rResize.left;
	rWnd.top += rResize.top;
	rWnd.right += rResize.right;
	rWnd.bottom += rResize.bottom;

	// 윈도우의 크기를 변경
	this->MoveWindow(rWnd);
}

// 클래스 생성시 실행되는 함수
int CThemeDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 윈도우의시스템박스를표시하지않기위해윈도우스타일을수정함
	// 대신Maximize 처리를하기위해리소스에서는시스템관련윈도우스타일을Enable 시켜야함
	ModifyStyle( WS_SYSMENU, 0);

	// Caption 영역좌표 설정
	CRect rScreen;
	this->GetClientRect(&rScreen);
	m_stThemeInfo.rCaption.right = rScreen.right - m_stThemeInfo.rCaption.left;

	// 배경을 투명으로 설정(투명 컬러 지정)
	//SetTransparentBack(dfCOLOR_TRANSPARENT);

	// 초기화를 위한 타이머 발생
	SetTimer(dfTIMER_INITIALIZE, 10, NULL);

	// Resize 테두리 윈도우 생성
	m_pDlgResizeWnd = new CDlgResizeWnd();
	m_pDlgResizeWnd->Create(NULL, NULL, WS_TABSTOP, CRect(0,0,10,10), this, dfIDC_DLG_RESIZE_WND);
	m_pDlgResizeWnd->SetWindowPos(&wndTopMost, 0,0,10,10, SWP_NOSIZE);

	
	return 0;
}

// 스타일 'Resize'를 제거
void CThemeDialog::RemoveStyleResize()
{
	// 다이얼로그 스타일을 구한다
	LONG nStyle;
	nStyle = ::GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	
	// 다이얼로그 스타일에 'Resize'가 포함되어 있다면
	if( (nStyle & WS_THICKFRAME) == WS_THICKFRAME )
	{
		// 다이얼로그 스타일에 'Resize' 옵션을 제거
		nStyle -= WS_THICKFRAME;
		::SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, nStyle);
		// 전역변수에 'Resize' 옵션 설정
		m_stThemeInfo.bStyleResize = TRUE;
	}
}

// Caption 문자열을 화면에 표시한다
void CThemeDialog::DrawCaptionText(CPaintDC *pdc)
{
	CRect rFont, rShadow;

	// Caption 영역좌표를 절대 영역좌표로 변환
	rFont = gf_RelativeToAbsolute(m_stThemeInfo.rCaption, this);
	// Caption 그림자 영역을 구한다
	rShadow = rFont;
	rShadow.left ++;
	rShadow.top ++;
	rShadow.right ++;
	rShadow.bottom ++;

	// 그림자 컬러가 투명이 아니라면
	if( m_stThemeInfo.crShadow != dfCOLOR_TRANSPARENT )
		// 그림자를 화면에 표시
		gf_DrawText(pdc, m_stThemeInfo.strCaption, m_stThemeInfo.nFontSize, 
			m_stThemeInfo.crShadow, rShadow, 
			m_stThemeInfo.nAlignFormat|DT_SINGLELINE, m_stThemeInfo.bFontBold);

	// 텍스트를 화면에 표시
	gf_DrawText(pdc, m_stThemeInfo.strCaption, m_stThemeInfo.nFontSize, 
			m_stThemeInfo.crFont, rFont, 
			m_stThemeInfo.nAlignFormat|DT_SINGLELINE, m_stThemeInfo.bFontBold);
}

// 툴팁 삭제
void CThemeDialog::DeleteToolTip()
{
	if( !m_ToolTip.GetSafeHwnd() )
		return;

	m_ToolTip.DelTool(this);
}

// 툴팁을 위한 이벤트를 받는 함수
LRESULT CThemeDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
	
	return CDialog::WindowProc(message, wParam, lParam);
}

// 툴팁을 생성한다
void CThemeDialog::MakeToolTip(CString strToolTip)
{
	// 현 다이얼로그 ID를 구한다
	//UINT unWindowID = (UINT)this->GetSafeHwnd();

	// 툴팁 생성
	m_ToolTip.CreateToolTip(this, m_unDlgID, strToolTip);
}

// 컨트롤 화면 크기 변경시 실행되는 함수
void CThemeDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	int nLimitSize=8;			// 최소 크기

	if( cx < nLimitSize || cy < nLimitSize )
		return;

	// 초기 위치 정보에 따라서 모든 컨트롤 위치를 재배치한다
	RepositionAllCtrlByInitPos();
	
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

// 툴팁 텍스트 변경
void CThemeDialog::ChangeToolTip(CString strToolTip)
{
	// 툴팁 텍스트 변경
	m_ToolTip.ChangeToolTip(strToolTip);
}

// 툴팁 문자열 설정
void CThemeDialog::SetToolTip(CString strText)
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

// '확인' 버튼 클릭시 실행되는 함수
void CThemeDialog::OnOK() 
{
	CWnd* pParent;

	pParent = this->GetParent();
	// 부모 윈도우가 존재한다면 버튼 클릭 이벤트를 전송
	if( pParent )
		pParent->PostMessage(dfWM_DLG_POPUP_BTN_OK, m_unDlgID,0);
	
	CDialog::OnOK();
}

// '취소' 버튼 클릭시 실행되는 함수
void CThemeDialog::OnCancel() 
{
	CWnd* pParent;

	pParent = this->GetParent();
	// 부모 윈도우가 존재한다면 버튼 클릭 이벤트를 전송
	if( pParent )
		pParent->PostMessage(dfWM_DLG_POPUP_BTN_CANCEL, m_unDlgID,0);
	
	CDialog::OnCancel();
}

// '종료' 버튼을 수행
void CThemeDialog::OnBtnClickExit()
{
	OnCancel();
}

// '최소화' 버튼을 수행
void CThemeDialog::OnBtnClickMinimize()
{
	this->ShowWindow(SW_MINIMIZE);
}

// '최대화' 버튼을 수행
void CThemeDialog::OnBtnClickMaximize()
{
	// 최대화 상태 일때
	if( IsZoomed() )
		this->ShowWindow(SW_NORMAL);
	// 최대화 상태가 아닐때
	else
		this->ShowWindow(SW_MAXIMIZE);
}

// 타이머 이벤트 발생시 실행되는 함수
void CThemeDialog::OnTimer(UINT_PTR nIDEvent)
{
	// 초기화를 위한 타이머 일때
	if( nIDEvent == dfTIMER_INITIALIZE )
	{
		// 타이머 종료
		KillTimer(dfTIMER_INITIALIZE);

		// 초기 위치 정보에 따라서 Theme 다이얼로그 위치를 재배치한다
		RepositionThemeDialog();

		// 초기 위치 정보에 따라서 비 Theme 컨트롤 위치를 재배치한다
		RepositionNoneThemeCtrl();

		// 하위 컨트롤 둥근 테두리 영역을 그린다
		Invalidate(TRUE);
	}
	
	CDialog::OnTimer(nIDEvent);
}

// 클래스 종료시에 실행되는 함수
void CThemeDialog::OnDestroy() 
{
	// 모든 컨트롤 정보를 INI 파일에 저장
	//SaveAllCtrlInfoToIni();

	CDialog::OnDestroy();
	
	int i=0;

	// Theme Staic 컨트롤 삭제
	for(i=0 ; i < m_arrThemeStatic.GetSize() ; i++ )
	{
		if( (m_arrThemeStatic)[i] )
		{
			M_W_DELETE((m_arrThemeStatic)[i]);
		}
	}
	// Static 컨트롤 Array 모두 삭제
	m_arrThemeStatic.RemoveAll();

	// Theme Group 컨트롤 삭제
	for(i=0 ; i < m_arrThemeGroup.GetSize() ; i++ )
	{
		if( (m_arrThemeGroup)[i] )
		{
			M_W_DELETE((m_arrThemeGroup)[i]);
		}
	}
	// Group 컨트롤 Array 모두 삭제
	m_arrThemeGroup.RemoveAll();

	// Theme Button 컨트롤 삭제
	for(i=0 ; i < m_arrThemeButton.GetSize() ; i++ )
	{
		if( (m_arrThemeButton)[i] )
		{
			M_W_DELETE((m_arrThemeButton)[i]);
		}
	}
	// Button 컨트롤 Array 모두 삭제
	m_arrThemeButton.RemoveAll();

	// Theme Check 컨트롤 삭제
	for(i=0 ; i < m_arrThemeCheck.GetSize() ; i++ )
	{
		if( (m_arrThemeCheck)[i] )
		{
			M_W_DELETE((m_arrThemeCheck)[i]);
		}
	}
	// Check 컨트롤 Array 모두 삭제
	m_arrThemeCheck.RemoveAll();

	// Theme Edit 컨트롤 삭제
	for(i=0 ; i < m_arrThemeEdit.GetSize() ; i++ )
	{
		if( (m_arrThemeEdit)[i] )
		{
			M_W_DELETE((m_arrThemeEdit)[i]);
		}
	}
	// Edit 컨트롤 Array 모두 삭제
	m_arrThemeEdit.RemoveAll();

	// Theme List Box 컨트롤 삭제
	for(i=0 ; i < m_arrThemeListBox.GetSize() ; i++ )
	{
		if( (m_arrThemeListBox)[i] )
		{
			M_W_DELETE((m_arrThemeListBox)[i]);
		}
	}
	// List Box 컨트롤 Array 모두 삭제
	m_arrThemeListBox.RemoveAll();

	// Theme Combo Box 컨트롤 삭제
	for(i=0 ; i < m_arrThemeCombo.GetSize() ; i++ )
	{
		if( (m_arrThemeCombo)[i] )
		{
			M_W_DELETE((m_arrThemeCombo)[i]);
		}
	}
	// Combo Box 컨트롤 Array 모두 삭제
	m_arrThemeCombo.RemoveAll();

	
	// 툴팁 삭제
	DeleteToolTip();
}

// 테마 그룹명 설정
void CThemeDialog::SetThemeGroupName(CString strGroupName)
{
	SetImageFilePath(g_strThemePath);
	SetConfigFilePath(g_strThemePath);

	m_stThemeInfo.strThemeGroupName = strGroupName;

	// INI 파일의 정보로 다이얼로그에 테마를 적용
	SetStyleDlgFromIniFile();
}

// 테마 그룹이 존재하는지 여부를 판단
BOOL CThemeDialog::IsThemeGroup()
{
	// 테마 그룹명이 존재하지 않으면 함수 탈출
	if( m_stThemeInfo.strThemeGroupName.GetLength() < 2 )
		return FALSE;
	// 테마 그룹명이 폴더 구분자로 끝나지 않는다면 함수 탈출
	//if( m_stThemeInfo.strThemeGroupName.Right(1) != "\\" )
	//	return FALSE;

	return TRUE;
}

// 컨트롤 컬러 변경시 실행되는 함수
HBRUSH CThemeDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	/*// 배경을 그린다면
	if( m_stThemeInfo.nBackDrawType == dfBACK_DRAW_TYPE_COLOR 
		|| m_stThemeInfo.nBackDrawType == dfBACK_DRAW_TYPE_IMAGE )
	{
		switch (nCtlColor)
		{
			case CTLCOLOR_STATIC:
				// The Slider Control has CTLCOLOR_STATIC, but doesn't let
				// the background shine through,
				TCHAR lpszClassName[255];
				GetClassName(pWnd->m_hWnd, lpszClassName, 255);
				if(_tcscmp(lpszClassName, TRACKBAR_CLASS) == 0)
					break;

				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
				break;

			case CTLCOLOR_DLG:
				hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);

			default:
				break;
		}
	}*/
	
	return hbr;
}

// 마우스 포인터가 Hover 상태인지 체크
//DEL void CThemeDialog::CheckHover(CPoint point)
//DEL {
//DEL 	if (!::IsWindow(GetSafeHwnd()))
//DEL 		return;
//DEL 
//DEL 	// hover상태에서 Disable시 Click가능 현상을 없애준다.
//DEL 	if (!IsWindowEnabled())
//DEL 	{
//DEL 		//m_bHover = false;
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	TRACKMOUSEEVENT EventTrack1, EventTrack2;
//DEL 	EventTrack1.cbSize = sizeof(TRACKMOUSEEVENT);
//DEL 	EventTrack1.dwFlags = TME_HOVER;
//DEL 	EventTrack1.hwndTrack = m_hWnd;
//DEL 	EventTrack1.dwHoverTime = 10;
//DEL 
//DEL 	_TrackMouseEvent(&EventTrack1);
//DEL 	
//DEL 	EventTrack2.cbSize = sizeof(TRACKMOUSEEVENT);
//DEL 	EventTrack2.dwFlags = TME_LEAVE;
//DEL 	EventTrack2.hwndTrack = m_hWnd;
//DEL 	EventTrack2.dwHoverTime = 0;
//DEL 
//DEL 	_TrackMouseEvent(&EventTrack2);
//DEL }

// 마우스 Hover시 실행되는 함수
//DEL LRESULT CThemeDialog::OnMouseHover(WPARAM wParam, LPARAM lParam)
//DEL {
//DEL 
//DEL 	return 0;
//DEL }

// 마우스 Leave시 실행되는 함수
//DEL LRESULT CThemeDialog::OnMouseLeave(WPARAM wParam, LPARAM lParam)
//DEL {
//DEL 	CPoint poMouse;
//DEL 
//DEL 	// 현재 Resizing 상태이고
//DEL 	// 마우스 포인터가 가장자리 영역에 있다면
//DEL 	if( m_stThemeInfo.bStyleResize == TRUE && m_stThemeInfo.nResizingDirection != dfRESIZE_DIRECTION_NONE )
//DEL 	{
//DEL 		// 마우스 위치좌표를 구한다
//DEL 		GetCursorPos( &poMouse );
//DEL 
//DEL 		// 마우스 포인터 모양을 방향에 맞추어 변경하는 함수
//DEL 		ChangeMousePointImage(m_stThemeInfo.nResizingDirection);
//DEL 			
//DEL 		// 방향에 따라서 윈도우 크기를 변경하는 함수
//DEL 		ResizeWndByDirection(m_stThemeInfo.nResizingDirection, m_stThemeInfo.poMouse, poMouse);
//DEL 
//DEL 		// 초기 위치 정보에 따라서 모든 컨트롤 위치를 재배치한다
//DEL 		RepositionAllCtrlByInitPos();
//DEL 
//DEL 		// 마우스 포인터 위치좌표를 저장
//DEL 		m_stThemeInfo.poMouse = poMouse;
//DEL 	}
//DEL 
//DEL 	return 0;
//DEL }

// 문자열에서 Rect 정보를 추출하여 반환하는 함수
CRect CThemeDialog::CnvtStringToRect(CString sRect)
{
	int	i=0, iField = 0;
	CString sPos[4];
	for (i = 0; i < 4; i++)
		sPos[i] = _T("");	

	for (i = 0; i < sRect.GetLength(); i++)
	{
		if (sRect.GetAt(i) == 0x2C)
		{
			iField++;
			continue;
		}

		sPos[iField] += sRect.GetAt(i);
	}

	return CRect(_wtol(sPos[0]), _wtol(sPos[1]), _wtol(sPos[2]), _wtol(sPos[3]));
}

// 문자열에서 컬러 정보를 추출하여 반환하는 함수
COLORREF CThemeDialog::CnvtStringToColor(CString sRect)
{
	int	i=0, iField = 0;
	CString sPos[4];
	for (i = 0; i < 4; i++)
		sPos[i] = TEXT("");	

	for (i = 0; i < sRect.GetLength(); i++)
	{
		if (sRect.GetAt(i) == 0x2C)
		{
			iField++;
			continue;
		}

		sPos[iField] += sRect.GetAt(i);
	}

	return RGB(_wtoi(sPos[0]), _wtoi(sPos[1]), _wtoi(sPos[2]));
}

// 컨트롤 INI 파일에서 컨트롤 관련숫자 정보를 가져오는 함수
BOOL CThemeDialog::GetCtrlValueFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, int& nValue)
{
	CString strIniFileName;

	// 컨트롤 INI파일 전체 경로
	strIniFileName = g_strThemePath + dfINI_VALUE_FILE_NAME;

	// INI파일에서 숫자 정보를 가져오는 함수
	BOOL bResult = GetValueFromIniFile(strIniFileName, lpGrpName, lpCtrlName, nValue);

	return bResult;
}

// Theme Group 컨트롤을 추가
CThemeGroup* CThemeDialog::AddThemeGroupCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	CRect rCtrl;
	CString strCaption;
	CThemeGroup* pCtrl;

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("Group%d"), m_arrThemeGroup.GetSize());

	// 원본 컨트롤 정보를 구한다
	this->GetDlgItem(nCtrlID)->GetWindowRect(&rCtrl);
	ScreenToClient(&rCtrl);
	this->GetDlgItem(nCtrlID)->GetWindowText(strCaption);
	this->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	// 원본 컨트롤을 제거
	this->GetDlgItem(nCtrlID)->DestroyWindow();

	// Theme 컨트롤 생성
	pCtrl = new CThemeGroup;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP, rCtrl, this, nCtrlID);
	// Theme 컨트롤 Caption 텍스트 설정
	pCtrl->SetCaptionText(strCaption);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	// 어레이에 새로운 컨트롤 추가
	m_arrThemeGroup.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (m_arrThemeGroup)[m_arrThemeGroup.GetSize()-1];
}

// Static 컨트롤에 Style 설정
void CThemeDialog::SetStyleStaticCtrl(CThemeStatic *pCtrl, CString strIniItemName)
{
	CString strTypeName;
	COLORREF crColor=RGB(0,0,0);
	CString strText;
	CRect rCtrl;
	int nValue=0;

	// 컨트롤이 생성되지 않았다면
	if( pCtrl == NULL )
		return;

	// 텍스트 INI파일에서 툴팁 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(ToolTip)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// Caption 문자열 설정
		pCtrl->SetToolTip(strText);

	// Group 컨트롤에 Style 설정
	SetStyleGroupCtrl((CThemeGroup*)pCtrl, strIniItemName);

}

// Edit 컨트롤에 Style 설정
void CThemeDialog::SetStyleEditCtrl(CThemeEdit *pCtrl, CString strIniItemName)
{
	CString strTypeName;
	COLORREF crColor;
	CString strText;
	CRect rCtrl;
	int nValue=0;

	// 컨트롤이 생성되지 않았다면
	if( pCtrl == NULL )
		return;

	// 위치 정보

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		// 초기 위치 설정
		pCtrl->SetInitPos(rCtrl);
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rCtrl = gf_TransCtrlPosToAbsolute(rCtrl, this);
		// 컨트롤 위치좌표 설정
		pCtrl->MoveWindow(rCtrl);
	}


	// 텍스트 정보

	// 텍스트 INI파일에서 캡션 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(Text)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// 텍스트 문자열 설정
		pCtrl->SetWindowText(strText);

	// 텍스트 INI파일에서 ToolTip 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(ToolTip)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// ToolTip 문자열 설정
		pCtrl->SetToolTip(strText);


	// 컬러 정보

	// INI 파일에서 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 배경 컬러 설정
		pCtrl->SetBackColor(crColor);

	// INI 파일에서 외곽선 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Outline)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선 컬러 설정
		pCtrl->SetOutlineColor(crColor);

	// INI 파일에서 외곽선(Left-Top) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineLT)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Left-Top) 컬러 설정
		pCtrl->SetOutlineLTColor(crColor);

	// INI 파일에서 외곽선(Right-Bottom) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineRB)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Right-Bottom) 컬러 설정
		pCtrl->SetOutlineRBColor(crColor);

	// INI 파일에서 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Font)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 컬러 설정
		pCtrl->SetFontColor(crColor);

	// Value 정보

	// INI 파일에서 폰트 크기를 구한다
	strTypeName = strIniItemName + TEXT("(FontSize)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 크기 설정
		pCtrl->SetFontSize(nValue);

	// INI 파일에서 폰트 두께를 구한다
	strTypeName = strIniItemName + TEXT("(FontBold)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 두께 설정
		pCtrl->SetFontBold(nValue);

	// INI 파일에서 외곽선 두께를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineThick)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 외곽선 설정
		pCtrl->SetOutlineThick(nValue);

	// INI 파일에서 Show/Hide를 구한다
	strTypeName = strIniItemName + TEXT("(Visible)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
	{
		nValue = (nValue == 0) ? SW_HIDE : SW_SHOW;
		// Show/Hide 설정
		pCtrl->ShowWindow(nValue);
	}

}

// Theme Button 컨트롤을 추가
CThemeButton* CThemeDialog::AddThemeButtonCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	CRect rCtrl;
	CString strCaption;
	CThemeButton* pCtrl;

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("Btn%d"), m_arrThemeButton.GetSize());

	// 원본 컨트롤 정보를 구한다
	this->GetDlgItem(nCtrlID)->GetWindowRect(&rCtrl);
	ScreenToClient(&rCtrl);
	this->GetDlgItem(nCtrlID)->GetWindowText(strCaption);
	this->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	// 원본 컨트롤을 제거
	this->GetDlgItem(nCtrlID)->DestroyWindow();

	// Theme 컨트롤 생성
	pCtrl = new CThemeButton;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP, rCtrl, this, nCtrlID);
	// Theme 컨트롤 Caption 텍스트 설정
	pCtrl->SetCaptionText(strCaption);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	// 어레이에 새로운 컨트롤 추가
	m_arrThemeButton.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (m_arrThemeButton)[m_arrThemeButton.GetSize()-1];
}

// Theme Check Box 컨트롤을 추가
CThemeCheck* CThemeDialog::AddThemeCheckCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	CRect rCtrl;
	CString strCaption;
	CThemeCheck* pCtrl;
	DWORD dwStyle;

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("Check%d"), m_arrThemeCheck.GetSize());

	// 원본 컨트롤 정보를 구한다
	this->GetDlgItem(nCtrlID)->GetWindowRect(&rCtrl);
	ScreenToClient(&rCtrl);
	this->GetDlgItem(nCtrlID)->GetWindowText(strCaption);
	dwStyle = this->GetDlgItem(nCtrlID)->GetStyle();
	this->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	// 원본 컨트롤을 제거
	this->GetDlgItem(nCtrlID)->DestroyWindow();

	// Theme 컨트롤 생성
	pCtrl = new CThemeCheck;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX|dwStyle, rCtrl, this, nCtrlID);
	// Theme 컨트롤 Caption 텍스트 설정
	pCtrl->SetCaptionText(strCaption);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	// 어레이에 새로운 컨트롤 추가
	m_arrThemeCheck.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (m_arrThemeCheck)[m_arrThemeCheck.GetSize()-1];
}

// Button 컨트롤에 Style 설정
void CThemeDialog::SetStyleButtonCtrl(CThemeButton *pCtrl, CString strIniItemName)
{
	CString strTypeName;
	COLORREF crColor;
	CString strText;
	CRect rCtrl;
	int nValue=0;

	// 컨트롤이 생성되지 않았다면
	if( pCtrl == NULL )
		return;

	// 위치 정보

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		// 초기 위치 설정
		pCtrl->SetInitPos(rCtrl);
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rCtrl = gf_TransCtrlPosToAbsolute(rCtrl, this);
		// 컨트롤 위치좌표 설정
		pCtrl->MoveWindow(rCtrl);
	}

	// INI 파일에서 캡션 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(CaptionArea)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
		// Caption 영역좌표 설정
		pCtrl->SetCaptionArea(rCtrl);


	// 텍스트 정보

	// 텍스트 INI파일에서 캡션 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(Caption)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// Caption 문자열 설정
		pCtrl->SetCaptionText(strText);

	// 텍스트 INI파일에서 툴팁 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(ToolTip)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// Caption 문자열 설정
		pCtrl->SetToolTip(strText);


	// 컬러 정보

	// INI 파일에서 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 배경 컬러 설정
		pCtrl->SetBackColor(crColor);

	// INI 파일에서 외곽선 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Outline)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선 컬러 설정
		pCtrl->SetOutlineColor(crColor);

	// INI 파일에서 외곽선(Left-Top) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineLT)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Left-Top) 컬러 설정
		pCtrl->SetOutlineLTColor(crColor);

	// INI 파일에서 외곽선(Right-Bottom) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineRB)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Right-Bottom) 컬러 설정
		pCtrl->SetOutlineRBColor(crColor);

	// INI 파일에서 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Font)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 컬러 설정
		pCtrl->SetFontColor(crColor);

	// INI 파일에서 폰트 Hover 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FontHover)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 Hover 컬러 설정
		pCtrl->SetFontHoverColor(crColor);

	// INI 파일에서 폰트 Pressed 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FontPressed)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 Pressed 컬러 설정
		pCtrl->SetFontPressedColor(crColor);

	// INI 파일에서 폰트 Disabled 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FontDisabled)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 Disabled 컬러 설정
		pCtrl->SetFontDisabledColor(crColor);

	// INI 파일에서 그림자 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Shadow)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 컬러 설정
		pCtrl->SetShadowColor(crColor);

	// INI 파일에서 그림자 Hover 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(ShadowHover)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 그림자 Hover 컬러 설정
		pCtrl->SetShadowHoverColor(crColor);

	// INI 파일에서 그림자 Pressed 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(ShadowPressed)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 그림자 Pressed 컬러 설정
		pCtrl->SetShadowPressedColor(crColor);

	// INI 파일에서 그림자 Disabled 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(ShadowDisabled)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 그림자 Disabled 컬러 설정
		pCtrl->SetShadowDisabledColor(crColor);

	// Value 정보

	// INI 파일에서 폰트 크기를 구한다
	strTypeName = strIniItemName + TEXT("(FontSize)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 크기 설정
		pCtrl->SetFontSize(nValue);

	// INI 파일에서 정렬 방식을 구한다
	strTypeName = strIniItemName + TEXT("(AlignFormat)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 정렬 방식 설정
		pCtrl->SetAlignFormat(nValue);

	// INI 파일에서 폰트 두께를 구한다
	strTypeName = strIniItemName + TEXT("(FontBold)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 두께 설정
		pCtrl->SetFontBold(nValue);

	// INI 파일에서 배경 그리기 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackDrawType)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 그리기 방식 설정
		pCtrl->SetBackDrawType(nValue);

	// INI 파일에서 배경 이미지 채우기 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackImageFillType)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 이미지 채우기 방식 설정
		pCtrl->SetBackImageFillType(nValue);

	// INI 파일에서 배경 이미지 정렬 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackImageAlignFormat)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 이미지 정렬 방식 설정
		pCtrl->SetBackImageAlignFormat(nValue);

	// INI 파일에서 배경 이미지 블럭 개수를 구한다
	strTypeName = strIniItemName + TEXT("(BackImageBlockCount)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 이미지 블럭 개수 설정
		pCtrl->SetBackImageBlockCount(nValue);

	// INI 파일에서 모서리 둥근 정도를 구한다
	strTypeName = strIniItemName + TEXT("(EdgeRoundValue)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 모서리 둥근 정도 설정
		pCtrl->SetEdgeRoundValue(nValue);

	// INI 파일에서 외곽선 두께를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineThick)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 외곽선 설정
		pCtrl->SetOutlineThick(nValue);

	// INI 파일에서 Show/Hide를 구한다
	strTypeName = strIniItemName + TEXT("(Visible)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
	{
		nValue = (nValue == 0) ? SW_HIDE : SW_SHOW;
		// Show/Hide 설정
		pCtrl->ShowWindow(nValue);
	}

	// Image 파일 정보
	
	// 컨트롤 INI파일에서 배경 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// 배경 이미지 설정
		pCtrl->SetBackImage(strText);
	}
}

// Radio 컨트롤에 Style 설정
void CThemeDialog::SetStyleRadioCtrl(CThemeRadio *pCtrl, CString strIniItemName)
{
	// Check 컨트롤에 Style 설정
	SetStyleCheckCtrl((CThemeCheck*)pCtrl, strIniItemName);
}

// Theme Edit 컨트롤을 추가
CThemeEdit* CThemeDialog::AddThemeEditCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	CRect rCtrl;
	CString strCaption;
	CThemeEdit* pCtrl;
	DWORD dwStyle;

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("Edit%d"), m_arrThemeEdit.GetSize());

	// 원본 컨트롤 정보를 구한다
	this->GetDlgItem(nCtrlID)->GetWindowRect(&rCtrl);
	ScreenToClient(&rCtrl);
	dwStyle = this->GetDlgItem(nCtrlID)->GetStyle();
	this->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	// 원본 컨트롤을 제거
	this->GetDlgItem(nCtrlID)->DestroyWindow();

	// Theme 컨트롤 생성
	pCtrl = new CThemeEdit();
	pCtrl->Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE|dwStyle, rCtrl, this, nCtrlID);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	// 어레이에 새로운 컨트롤 추가
	m_arrThemeEdit.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (m_arrThemeEdit)[m_arrThemeEdit.GetSize()-1];
}

// Theme List Box 컨트롤을 추가
CThemeListBox* CThemeDialog::AddThemeListBoxCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	CRect rCtrl;
	CString strCaption;
	CThemeListBox* pCtrl;
	DWORD dwStyle=0;

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("ListBox%d"), m_arrThemeListBox.GetSize());

	// 원본 컨트롤 정보를 구한다
	this->GetDlgItem(nCtrlID)->GetWindowRect(&rCtrl);
	ScreenToClient(&rCtrl);
	dwStyle = this->GetDlgItem(nCtrlID)->GetStyle();
	this->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	// 원본 컨트롤을 제거
	this->GetDlgItem(nCtrlID)->DestroyWindow();

	// Theme 컨트롤 생성
	pCtrl = new CThemeListBox();
	pCtrl->Create(WS_CHILD|WS_TABSTOP|WS_VISIBLE, rCtrl, this, nCtrlID);
	//if( (dwStyle & WS_VSCROLL) != WS_VSCROLL )
	//	pCtrl->SetVScroll(FALSE);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	// 어레이에 새로운 컨트롤 추가
	m_arrThemeListBox.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (m_arrThemeListBox)[m_arrThemeListBox.GetSize()-1];
}

// Check 컨트롤에 Style 설정
void CThemeDialog::SetStyleCheckCtrl(CThemeCheck *pCtrl, CString strIniItemName)
{
	CString strTypeName;
	COLORREF crColor;
	CString strText;
	CRect rCtrl;
	int nValue=0;

	// 컨트롤이 생성되지 않았다면
	if( pCtrl == NULL )
		return;

	// 위치 정보

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		// 초기 위치 설정
		pCtrl->SetInitPos(rCtrl);
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rCtrl = gf_TransCtrlPosToAbsolute(rCtrl, this);
		// 컨트롤 위치좌표 설정
		pCtrl->MoveWindow(rCtrl);
	}

	// INI 파일에서 캡션 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(CaptionArea)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
		// Caption 영역좌표 설정
		pCtrl->SetCaptionArea(rCtrl);


	// 텍스트 정보

	// 텍스트 INI파일에서 캡션 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(Caption)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// Caption 문자열 설정
		pCtrl->SetCaptionText(strText);

	// 텍스트 INI파일에서 툴팁 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(ToolTip)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// Caption 문자열 설정
		pCtrl->SetToolTip(strText);


	// 컬러 정보

	// INI 파일에서 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 배경 컬러 설정
		pCtrl->SetBackColor(crColor);

	// INI 파일에서 외곽선 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Outline)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선 컬러 설정
		pCtrl->SetOutlineColor(crColor);

	// INI 파일에서 외곽선(Left-Top) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineLT)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Left-Top) 컬러 설정
		pCtrl->SetOutlineLTColor(crColor);

	// INI 파일에서 외곽선(Right-Bottom) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineRB)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Right-Bottom) 컬러 설정
		pCtrl->SetOutlineRBColor(crColor);

	// INI 파일에서 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Font)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 컬러 설정
		pCtrl->SetFontColor(crColor);

	// INI 파일에서 폰트 Hover 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FontHover)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 Hover 컬러 설정
		pCtrl->SetFontHoverColor(crColor);

	// INI 파일에서 폰트 Pressed 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FontPressed)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 Pressed 컬러 설정
		pCtrl->SetFontPressedColor(crColor);

	// INI 파일에서 폰트 Disabled 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FontDisabled)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 Disabled 컬러 설정
		pCtrl->SetFontDisabledColor(crColor);

	// INI 파일에서 그림자 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Shadow)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		//  폰트 컬러 설정
		pCtrl->SetShadowColor(crColor);

	// INI 파일에서 그림자 Hover 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(ShadowHover)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 그림자 Hover 컬러 설정
		pCtrl->SetShadowHoverColor(crColor);

	// INI 파일에서 그림자 Pressed 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(ShadowPressed)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 그림자 Pressed 컬러 설정
		pCtrl->SetShadowPressedColor(crColor);

	// INI 파일에서 그림자 Disabled 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(ShadowDisabled)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 그림자 Disabled 컬러 설정
		pCtrl->SetShadowDisabledColor(crColor);

	// INI 파일에서 Mark 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Mark)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// Mark 컬러 설정
		pCtrl->SetMarkColor(crColor);


	// Value 정보

	// INI 파일에서 폰트 크기를 구한다
	strTypeName = strIniItemName + TEXT("(FontSize)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 크기 설정
		pCtrl->SetFontSize(nValue);

	// INI 파일에서 Caption 정렬 방식을 구한다
	strTypeName = strIniItemName + TEXT("(AlignFormat)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// Caption 정렬 방식 설정
		pCtrl->SetAlignFormat(nValue);

	// INI 파일에서 폰트 두께를 구한다
	strTypeName = strIniItemName + TEXT("(FontBold)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 두께 설정
		pCtrl->SetFontBold(nValue);

	// INI 파일에서 배경 그리기 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackDrawType)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 그리기 방식 설정
		pCtrl->SetBackDrawType(nValue);

	// INI 파일에서 배경 이미지 채우기 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackImageFillType)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 이미지 채우기 방식 설정
		pCtrl->SetBackImageFillType(nValue);

	// INI 파일에서 배경 이미지 정렬 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackImageAlignFormat)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 이미지 정렬 방식 설정
		pCtrl->SetBackImageAlignFormat(nValue);

	// INI 파일에서 배경 이미지 블럭 개수를 구한다
	strTypeName = strIniItemName + TEXT("(BackImageBlockCount)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 이미지 블럭 개수 설정
		pCtrl->SetBackImageBlockCount(nValue);

	// INI 파일에서 모서리 둥근 정도를 구한다
	strTypeName = strIniItemName + TEXT("(EdgeRoundValue)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 모서리 둥근 정도 설정
		pCtrl->SetEdgeRoundValue(nValue);

	// INI 파일에서 외곽선 두께를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineThick)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 외곽선 설정
		pCtrl->SetOutlineThick(nValue);

	// INI 파일에서 Check 상태를 구한다
	strTypeName = strIniItemName + TEXT("(Check)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// Check 설정
		pCtrl->SetCheck(nValue);

	// INI 파일에서 PushLike 상태를 구한다
	strTypeName = strIniItemName + TEXT("(PushLike)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// PushLike 상태 설정
		pCtrl->SetPushLike(nValue);

	// INI 파일에서 마크 그리기 방식을 구한다
	strTypeName = strIniItemName + TEXT("(MarkDrawType)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 마크 그리기 방식 설정
		pCtrl->SetMarkDrawType(nValue);

	// INI 파일에서 Show/Hide를 구한다
	strTypeName = strIniItemName + TEXT("(Visible)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
	{
		nValue = (nValue == 0) ? SW_HIDE : SW_SHOW;
		// Show/Hide 설정
		pCtrl->ShowWindow(nValue);
	}


	// Image 파일 정보
	
	// 컨트롤 INI파일에서 배경 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// 배경 이미지 설정
		pCtrl->SetBackImage(strText);
	}

	// 컨트롤 INI파일에서 Mark 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Mark)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// Mark 이미지 설정
		pCtrl->SetMarkImage(strText);
	}
}

// Theme Radio Btn 컨트롤을 추가
CThemeRadio* CThemeDialog::AddThemeRadioCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	CRect rCtrl;
	CString strCaption;
	CThemeRadio* pCtrl;
	DWORD dwStyle;

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("Radio%d"), m_arrThemeRadio.GetSize());

	// 원본 컨트롤 정보를 구한다
	this->GetDlgItem(nCtrlID)->GetWindowRect(&rCtrl);
	ScreenToClient(&rCtrl);
	this->GetDlgItem(nCtrlID)->GetWindowText(strCaption);
	dwStyle = this->GetDlgItem(nCtrlID)->GetStyle();
	this->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	// 원본 컨트롤을 제거
	this->GetDlgItem(nCtrlID)->DestroyWindow();

	// Theme 컨트롤 생성
	pCtrl = new CThemeRadio;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTORADIOBUTTON|dwStyle, rCtrl, this, nCtrlID);
	// Theme 컨트롤 Caption 텍스트 설정
	pCtrl->SetCaptionText(strCaption);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	// 어레이에 새로운 컨트롤 추가
	m_arrThemeRadio.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (m_arrThemeRadio)[m_arrThemeRadio.GetSize()-1];
}

// Theme Combo Btn 컨트롤을 추가
CThemeCombo* CThemeDialog::AddThemeComboCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	CRect rCtrl;
	CString strCaption;
	CThemeCombo* pCtrl;
	DWORD dwStyle;
	CComboBox* pComboCtrl;
	int i=0, nItemCount=0;

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("Combo%d"), m_arrThemeCombo.GetSize());

	// 원본 컨트롤 정보를 구한다
	this->GetDlgItem(nCtrlID)->GetWindowRect(&rCtrl);
	ScreenToClient(&rCtrl);
	this->GetDlgItem(nCtrlID)->GetWindowText(strCaption);
	dwStyle = this->GetDlgItem(nCtrlID)->GetStyle();
	this->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	// 원본 컨트롤을 제거
	//this->GetDlgItem(nCtrlID)->DestroyWindow();

	// Theme 컨트롤 생성
	pCtrl = new CThemeCombo;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|dwStyle, rCtrl, this, nCtrlID);
	// Theme 컨트롤 Caption 텍스트 설정
	pCtrl->SetWindowText(strCaption);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	
	// 콤보박스 항목을 복사
	pComboCtrl = (CComboBox*)this->GetDlgItem(nCtrlID);
	nItemCount = pComboCtrl->GetCount();
	for(i=0 ; i < nItemCount ; i++)
	{
		pComboCtrl->GetLBText(i, strCaption);
		pCtrl->AddString(strCaption);
	}

	// 어레이에 새로운 컨트롤 추가
	m_arrThemeCombo.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (m_arrThemeCombo)[m_arrThemeCombo.GetSize()-1];
}

// Combo Box 컨트롤에 Style 설정
void CThemeDialog::SetStyleComboCtrl(CThemeCombo *pCtrl, CString strIniItemName)
{
	CString strTypeName;
	COLORREF crColor;
	CString strText;
	CRect rCtrl;
	int nValue=0;

	// 컨트롤이 생성되지 않았다면
	if( pCtrl == NULL )
		return;

	// 위치 정보

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		// 초기 위치 설정
		pCtrl->SetInitPos(rCtrl);
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rCtrl = gf_TransCtrlPosToAbsolute(rCtrl, this);
		// 컨트롤 위치좌표 설정
		pCtrl->MoveWindow(rCtrl);
	}


	// 텍스트 정보
	
	// 텍스트 INI파일에서 ToolTip 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(ToolTip)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// ToolTip 문자열 설정
		pCtrl->SetToolTip(strText);


	// 컬러 정보

	// INI 파일에서 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 배경 컬러 설정
		pCtrl->SetBackColor(crColor);

	// INI 파일에서 외곽선 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Outline)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선 컬러 설정
		pCtrl->SetOutlineColor(crColor);

	// INI 파일에서 외곽선(Left-Top) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineLT)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Left-Top) 컬러 설정
		pCtrl->SetOutlineLTColor(crColor);

	// INI 파일에서 외곽선(Right-Bottom) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineRB)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Right-Bottom) 컬러 설정
		pCtrl->SetOutlineRBColor(crColor);

	// INI 파일에서 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Font)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 컬러 설정
		pCtrl->SetFontColor(crColor);

	// INI 파일에서 선택 항목 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(SelectFont)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 선택 항목 폰트 컬러 설정
		pCtrl->SetSelectItemFontColor(crColor);

	// INI 파일에서 선택 항목 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(SelectBack)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 선택 항목 배경 컬러 설정
		pCtrl->SetSelectItemBackColor(crColor);

	// INI 파일에서 일반 항목 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(NormalBack)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 항목 배경 컬러 설정
		pCtrl->SetNormalItemBackColor(crColor);

	// INI 파일에서 일반 항목(짝수번째) 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Normal1Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 항목(짝수번째) 배경 컬러 설정
		pCtrl->SetNormalItem1BackColor(crColor);

	// INI 파일에서 일반 항목(홀수번째) 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Normal2Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 항목(홀수번째) 배경 컬러 설정
		pCtrl->SetNormalItem2BackColor(crColor);

	// INI 파일에서 항목 구분선 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Frame)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 항목 구분선 컬러 설정
		pCtrl->SetItemFrameColor(crColor);


	// Value 정보

	// INI 파일에서 폰트 크기를 구한다
	strTypeName = strIniItemName + TEXT("(FontSize)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 크기 설정
		pCtrl->SetFontSize(nValue);

	// INI 파일에서 정렬 방식을 구한다
	strTypeName = strIniItemName + TEXT("(AlignFormat)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 정렬 방식 설정
		pCtrl->SetAlignFormat(nValue);

	// INI 파일에서 폰트 두께를 구한다
	strTypeName = strIniItemName + TEXT("(FontBold)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 두께 설정
		pCtrl->SetFontBold(nValue);

	// INI 파일에서 항목 높이를 구한다
	strTypeName = strIniItemName + TEXT("(ItemHeight)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 항목 높이 설정
		pCtrl->SetItemHeight(nValue);

	// INI 파일에서 왼쪽 여백을 구한다
	strTypeName = strIniItemName + TEXT("(LeftGap)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 왼쪽 여백 설정
		pCtrl->SetLeftGap(nValue);

	// INI 파일에서 오른쪽 여백을 구한다
	strTypeName = strIniItemName + TEXT("(RightGap)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 오른쪽 여백 설정
		pCtrl->SetRightGap(nValue);

	// INI 파일에서 외곽선 두께를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineThick)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 외곽선 두께를 설정
		pCtrl->SetOutlineThick(nValue);

	// INI 파일에서 리스트박스 높이를 구한다
	strTypeName = strIniItemName + TEXT("(ListBoxHeight)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 리스트 박스 높이를 설정
		pCtrl->SetListBoxHeight(nValue);

	// INI 파일에서 Show/Hide를 구한다
	strTypeName = strIniItemName + TEXT("(Visible)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
	{
		nValue = (nValue == 0) ? SW_HIDE : SW_SHOW;
		// Show/Hide 설정
		pCtrl->ShowWindow(nValue);
	}


	// Image 파일 정보
	
	// 컨트롤 INI파일에서 버튼 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Button)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// 배경 이미지 설정
		pCtrl->SetButtonImage(strText);
	}
}

// 타이틀바 영역을 새로 그리는 함수
void CThemeDialog::InvalidateTitleBar()
{
	CRect rScreen, rTitleBar;

	this->GetClientRect(&rScreen);

	// 위쪽 타이틀바 영역좌표를 구한다
	rTitleBar = rScreen;
	rTitleBar.bottom = rTitleBar.top + 30;

	InvalidateRect(rTitleBar, TRUE);
	
	/*// 위쪽 타이틀바 영역좌표를 구한다
	rTitleBar = rScreen;
	rTitleBar.bottom = rTitleBar.top + 30;

	InvalidateRect(rTitleBar, TRUE);*/
}

// 해당 윈도우 포인터가 하위 컨트롤인지 여부를 판단
BOOL CThemeDialog::CheckChildControl(CWnd *pWndFocus)
{
	CWnd *pChildPrev1, *pChildNext1, *pChildPrev2, *pChildNext2;

	// 첫번째 자식 컨트롤의 포인터를 구한다
	pChildNext1 = this->GetWindow(GW_CHILD);
	// 자식 컨트롤이 하나도 없다면 함수 탈출
	if( pChildNext1 == NULL )
		return FALSE;

	do {
		// 일치하는 하위 컨트롤을 찾았다면 TRUE를 반환
		if( pChildNext1->m_hWnd == pWndFocus->m_hWnd )
			return TRUE;
	
		// 첫번째 자식 컨트롤의 포인터를 구한다
		pChildNext2 = pChildNext1->GetWindow(GW_CHILD);

		do {
			// 자식 컨트롤이 하나도 없다면 루프 탈출
			if( pChildNext2 == NULL )
				break;
			
			// 일치하는 하위 컨트롤을 찾았다면 TRUE를 반환
			if( pChildNext2->m_hWnd == pWndFocus->m_hWnd )
				return TRUE;
	
			pChildPrev2 = pChildNext2;
		} while( (pChildNext2 = pChildPrev2->GetWindow(GW_HWNDNEXT)) != NULL );

		pChildPrev1 = pChildNext1;
	} while( (pChildNext1 = pChildPrev1->GetWindow(GW_HWNDNEXT)) != NULL );

	return FALSE;
}

// Theme ProgressBar 컨트롤을 추가
CThemeProgress* CThemeDialog::AddThemeProgressCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	CRect rCtrl;
	CString strCaption;
	CThemeProgress* pCtrl;

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("Progress%d"), m_arrThemeProgress.GetSize());

	// 원본 컨트롤 정보를 구한다
	this->GetDlgItem(nCtrlID)->GetWindowRect(&rCtrl);
	ScreenToClient(&rCtrl);
	//this->GetDlgItem(nCtrlID)->GetWindowText(strCaption);
	this->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	// 원본 컨트롤을 제거
	this->GetDlgItem(nCtrlID)->DestroyWindow();

	// Theme 컨트롤 생성
	pCtrl = new CThemeProgress;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP, rCtrl, this, nCtrlID);
	// Theme 컨트롤 텍스트 설정
	//pProgress->SetWindowText(strCaption);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	// 어레이에 새로운 컨트롤 추가
	m_arrThemeProgress.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (m_arrThemeProgress)[m_arrThemeProgress.GetSize()-1];
}

// Progress Bar 컨트롤에 Style 설정
void CThemeDialog::SetStyleProgressCtrl(CThemeProgress *pCtrl, CString strIniItemName)
{
	CString strTypeName;
	COLORREF crColor;
	CString strText;
	CRect rCtrl;
	int nValue=0;

	// 컨트롤이 생성되지 않았다면
	if( pCtrl == NULL )
		return;

	// 위치 정보

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		// 초기 위치 설정
		pCtrl->SetInitPos(rCtrl);
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rCtrl = gf_TransCtrlPosToAbsolute(rCtrl, this);
		// 컨트롤 위치좌표 설정
		pCtrl->MoveWindow(rCtrl);
	}


	// 컬러 정보

	// INI 파일에서 Bar 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Bar)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// Bar 컬러 설정
		pCtrl->SetBarColor(crColor);

	// INI 파일에서 Bar-1 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Bar1)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// Bar-1 컬러 설정
		pCtrl->SetBar1Color(crColor);

	// INI 파일에서 Bar-2 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Bar2)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// Bar-2 컬러 설정
		pCtrl->SetBar2Color(crColor);

	// INI 파일에서 'Bar1 윗부분' 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Bar1Up)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 'Bar1 윗부분' 컬러 설정
		pCtrl->SetBar1UpColor(crColor);

	// INI 파일에서 'Bar1 아래부분' 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Bar1Down)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 'Bar1 아래부분' 컬러 설정
		pCtrl->SetBar1DownColor(crColor);

	// INI 파일에서 'Bar1 Outline' 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Bar1Outline)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 'Bar1 Outline' 컬러 설정
		pCtrl->SetBar1OutlineColor(crColor);

	// INI 파일에서 'Bar2 윗부분' 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Bar2Up)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 'Bar2 윗부분' 컬러 설정
		pCtrl->SetBar2UpColor(crColor);

	// INI 파일에서 'Bar2 아래부분' 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Bar2Down)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 'Bar2 아래부분' 컬러 설정
		pCtrl->SetBar2DownColor(crColor);

	// INI 파일에서 'Bar2 Outline' 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Bar2Outline)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 'Bar2 Outline' 컬러 설정
		pCtrl->SetBar2OutlineColor(crColor);

	// INI 파일에서 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 배경 컬러 설정
		pCtrl->SetBackColor(crColor);

	// INI 파일에서 외곽선 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Outline)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선 컬러 설정
		pCtrl->SetOutlineColor(crColor);

	// INI 파일에서 외곽선(Left-Top) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineLT)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Left-Top) 컬러 설정
		pCtrl->SetOutlineLTColor(crColor);

	// INI 파일에서 외곽선(Right-Bottom) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineRB)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Right-Bottom) 컬러 설정
		pCtrl->SetOutlineRBColor(crColor);

	// INI 파일에서 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Font)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 컬러 설정
		pCtrl->SetFontColor(crColor);

	// INI 파일에서 그림자 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Shadow)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 그림자 컬러 설정
		pCtrl->SetShadowColor(crColor);

	
	// 텍스트 정보

	// 텍스트 INI파일에서 문자열 단위를 구한다
	strTypeName = strIniItemName + TEXT("(TextUnit)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// 문자열 단위 설정
		pCtrl->SetTextUnit(strText);

	// 텍스트 INI파일에서 툴팁 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(ToolTip)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// 툴팁 문자열 설정
		pCtrl->SetToolTip(strText);

	
	// Value 정보

	// INI 파일에서 '바 컬러 변경 퍼센트' 구한다
	strTypeName = strIniItemName + TEXT("(ColorChangePercent)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// '바 컬러 변경 퍼센트' 설정
		pCtrl->SetColorChangePercent((double)nValue);

	// INI 파일에서 '그라데이션 여부' 구한다
	strTypeName = strIniItemName + TEXT("(Gradient)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// '그라데이션 여부' 설정
		pCtrl->SetGradient(nValue);

	// INI 파일에서 '외곽선 두께' 구한다
	strTypeName = strIniItemName + TEXT("(OutlineThick)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// '외곽선 두께' 설정
		pCtrl->SetOutlineThick(nValue);

	// INI 파일에서 '범위-최소값' 구한다
	strTypeName = strIniItemName + TEXT("(RangeMin)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// '범위-최소값' 설정
		pCtrl->SetRangeMin(nValue);

	// INI 파일에서 '범위-최대값' 구한다
	strTypeName = strIniItemName + TEXT("(RangeMax)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// '범위-최대값' 설정
		pCtrl->SetRangeMax(nValue);

	// INI 파일에서 '진행율값' 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// '진행율값' 설정
		pCtrl->SetPos(nValue);

	// INI 파일에서 '문자열 표시 여부'를 구한다
	strTypeName = strIniItemName + TEXT("(DrawText)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// '문자열 표시 여부' 설정
		pCtrl->SetDrawText(nValue);

	// INI 파일에서 폰트 크기를 구한다
	strTypeName = strIniItemName + TEXT("(FontSize)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 크기 설정
		pCtrl->SetFontSize(nValue);

	// INI 파일에서 폰트 두께를 구한다
	strTypeName = strIniItemName + TEXT("(FontBold)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 두께 설정
		pCtrl->SetFontBold(nValue);

	// INI 파일에서 Show/Hide를 구한다
	strTypeName = strIniItemName + TEXT("(Visible)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
	{
		nValue = (nValue == 0) ? SW_HIDE : SW_SHOW;
		// Show/Hide 설정
		pCtrl->ShowWindow(nValue);
	}

}

// Theme Slider 컨트롤을 추가
CThemeSlider* CThemeDialog::AddThemeSliderCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	CRect rCtrl;
	CString strCaption;
	CThemeSlider* pCtrl;
	DWORD dwStyle;

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("Slider%d"), m_arrThemeSlider.GetSize());

	// 원본 컨트롤 정보를 구한다
	this->GetDlgItem(nCtrlID)->GetWindowRect(&rCtrl);
	ScreenToClient(&rCtrl);
	dwStyle = this->GetDlgItem(nCtrlID)->GetStyle();
	//this->GetDlgItem(nCtrlID)->GetWindowText(strCaption);
	this->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	// 원본 컨트롤을 제거
	//this->GetDlgItem(nCtrlID)->DestroyWindow();

	// Theme 컨트롤 생성
	pCtrl = new CThemeSlider;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|dwStyle, rCtrl, this, nCtrlID);
	// Theme 컨트롤 텍스트 설정
	//pProgress->SetWindowText(strCaption);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	// 어레이에 새로운 컨트롤 추가
	m_arrThemeSlider.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (m_arrThemeSlider)[m_arrThemeSlider.GetSize()-1];
}

// Slider 컨트롤에 Style 설정
void CThemeDialog::SetStyleSliderCtrl(CThemeSlider *pCtrl, CString strIniItemName)
{
	CString strTypeName;
	COLORREF crColor;
	CString strText;
	CRect rCtrl;
	int nValue=0;

	// 컨트롤이 생성되지 않았다면
	if( pCtrl == NULL )
		return;

	// 위치 정보

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		// 초기 위치 설정
		pCtrl->SetInitPos(rCtrl);
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rCtrl = gf_TransCtrlPosToAbsolute(rCtrl, this);
		// 컨트롤 위치좌표 설정
		pCtrl->MoveWindow(rCtrl);
	}


	// 텍스트 정보

	// 텍스트 INI파일에서 툴팁 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(ToolTip)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// Caption 문자열 설정
		pCtrl->SetToolTip(strText);


	// 컬러 정보

	// INI 파일에서 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 배경 컬러 설정
		pCtrl->SetBackColor(crColor);

	// INI 파일에서 외곽선 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Outline)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선 컬러 설정
		pCtrl->SetOutlineColor(crColor);

	// INI 파일에서 외곽선(Left-Top) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineLT)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Left-Top) 컬러 설정
		pCtrl->SetOutlineLTColor(crColor);

	// INI 파일에서 외곽선(Right-Bottom) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineRB)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Right-Bottom) 컬러 설정
		pCtrl->SetOutlineRBColor(crColor);

	
	// Value 정보

	// INI 파일에서 방향-수직 여부를 구한다
	strTypeName = strIniItemName + TEXT("(Vertical)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 방향-수직 여부 설정
		pCtrl->SetVertical(nValue);

	// INI 파일에서 배경 그리기 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackDrawType)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 그리기 방식 설정
		pCtrl->SetBackDrawType(nValue);

	// INI 파일에서 배경 이미지 채우기 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackImageFillType)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 이미지 채우기 방식 설정
		pCtrl->SetBackImageFillType(nValue);

	// INI 파일에서 배경 이미지 정렬 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackImageAlignFormat)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 이미지 정렬 방식 설정
		pCtrl->SetBackImageAlignFormat(nValue);

	// INI 파일에서 모서리 둥근 정도를 구한다
	strTypeName = strIniItemName + TEXT("(EdgeRoundValue)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 모서리 둥근 정도 설정
		pCtrl->SetEdgeRoundValue(nValue);

	// INI 파일에서 외곽선 두께를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineThick)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 외곽선 설정
		pCtrl->SetOutlineThick(nValue);

	// INI 파일에서 자동 툴팁 여부를 구한다
	strTypeName = strIniItemName + TEXT("(AutoToolTip)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 자동 툴팁 여부 설정
		pCtrl->SetAutoToolTip(nValue);

	// INI 파일에서 Bar 이미지 블럭 개수를 구한다
	strTypeName = strIniItemName + TEXT("(BarImageBlockCount)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// Bar 이미지 블럭 개수 설정
		pCtrl->SetBarImageBlockCount(nValue);

	// INI 파일에서 범위-최소값 구한다
	strTypeName = strIniItemName + TEXT("(RangeMin)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 범위-최소값 설정
		pCtrl->SetRangeMin(nValue);

	// INI 파일에서 범위-최대값 구한다
	strTypeName = strIniItemName + TEXT("(RangeMax)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 범위-최대값 설정
		pCtrl->SetRangeMax(nValue);

	// INI 파일에서 설정값을 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 설정값 설정
		pCtrl->SetPos(nValue);

	// INI 파일에서 페이지 단위를 구한다
	strTypeName = strIniItemName + TEXT("(PageUnit)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 페이지 단위 설정
		pCtrl->SetPageUnit(nValue);

	// INI 파일에서 Show/Hide를 구한다
	strTypeName = strIniItemName + TEXT("(Visible)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
	{
		nValue = (nValue == 0) ? SW_HIDE : SW_SHOW;
		// Show/Hide 설정
		pCtrl->ShowWindow(nValue);
	}


	// Image 파일 정보
	
	// 컨트롤 INI파일에서 배경 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// 배경 이미지 설정
		pCtrl->SetBackImage(strText);
	}

	// 컨트롤 INI파일에서 Bar 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Bar)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다	
		strText = FilteringImageFilePath(strText);
		// Bar 이미지 설정
		pCtrl->SetBarImage(strText);
	}

}

// Scroll 컨트롤에 Style 설정
void CThemeDialog::SetStyleScrollCtrl(CThemeScroll *pCtrl, CString strIniItemName)
{
	CString strTypeName;
	COLORREF crColor;
	CString strText;
	CRect rCtrl;
	int nValue=0;

	// 컨트롤이 생성되지 않았다면
	if( pCtrl == NULL )
		return;

	// 위치 정보

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		// 초기 위치 설정
		pCtrl->SetInitPos(rCtrl);
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rCtrl = gf_TransCtrlPosToAbsolute(rCtrl, this);
		// 컨트롤 위치좌표 설정
		pCtrl->MoveWindow(rCtrl);
	}


	// 텍스트 정보

	// 텍스트 INI파일에서 툴팁 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(ToolTip)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// Caption 문자열 설정
		pCtrl->SetToolTip(strText);


	// 컬러 정보

	// INI 파일에서 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 배경 컬러 설정
		pCtrl->SetBackColor(crColor);

	// INI 파일에서 외곽선 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Outline)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선 컬러 설정
		pCtrl->SetOutlineColor(crColor);

	// INI 파일에서 외곽선(Left-Top) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineLT)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Left-Top) 컬러 설정
		pCtrl->SetOutlineLTColor(crColor);

	// INI 파일에서 외곽선(Right-Bottom) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineRB)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Right-Bottom) 컬러 설정
		pCtrl->SetOutlineRBColor(crColor);

	
	// Value 정보

	// INI 파일에서 방향-수직 여부를 구한다
	strTypeName = strIniItemName + TEXT("(Vertical)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 방향-수직 여부 설정
		pCtrl->SetVertical(nValue);

	// INI 파일에서 배경 그리기 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackDrawType)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 그리기 방식 설정
		pCtrl->SetBackDrawType(nValue);

	// INI 파일에서 배경 이미지 채우기 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackImageFillType)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 이미지 채우기 방식 설정
		pCtrl->SetBackImageFillType(nValue);

	// INI 파일에서 배경 이미지 정렬 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackImageAlignFormat)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 이미지 정렬 방식 설정
		pCtrl->SetBackImageAlignFormat(nValue);

	// INI 파일에서 모서리 둥근 정도를 구한다
	strTypeName = strIniItemName + TEXT("(EdgeRoundValue)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 모서리 둥근 정도 설정
		pCtrl->SetEdgeRoundValue(nValue);

	// INI 파일에서 외곽선 두께를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineThick)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 외곽선 설정
		pCtrl->SetOutlineThick(nValue);

	// INI 파일에서 자동 툴팁 여부를 구한다
	strTypeName = strIniItemName + TEXT("(AutoToolTip)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 자동 툴팁 여부 설정
		pCtrl->SetAutoToolTip(nValue);

	// INI 파일에서 Bar 이미지 블럭 개수를 구한다
	strTypeName = strIniItemName + TEXT("(BarImageBlockCount)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// Bar 이미지 블럭 개수 설정
		pCtrl->SetBarImageBlockCount(nValue);

	// INI 파일에서 범위-최소값 구한다
	strTypeName = strIniItemName + TEXT("(RangeMin)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 범위-최소값 설정
		pCtrl->SetRangeMin(nValue);

	// INI 파일에서 범위-최대값 구한다
	strTypeName = strIniItemName + TEXT("(RangeMax)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 범위-최대값 설정
		pCtrl->SetRangeMax(nValue);

	// INI 파일에서 설정값을 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 설정값 설정
		pCtrl->SetPos(nValue);

	// INI 파일에서 페이지 단위를 구한다
	strTypeName = strIniItemName + TEXT("(PageUnit)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 페이지 단위 설정
		pCtrl->SetPageUnit(nValue);

	// INI 파일에서 라인 단위를 구한다
	strTypeName = strIniItemName + TEXT("(LineUnit)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 라인 단위 설정
		pCtrl->SetLineUnit(nValue);

	// INI 파일에서 '이전' 이미지 블럭 개수를 구한다
	strTypeName = strIniItemName + TEXT("(PrevImageBlockCount)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// '이전' 이미지 블럭 개수 설정
		pCtrl->SetPrevImageBlockCount(nValue);

	// INI 파일에서 '다음' 이미지 블럭 개수를 구한다
	strTypeName = strIniItemName + TEXT("(NextImageBlockCount)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// '다음' 이미지 블럭 개수 설정
		pCtrl->SetNextImageBlockCount(nValue);

	// INI 파일에서 Show/Hide를 구한다
	strTypeName = strIniItemName + TEXT("(Visible)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
	{
		nValue = (nValue == 0) ? SW_HIDE : SW_SHOW;
		// Show/Hide 설정
		pCtrl->ShowWindow(nValue);
	}


	// Image 파일 정보
	
	// 컨트롤 INI파일에서 배경 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// 배경 이미지 설정
		pCtrl->SetBackImage(strText);
	}

	// 컨트롤 INI파일에서 Bar 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Bar)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// Bar 이미지 설정
		pCtrl->SetBarImage(strText);
	}

	// 컨트롤 INI파일에서 '이전' 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Prev)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// '이전' 이미지 설정
		pCtrl->SetPrevImage(strText);
	}

	// 컨트롤 INI파일에서 '다음' 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Next)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// '다음' 이미지 설정
		pCtrl->SetNextImage(strText);
	}
}

// Theme Static 컨트롤을 추가
CThemeStatic* CThemeDialog::AddThemeStaticCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	CRect rCtrl;
	CString strCaption;
	CThemeStatic* pCtrl;

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("Static%d"), m_arrThemeStatic.GetSize());

	// 원본 컨트롤 정보를 구한다
	this->GetDlgItem(nCtrlID)->GetWindowRect(&rCtrl);
	ScreenToClient(&rCtrl);
	this->GetDlgItem(nCtrlID)->GetWindowText(strCaption);
	this->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	// 원본 컨트롤을 제거
	this->GetDlgItem(nCtrlID)->DestroyWindow();

	// Theme 컨트롤 생성
	pCtrl = new CThemeStatic;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP, rCtrl, this, nCtrlID);
	// Theme 컨트롤 Caption 텍스트 설정
	pCtrl->SetCaptionText(strCaption);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	// 어레이에 새로운 컨트롤 추가
	m_arrThemeStatic.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (m_arrThemeStatic)[m_arrThemeStatic.GetSize()-1];
}

// Group 컨트롤에 Style 설정
void CThemeDialog::SetStyleGroupCtrl(CThemeGroup *pCtrl, CString strIniItemName)
{
	CString strTypeName;
	COLORREF crColor;
	CString strText;
	CRect rCtrl;
	int nValue=0;

	// 컨트롤이 생성되지 않았다면
	if( pCtrl == NULL )
		return;

	// 위치 정보

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		// 초기 위치 설정
		pCtrl->SetInitPos(rCtrl);
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rCtrl = gf_TransCtrlPosToAbsolute(rCtrl, this);
		// 컨트롤 위치좌표 설정
		pCtrl->MoveWindow(rCtrl);
	}

	// INI 파일에서 캡션 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(CaptionArea)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
		// Caption 영역좌표 설정
		pCtrl->SetCaptionArea(rCtrl);


	// 텍스트 정보

	// 텍스트 INI파일에서 캡션 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(Caption)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// Caption 문자열 설정
		pCtrl->SetCaptionText(strText);

	/*// 텍스트 INI파일에서 툴팁 문자열을 구한다
	strTypeName = strIniItemName + "(ToolTip)";
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// Caption 문자열 설정
		pCtrl->SetToolTip(strText);*/


	// 컬러 정보

	// INI 파일에서 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 배경 컬러 설정
		pCtrl->SetBackColor(crColor);

	// INI 파일에서 외곽선 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Outline)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선 컬러 설정
		pCtrl->SetOutlineColor(crColor);

	// INI 파일에서 외곽선(Left-Top) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineLT)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Left-Top) 컬러 설정
		pCtrl->SetOutlineLTColor(crColor);

	// INI 파일에서 외곽선(Right-Bottom) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineRB)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Right-Bottom) 컬러 설정
		pCtrl->SetOutlineRBColor(crColor);

	// INI 파일에서 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Font)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 컬러 설정
		pCtrl->SetFontColor(crColor);

	// INI 파일에서 그림자 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Shadow)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 컬러 설정
		pCtrl->SetShadowColor(crColor);


	// Value 정보

	// INI 파일에서 폰트 크기를 구한다
	strTypeName = strIniItemName + TEXT("(FontSize)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 크기 설정
		pCtrl->SetFontSize(nValue);

	// INI 파일에서 Caption 정렬 방식을 구한다
	strTypeName = strIniItemName + TEXT("(AlignFormat)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 정렬 방식 설정
		pCtrl->SetAlignFormat(nValue);

	// INI 파일에서 폰트 두께를 구한다
	strTypeName = strIniItemName + TEXT("(FontBold)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 두께 설정
		pCtrl->SetFontBold(nValue);

	// INI 파일에서 배경 그리기 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackDrawType)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 그리기 방식 설정
		pCtrl->SetBackDrawType(nValue);

	// INI 파일에서 배경 이미지 채우기 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackImageFillType)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 이미지 채우기 방식 설정
		pCtrl->SetBackImageFillType(nValue);

	// INI 파일에서 배경 이미지 정렬 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackImageAlignFormat)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 이미지 정렬 방식 설정
		pCtrl->SetBackImageAlignFormat(nValue);

	// INI 파일에서 모서리 둥근 정도를 구한다
	strTypeName = strIniItemName + TEXT("(EdgeRoundValue)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 모서리 둥근 정도 설정
		pCtrl->SetEdgeRoundValue(nValue);

	// INI 파일에서 외곽선 두께를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineThick)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 외곽선 설정
		pCtrl->SetOutlineThick(nValue);

	// INI 파일에서 Show/Hide를 구한다
	strTypeName = strIniItemName + TEXT("(Visible)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
	{
		nValue = (nValue == 0) ? SW_HIDE : SW_SHOW;
		// Show/Hide 설정
		pCtrl->ShowWindow(nValue);
	}


	// Image 파일 정보
	
	// 컨트롤 INI파일에서 배경 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// 배경 이미지 설정
		pCtrl->SetBackImage(strText);
	}
}

// Theme Picture 컨트롤을 추가
CThemePicture* CThemeDialog::AddThemePictureCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	CRect rCtrl;
	CString strCaption;
	CThemePicture* pCtrl;

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("Picture%d"), m_arrThemePicture.GetSize());

	// 원본 컨트롤 정보를 구한다
	this->GetDlgItem(nCtrlID)->GetWindowRect(&rCtrl);
	ScreenToClient(&rCtrl);
	this->GetDlgItem(nCtrlID)->GetWindowText(strCaption);
	this->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	// 원본 컨트롤을 제거
	this->GetDlgItem(nCtrlID)->DestroyWindow();

	// Theme 컨트롤 생성
	pCtrl = new CThemePicture;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP, rCtrl, this, nCtrlID);
	// Theme 컨트롤 Caption 텍스트 설정
	pCtrl->SetCaptionText(strCaption);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	// 어레이에 새로운 컨트롤 추가
	m_arrThemePicture.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (m_arrThemePicture)[m_arrThemePicture.GetSize()-1];
}

// Picture 컨트롤에 Style 설정
void CThemeDialog::SetStylePictureCtrl(CThemePicture *pCtrl, CString strIniItemName)
{
	CString strTypeName;
	COLORREF crColor;
	CString strText;
	CRect rCtrl;
	int nValue=0;

	// 컨트롤이 생성되지 않았다면
	if( pCtrl == NULL )
		return;

	// 위치 정보

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		// 초기 위치 설정
		pCtrl->SetInitPos(rCtrl);
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rCtrl = gf_TransCtrlPosToAbsolute(rCtrl, this);
		// 컨트롤 위치좌표 설정
		pCtrl->MoveWindow(rCtrl);
	}

	// INI 파일에서 캡션 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(CaptionArea)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
		// Caption 영역좌표 설정
		pCtrl->SetCaptionArea(rCtrl);


	// 텍스트 정보

	// 텍스트 INI파일에서 캡션 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(Caption)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// Caption 문자열 설정
		pCtrl->SetCaptionText(strText);

	// 텍스트 INI파일에서 툴팁 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(ToolTip)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// Caption 문자열 설정
		pCtrl->SetToolTip(strText);


	// 컬러 정보

	// INI 파일에서 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 배경 컬러 설정
		pCtrl->SetBackColor(crColor);

	// INI 파일에서 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Font)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 컬러 설정
		pCtrl->SetFontColor(crColor);

	// INI 파일에서 그림자 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Shadow)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 컬러 설정
		pCtrl->SetShadowColor(crColor);


	// Value 정보

	// INI 파일에서 폰트 크기를 구한다
	strTypeName = strIniItemName + TEXT("(FontSize)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 크기 설정
		pCtrl->SetFontSize(nValue);

	// INI 파일에서 Caption 정렬 방식을 구한다
	strTypeName = strIniItemName + TEXT("(AlignFormat)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 정렬 방식 설정
		pCtrl->SetAlignFormat(nValue);

	// INI 파일에서 폰트 두께를 구한다
	strTypeName = strIniItemName + TEXT("(FontBold)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 두께 설정
		pCtrl->SetFontBold(nValue);

	// INI 파일에서 배경 이미지 채우기 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackImageFillType)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 이미지 채우기 방식 설정
		pCtrl->SetBackImageFillType(nValue);

	// INI 파일에서 배경 이미지 정렬 방식을 구한다
	strTypeName = strIniItemName + TEXT("(BackImageAlignFormat)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 이미지 정렬 방식 설정
		pCtrl->SetBackImageAlignFormat(nValue);

	// INI 파일에서 Show/Hide를 구한다
	strTypeName = strIniItemName + TEXT("(Visible)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
	{
		nValue = (nValue == 0) ? SW_HIDE : SW_SHOW;
		// Show/Hide 설정
		pCtrl->ShowWindow(nValue);
	}

	
	// Image 파일 정보
	
	// 컨트롤 INI파일에서 배경 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// 배경 이미지 설정
		pCtrl->SetBackImage(strText);
	}
}

// Menu 컨트롤에 Style 설정
void CThemeDialog::SetStyleMenuCtrl(CThemeMenu *pCtrl, CString strIniItemName)
{
	CString strTypeName;
	COLORREF crColor;
	CString strText;
	CRect rCtrl;
	int nValue=0;

	// 컨트롤이 생성되지 않았다면
	if( pCtrl == NULL )
		return;

	// 위치 정보

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		// 초기 위치 설정
		pCtrl->SetInitPos(rCtrl);
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rCtrl = gf_TransCtrlPosToAbsolute(rCtrl, this);
		// 컨트롤 위치좌표 설정
		pCtrl->MoveWindow(rCtrl);
	}


	// 컬러 정보

	// INI 파일에서 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 배경 컬러 설정
		pCtrl->SetBackColor(crColor);

	// INI 파일에서 폰트 컬러(활성) 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Font)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 컬러(활성) 설정
		pCtrl->SetFontColor(crColor);

	// INI 파일에서 폰트 컬러(활성) 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FontEnable)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 컬러(활성) 설정
		pCtrl->SetFontEnableColor(crColor);

	// INI 파일에서 폰트 컬러(비활성) 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FontDisable)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 컬러(비활성) 설정
		pCtrl->SetFontDisableColor(crColor);

	// INI 파일에서 일반 항목 배경 컬러를 구한다
	strTypeName = strIniItemName + TEXT("(NormalBack)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 항목 배경 컬러 설정
		pCtrl->SetNormalBackColor(crColor);

	// INI 파일에서 Hover 항목 배경 컬러를 구한다
	strTypeName = strIniItemName + TEXT("(HoverBack)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// Over 항목 배경 컬러 설정
		pCtrl->SetHoverBackColor(crColor);

	// INI 파일에서 선택 항목 배경 컬러를 구한다
	strTypeName = strIniItemName + TEXT("(PressedBack)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 선택 항목 배경 컬러 설정
		pCtrl->SetPressedBackColor(crColor);

	// INI 파일에서 외곽선 컬러를 구한다
	strTypeName = strIniItemName + TEXT("(Outline)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선 컬러 설정
		pCtrl->SetOutlineColor(crColor);

	// INI 파일에서 외곽선(Left-Top) 컬러를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineLT)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Left-Top) 컬러 설정
		pCtrl->SetOutlineLTColor(crColor);

	// INI 파일에서 외곽선(Right-Bottom) 컬러를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineRB)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Right-Bottom) 컬러 설정
		pCtrl->SetOutlineRBColor(crColor);

	
	// Value 정보

	// INI 파일에서 배경 두께를 구한다
	strTypeName = strIniItemName + TEXT("(BackThick)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 배경 두께 설정
		pCtrl->SetBackThick(nValue);

	// INI 파일에서 폰트 크기를 구한다
	strTypeName = strIniItemName + TEXT("(FontSize)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 크기 설정
		pCtrl->SetFontSize(nValue);

	// INI 파일에서 항목 높이를 구한다
	strTypeName = strIniItemName + TEXT("(ItemHeight)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 항목 높이 설정
		pCtrl->SetItemHeight(nValue);

	// INI 파일에서 항목 이미지 개수를 구한다
	strTypeName = strIniItemName + TEXT("(ItemImageCount)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 항목 이미지 개수 설정
		pCtrl->SetItemImageCount(nValue);

	// INI 파일에서 항목 왼쪽 여백을 구한다
	strTypeName = strIniItemName + TEXT("(LeftGap)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 항목 왼쪽 여백 설정
		pCtrl->SetItemLeftGap(nValue);

	// INI 파일에서 항목 오른쪽 여백을 구한다
	strTypeName = strIniItemName + TEXT("(RightGap)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 항목 오른쪽 여백 설정
		pCtrl->SetItemRightGap(nValue);

	// INI 파일에서 외곽선 두께 구한다
	strTypeName = strIniItemName + TEXT("(OutlineThick)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 외곽선 두께 설정
		pCtrl->SetOutlineThick(nValue);

	// INI 파일에서 구분선 높이를 구한다
	strTypeName = strIniItemName + TEXT("(SeperateHeight)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 구분선 높이 설정
		pCtrl->SetSeperateHeight(nValue);

	// INI 파일에서 투명도를 구한다
	strTypeName = strIniItemName + TEXT("(Transparent)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 투명도 설정
		pCtrl->SetTransparent(nValue);


	// Image 파일 정보
	
	// 컨트롤 INI파일에서 항목 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Item)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// 항목 이미지 설정
		pCtrl->SetItemImage(strText);
	}

	// 컨트롤 INI파일에서 구분선 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Seperate)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// 구분선 이미지 설정
		pCtrl->SetSeperateImage(strText);
	}
}

// Theme Menu 컨트롤을 추가
CThemeMenu* CThemeDialog::AddThemeMenuCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	//CRect rCtrl;
	CString strCaption;
	CThemeMenu* pCtrl;

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("Menu%d"), m_arrThemeMenu.GetSize());

	// Theme 컨트롤 생성
	pCtrl = new CThemeMenu;
	pCtrl->Create(WS_CHILD|/*WS_VISIBLE|*/WS_TABSTOP, CRect(0,0,10,10), this, nCtrlID);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	// 컨트롤 어레이에 새로운 컨트롤 추가
	m_arrThemeMenu.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (m_arrThemeMenu)[m_arrThemeMenu.GetSize()-1];
}

// INI 파일의 정보로 다이얼로그에 테마를 적용
void CThemeDialog::SetStyleDlgFromIniFile()
{
	CString strText;
	COLORREF crColor;
	CRect rDlg;
	int nValue, nChild;
	
	// INI 파일에서 '차일드 형식 여부'를 구한다
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(Child)"), nChild) )
		this->SetChild(nChild);

	// 위치 정보

	// Child 형식이 아니고 INI 파일에서 다이얼로그 위치 정보를 구한다
	if( nChild==0 && GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(Pos)"), rDlg) )
	{
		// 초기 위치 설정
		//this->SetInitPos(rDlg);
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rDlg = gf_TransCtrlPosToAbsolute(rDlg, this);
		// 컨트롤 위치좌표 설정
		this->MoveWindow(rDlg);
	}

	// INI 파일에서 Caption 위치 정보를 구한다
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(CaptionArea)"), rDlg) )
		// Caption 위치 설정
		this->SetCaptionArea(rDlg);


	// 텍스트 정보

	// 텍스트 INI파일에서 캡션 문자열을 구한다
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(Caption)"), strText) )
		// Caption 문자열 설정
		this->SetCaptionText(strText);

	// 텍스트 INI파일에서 툴팁 문자열을 구한다
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(ToolTip)"), strText) )
		// 툴팁 설정
		this->SetToolTip(strText);


	// 컬러 정보

	// INI 파일에서 배경 컬러 정보를 구한다
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(Back)"), crColor) )
		// 배경 컬러 설정
		this->SetBackColor(crColor);

	// INI 파일에서 Caption 폰트 컬러 정보를 구한다
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(Font)"), crColor) )
		// Caption 폰트 컬러 설정
		this->SetFontColor(crColor);

	// INI 파일에서 Caption 그림자 컬러 정보를 구한다
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(Shadow)"), crColor) )
		// Caption 그림자 컬러 설정
		this->SetShadowColor(crColor);

	// INI 파일에서 투명으로 처리할 컬러를 구한다
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(Transparent)"), crColor) )
		// 투명으로 처리할 컬러 설정
		this->SetTransparentColor(crColor);

	
	// Value 정보

	// INI 파일에서 Caption 정렬 방식을 구한다
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(AlignFormat)"), nValue) )
		// 정렬 방식 설정
		this->SetAlignFormat(nValue);

	// INI 파일에서 배경 그리기 방식을 구한다
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(BackDrawType)"), nValue) )
		// 배경 그리기 방식 설정
		this->SetBackDrawType(nValue);

	// INI 파일에서 배경 이미지 채우기 방식을 구한다
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(BackImageFillType)"), nValue) )
		// 배경 이미지 채우기 방식 설정
		this->SetBackImageFillType(nValue);
	
	// INI 파일에서 폰트 크기를 구한다
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(FontSize)"), nValue) )
		// 캡션 폰트 크기 설정
		this->SetFontSize(nValue);

	// INI 파일에서 폰트 Bold 여부를 구한다
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(FontBold)"), nValue) )
		this->SetFontBold(nValue);

	// INI 파일에서 투명도를 구한다
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(Transparent)"), nValue) )
		this->SetTransparent(nValue);

	// INI 파일에서 Resize 여부를 구한다
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(Resize)"), nValue) )
		this->SetStyleResize(nValue);

	// INI 파일에서 마우스 드래그로 이동 여부를 구한다
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(MouseDragMove)"), nValue) )
		this->SetMouseDragMove(nValue);

	// INI 파일에서 '최소 넓이'를 구한다
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(MinWidth)"), nValue) )
		this->SetMinWidth(nValue);

	// INI 파일에서 '최소 높이'를 구한다
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(MinHeight)"), nValue) )
		this->SetMinHeight(nValue);

	// INI 파일에서 '최대 넓이'를 구한다
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(MaxWidth)"), nValue) )
		this->SetMaxWidth(nValue);

	// INI 파일에서 '최대 높이'를 구한다
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(MaxHeight)"), nValue) )
		this->SetMaxHeight(nValue);

	// INI 파일에서 컬러로 투명 처리 여부를 구한다
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(TransparentByColor)"), nValue) )
		this->SetTransparentByColor(nValue);


	// Image 파일 정보
	
	// 컨트롤 INI파일에서 배경 이미지 파일 이름을 구한다
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(Back)"), strText) )
		// 배경 이미지를 여는 함수(포커스 설정-파일)
		this->SetBackImageOnFocus(strText);

	// 컨트롤 INI파일에서 포커스 설정-이미지 파일 이름을 구한다
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(BackOnFocus)"), strText) )
		// 배경 이미지를 여는 함수(포커스 설정-파일)
		this->SetBackImageOnFocus(strText);

	// 컨트롤 INI파일에서 포커스 해제-이미지 파일 이름을 구한다
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(BackKillFocus)"), strText) )
		// 배경 이미지를 여는 함수(포커스 해제-파일)
		this->SetBackImageKillFocus(strText);

}

// Theme List 컨트롤을 추가
CThemeListCtrl* CThemeDialog::AddThemeListCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	CRect rCtrl;
	CThemeListCtrl* pCtrl;
	DWORD dwStyle;

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("ListCtrl%d"), m_arrThemeListCtrl.GetSize());

	// 원본 컨트롤 정보를 구한다
	this->GetDlgItem(nCtrlID)->GetWindowRect(&rCtrl);
	ScreenToClient(&rCtrl);
	dwStyle = this->GetDlgItem(nCtrlID)->GetStyle();
	this->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	// 원본 컨트롤을 제거
	this->GetDlgItem(nCtrlID)->DestroyWindow();

	// Theme 컨트롤 생성
	pCtrl = new CThemeListCtrl();
	pCtrl->Create(rCtrl, this, nCtrlID);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	// 어레이에 새로운 컨트롤 추가
	m_arrThemeListCtrl.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (m_arrThemeListCtrl)[m_arrThemeListCtrl.GetSize()-1];
}

// List Box 컨트롤에 Style 설정
void CThemeDialog::SetStyleListBoxCtrl(CThemeListBox *pCtrl, CString strIniItemName)
{
	CString strTypeName;
	COLORREF crColor;
	CString strText;
	CRect rCtrl;
	int nValue=0;

	// 컨트롤이 생성되지 않았다면
	if( pCtrl == NULL )
		return;

	// 위치 정보

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		// 초기 위치 설정
		pCtrl->SetInitPos(rCtrl);
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rCtrl = gf_TransCtrlPosToAbsolute(rCtrl, this);
		// 컨트롤 위치좌표 설정
		pCtrl->MoveWindow(rCtrl);
	}


	// 텍스트 정보

	// 텍스트 INI파일에서 툴팁 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(ToolTip)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// Caption 문자열 설정
		pCtrl->SetToolTip(strText);


	// 컬러 정보

	// INI 파일에서 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 배경 컬러 설정
		pCtrl->SetBackColor(crColor);

	// INI 파일에서 외곽선 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Outline)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선 컬러 설정
		pCtrl->SetOutlineColor(crColor);

	// INI 파일에서 외곽선(Left-Top) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineLT)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Left-Top) 컬러 설정
		pCtrl->SetOutlineLTColor(crColor);

	// INI 파일에서 외곽선(Right-Bottom) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineRB)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 외곽선(Right-Bottom) 컬러 설정
		pCtrl->SetOutlineRBColor(crColor);

	// INI 파일에서 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Font)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 폰트 컬러 설정
		pCtrl->SetFontColor(crColor);

	// INI 파일에서 '상태-1' 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FontState1)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// '상태-1' 폰트 컬러 설정
		pCtrl->SetFontState1Color(crColor);

	// INI 파일에서 '상태-2' 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FontState2)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// '상태-2' 폰트 컬러 설정
		pCtrl->SetFontState2Color(crColor);

	// INI 파일에서 '상태-3' 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FontState3)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// '상태-3' 폰트 컬러 설정
		pCtrl->SetFontState3Color(crColor);

	// INI 파일에서 선택 항목 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(SelectFont)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 선택 항목 폰트 컬러 설정
		pCtrl->SetSelectItemFontColor(crColor);

	// INI 파일에서 선택 항목 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(SelectBack)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 선택 항목 배경 컬러 설정
		pCtrl->SetSelectItemBackColor(crColor);

	// INI 파일에서 일반 항목 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(NormalBack)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 항목 배경 컬러 설정
		pCtrl->SetNormalItemBackColor(crColor);

	// INI 파일에서 일반 항목(홀수번째) 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Normal1Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 항목(홀수번째) 배경 컬러 설정
		pCtrl->SetNormalItem1BackColor(crColor);

	// INI 파일에서 일반 항목(짝수번째) 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Normal2Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 항목(짝수번째) 배경 컬러 설정
		pCtrl->SetNormalItem2BackColor(crColor);

	// INI 파일에서 항목 구분선 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Frame)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 항목 구분선 컬러 설정
		pCtrl->SetItemFrameColor(crColor);


	// Value 정보

	// INI 파일에서 폰트 크기를 구한다
	strTypeName = strIniItemName + TEXT("(FontSize)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 크기 설정
		pCtrl->SetFontSize(nValue);

	// INI 파일에서 정렬 방식을 구한다
	strTypeName = strIniItemName + TEXT("(AlignFormat)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 정렬 방식 설정
		pCtrl->SetAlignFormat(nValue);

	// INI 파일에서 폰트 두께를 구한다
	strTypeName = strIniItemName + TEXT("(FontBold)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 두께 설정
		pCtrl->SetFontBold(nValue);

	// INI 파일에서 항목 높이를 구한다
	strTypeName = strIniItemName + TEXT("(ItemHeight)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 항목 높이 설정
		pCtrl->SetItemHeight(nValue);

	// INI 파일에서 왼쪽 여백을 구한다
	strTypeName = strIniItemName + TEXT("(LeftGap)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 왼쪽 여백 설정
		pCtrl->SetLeftGap(nValue);

	// INI 파일에서 오른쪽 여백을 구한다
	strTypeName = strIniItemName + TEXT("(RightGap)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 오른쪽 여백 설정
		pCtrl->SetRightGap(nValue);

	// INI 파일에서 외곽선 두께를 구한다
	strTypeName = strIniItemName + TEXT("(OutlineThick)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 외곽선 두께를 설정
		pCtrl->SetOutlineThick(nValue);

	// INI 파일에서 수직 스크롤바 사용여부를 구한다
	strTypeName = strIniItemName + TEXT("(VScroll)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 수직 스크롤바 사용여부 설정
		pCtrl->SetVScroll(nValue);
	
	// INI 파일에서 Show/Hide를 구한다
	strTypeName = strIniItemName + TEXT("(Visible)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
	{
		nValue = (nValue == 0) ? SW_HIDE : SW_SHOW;
		// Show/Hide 설정
		pCtrl->ShowWindow(nValue);
	}

}

// Theme Scroll 컨트롤을 추가
CThemeScroll* CThemeDialog::AddThemeScrollCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	CRect rCtrl;
	CString strCaption;
	CThemeScroll* pCtrl;
	DWORD dwStyle;

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("Scroll%d"), m_arrThemeScroll.GetSize());

	// 원본 컨트롤 정보를 구한다
	this->GetDlgItem(nCtrlID)->GetWindowRect(&rCtrl);
	ScreenToClient(&rCtrl);
	//this->GetDlgItem(nCtrlID)->GetWindowText(strCaption);
	dwStyle = this->GetDlgItem(nCtrlID)->GetStyle();
	this->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	// 원본 컨트롤을 제거
	//this->GetDlgItem(nCtrlID)->DestroyWindow();

	// Theme 컨트롤 생성
	pCtrl = new CThemeScroll;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|dwStyle, rCtrl, this, nCtrlID);
	// Theme 컨트롤 텍스트 설정
	//pScroll->SetWindowText(strCaption);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	// 어레이에 새로운 컨트롤 추가
	m_arrThemeScroll.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (m_arrThemeScroll)[m_arrThemeScroll.GetSize()-1];
}

// Theme Grid 컨트롤을 추가
CThemeGrid* CThemeDialog::AddThemeGridCtrl(UINT nCtrlID, CString strIniCtrlName/*=""*/)
{
	CRect rCtrl;
	CThemeGrid* pCtrl;
	wchar_t szClassName[MAX_PATH];
	CString strClassName;

	// 컨트롤 클래스 네임을 구한다
	GetClassName(this->GetDlgItem(nCtrlID)->GetSafeHwnd(), szClassName, MAX_PATH);
	strClassName.Format(TEXT("%s"), szClassName);

	// Ini 조회용 컨트롤명이 지정되지 않았을경우 생성한다
	if( strIniCtrlName.GetLength() < 1 )
		strIniCtrlName.Format(TEXT("Grid%d"), m_arrThemeGrid.GetSize());
	
	// 원본 컨트롤 정보를 구한다
	this->GetDlgItem(nCtrlID)->GetWindowRect(&rCtrl);
	ScreenToClient(&rCtrl);
	this->GetDlgItem(nCtrlID)->ShowWindow(SW_HIDE);
	// 원본 컨트롤을 제거
	this->GetDlgItem(nCtrlID)->DestroyWindow();

	// Theme 컨트롤 생성
	pCtrl = new CThemeGrid;
	pCtrl->Create(rCtrl, this, nCtrlID);
	// Ini 조회용 컨트롤명 설정
	pCtrl->SetIniCtrlName(strIniCtrlName);
	// Base 컨트롤이 List 컨트롤이라면
	if( strClassName.Find(TEXT("SysListView32")) >= 0 )
		pCtrl->SetBaseCtrlList(TRUE);
	// 어레이에 새로운 컨트롤 추가
	m_arrThemeGrid.Add(pCtrl);

	// Theme 컨트롤 포인터를 반환
	return (CThemeGrid*)(m_arrThemeGrid)[m_arrThemeGrid.GetSize()-1];
}

// List Ctrl 컨트롤에 Style 설정
void CThemeDialog::SetStyleListCtrl(CThemeListCtrl *pCtrl, CString strIniItemName)
{
	CString strTypeName;
	COLORREF crColor;
	CString strText;
	CRect rCtrl;
	int nValue=0;

	// 컨트롤이 생성되지 않았다면
	if( pCtrl == NULL )
		return;

	// 위치 정보

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		// 초기 위치 설정
		pCtrl->SetInitPos(rCtrl);
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rCtrl = gf_TransCtrlPosToAbsolute(rCtrl, this);
		// 컨트롤 위치좌표 설정
		pCtrl->MoveWindow(rCtrl);
	}


	// 텍스트 정보

	// 텍스트 INI파일에서 툴팁 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(ToolTip)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// Caption 문자열 설정
		pCtrl->SetToolTip(strText);


	// 컬러 정보

	// INI 파일에서 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 배경 컬러 설정
		pCtrl->SetBackColor(crColor);

	// INI 파일에서 항목 셀 테두리 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Frame)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 항목 셀 테두리 컬러 설정
		pCtrl->SetFrameColor(crColor);

	// INI 파일에서 항목 셀 테두리 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(ItemFrame)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 항목 셀 테두리 컬러 설정
		pCtrl->SetItemFrameColor(crColor);

	// INI 파일에서 헤더 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(HeadBack)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 헤더 배경 컬러 설정
		pCtrl->SetHeadBackColor(crColor);

	// INI 파일에서 헤더 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(HeadFont)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 헤더 폰트 컬러 설정
		pCtrl->SetHeadFontColor(crColor);

	// INI 파일에서 헤더 셀 테두리 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(HeadFrame)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 헤더 셀 테두리 컬러 설정
		pCtrl->SetHeadFrameColor(crColor);

	// INI 파일에서 일반 항목 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Font)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 항목 폰트 컬러 설정
		pCtrl->SetFontColor(crColor);

	// INI 파일에서 일반 항목 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(NormalFont)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 항목 폰트 컬러 설정
		pCtrl->SetNormalItemFontColor(crColor);

	// INI 파일에서 일반 항목 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(NormalBack)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 항목 배경 컬러 설정
		pCtrl->SetNormalItemBackColor(crColor);

	// INI 파일에서 일반 항목-1 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Normal1Font)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 항목-1 폰트 컬러 설정
		pCtrl->SetNormalItem1FontColor(crColor);

	// INI 파일에서 일반 항목-1 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Normal1Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 항목-1 배경 컬러 설정
		pCtrl->SetNormalItem1BackColor(crColor);

	// INI 파일에서 일반 항목-2 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Normal2Font)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 항목-2 폰트 컬러 설정
		pCtrl->SetNormalItem2FontColor(crColor);

	// INI 파일에서 일반 항목-2 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Normal2Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 항목-2 배경 컬러 설정
		pCtrl->SetNormalItem2BackColor(crColor);

	// INI 파일에서 선택 항목 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(SelectBack)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 선택 항목 배경 컬러 설정
		pCtrl->SetSelectItemBackColor(crColor);

	// INI 파일에서 선택 항목 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(SelectFont)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 선택 항목 폰트 컬러 설정
		pCtrl->SetSelectItemFontColor(crColor);


	// Value 정보

	// INI 파일에서 체크박스 사용여부를 구한다
	strTypeName = strIniItemName + TEXT("(CheckBoxOn)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 체크박스 사용여부 설정
		pCtrl->SetCheckBoxOn(nValue);

	// INI 파일에서 폰트 크기를 구한다
	strTypeName = strIniItemName + TEXT("(FontSize)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 크기 설정
		pCtrl->SetFontSize(nValue);

	// INI 파일에서 헤더 체크박스 사용여부를 구한다
	strTypeName = strIniItemName + TEXT("(HeadCheckBoxOn)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 헤더 체크박스 사용여부 설정
		pCtrl->SetHeadCheckBoxOn(nValue);

	// INI 파일에서 항목 높이를 구한다
	strTypeName = strIniItemName + TEXT("(ItemHeight)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 항목 높이 설정
		pCtrl->SetItemHeight(nValue);

	// INI 파일에서 더블클릭으로 편집 여부를 구한다
	strTypeName = strIniItemName + TEXT("(EditDblClk)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 더블클릭으로 편집 여부 설정
		pCtrl->SetEditDblClk(nValue);

	// INI 파일에서 정렬 방식을 구한다
	strTypeName = strIniItemName + TEXT("(SortType)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 정렬 방식 설정
		pCtrl->SetSortType(nValue);

	// INI 파일에서 Show/Hide를 구한다
	strTypeName = strIniItemName + TEXT("(Visible)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
	{
		nValue = (nValue == 0) ? SW_HIDE : SW_SHOW;
		// Show/Hide 설정
		pCtrl->ShowWindow(nValue);
	}


	// Image 파일 정보
	
	// 컨트롤 INI파일에서 체크박스 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Check)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// 체크박스 이미지 설정
		pCtrl->SetCheckBoxImage(strText);
	}

	// 컨트롤 INI파일에서 체크박스 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(CheckBox)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// 체크박스 이미지 설정
		pCtrl->SetCheckBoxImage(strText);
	}
}

// Grid Ctrl 컨트롤에 Style 설정
void CThemeDialog::SetStyleGridCtrl(CThemeGrid *pCtrl, CString strIniItemName)
{
	CString strTypeName;
	COLORREF crColor;
	CString strText;
	CRect rCtrl;
	int nValue=0;

	// 컨트롤이 생성되지 않았다면
	if( pCtrl == NULL )
		return;


	// 위치 정보

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		// 초기 위치 설정
		pCtrl->SetInitPos(rCtrl);
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rCtrl = gf_TransCtrlPosToAbsolute(rCtrl, this);
		// 컨트롤 위치좌표 설정
		pCtrl->MoveWindow(rCtrl);
	}


	// 텍스트 정보

	// 텍스트 INI파일에서 툴팁 문자열을 구한다
	strTypeName = strIniItemName + TEXT("(ToolTip)");
	if( GetCtrlTextFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
		// Caption 문자열 설정
		pCtrl->SetToolTip(strText);

	

	// 컬러 정보

	// INI 파일에서 고정 셀 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FixedFont)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 고정 셀 폰트 컬러 설정
		pCtrl->SetFixedFontColor(crColor);

	// INI 파일에서 고정 셀 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FixedBack)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 고정 셀 배경 컬러 설정
		pCtrl->SetFixedBackColor(crColor);

	// INI 파일에서 고정 셀 외곽선 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FixedFrame)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 고정 셀 외곽선 컬러 설정
		pCtrl->SetFixedFrameColor(crColor);

	// INI 파일에서 고정 셀 외곽선(Left-Top) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FixedFrameLT)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 고정 셀 외곽선(Left-Top) 컬러 설정
		pCtrl->SetFixedFrameLTColor(crColor);

	// INI 파일에서 고정 셀 외곽선(Right-Bottom) 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(FixedFrameRB)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 고정 셀 외곽선(Right-Bottom) 컬러 설정
		pCtrl->SetFixedFrameRBColor(crColor);

	// INI 파일에서 일반 셀 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Font)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 셀 폰트 컬러 설정
		pCtrl->SetNormalFontColor(crColor);

	// INI 파일에서 일반 셀 폰트 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(NormalFont)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 셀 폰트 컬러 설정
		pCtrl->SetNormalFontColor(crColor);

	// INI 파일에서 일반 셀 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(NormalBack)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 셀 배경 컬러 설정
		pCtrl->SetNormalBackColor(crColor);

	// INI 파일에서 일반 셀 외곽선 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Frame)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 일반 셀 외곽선 컬러 설정
		pCtrl->SetFrameColor(crColor);

	// INI 파일에서 배경 컬러 정보를 구한다
	strTypeName = strIniItemName + TEXT("(Back)");
	if( GetCtrlColorFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, crColor) )
		// 배경 컬러 설정
		pCtrl->SetBackColor(crColor);


	// Value 정보

	// INI 파일에서 행 개수를 구한다
	strTypeName = strIniItemName + TEXT("(RowCount)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 행 개수 설정
		pCtrl->SetRowCount(nValue);

	// INI 파일에서 열 개수를 구한다
	strTypeName = strIniItemName + TEXT("(ColCount)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 열 개수 설정
		pCtrl->SetColCount(nValue);

	// INI 파일에서 고정 행 개수를 구한다
	strTypeName = strIniItemName + TEXT("(FixedRowCount)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 고정 행 개수 설정
		pCtrl->SetFixedRowCount(nValue);

	// INI 파일에서 고정 열 개수를 구한다
	strTypeName = strIniItemName + TEXT("(FixedColCount)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 고정 열 개수 설정
		pCtrl->SetFixedColCount(nValue);

	// INI 파일에서 편집 가능 여부를 구한다
	strTypeName = strIniItemName + TEXT("(Editable)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 편집 가능 여부 설정
		pCtrl->SetEditable(nValue);

	// INI 파일에서 더블클릭으로 편집 여부를 구한다
	strTypeName = strIniItemName + TEXT("(EditDblClk)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 더블클릭으로 편집 여부 설정
		pCtrl->SetEditDblClk(nValue);

	// INI 파일에서 단일 행 선택 여부를 구한다
	strTypeName = strIniItemName + TEXT("(SingleRowSelect)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 단일 행 선택 여부 설정
		pCtrl->SetSingleRowSelection(nValue);

	// INI 파일에서 헤더 정렬 여부를 구한다
	strTypeName = strIniItemName + TEXT("(HeadSort)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 헤더 정렬 여부 설정
		pCtrl->SetHeadSort(nValue);

	// INI 파일에서 텍스트 정렬 방식을 구한다
	strTypeName = strIniItemName + TEXT("(AlignFormat)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 텍스트 정렬 방식 설정
		pCtrl->SetAlignFormat(nValue);

	// INI 파일에서 셀 높이를 구한다
	strTypeName = strIniItemName + TEXT("(CellHeight)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 셀 높이 설정
		pCtrl->SetCellHeight(nValue);

	// INI 파일에서 셀 넓이를 구한다
	strTypeName = strIniItemName + TEXT("(CellWidth)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 셀 넓이 설정
		pCtrl->SetCellWidth(nValue);

	// INI 파일에서 행단위 선택 여부를 구한다
	strTypeName = strIniItemName + TEXT("(ListMode)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 행단위 선택 여부 설정
		pCtrl->SetListMode(nValue);

	// INI 파일에서 폰트 크기를 구한다
	strTypeName = strIniItemName + TEXT("(FontSize)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
		// 폰트 크기 설정
		pCtrl->SetFontSize(nValue);

	// INI 파일에서 Show/Hide를 구한다
	strTypeName = strIniItemName + TEXT("(Visible)");
	if( GetCtrlValueFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, nValue) )
	{
		nValue = (nValue == 0) ? SW_HIDE : SW_SHOW;
		// Show/Hide 설정
		pCtrl->ShowWindow(nValue);
	}


	// Image 파일 정보
	
	// 컨트롤 INI파일에서 체크박스 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(Check)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// 체크박스 이미지 설정
		pCtrl->SetCheckBoxImage(strText);
	}

	// 컨트롤 INI파일에서 체크박스 이미지 파일 이름을 구한다
	strTypeName = strIniItemName + TEXT("(CheckBox)");
	if( GetCtrlImageFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, strText) )
	{
		// 이미지 파일 전체 경로를 구한다
		strText = FilteringImageFilePath(strText);
		// 체크박스 이미지 설정
		pCtrl->SetCheckBoxImage(strText);
	}

}

// 컨트롤 INI 파일에서 컨트롤 관련 컬러 정보를 가져오는 함수
BOOL CThemeDialog::GetCtrlColorFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, COLORREF& crCtrlColor)
{
	CString strIniFileName;

	// 컨트롤 INI파일 전체 경로
	strIniFileName = g_strThemePath + dfINI_COLOR_FILE_NAME;

	// INI파일에서 컬러 정보를 가져오는 함수
	BOOL bResult = GetColorFromIniFile(strIniFileName, lpGrpName, lpCtrlName, crCtrlColor);

	return bResult;
}

// 컨트롤 INI파일에서 컨트롤 관련 텍스트 정보를 가져오는 함수
BOOL CThemeDialog::GetCtrlTextFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString &strText)
{
	CString strIniFileName;

	// 컨트롤 INI파일 전체 경로
	strIniFileName = g_strThemePath + dfINI_TEXT_FILE_NAME;

	// INI파일에서 텍스트 정보를 가져오는 함수
	BOOL bResult = GetTextFromIniFile(strIniFileName, lpGrpName, lpCtrlName, strText);

	return bResult;
}

// 컨트롤 INI파일에서 컨트롤 관련 이미지 파일 이름을 가져오는 함수
BOOL CThemeDialog::GetCtrlImageFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString &strText)
{
	CString strIniFileName;

	// 컨트롤 INI파일 전체 경로
	strIniFileName = g_strThemePath + dfINI_IMAGE_FILE_NAME;

	// INI파일에서 텍스트 정보를 가져오는 함수
	BOOL bResult = GetTextFromIniFile(strIniFileName, lpGrpName, lpCtrlName, strText);

	return bResult;
}

//컨트롤 INI 파일에서 컨트롤 관련 위치 정보를 가져오는 함수
BOOL CThemeDialog::GetCtrlPosFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CRect& RectCtrl)
{
	CString strIniFileName;

	// 컨트롤 INI파일 전체 경로
	strIniFileName = g_strThemePath + dfINI_POSITION_FILE_NAME;

	// INI파일에서 위치 정보를 가져오는 함수
	BOOL bResult = GetPosFromIniFile(strIniFileName, lpGrpName, lpCtrlName, RectCtrl);

	return bResult;
}

/*//Value INI 파일에 문자열 정보를 저장하는 함수
BOOL gf_SetValueText(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString strText)
{
	CString sPath = ::gf_GetModulePath() + dfINI_VALUE_FILE_NAME;

	BOOL bResult = ::WritePrivateProfileString(lpGrpName, lpCtrlName, strText, sPath);
	
	return bResult;
}*/

// 컨트롤 INI파일에 컨트롤 관련 텍스트 정보를 저장
BOOL CThemeDialog::SetCtrlTextToIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString strText)
{
	CString strIniFileName;

	// 컨트롤 INI파일 전체 경로
	strIniFileName = g_strThemePath + dfINI_TEXT_FILE_NAME;

	// INI 파일에 텍스트 정보를 저장
	BOOL bResult = gf_SetTextToIniFile(strIniFileName, lpGrpName, lpCtrlName, strText);
	
	return bResult;
}

// 컨트롤 INI 파일에 컨트롤 관련 컬러 정보를 저장
BOOL CThemeDialog::SetCtrlColorFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, COLORREF crCtrlColor)
{
	CString strIniFileName, strText;

	// 컨트롤 INI파일 전체 경로
	strIniFileName = g_strThemePath + dfINI_COLOR_FILE_NAME;

	// 컬러 정보를 문자열로 변경
	strText.Format(TEXT("%d,%d,%d"), GetRValue(crCtrlColor), GetGValue(crCtrlColor), GetBValue(crCtrlColor));
	
	// INI 파일에 컬러 정보를 저장
	BOOL bResult = gf_SetTextToIniFile(strIniFileName, lpGrpName, lpCtrlName, strText);
	
	return bResult;
}

// 컨트롤 INI파일에 컨트롤 관련 이미지 파일 이름을 저장
BOOL CThemeDialog::SetCtrlImageFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString strText)
{
	CString strIniFileName;

	// 컨트롤 INI파일 전체 경로
	strIniFileName = g_strThemePath + dfINI_IMAGE_FILE_NAME;

	// INI 파일에 이미지 파일명을 저장
	BOOL bResult = gf_SetTextToIniFile(strIniFileName, lpGrpName, lpCtrlName, strText);
	
	return bResult;
}

// 컨트롤 INI 파일에 컨트롤 관련 위치 정보를 저장
BOOL CThemeDialog::SetCtrlPosFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CRect RectCtrl)
{
	CString strIniFileName, strText;

	// 컨트롤 INI파일 전체 경로
	strIniFileName = g_strThemePath + dfINI_POSITION_FILE_NAME;

	// 위치 정보를 문자열로 변경
	strText.Format(TEXT("%d,%d,%d,%d"), RectCtrl.left, RectCtrl.top, RectCtrl.right, RectCtrl.bottom);

	// INI 파일에 이미지 파일명을 저장
	BOOL bResult = gf_SetTextToIniFile(strIniFileName, lpGrpName, lpCtrlName, strText);
	
	return bResult;
}

// 컨트롤 INI 파일에 컨트롤 관련숫자 정보를 저장
BOOL CThemeDialog::SetCtrlValueFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, int nValue)
{
	CString strIniFileName, strText;

	// 컨트롤 INI파일 전체 경로
	strIniFileName = g_strThemePath + dfINI_VALUE_FILE_NAME;

	// 숫자 정보를 문자열로 변경
	strText.Format(TEXT("%d"), nValue);

	// INI 파일에 이미지 파일명을 저장
	BOOL bResult = gf_SetTextToIniFile(strIniFileName, lpGrpName, lpCtrlName, strText);
	
	return bResult;
}

//INI 파일에서 위치 정보를 가져오는 함수
BOOL CThemeDialog::GetPosFromIniFile(CString strIniFileName, LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CRect& RectCtrl)
{
	CString sPath;
	
	// INI 파일 경로에 하드 드라이브명이 포함되지 않았다면
	if( strIniFileName.Find(TEXT(":")) < 0 )
		sPath = gf_GetModulePath() + strIniFileName;
	// INI 파일 경로에 하드 드라이브명이 포함되었다면
	else
		sPath = strIniFileName;

	wchar_t szValue[MAX_PATH];
	memset(szValue, 0, MAX_PATH);

	BOOL bResult = ::GetPrivateProfileString(lpGrpName,
											lpCtrlName,
											_T(""),
											szValue,
											MAX_PATH,
											LPCTSTR(sPath));

	RectCtrl = CnvtStringToRect(szValue);

	return bResult;
}

// INI 파일에서 컬러 정보를 가져오는 함수
BOOL CThemeDialog::GetColorFromIniFile(CString strIniFileName, LPCTSTR lpGrpName, LPCTSTR lpCtrlName, COLORREF& crCtrlColor)
{
	CString sPath;
	
	// INI 파일 경로에 하드 드라이브명이 포함되지 않았다면
	if( strIniFileName.Find(TEXT(":")) < 0 )
		sPath = gf_GetModulePath() + strIniFileName;
	// INI 파일 경로에 하드 드라이브명이 포함되었다면
	else
		sPath = strIniFileName;

	wchar_t szValue[MAX_PATH];
	memset(szValue, 0, MAX_PATH);

	BOOL bResult = ::GetPrivateProfileString(lpGrpName,
											lpCtrlName,
											_T(""),
											szValue,
											MAX_PATH,
											LPCTSTR(sPath));

	crCtrlColor = CnvtStringToColor(szValue);

	return bResult;
}

// INI파일에서 텍스트 정보를 가져오는 함수
BOOL CThemeDialog::GetTextFromIniFile(CString strIniFileName, LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString &strText)
{
	CString sPath;
	
	// INI 파일 경로에 하드 드라이브명이 포함되지 않았다면
	if( strIniFileName.Find(TEXT(":")) < 0 )
		sPath = gf_GetModulePath() + strIniFileName;
	// INI 파일 경로에 하드 드라이브명이 포함되었다면
	else
		sPath = strIniFileName;

	wchar_t szValue[MAX_PATH];
	memset(szValue, 0, MAX_PATH);

	BOOL bResult = ::GetPrivateProfileString(lpGrpName,
											lpCtrlName,
											_T(""),
											szValue,
											MAX_PATH,
											LPCTSTR(sPath));

	strText = szValue;

	return bResult;
}

// INI 파일에서 숫자 정보를 가져오는 함수
BOOL CThemeDialog::GetValueFromIniFile(CString strIniFileName, LPCTSTR lpGrpName, LPCTSTR lpCtrlName, int& nValue)
{
	CString sPath;
	
	// INI 파일 경로에 하드 드라이브명이 포함되지 않았다면
	if( strIniFileName.Find(TEXT(":")) < 0 )
		sPath = gf_GetModulePath() + strIniFileName;
	// INI 파일 경로에 하드 드라이브명이 포함되었다면
	else
		sPath = strIniFileName;

	wchar_t szValue[MAX_PATH];
	memset(szValue, 0, MAX_PATH);

	BOOL bResult = ::GetPrivateProfileString(lpGrpName,
											lpCtrlName,
											_T(""),
											szValue,
											MAX_PATH,
											LPCTSTR(sPath));

	nValue = _wtoi(szValue);

	return bResult;
}

// Theme 컨트롤을 추가하고 Style 설정
CWnd* CThemeDialog::AddThemeCtrl(UINT nCtrlID, CString strIniCtrlName)
{
	CWnd* pCtrl;
	int nCtrlType;

	// Theme 컨트롤을 추가
	pCtrl = AddThemeCtrl(nCtrlID, nCtrlType, strIniCtrlName);

	// 컨트롤 종류에 따라서 Style 설정
	switch( nCtrlType )
	{
	case dfTHEME_CONTROL_TYPE_STATIC :		// 컨트롤 종류 - Static
		// Static 컨트롤에 Style 설정
		SetStyleStaticCtrl((CThemeStatic*)pCtrl, strIniCtrlName);
		break;
	case dfTHEME_CONTROL_TYPE_GROUP :		// 컨트롤 종류 - Group
		// Group 컨트롤에 Style 설정
		SetStyleGroupCtrl((CThemeGroup*)pCtrl, strIniCtrlName);
		break;
	case dfTHEME_CONTROL_TYPE_BUTTON :		// 컨트롤 종류 - Button
		// Button 컨트롤에 Style 설정
		SetStyleButtonCtrl((CThemeButton*)pCtrl, strIniCtrlName);
		break;
	case dfTHEME_CONTROL_TYPE_CHECK :		// 컨트롤 종류 - CheckBox
		// Check 컨트롤에 Style 설정
		SetStyleCheckCtrl((CThemeCheck*)pCtrl, strIniCtrlName);
		break;
	case dfTHEME_CONTROL_TYPE_RADIO :		// 컨트롤 종류 - Radio
		// Radio 컨트롤에 Style 설정
		SetStyleRadioCtrl((CThemeRadio*)pCtrl, strIniCtrlName);
		break;
	case dfTHEME_CONTROL_TYPE_EDIT :		// 컨트롤 종류 - Edit
		// Edit 컨트롤에 Style 설정
		SetStyleEditCtrl((CThemeEdit*)pCtrl, strIniCtrlName);
		break;
	case dfTHEME_CONTROL_TYPE_LISTBOX :		// 컨트롤 종류 - ListBox
		// ListBox 컨트롤에 Style 설정
		SetStyleListBoxCtrl((CThemeListBox*)pCtrl, strIniCtrlName);
		break;
	case dfTHEME_CONTROL_TYPE_COMBO :		// 컨트롤 종류 - ComboBox
		// Combo Box 컨트롤에 Style 설정
		SetStyleComboCtrl((CThemeCombo*)pCtrl, strIniCtrlName);
		break;
	case dfTHEME_CONTROL_TYPE_PROGRESS :	// 컨트롤 종류 - ProgressBar
		// Progress Bar 컨트롤에 Style 설정
		SetStyleProgressCtrl((CThemeProgress*)pCtrl, strIniCtrlName);
		break;
	case dfTHEME_CONTROL_TYPE_LISTCTRL :	// 컨트롤 종류 - ListCtrl
		// List Ctrl 컨트롤에 Style 설정
		SetStyleListCtrl((CThemeListCtrl*)pCtrl, strIniCtrlName);
		break;
	case dfTHEME_CONTROL_TYPE_SLIDER :		// 컨트롤 종류 - Slider
		// Slider 컨트롤에 Style 설정
		SetStyleSliderCtrl((CThemeSlider*)pCtrl, strIniCtrlName);
		break;
	case dfTHEME_CONTROL_TYPE_SCROLL :		// 컨트롤 종류 - Scroll
		// Scroll 컨트롤에 Style 설정
		SetStyleScrollCtrl((CThemeScroll*)pCtrl, strIniCtrlName);
		break;
	case dfTHEME_CONTROL_TYPE_PICTURE :		// 컨트롤 종류 - Picture
		// Picture 컨트롤에 Style 설정
		SetStylePictureCtrl((CThemePicture*)pCtrl, strIniCtrlName);
		break;
	case dfTHEME_CONTROL_TYPE_MENU :		// 컨트롤 종류 - Menu
		// Menu 컨트롤에 Style 설정
		SetStyleMenuCtrl((CThemeMenu*)pCtrl, strIniCtrlName);
		break;
	case dfTHEME_CONTROL_TYPE_GRID :		// 컨트롤 종류 - Grid
		break;
	}

	// 생성된 컨트롤을 반환
	return pCtrl;
}

// Theme 컨트롤을 추가
CWnd* CThemeDialog::AddThemeCtrl(UINT nCtrlID)
{
	int nCtrlType=0;

	// Theme 컨트롤을 추가
	return AddThemeCtrl(nCtrlID, nCtrlType);
}

// Theme 컨트롤을 추가
CWnd* CThemeDialog::AddThemeCtrl(UINT nCtrlID, int &nCtrlType, CString strIniCtrlName/*=""*/)
{
	wchar_t szClassName[MAX_PATH];
	CString strClassName;
	DWORD nStyle;

	nCtrlType = 0;

	// 컨트롤 클래스 네임을 구한다
	GetClassName(this->GetDlgItem(nCtrlID)->GetSafeHwnd(), szClassName, MAX_PATH);
	strClassName.Format(TEXT("%s"), szClassName);
	// 컨트롤 Style을 구한다
	nStyle = GetWindowLong(this->GetDlgItem(nCtrlID)->GetSafeHwnd(), GWL_STYLE);

	CString strCaption;
	this->GetDlgItem(nCtrlID)->GetWindowText(strCaption);

	// 클래스 종류가 버튼 일때
	if( strClassName == "Button" )
	{
		// 그룹박스 일때
		if( (nStyle & BS_GROUPBOX) == BS_GROUPBOX )
		{
			// 컨트롤 종류 지정
			nCtrlType = dfTHEME_CONTROL_TYPE_GROUP;
			// Theme Group 컨트롤을 추가
			return (CWnd*)AddThemeGroupCtrl(nCtrlID, strIniCtrlName);
		}
		// 체크박스 일때
		if( (nStyle & BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX )
		{
			// 컨트롤 종류 지정
			nCtrlType = dfTHEME_CONTROL_TYPE_CHECK;
			// Theme Check Box 컨트롤을 추가
			return (CWnd*)AddThemeCheckCtrl(nCtrlID, strIniCtrlName);
		}
		// 라디오버튼 일때
		if( (nStyle & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON )
		{
			// 컨트롤 종류 지정
			nCtrlType = dfTHEME_CONTROL_TYPE_RADIO;
			// Theme Radio Btn 컨트롤을 추가
			return (CWnd*)AddThemeRadioCtrl(nCtrlID, strIniCtrlName);
		}
		// 푸쉬버튼 일때
		if( (nStyle & BS_PUSHBUTTON) == BS_PUSHBUTTON )
		{
			// 컨트롤 종류 지정
			nCtrlType = dfTHEME_CONTROL_TYPE_BUTTON;
			// Theme Button 컨트롤을 추가
			return (CWnd*)AddThemeButtonCtrl(nCtrlID, strIniCtrlName);
		}
	}
	// 클래스 종류가 Static 일때
	else if( strClassName == "Static" )
	{
		// Picture 컨트롤 일때
		if( (nStyle & BS_GROUPBOX) == BS_GROUPBOX )
		{
			// 컨트롤 종류 지정
			nCtrlType = dfTHEME_CONTROL_TYPE_PICTURE;
			// Theme Picture 컨트롤을 추가
			return (CWnd*)AddThemePictureCtrl(nCtrlID, strIniCtrlName);
		}
		// Static 컨트롤 일때
		else
		{
			// 컨트롤 종류 지정
			nCtrlType = dfTHEME_CONTROL_TYPE_STATIC;
			// Theme Static 컨트롤을 추가
			return (CWnd*)AddThemeStaticCtrl(nCtrlID, strIniCtrlName);
		}
	}
	// 클래스 종류가 Editbox 일때
	else if( strClassName == "Edit" )
	{
		// 컨트롤 종류 지정
		nCtrlType = dfTHEME_CONTROL_TYPE_EDIT;
		// Theme Edit 컨트롤을 추가
		return (CWnd*)AddThemeEditCtrl(nCtrlID, strIniCtrlName);
	}
	// 클래스 종류가 List Box 일때
	else if( strClassName == "ListBox" )
	{
		// 컨트롤 종류 지정
		nCtrlType = dfTHEME_CONTROL_TYPE_LISTBOX;
		// Theme List Box 컨트롤을 추가
		return (CWnd*)AddThemeListBoxCtrl(nCtrlID, strIniCtrlName);
	}
	// 클래스 종류가 Combo Box 일때
	else if( strClassName == "ComboBox" )
	{
		// 컨트롤 종류 지정
		nCtrlType = dfTHEME_CONTROL_TYPE_COMBO;
		// Theme Combo Btn 컨트롤을 추가
		return (CWnd*)AddThemeComboCtrl(nCtrlID, strIniCtrlName);
	}
	// 클래스 종류가 Progress Bar 일때
	else if( strClassName.Find(TEXT("_progress")) >= 0 )
	{
		// 컨트롤 종류 지정
		nCtrlType = dfTHEME_CONTROL_TYPE_PROGRESS;
		// Theme ProgressBar 컨트롤을 추가
		return (CWnd*)AddThemeProgressCtrl(nCtrlID, strIniCtrlName);
	}
	// 클래스 종류가 Slider 일때
	else if( strClassName.Find(TEXT("_trackbar")) >= 0 )
	{
		// 컨트롤 종류 지정
		nCtrlType = dfTHEME_CONTROL_TYPE_SLIDER;
		// Theme Slider 컨트롤을 추가
		return (CWnd*)AddThemeSliderCtrl(nCtrlID, strIniCtrlName);
	}
	// 클래스 종류가 List Ctrl 일때
	else if( strClassName.Find(TEXT("SysListView32")) >= 0 )
	{
		// 컨트롤 종류 지정
		nCtrlType = dfTHEME_CONTROL_TYPE_LISTCTRL;
		// Theme List Ctrl 컨트롤을 추가
		return (CWnd*)AddThemeListCtrl(nCtrlID, strIniCtrlName);
	}
	
	return NULL;
}

// 최소 넓이 지정
void CThemeDialog::SetMinWidth(int nMinWidth)
{
	m_stThemeInfo.nMinWidth = nMinWidth;
}

// 최소 높이 지정
void CThemeDialog::SetMinHeight(int nMinHeight)
{
	m_stThemeInfo.nMinHeight = nMinHeight;
}

// 최대 넓이 지정
void CThemeDialog::SetMaxWidth(int nMaxWidth)
{
	m_stThemeInfo.nMaxWidth = nMaxWidth;
}

// 최대 높이 지정
void CThemeDialog::SetMaxHeight(int nMaxHeight)
{
	m_stThemeInfo.nMaxHeight = nMaxHeight;
}

// 절대좌표 영역좌표인지를 판단
BOOL CThemeDialog::IsAbsoluteArea(CRect rArea)
{
	int nRelativeLevel=10000;

	// 퍼센트 단위를 사용한다면 상대좌표로 판단
	if( abs(rArea.left) >= nRelativeLevel || abs(rArea.top) >= nRelativeLevel 
		|| abs(rArea.right) >= nRelativeLevel || abs(rArea.bottom) >= nRelativeLevel )
		return FALSE;

	if( rArea.left >= 0 && rArea.top >= 0 && rArea.right >= 0 && rArea.bottom >= 0 )
		return TRUE;
	if( rArea.left == 0 && rArea.top == 0 && rArea.right == 0 && rArea.bottom == 0 )
		return TRUE;
	if( rArea.left == -1 && rArea.top == -1 && rArea.right == -1 && rArea.bottom == -1 )
		return TRUE;

	return FALSE;
}

// 다이얼로그 모든 항목 그린다
void CThemeDialog::DrawAllDialogItem(CPaintDC *pdc)
{
	// 화면 영역좌표 구하기
	CRect rScreen;
	this->GetClientRect(&rScreen);

	// 배경 그리기 방식에 따라서
	switch( m_stThemeInfo.nBackDrawType )
	{
	// 컬러로 배경을 그린다면
	case dfBACK_DRAW_TYPE_COLOR :
		// 컬러로 배경을 그린다
		DrawBackByColor(pdc, m_stThemeInfo.crBackground, rScreen);
		break;
	// 이미지로 배경을 그린다면
	case dfBACK_DRAW_TYPE_IMAGE :
		// 포커스 해제 상태이고, 포커스 해제용 배경 이미작 존재한다면
		if( !m_stThemeInfo.bFocus && m_stThemeInfo.bSettledImageKillFocus )
			// 비트맵 이미지(포커스 해제용)로 배경을 그린다
			DrawBackByImage(pdc, &m_stThemeInfo.bmpBackKillFocus, rScreen);
		else
			// 비트맵 이미지(포커스 설정용)로 배경을 그린다
			DrawBackByImage(pdc, &m_stThemeInfo.bmpBackOnFocus, rScreen);

		/*// 포커스 설정 상태라면
		if( m_stThemeInfo.bFocus )
			// 비트맵 이미지(포커스 설정용)로 배경을 그린다
			DrawBackByImage(pdc, &m_stThemeInfo.bmpBackOnFocus, rScreen);
		// 포커스 해제 상태라면
		else
		{
			// 포커스 해제용 배경 이미지가 존재한다면
			if( m_stThemeInfo.bSettledImageKillFocus )
				// 비트맵 이미지(포커스 해제용)로 배경을 그린다
				DrawBackByImage(pdc, &m_stThemeInfo.bmpBackKillFocus, rScreen);
			// 포커스 해제용 배경 이미지가 존재하지 않는다면
			else
				// 비트맵 이미지(포커스 설정용)로 배경을 그린다
				DrawBackByImage(pdc, &m_stThemeInfo.bmpBackOnFocus, rScreen);
		}*/
		break;
	// 배경을 그리지 않는다면
	default :
		break;
	}

	// Caption 문자열을 화면에 표시한다
	DrawCaptionText(pdc);
}

// 포커스 설정/해제 이벤트
void CThemeDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	BOOL bFocus=false;
	// 포커스 지정 여부
	bFocus = (nState!=0) ? TRUE : FALSE;
	// 포커스 지정 상태가 변경되었다면
	if( m_stThemeInfo.bFocus != bFocus )
	{
		m_stThemeInfo.bFocus = bFocus;

		// 타이틀바 영역을 새로 그리는 함수
		InvalidateTitleBar();
	}
}

// 배경 화면 삭제시 실행되는 함수
BOOL CThemeDialog::OnEraseBkgnd(CDC* pDC) 
{
	// 다이얼로그 모든 항목 그린다
	//DrawAllDialogItem((CPaintDC*)pDC);

	// 화면 영역좌표 구하기
	CRect rScreen;
	this->GetClientRect(&rScreen);

	// 메모리 버퍼 구하기
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	// CBitmap 을 생성하고 메모리 DC에 할당
	CBitmap bmp, *pOldBitmap;
	bmp.CreateCompatibleBitmap(pDC, rScreen.Width(), rScreen.Height());
	pOldBitmap = MemDC.SelectObject(&bmp);
	MemDC.SetBkMode(TRANSPARENT);
	// 컬러로 투명 처리를 한다면
	if( m_stThemeInfo.bTransparentByColor )
		// 화면 전체에 투명 컬러를 칠한다
		MemDC.FillRect(rScreen, &CBrush(m_stThemeInfo.crTransparent));
	else
		// 화면 전체에 배경 컬러를 칠한다
		MemDC.FillRect(rScreen, &CBrush(m_stThemeInfo.crBackground));

	// 다이얼로그 모든 항목 그린다
	DrawAllDialogItem((CPaintDC*)&MemDC);

	// CPaintDC에 메모리 버퍼를 복사
	pDC->BitBlt(rScreen.left,rScreen.top,rScreen.Width(),rScreen.Height(), &MemDC, 0,0,SRCCOPY);
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();

	// 배경을 지우지 않고 함수 종료
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

//DEL void CThemeDialog::OnNcPaint()
//DEL {
//DEL 	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//DEL 	// 그리기 메시지에 대해서는 CDialog::OnNcPaint()을(를) 호출하지 마십시오.
//DEL 	/*CClientDC dc(this);
//DEL 
//DEL 	// 화면 영역좌표 구하기
//DEL 	CRect rScreen;
//DEL 	this->GetClientRect(&rScreen);
//DEL 
//DEL 	// 메모리 버퍼 구하기
//DEL 	CDC MemDC;
//DEL 	MemDC.CreateCompatibleDC(&dc);
//DEL 
//DEL 	// CBitmap 을 생성하고 메모리 DC에 할당
//DEL 	CBitmap bmp, *pOldBitmap;
//DEL 	bmp.CreateCompatibleBitmap(&dc, rScreen.Width(), rScreen.Height());
//DEL 	pOldBitmap = MemDC.SelectObject(&bmp);
//DEL 	MemDC.SetBkMode(TRANSPARENT);
//DEL 	// 화면 전체에 투명 컬러를 칠한다
//DEL 	MemDC.FillRect(rScreen, &CBrush(dfCOLOR_TRANSPARENT));
//DEL 
//DEL 	// 다이얼로그 모든 항목 그린다
//DEL 	DrawAllDialogItem((CPaintDC*)&MemDC);
//DEL 
//DEL 	// CPaintDC에 메모리 버퍼를 복사
//DEL 	dc.BitBlt(rScreen.left,rScreen.top,rScreen.Width(),rScreen.Height(), &MemDC, 0,0,SRCCOPY);
//DEL 	MemDC.SelectObject(pOldBitmap);
//DEL 	MemDC.DeleteDC();*/
//DEL 
//DEL }

// 모든 컨트롤 정보를 INI 파일에 저장
void CThemeDialog::SaveAllCtrlInfoToIni()
{
	int i=0, nCtrlCount=0;
	CString strIniCtrlName, strItemName;
	CRect rCtrl, rDialog;

	// 자신의 위치좌표
	this->GetWindowRect(&rDialog);

	// 다이얼로그 위치정보를 INI파일에 저장
	SaveDlgPosToIni();


	// 'Group' 컨트롤 개수
	nCtrlCount = m_arrThemeGroup.GetSize();

	// 'Group' 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemeGroup)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		(m_arrThemeGroup)[i]->GetWindowRect(&rCtrl);
		ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}


	// 'Static' 컨트롤 개수
	nCtrlCount = m_arrThemeStatic.GetSize();

	// 'Static' 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemeStatic)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		(m_arrThemeStatic)[i]->GetWindowRect(&rCtrl);
		ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}


	// 'Button' 컨트롤 개수
	nCtrlCount = m_arrThemeButton.GetSize();

	// 'Button' 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemeButton)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		(m_arrThemeButton)[i]->GetWindowRect(&rCtrl);
		ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}


	// 'Check' 컨트롤 개수
	nCtrlCount = m_arrThemeCheck.GetSize();

	// 'Check' 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemeCheck)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		(m_arrThemeCheck)[i]->GetWindowRect(&rCtrl);
		ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}


	// 'Radio' 컨트롤 개수
	nCtrlCount = m_arrThemeRadio.GetSize();

	// 'Radio' 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemeRadio)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		(m_arrThemeRadio)[i]->GetWindowRect(&rCtrl);
		ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}


	// 'Edit' 컨트롤 개수
	nCtrlCount = m_arrThemeEdit.GetSize();

	// 'Edit' 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemeEdit)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		(m_arrThemeEdit)[i]->GetWindowRect(&rCtrl);
		ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}


	// 'ListBox' 컨트롤 개수
	nCtrlCount = m_arrThemeListBox.GetSize();

	// 'ListBox' 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemeListBox)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		(m_arrThemeListBox)[i]->GetWindowRect(&rCtrl);
		ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}


	// 'Combo' 컨트롤 개수
	nCtrlCount = m_arrThemeCombo.GetSize();

	// 'Combo' 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemeCombo)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		(m_arrThemeCombo)[i]->GetWindowRect(&rCtrl);
		ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}


	// 'Progress' 컨트롤 개수
	nCtrlCount = m_arrThemeProgress.GetSize();

	// 'Progress' 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemeProgress)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		(m_arrThemeProgress)[i]->GetWindowRect(&rCtrl);
		ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}


	// 'ListCtrl' 컨트롤 개수
	nCtrlCount = m_arrThemeListCtrl.GetSize();

	// 'ListCtrl' 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemeListCtrl)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		(m_arrThemeListCtrl)[i]->GetWindowRect(&rCtrl);
		ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}


	// 'Slider' 컨트롤 개수
	nCtrlCount = m_arrThemeSlider.GetSize();

	// 'Slider' 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemeSlider)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		(m_arrThemeSlider)[i]->GetWindowRect(&rCtrl);
		ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}


	// 'Scroll' 컨트롤 개수
	nCtrlCount = m_arrThemeScroll.GetSize();

	// 'Scroll' 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemeScroll)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		(m_arrThemeScroll)[i]->GetWindowRect(&rCtrl);
		ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}


	// 'Picture' 컨트롤 개수
	nCtrlCount = m_arrThemePicture.GetSize();

	// 'Picture' 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemePicture)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		(m_arrThemePicture)[i]->GetWindowRect(&rCtrl);
		ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}
	

	// 'Grid' 컨트롤 개수
	nCtrlCount = m_arrThemeGrid.GetSize();

	// 'Grid' 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemeGrid)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		(m_arrThemeGrid)[i]->GetWindowRect(&rCtrl);
		ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}
	

	DWORD dwStyle=0;
	BOOL bFlag=FALSE;

	// Theme Dialog 개수
	nCtrlCount = m_arrThemeDialog.GetSize();

	// Theme Dialog 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤명울 구한다
		strIniCtrlName = (m_arrThemeDialog)[i]->GetIniCtrlName();
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		if( (m_arrThemeDialog)[i]->GetSafeHwnd() )
			(m_arrThemeDialog)[i]->GetWindowRect(&rCtrl);
		else
			rCtrl = CRect(10,10,100,100);

		// 차일드 형식 여부를 구한다
		bFlag = (m_arrThemeDialog)[i]->GetChild();
		// Child 형식이라면 절대좌표를 상대좌표로 변경
		if( bFlag )
			ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}
	

	// 비 Theme 컨트롤 개수
	nCtrlCount = m_arrNoneThemeCtrl.GetSize();

	// 비 Theme 컨트롤 정보를 하나씩 추출하여 INI 파일에 저장
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 INI 파일 조회용 컨트롤 ID를 구한다
		strIniCtrlName = (m_arrNoneThemeCtrl)[i].strCtrlID;
		// 컨트롤명에 위치정보 항목명을 추가
		strItemName = strIniCtrlName + TEXT("(Pos)");
		// 컨트롤 위치 좌표를 구한다
		if( (m_arrNoneThemeCtrl)[i].pCtrl->GetSafeHwnd() )
			(m_arrNoneThemeCtrl)[i].pCtrl->GetWindowRect(&rCtrl);
		else
			rCtrl = (m_arrNoneThemeCtrl)[i].rCtrlInit;

		// 컨트롤 Style 정보를 구한다
		dwStyle = (m_arrNoneThemeCtrl)[i].dwStyle;
		// Child 형식이라면 절대좌표를 상대좌표로 변경
		if( (dwStyle&WS_CHILD) == WS_CHILD )
			ScreenToClient(&rCtrl);

		// 컨트롤 INI 파일에 컨트롤 위치 정보를 저장
		SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strItemName, rCtrl);
	}
	
}

// 다이얼로그 위치정보를 INI파일에 저장
void CThemeDialog::SaveDlgPosToIni()
{
	CRect rCtrl;

	// 다이얼로그 위치 좌표를 구한다
	this->GetWindowRect(&rCtrl);
	// 컨트롤 INI 파일에 다이얼로그 위치 정보를 저장
	SetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, TEXT("Dlg(Pos)"), rCtrl);

}

// 마우스 포인터 이동시 실행되는 함수
void CThemeDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPoint poMouse;
	int nResizingDirection=0;

	// 상대 좌표를 절대좌표로 변경하는 함수
	poMouse = Point2Absolute(point);
				
	// 마우스 왼쪽 버튼 클릭된 상태로 이동한다면
	if( (nFlags & MK_LBUTTON) == MK_LBUTTON )
	{
		// 현재 Resizing 상태이고
		// 마우스 포인터가 가장자리 영역에 있고 최대화 상태가 아니라면
		if( m_stThemeInfo.bStyleResize == TRUE && m_stThemeInfo.nResizingDirection != dfRESIZE_DIRECTION_NONE && !this->IsZoomed())
		{
			// 마우스 포인터 모양을 방향에 맞추어 변경하는 함수
			//ChangeMousePointImage(m_stThemeInfo.nResizingDirection);
			
			// 방향에 따라서 윈도우 크기를 변경하는 함수
			//ResizeWndByDirection(m_stThemeInfo.nResizingDirection, m_stThemeInfo.poMouse, poMouse);

			// 초기 위치 정보에 따라서 모든 컨트롤 위치를 재배치한다
			//RepositionAllCtrlByInitPos();

			// 특정 윈도우에서 마우스 드래그시 실행되는 함수
			m_pDlgResizeWnd->OnTargetWindowMouseDrag(this, poMouse);

			// 마우스 포인터 위치좌표를 저장
			m_stThemeInfo.poMouse = poMouse;
		}
	}
	// 마우스 왼쪽 버튼이 클릭되지 않고
	// 마우스 포인터가 가장자리 영역에 위치하고
	// 현재 Resizing 상태이고 최대화 상태가 아니라면
	else if( (nResizingDirection=IsMouseInEdgeRange(point)) != 0 && m_stThemeInfo.bStyleResize == TRUE && !this->IsZoomed() )
	{
		// 마우스 포인터 모양을 방향에 맞추어 변경하는 함수
		ChangeMousePointImage(nResizingDirection);
	}

	// 마우스 Hover인지 체크
	//CheckHover(point);
	
	CDialog::OnMouseMove(nFlags, point);
}

// 마우스 왼쪽 버튼 클릭 다운시 실행되는 함수
void CThemeDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPoint poMouse;

	// 상대 좌표를 절대좌표로 변경하는 함수
	poMouse = Point2Absolute(point);

	// 마우스 포인터가 가장자리 영역에 위치하는지 판단
	m_stThemeInfo.nResizingDirection=IsMouseInEdgeRange(point);

	// 마우스 포인터가 가장자리 영역에 위치하지 않고
	// 마우스 드래그로 이동한다면
	if( m_stThemeInfo.bMouseDragMove && m_stThemeInfo.nResizingDirection == dfRESIZE_DIRECTION_NONE )
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));

	// 마우스 포인터가 가장자리 영역에 위치하고
	// Resize 상태이고 최대화 상태가 아니라면
	if( m_stThemeInfo.nResizingDirection != dfRESIZE_DIRECTION_NONE && m_stThemeInfo.bStyleResize == TRUE && !this->IsZoomed())
	{
		// Resize 상태 설정
		//m_stThemeInfo.bResizeState = TRUE;
		// 마우스 이벤트를 받기위해 호출
		SetCapture();

		// 마우스 포인터 모양을 방향에 맞추어 변경하는 함수
		ChangeMousePointImage(m_stThemeInfo.nResizingDirection);

		// 마우스 포인터 위치를 전역변수에 저장
		m_stThemeInfo.poMouse = poMouse;

		// 특정 윈도우에서 마우스 왼쪽 버튼 클릭시 실행되는 함수
		m_pDlgResizeWnd->OnTargetWindowLButtonDown(this, poMouse);
		
		// Resize 하기전에 방향에 따라 윈도우 크기를 변경
		//ResizeWndInit(m_stThemeInfo.nResizingDirection);
	}

	// 포커스 해제 상태라면
	if( !m_stThemeInfo.bFocus )
	{
		m_stThemeInfo.bFocus = TRUE;
		// 타이틀바 영역을 새로 그리는 함수
		InvalidateTitleBar();
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

// 마우스 왼쪽 버튼 클릭 해제시 실행되는 함수
void CThemeDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect rScreen;
	CPoint poMouse;

	// 상대 좌표를 절대좌표로 변경하는 함수
	poMouse = Point2Absolute(point);

	// 마우스 포인터가 가장자리 영역에 위치하고 
	// Resize 상태라면
	if( m_stThemeInfo.nResizingDirection != dfRESIZE_DIRECTION_NONE 
		&& m_stThemeInfo.bStyleResize )
	{
		// Resize 상태 해제
		//if( m_stThemeInfo.bResizeState )
		//	m_stThemeInfo.bResizeState = FALSE;
		// 마우스 이벤트 가로채기 해제
		ReleaseCapture();

		// Resize 방향 해제
		m_stThemeInfo.nResizingDirection = dfRESIZE_DIRECTION_NONE;

		// 마우스 포인터 모양을 방향에 맞추어 변경하는 함수
		ChangeMousePointImage(m_stThemeInfo.nResizingDirection);

		// 특정 윈도우 마우스 왼쪽 버튼 클릭 해제 되었을때 실행되는 함수
		rScreen = m_pDlgResizeWnd->OnTargetWindowLButtonUp(this, poMouse);
		
		// 윈도우 크기가 범위를 벗어난다면 함수 탈출
		if( rScreen.Width() < m_stThemeInfo.nMinWidth )
			rScreen.right = rScreen.left + m_stThemeInfo.nMinWidth;
		if( rScreen.Height() < m_stThemeInfo.nMinHeight )
			rScreen.bottom = rScreen.top + m_stThemeInfo.nMinHeight;
		if( rScreen.Width() > m_stThemeInfo.nMaxWidth )
			rScreen.right = rScreen.left + m_stThemeInfo.nMaxWidth;
		if( rScreen.Height() > m_stThemeInfo.nMaxHeight )
			rScreen.bottom = rScreen.top + m_stThemeInfo.nMaxHeight;

		// 다이얼로그 위치를 이동시킨다
		this->MoveWindow(rScreen);
		
		// 화면을 새로 그린다
		//Invalidate(TRUE);
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

// Image 파일 폴더 경로 지정
void CThemeDialog::SetImageFilePath(CString strFilePath)
{
	m_stThemeInfo.strImageFilePath = strFilePath;

	if( m_stThemeInfo.strImageFilePath.Right(1) != "\\" )
		m_stThemeInfo.strImageFilePath += "\\";
}

// Config 파일 폴더 경로 지정
void CThemeDialog::SetConfigFilePath(CString strFilePath)
{
	m_stThemeInfo.strConfigFilePath = strFilePath;
	
	if( m_stThemeInfo.strConfigFilePath.Right(1) != "\\" )
		m_stThemeInfo.strConfigFilePath += "\\";
}

// 테마 폴더 이름 목록을 반환
CString CThemeDialog::GetThemeFolderList()
{
	CString strModulePath, strPattern, strThemeFolderList;

	// 실행파일이 존재하는 폴더 경로를 반환
	strModulePath = gf_GetModulePath();
	if( strModulePath.GetLength() < 1 )
		return TEXT("");
	//if( strModulePath.Right(1) == "\\" || strModulePath.Right(1) == "/" )
	//	strModulePath = strModulePath.Left(strModulePath.GetLength()-1);
	strPattern = strModulePath + TEXT("*.*");

	CString strFileName, strFolderPath;
	HANDLE hFile;
	WIN32_FIND_DATA FileInformation;

	hFile = ::FindFirstFile(strPattern, &FileInformation);

	if(hFile != INVALID_HANDLE_VALUE) {
		do {
			strFileName = FileInformation.cFileName;

			// 폴더명이라면 테마폴더인지를 판단한다
			if( strFileName.Find(TEXT(".")) < 0 )
			{
				strFolderPath = strModulePath + strFileName + TEXT("\\");
				// 특정 폴더에 해당 파일이 존재하는지 검사해서 반환
				if( gf_FindFileInFolder(dfINI_POSITION_FILE_NAME, strFolderPath) )
				{
					// 테마 폴더 목록에 새 폴더명을 추가
					strThemeFolderList += (strFileName + TEXT("\n"));
				}
			}
			
		} while(::FindNextFile(hFile, &FileInformation) == TRUE);
		::FindClose(hFile);
	}

	if( strThemeFolderList.GetLength() < 1 )
		return TEXT("");
	if( strThemeFolderList.Right(1) == "\n" )
		strThemeFolderList = strThemeFolderList.Left(strThemeFolderList.GetLength() - 1);

	// 테마 폴더 리스트를 반환
	return strThemeFolderList;
}

// 비 Theme 컨트롤을 추가
void CThemeDialog::AddNoneThemeCtrl(CWnd *pCtrl, CString strIniCtrlName/*=""*/)
{
	ST_NONE_THEME_CTRL stNoneThemeCtrl;
	CString strTypeName;
	CRect rCtrl=CRect(0,0,0,0);

	// 컨트롤이 생성되지 않았다면 함수 탈출
	//if( !pCtrl->GetSafeHwnd() )
	//	return;

	// 컨트롤 포인터 주소
	stNoneThemeCtrl.pCtrl = pCtrl;
	// INI 조회용 컨트롤 ID
	stNoneThemeCtrl.strCtrlID = strIniCtrlName;
	stNoneThemeCtrl.rCtrlInit = rCtrl;

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniCtrlName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		/*CString strMessage;
		strMessage.Format("%d, %d, %d, %d", rCtrl.left, rCtrl.top, rCtrl.right, rCtrl.bottom);
		MessageBox(strMessage);*/

		stNoneThemeCtrl.rCtrlInit = rCtrl;
	}
	// INI 파일에 컨트롤 위치 정보가 존재하지 않는다면
	else
	{
		pCtrl->GetWindowRect(&rCtrl);
		stNoneThemeCtrl.rCtrlInit = rCtrl;
	}

	// 컨트롤 Style 정보를 구한다
	stNoneThemeCtrl.dwStyle = pCtrl->GetStyle();

	// 비 Theme 컨트롤 정보를 Array에 추가
	m_arrNoneThemeCtrl.Add(stNoneThemeCtrl);
}

// 초기 위치 정보에 따라서 모든 컨트롤 위치를 재배치한다
void CThemeDialog::RepositionAllCtrlByInitPos()
{
	int i=0, nCtrlCount=0;
	CRect rInitPos, rNewPos, rDialog;

	// 자신의 위치좌표
	this->GetWindowRect(&rDialog);
	

	// 'Static' 컨트롤 개수
	nCtrlCount = m_arrThemeStatic.GetSize();

	// 'Static' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeStatic)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemeStatic)[i]->MoveWindow(rNewPos);
	}


	// 'Group' 컨트롤 개수
	nCtrlCount = m_arrThemeGroup.GetSize();

	// 'Group' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeGroup)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemeGroup)[i]->MoveWindow(rNewPos);
	}


	// 'Button' 컨트롤 개수
	nCtrlCount = m_arrThemeButton.GetSize();

	// 'Button' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeButton)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemeButton)[i]->MoveWindow(rNewPos);
	}


	// 'Check' 컨트롤 개수
	nCtrlCount = m_arrThemeCheck.GetSize();

	// 'Check' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeCheck)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemeCheck)[i]->MoveWindow(rNewPos);
	}


	// 'Radio' 컨트롤 개수
	nCtrlCount = m_arrThemeRadio.GetSize();

	// 'Radio' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeRadio)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemeRadio)[i]->MoveWindow(rNewPos);
	}


	// 'Edit' 컨트롤 개수
	nCtrlCount = m_arrThemeEdit.GetSize();

	// 'Edit' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeEdit)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemeEdit)[i]->MoveWindow(rNewPos);
	}


	// 'ListBox' 컨트롤 개수
	nCtrlCount = m_arrThemeListBox.GetSize();

	// 'ListBox' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeListBox)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemeListBox)[i]->MoveWindow(rNewPos);
	}


	// 'Combo' 컨트롤 개수
	nCtrlCount = m_arrThemeCombo.GetSize();

	// 'Combo' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeCombo)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemeCombo)[i]->MoveWindow(rNewPos);
	}


	// 'Progress' 컨트롤 개수
	nCtrlCount = m_arrThemeProgress.GetSize();

	// 'Progress' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeProgress)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemeProgress)[i]->MoveWindow(rNewPos);
	}


	// 'ListCtrl' 컨트롤 개수
	nCtrlCount = m_arrThemeListCtrl.GetSize();

	// 'ListCtrl' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeListCtrl)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemeListCtrl)[i]->MoveWindow(rNewPos);
	}


	// 'Slider' 컨트롤 개수
	nCtrlCount = m_arrThemeSlider.GetSize();

	// 'Slider' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeSlider)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemeSlider)[i]->MoveWindow(rNewPos);
	}


	// 'Scroll' 컨트롤 개수
	nCtrlCount = m_arrThemeScroll.GetSize();

	// 'Scroll' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeScroll)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemeScroll)[i]->MoveWindow(rNewPos);
	}


	// 'Picture' 컨트롤 개수
	nCtrlCount = m_arrThemePicture.GetSize();

	// 'Picture' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemePicture)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemePicture)[i]->MoveWindow(rNewPos);
	}


	// 'Grid' 컨트롤 개수
	nCtrlCount = m_arrThemeGrid.GetSize();

	// 'Grid' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeGrid)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemeGrid)[i]->MoveWindow(rNewPos);
	}


	// 'Menu' 컨트롤 개수
	nCtrlCount = m_arrThemeMenu.GetSize();

	// 'Menu' 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeMenu)[i]->GetInitPos();

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		// 컨트롤을 새로운 위치로 변경
		(m_arrThemeMenu)[i]->MoveWindow(rNewPos);
	}

	
	// 초기 위치 정보에 따라서 Theme 다이얼로그 위치를 재배치한다
	RepositionThemeDialog();


	// 초기 위치 정보에 따라서 비 Theme 컨트롤 위치를 재배치한다
	RepositionNoneThemeCtrl();

}

// Theme 다이얼로그를 추가
void CThemeDialog::AddThemeDialog(CThemeDialog *pDlg, CString strIniCtrlName/*=""*/)
{
	CString strTypeName;
	CRect rCtrl;

	// 다이얼로그가 생성되지 않았다면 함수 탈출
	if( !pDlg->GetSafeHwnd() )
		return;

	// INI 파일 조회용 컨트롤 ID를 저장
	pDlg->SetIniCtrlName(strIniCtrlName);

	// INI 파일에서 컨트롤 위치 정보를 구한다
	strTypeName = strIniCtrlName + TEXT("(Pos)");
	if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
	{
		// 초기 위치 정보 저장
		pDlg->SetInitPos(rCtrl);
	}
	// INI 파일에 컨트롤 위치 정보가 존재하지 않는다면
	else
	{
		// 현재 위치좌표를 구해서 초기 위치 정보 저장
		pDlg->GetWindowRect(&rCtrl);
		pDlg->SetInitPos(rCtrl);
	}

	// Theme 다이얼로그를 Array에 추가
	m_arrThemeDialog.Add(pDlg);
}

// 초기 위치 정보에 따라서 비 Theme 컨트롤 위치를 재배치한다
void CThemeDialog::RepositionNoneThemeCtrl()
{
	int i=0, nCtrlCount;
	CRect rInitPos, rNewPos;
	DWORD dwStyle=0;

	// 비 Theme 컨트롤 개수
	nCtrlCount = m_arrNoneThemeCtrl.GetSize();

	// 비 Theme 컨트롤 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 컨트롤 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrNoneThemeCtrl)[i].rCtrlInit;

		// 상대좌표를 사용하지 않았다면 건너뛴다
		if( IsAbsoluteArea(rInitPos) )
			continue;
		
		CRect rDlg;
		this->GetClientRect(&rDlg);

		// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);

		/*// 컨트롤 스타일 정보
		dwStyle = (m_arrNoneThemeCtrl)[i].dwStyle;
		// 컨트롤이 Child 형식이 아니라면
		if( (dwStyle&WS_CHILD) != WS_CHILD )
		{
			// 컨트롤 위치좌표에 부모 다이얼로그 위좌표를 추가
			ClientToScreen(&rNewPos);
		}*/
		
		// 컨트롤을 새로운 위치로 변경
		(m_arrNoneThemeCtrl)[i].pCtrl->MoveWindow(rNewPos);
	}
}

// 다이얼로그 이동시에 실행되는 함수
void CThemeDialog::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
		
	// 초기 위치 정보에 따라서 Theme 다이얼로그 위치를 재배치한다
	RepositionThemeDialog();

	// 초기 위치 정보에 따라서 비 Theme 컨트롤 위치를 재배치한다
	RepositionNoneThemeCtrl();

}

// 초기 위치 정보에 따라서 Theme 다이얼로그 위치를 재배치한다
void CThemeDialog::RepositionThemeDialog()
{
	int i=0, nCtrlCount;
	CRect rInitPos, rNewPos;
	BOOL bChild=FALSE;

	// Theme 다이얼로그 개수
	nCtrlCount = m_arrThemeDialog.GetSize();

	// Theme 다이얼로그 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 차일드 형식 여부
		bChild = (m_arrThemeDialog)[i]->GetChild();
		// 다이얼로그가 Child 형식이 아니라면 건너뛴다
		if( !bChild )
			continue;
		
		// 다이얼로그 변수에서 초기 위치 정보를 구한다
		rInitPos = (m_arrThemeDialog)[i]->GetInitPos();

		// 다이얼로그의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환
		rNewPos = gf_TransCtrlPosToAbsolute(rInitPos, this);
	
		// 다이얼로그 위치좌표에 부모 다이얼로그 위치좌표를 추가
		ClientToScreen(&rNewPos);
		
		// 다이얼로그를 새로운 위치로 변경
		(m_arrThemeDialog)[i]->MoveWindow(rNewPos);
	}
}

// 테마 폴더 경로 설정
void CThemeDialog::SetThemePath(CString strThemePath)
{
	// 테마 폴더 경로 설정
	gf_SetThemePath(strThemePath);

	// Image 파일 폴더 경로 지정
	this->SetImageFilePath(g_strThemePath);

	// Config 파일 폴더 경로 지정
	this->SetConfigFilePath(g_strThemePath);

	int i=0, nCtrlCount;
	// Theme 다이얼로그 개수
	nCtrlCount = m_arrThemeDialog.GetSize();

	// Theme 다이얼로그 위치 정보를 하나씩 추출
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// Image 파일 폴더 경로 지정
		(m_arrThemeDialog)[i]->SetImageFilePath(g_strThemePath);
		// Config 파일 폴더 경로 지정
		(m_arrThemeDialog)[i]->SetConfigFilePath(g_strThemePath);
	}
}

// 배경 이미지 설정
BOOL CThemeDialog::SetBackImage(CString strFileName)
{
	// 배경 이미지를 여는 함수(포커스 설정-파일)
	return SetBackImageOnFocus(strFileName);
}

// 배경 이미지 설정
BOOL CThemeDialog::SetBackImage(UINT nImageID)
{
	// 배경 이미지를 여는 함수(포커스 설정-Resource ID)
	return SetBackImageOnFocus(nImageID);
}

// 클래스 생성 전에 실행되는 함수
BOOL CThemeDialog::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Child 속성이 존재한다면
	if( (cs.style & WS_CHILD) == WS_CHILD )
	{
		cs.style -= WS_CHILD;
		m_stThemeInfo.bChild = TRUE;
	}
	
	return CDialog::PreCreateWindow(cs);
}

// Theme Menu 컨트롤을 추가
CThemeMenu* CThemeDialog::AddThemeMenuCtrl(UINT nCtrlID, UINT nMenuID, int nSubMenuNo, CString strIniCtrlName/*=""*/)
{
	CThemeMenu* pCtrl;

	// Theme Menu 컨트롤을 추가
	pCtrl = (CThemeMenu*)AddThemeMenuCtrl(nCtrlID, strIniCtrlName);

	// 메뉴 로딩
	pCtrl->LoadMenu(nMenuID);
	// 서브 메뉴 번호 설정
	pCtrl->SetSubMenuNumber(nSubMenuNo);

	return pCtrl;
}

// Theme Menu 컨트롤을 추가
CThemeMenu* CThemeDialog::AddThemeMenuCtrl(UINT nMenuID, int nSubMenuNo, CString strIniCtrlName/*=""*/)
{
	// Theme Menu 컨트롤을 추가
	return AddThemeMenuCtrl(IDC_STATIC, nMenuID, nSubMenuNo, strIniCtrlName);
}

// 투명으로 처리할 컬러 설정
void CThemeDialog::SetTransparentColor(COLORREF crTransparent)
{
	m_stThemeInfo.crTransparent = crTransparent;

	// 컬러로 투명 처리 여부 설정
	SetTransparentByColor(TRUE);
}

// 컬러로 투명 처리 여부 설정
void CThemeDialog::SetTransparentByColor(BOOL bFlag)
{
	m_stThemeInfo.bTransparentByColor = bFlag;

	if( m_stThemeInfo.bTransparentByColor )
		// 배경을 투명으로 설정(투명 컬러 지정)
		SetTransparentBack(m_stThemeInfo.crTransparent);
}

// 배경을 투명으로 설정(투명 컬러 지정)
BOOL CThemeDialog::SetTransparentBack(COLORREF crTrans)
{
	//return TRUE;

	HINSTANCE hUserLib32 = ::GetModuleHandle(TEXT("USER32.DLL"));

	if (!hUserLib32)
		return FALSE;

	SLWA pSetLWA = (SLWA)GetProcAddress(hUserLib32, "SetLayeredWindowAttributes");

	if (!pSetLWA)
		return FALSE;

	HWND hWnd;
	hWnd = this->GetSafeHwnd();
	::SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	// 반투명이 적용되어 있을때
	if( m_stThemeInfo.byteTransparent < 255 )
		pSetLWA(hWnd, crTrans, m_stThemeInfo.byteTransparent, LWA_COLORKEY|LWA_ALPHA);
	else
		pSetLWA(hWnd, crTrans, 0, LWA_COLORKEY);

	return TRUE;
}

// 반투명 설정
BOOL CThemeDialog::SetTransparent(BYTE byteDegree)
{
	if (byteDegree <= 0)
		return FALSE;

	// 투명도 저장
	m_stThemeInfo.byteTransparent = byteDegree;

	HINSTANCE hUserLib32 = ::GetModuleHandle(TEXT("USER32.DLL"));

	if (!hUserLib32)
		return FALSE;

	SLWA pSetLWA = (SLWA)GetProcAddress(hUserLib32, "SetLayeredWindowAttributes");

	if (!pSetLWA)
		return FALSE;

	HWND hWnd;
	hWnd = this->GetSafeHwnd();
	::SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	// 컬러에 의해 배경 투명일때
	if( m_stThemeInfo.bTransparentByColor )
		pSetLWA(hWnd, m_stThemeInfo.crTransparent, byteDegree, LWA_COLORKEY|LWA_ALPHA);
	else
		pSetLWA(hWnd, 0, byteDegree, LWA_ALPHA);

	return TRUE;
}

// INI 파일의 테마 정보로 모든 컨트롤에 스타일 적용
void CThemeDialog::SetStyleAllCtrl()
{
	int i=0, nCtrlCount=0;
	CString strIniCtrlName;

	// 테마 관련 변수 초기화
	InitThemeVariable();

	// Static 컨트롤 개수
	nCtrlCount = m_arrThemeStatic.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemeStatic)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemeStatic)[i]->GetIniCtrlName();

		// Static 컨트롤에 Style 설정
		SetStyleStaticCtrl((m_arrThemeStatic)[i], strIniCtrlName);
	}

	// Group 컨트롤 개수
	nCtrlCount = m_arrThemeGroup.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemeGroup)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemeGroup)[i]->GetIniCtrlName();

		// Group 컨트롤에 Style 설정
		SetStyleGroupCtrl((m_arrThemeGroup)[i], strIniCtrlName);
	}

	// Button 컨트롤 개수
	nCtrlCount = m_arrThemeButton.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemeButton)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemeButton)[i]->GetIniCtrlName();

		// Button 컨트롤에 Style 설정
		SetStyleButtonCtrl((m_arrThemeButton)[i], strIniCtrlName);
	}

	// Check 컨트롤 개수
	nCtrlCount = m_arrThemeCheck.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemeCheck)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemeCheck)[i]->GetIniCtrlName();

		// Check 컨트롤에 Style 설정
		SetStyleCheckCtrl((m_arrThemeCheck)[i], strIniCtrlName);
	}

	// Radio 컨트롤 개수
	nCtrlCount = m_arrThemeRadio.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemeRadio)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemeRadio)[i]->GetIniCtrlName();

		// Radio 컨트롤에 Style 설정
		SetStyleRadioCtrl((m_arrThemeRadio)[i], strIniCtrlName);
	}

	// Edit 컨트롤 개수
	nCtrlCount = m_arrThemeEdit.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemeEdit)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemeEdit)[i]->GetIniCtrlName();

		// Edit 컨트롤에 Style 설정
		SetStyleEditCtrl((m_arrThemeEdit)[i], strIniCtrlName);
	}

	// ListBox 컨트롤 개수
	nCtrlCount = m_arrThemeListBox.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemeListBox)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemeListBox)[i]->GetIniCtrlName();

		// ListBox 컨트롤에 Style 설정
		SetStyleListBoxCtrl((m_arrThemeListBox)[i], strIniCtrlName);
	}

	// Combo 컨트롤 개수
	nCtrlCount = m_arrThemeCombo.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemeCombo)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemeCombo)[i]->GetIniCtrlName();

		// Combo 컨트롤에 Style 설정
		SetStyleComboCtrl((m_arrThemeCombo)[i], strIniCtrlName);
	}

	// Progress 컨트롤 개수
	nCtrlCount = m_arrThemeProgress.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemeProgress)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemeProgress)[i]->GetIniCtrlName();

		// Progress 컨트롤에 Style 설정
		SetStyleProgressCtrl((m_arrThemeProgress)[i], strIniCtrlName);
	}

	// ListCtrl 컨트롤 개수
	nCtrlCount = m_arrThemeListCtrl.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemeListCtrl)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemeListCtrl)[i]->GetIniCtrlName();

		// ListCtrl 컨트롤에 Style 설정
		SetStyleListCtrl((m_arrThemeListCtrl)[i], strIniCtrlName);
	}

	// Slider 컨트롤 개수
	nCtrlCount = m_arrThemeSlider.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemeSlider)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemeSlider)[i]->GetIniCtrlName();

		// Slider 컨트롤에 Style 설정
		SetStyleSliderCtrl((m_arrThemeSlider)[i], strIniCtrlName);
	}

	// Scroll 컨트롤 개수
	nCtrlCount = m_arrThemeScroll.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemeScroll)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemeScroll)[i]->GetIniCtrlName();

		// Scroll 컨트롤에 Style 설정
		SetStyleScrollCtrl((m_arrThemeScroll)[i], strIniCtrlName);
	}

	// Picture 컨트롤 개수
	nCtrlCount = m_arrThemePicture.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemePicture)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemePicture)[i]->GetIniCtrlName();

		// Picture 컨트롤에 Style 설정
		SetStylePictureCtrl((m_arrThemePicture)[i], strIniCtrlName);
	}

	// Grid 컨트롤 개수
	nCtrlCount = m_arrThemeGrid.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemeGrid)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemeGrid)[i]->GetIniCtrlName();

		// Grid 컨트롤에 Style 설정
		SetStyleGridCtrl((m_arrThemeGrid)[i], strIniCtrlName);
	}

	// Menu 컨트롤 개수
	nCtrlCount = m_arrThemeMenu.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// 전역 변수 초기화
		(m_arrThemeMenu)[i]->InitThemeVariable();

		// INI 컨트롤명을 구한다
		strIniCtrlName = (m_arrThemeMenu)[i]->GetIniCtrlName();

		// Menu 컨트롤에 Style 설정
		SetStyleMenuCtrl((m_arrThemeMenu)[i], strIniCtrlName);
	}

	CString strTypeName;
	CRect rCtrl;

	// Theme 다이얼로그 개수
	nCtrlCount = m_arrThemeDialog.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// INI 컨트롤 ID를 구한다
		strIniCtrlName = (m_arrThemeDialog)[i]->GetIniCtrlName();

		// INI 파일에서 컨트롤 위치 정보를 구한다
		strTypeName = strIniCtrlName + TEXT("(Pos)");
		if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
		{
			// 초기 위치 정보 저장
			(m_arrThemeDialog)[i]->SetInitPos(rCtrl);
		}

		// INI 파일의 테마 정보로 모든 컨트롤에 스타일 적용
		(m_arrThemeDialog)[i]->SetStyleAllCtrl();
	}

	// 비 Theme 컨트롤 개수
	nCtrlCount = m_arrNoneThemeCtrl.GetSize();
	for(i=0 ; i < nCtrlCount ; i++)
	{
		// INI 컨트롤 ID를 구한다
		strIniCtrlName = (m_arrNoneThemeCtrl)[i].strCtrlID;

		// INI 파일에서 컨트롤 위치 정보를 구한다
		strTypeName = strIniCtrlName + TEXT("(Pos)");
		if( GetCtrlPosFromIniFile(m_stThemeInfo.strThemeGroupName, strTypeName, rCtrl) )
		{
			// 초기 위치 정보 저장
			(m_arrNoneThemeCtrl)[i].rCtrlInit = rCtrl;
		}
	}

	// INI 파일의 정보로 다이얼로그에 테마를 적용
	SetStyleDlgFromIniFile();

	// 초기화를 위한 타이머 발생
	SetTimer(dfTIMER_INITIALIZE, 10, NULL);
}
