// ThemeGroup.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "ThemeGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThemeGroup

CThemeGroup::CThemeGroup()
{
	m_stThemeInfo.strIniCtrlName = "";							// INI 파일 조회용 컨트롤명
	m_stThemeInfo.strCaption = "";								// Caption 문자열

	// 테마 관련 변수 초기화
	InitThemeVariable();

}

// 테마 관련 변수 초기화
void CThemeGroup::InitThemeVariable()
{
	// 배경 그리기 방식이 이미지라면 기존 이미지를 삭제
	if( m_stThemeInfo.nBackDrawType == dfBACK_DRAW_TYPE_IMAGE )
		m_stThemeInfo.bmpBack.DeleteObject();

	// 테마 옵션 초기화
	m_stThemeInfo.rCaption = CRect(5,0,170,20);							// Caption 영역좌표
	m_stThemeInfo.nFontSize = 12;										// 폰트 크기
	m_stThemeInfo.bFontBold = FALSE;									// 폰트 두께
	m_stThemeInfo.crFont = RGB(0,0,0);									// 폰트 컬러
	m_stThemeInfo.crShadow = dfCOLOR_TRANSPARENT;						// 그림자 컬러
	m_stThemeInfo.nAlignFormat = DT_LEFT|DT_VCENTER;					// Caption 정렬방식
	m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_COLOR;				// 배경 그리기 방식
	m_stThemeInfo.nBackImageFillType = dfBACK_IMAGE_FILL_TYPE_BLOCK;	// 배경 이미지 채우기 방식
	m_stThemeInfo.nBackImageAlignFormat = DT_LEFT|DT_VCENTER;			// 배경 이미지 정렬방식
	m_stThemeInfo.crBackground = RGB(214,225,227);						// 배경 컬러
	m_stThemeInfo.crOutlineLT = RGB(168,179,197);						// 테두리 컬러(Left-Top)
	m_stThemeInfo.crOutlineRB = RGB(98,109,127);						// 테두리 컬러(Right-Bottom)
	m_stThemeInfo.nOutlineThick = 1;									// 테두리 두께
	m_stThemeInfo.nEdgeRoundValue = 5;									// 모서리 둥근 정보
	m_stThemeInfo.rInitPos = CRect(-1,-1,-1,-1);						// 초기 위치 정보	
}

CThemeGroup::~CThemeGroup()
{
}


BEGIN_MESSAGE_MAP(CThemeGroup, CStatic)
	//{{AFX_MSG_MAP(CThemeGroup)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThemeGroup message handlers

// WM_PAINT 이벤트 발생시 실행되는 함수
void CThemeGroup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// 화면 영역좌표 구하기
	CRect rScreen;
	this->GetClientRect(&rScreen);

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
	
	// Caption 문자열을 화면에 표시한다
	DrawCaptionText(&dc);

}

// 컨트롤 화면 크기 변경시 실행되는 함수
void CThemeGroup::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
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

// 컬러로 배경을 그린다
void CThemeGroup::DrawBackByColor(CPaintDC *pdc, COLORREF crBack, CRect rScreen)
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

// Caption 문자열을 화면에 표시한다
void CThemeGroup::DrawCaptionText(CPaintDC *pdc)
{
	CRect rCaption, rShadow;

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

	// Caption 영역좌표를 절대 영역좌표로 변환
	rCaption = gf_RelativeToAbsolute(m_stThemeInfo.rCaption, this);
	if( rCaption.right <= 0 || rCaption.bottom <= 0 )
		this->GetClientRect(&rCaption);
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
		if( m_stThemeInfo.crShadow != dfCOLOR_TRANSPARENT )
		{			
			// 그림자 영역좌표를 구한다
			rShadow = rText[i];
			rShadow.left ++;
			rShadow.top ++;
			rShadow.right ++;
			rShadow.bottom ++;

			// Caption 그림자를 화면에 표시
			gf_DrawText(pdc, strText[i], m_stThemeInfo.nFontSize, 
				m_stThemeInfo.crShadow, rShadow, 
				m_stThemeInfo.nAlignFormat|DT_SINGLELINE, m_stThemeInfo.bFontBold);
		}

		// Caption 텍스트를 화면에 표시
		gf_DrawText(pdc, strText[i], m_stThemeInfo.nFontSize, 
				m_stThemeInfo.crFont, rText[i], 
				m_stThemeInfo.nAlignFormat|DT_SINGLELINE, m_stThemeInfo.bFontBold);
	}
}

// Caption 영역좌표 설정
void CThemeGroup::SetCaptionArea(CRect rCaption)
{
	// 전역변수에 저장
	m_stThemeInfo.rCaption = rCaption;
}

// Caption 정렬 방식 설정
void CThemeGroup::SetAlignFormat(UINT nAlignFormat)
{
	// 전역변수에 저장
	m_stThemeInfo.nAlignFormat = nAlignFormat;
}

// 폰트 컬러 설정
void CThemeGroup::SetFontColor(COLORREF crFont)
{
	m_stThemeInfo.crFont = crFont;
}

// 그림자 컬러 설정
void CThemeGroup::SetShadowColor(COLORREF crShadow)
{
	m_stThemeInfo.crShadow = crShadow;
}

// 폰트 컬러 설정
void CThemeGroup::SetFontColor(COLORREF crFont, COLORREF crShadow)
{
	m_stThemeInfo.crFont = crFont;
	m_stThemeInfo.crShadow = crShadow;
}

// 폰트 크기 설정
void CThemeGroup::SetFontSize(int nFontSize)
{
	m_stThemeInfo.nFontSize = nFontSize;
}

// 폰트 두께 설정
void CThemeGroup::SetFontBold(BOOL bBold)
{
	m_stThemeInfo.bFontBold = bBold;
}

// 폰트 특성 설정
void CThemeGroup::SetFont(int nFontSize, BOOL bFontBold, COLORREF crFont, COLORREF crShadow, UINT nAlignFormat)
{
	// 폰트 특성을 전역변수에 저장
	m_stThemeInfo.nFontSize = nFontSize;
	m_stThemeInfo.bFontBold = bFontBold;
	m_stThemeInfo.crFont = crFont;
	m_stThemeInfo.crShadow = crShadow;
	m_stThemeInfo.nAlignFormat = nAlignFormat;
}

// 배경 이미지를 여는 함수(파일)
BOOL CThemeGroup::SetBackImage(CString strFileName)
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

	return bBmpLoad;
}

// 배경 이미지를 여는 함수(Resource ID)
BOOL CThemeGroup::SetBackImage(UINT nImageID)
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
		m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_NONE;
	}

	return FALSE;
}

// 배경 컬러 설정
void CThemeGroup::SetBackColor(COLORREF crBack)
{
	m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_COLOR;		// 배경 그리기 방식
	m_stThemeInfo.crBackground = crBack;
}

// 배경 모서리 둥근 정도 설정
void CThemeGroup::SetEdgeRoundValue(int nRoundValue)
{
	m_stThemeInfo.nEdgeRoundValue = nRoundValue;
}

// 외곽선 두께 설정
void CThemeGroup::SetOutlineThick(int nThick)
{
	m_stThemeInfo.nOutlineThick = nThick;
}

// 외곽선(Left-Top) 컬러 설정
void CThemeGroup::SetOutlineLTColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineLT = crOutline;
}

// 외곽선(Right-Bottom) 컬러 설정
void CThemeGroup::SetOutlineRBColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineRB = crOutline;
}

// 외곽선 컬러 설정
void CThemeGroup::SetOutlineColor(COLORREF crOutline)
{
	m_stThemeInfo.crOutlineLT = crOutline;
	m_stThemeInfo.crOutlineRB = crOutline;
}

// 배경 그리기 방식 설정
void CThemeGroup::SetBackDrawType(int nDrawType)
{
	m_stThemeInfo.nBackDrawType = nDrawType;
}

// 배경 이미지 채우기 방식 설정
void CThemeGroup::SetBackImageFillType(int nImageFillType)
{
	m_stThemeInfo.nBackImageFillType = nImageFillType;
}

// 배경 이미지 정렬방식 설정
void CThemeGroup::SetBackImageAlignFormat(UINT nAlignFormat)
{
	m_stThemeInfo.nBackImageAlignFormat = nAlignFormat;
}

// 이미지를 Pattern 브러쉬 형식으로 화면 영역에 그리는 함수(바둑판 형식)
void CThemeGroup::DrawImageByPatternBrush(CPaintDC *pdc, CBitmap *pBitmap, CRect rArea)
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

// 비트맵 이미지로 배경을 그린다
void CThemeGroup::DrawBackByImage(CPaintDC *pdc, CBitmap *pBitmap, CRect rScreen)
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

// 클래스 생성시 실행되는 함수
int CThemeGroup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	/*// Caption 영역좌표 설정
	CRect rScreen;
	this->GetClientRect(&rScreen);
	m_stThemeInfo.rCaption.right = rScreen.right - m_stThemeInfo.rCaption.left;
	m_stThemeInfo.rCaption.bottom = rScreen.bottom - m_stThemeInfo.rCaption.top;*/
		
	return 0;
}

// 클래스 생성
BOOL CThemeGroup::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(TEXT("Static"), NULL, dwStyle, rect, pParentWnd, nID, pContext);
}

/*// 툴팁 문자열 설정
void CThemeGroup::SetToolTip(CString strText)
{
	// 툴팁이 이미 생성되어 있다면
	if( m_ToolTip.GetSafeHwnd() )
		// 툴팁 문자열 변경
		ChangeToolTip(strText);
	// 툴팁이 아직 생성되지 않았다면
	else
		// 툴팁 생성
		MakeToolTip(strText);
}*/

// 툴팁을 생성한다
void CThemeGroup::MakeToolTip(CString strToolTip)
{
	// 현 컨트롤 ID를 구한다
	UINT unWindowID = (UINT)this->GetSafeHwnd();

	// 툴팁 생성
	m_ToolTip.CreateToolTip(this, unWindowID, strToolTip);
}

// 툴팁 텍스트 변경
void CThemeGroup::ChangeToolTip(CString strToolTip)
{
	// 툴팁 텍스트 변경
	m_ToolTip.ChangeToolTip(strToolTip);
}

// 툴팁 삭제
void CThemeGroup::DeleteToolTip()
{
	if( !m_ToolTip.GetSafeHwnd() )
		return;

	m_ToolTip.DelTool(this);
}

// 클래스 종료시 실행되는 함수
void CThemeGroup::OnDestroy() 
{
	CStatic::OnDestroy();
		
	// 툴팁 삭제
	DeleteToolTip();
}

// 툴팁을 위한 마우스 이벤트를 받는 함수
LRESULT CThemeGroup::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
	
	return CStatic::WindowProc(message, wParam, lParam);
}

// Caption 문자열 설정
void CThemeGroup::SetWindowText(CString strText)
{
	SetCaptionText(strText);
}

// Caption 문자열 설정
void CThemeGroup::SetCaptionText(CString strCaption)
{
	// 전역변수에 저장
	m_stThemeInfo.strCaption = strCaption;
	
	// 화면을 새로 그린다
	Invalidate(FALSE);
}
