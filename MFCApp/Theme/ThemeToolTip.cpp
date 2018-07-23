// ThemeToolTip.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "ThemeToolTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThemeToolTip

CThemeToolTip::CThemeToolTip()
{
	m_strToolTipText = "";

	m_pParent = NULL;
}

// 이미지를 원본 그대로 1회 화면에 정렬해서 그리는 함수(투명 지원)
void gf_DrawImagePartTransparent(CPaintDC *pdc, CBitmap *pBitmap, CRect rImgPart, CRect rScreen, UINT unAlignFormat)
{
	CRect rImgView;
	//BITMAP bm;

	//pBitmap->GetBitmap(&bm);

	// 이미지를 그릴 화면 영역좌표
	rImgView = rImgPart;
	// 이미지 정렬 방식 - Right 일때
	if( (unAlignFormat & DT_RIGHT) == DT_RIGHT )
	{
		rImgView.right = rScreen.right;
		rImgView.left = rImgView.right - rImgPart.Width();
	}
	// 이미지 정렬 방식 - Center 일때
	else if( (unAlignFormat & DT_CENTER) == DT_CENTER )
	{
		rImgView.left = rScreen.CenterPoint().x - (rImgPart.Width()/2);
		rImgView.right = rImgView.left + rImgPart.Width();
	}
	// 이미지 정렬 방식 - Left 일때
	else if( (unAlignFormat & DT_LEFT) == DT_LEFT )
	{
		rImgView.left = 0;
		rImgView.right = rImgPart.Width();
	}
	// 이미지 정렬 방식 - Bottom 일때
	if( (unAlignFormat & DT_BOTTOM) == DT_BOTTOM )
	{
		rImgView.bottom = rScreen.bottom;
		rImgView.top = rImgView.bottom - rImgPart.Height();
	}
	// 이미지 정렬 방식 - VCenter 일때
	else if( (unAlignFormat & DT_VCENTER) == DT_VCENTER )
	{
		rImgView.top = rScreen.CenterPoint().y - (rImgPart.Height()/2);
		rImgView.bottom = rImgView.top + rImgPart.Height();
	}	
	// 이미지 정렬 방식 - Top 일때
	else if( (unAlignFormat & DT_TOP) == DT_TOP )
	{
		rImgView.top = 0;
		rImgView.bottom = rImgPart.Height();
	}
	
	// 이미지 일부분을 원래 크기 대로 그리는 함수(투명 지원)
	gf_DrawTransparentImage(pBitmap, rImgPart, rImgView, pdc);

}

CThemeToolTip::~CThemeToolTip()
{
}


BEGIN_MESSAGE_MAP(CThemeToolTip, CToolTipCtrl)
	//{{AFX_MSG_MAP(CThemeToolTip)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThemeToolTip message handlers

// 툴팁 문자열 추가
void CThemeToolTip::AddToolEx(CWnd *pWnd, CRect rTool, UINT unID, CString strToolTip)
{
	m_strToolTipText = strToolTip;
	CString sToolTip = strToolTip;
	
	// size 계산
	CDC* pDC = GetDC();
	CFont* pFontOld = pDC->SelectObject(this->GetFont());

	SIZE sizeText = { 0, 0 };
	int nCX = 0;

	CString sFind;
	int		nPos;

	// "\n"을 줄바꿈 기호로 변경
	while( (nPos=strToolTip.Find(TEXT("\\n"))) >= 0 )
	{
		sToolTip = strToolTip.Left(nPos) + '\n';
		sToolTip += strToolTip.Mid(nPos+2);
		strToolTip = sToolTip;
	}

	// 툴팁 문자열이 여러줄이라면
	while ((nPos = sToolTip.Find('\n')) >= 0)
	{
		sFind = sToolTip.Left(nPos);

		sizeText = pDC->GetTextExtent(sFind);
		nCX = __max(nCX, sizeText.cx);

		sFind	 = sToolTip.Mid(nPos + 1);
		sToolTip = sFind;
	}

	if (sToolTip.GetLength() > 0)
	{
		sizeText = pDC->GetTextExtent(sToolTip);
		nCX = __max(nCX, sizeText.cx);
	}

	pDC->SelectObject(pFontOld);

	ReleaseDC(pDC);

	//
//	SetMargin(&rcBound);
	AddTool(pWnd, strToolTip, rTool, unID);

	RECT rcMargin = { 2, 2, 2, 2 };

	SetMargin(&rcMargin);
	SetMaxTipWidth(nCX);
}

// 툴팁 생성
void CThemeToolTip::CreateToolTip(CWnd *pParent, UINT unToolTipID, CString strToolTip)
{
	CRect rScreen;
	pParent->GetClientRect(&rScreen);
	
	//VisualStudio .net 버젼용
	//m_pToolTip->CreateEx(NULL, WS_EX_TOPMOST, TTS_ALWAYSTIP);
	//VisualStudio 6.0 버젼용
 	//CreateEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL);
	Create(pParent, WS_EX_TOPMOST);

	m_pParent = pParent;
	m_unToolTipID = unToolTipID;

	// 텍스트를 툴팁에 추가
	AddToolEx(pParent, &rScreen, unToolTipID, strToolTip);
}

// 툴팁 텍스트 변경
void CThemeToolTip::ChangeToolTip(CString strToolTip)
{
	// 부모 윈도우가 설정되지 않았다면 함수 탈출
	if( m_pParent == NULL )
		return;

	CRect rScreen;
	m_pParent->GetClientRect(&rScreen);

	this->DelTool(this);
	// 텍스트를 툴팁에 추가
	AddToolEx(m_pParent, &rScreen, m_unToolTipID, strToolTip);
}

// 툴팁 텍스트 반환
CString CThemeToolTip::GetToolTipText()
{
	return m_strToolTipText;
}

/*************************/
/* Global 변수/함수 정의 */
/*************************/

CString g_strThemePath;


// 테마 관련 전역변수를 초기화하는 함수
void gf_ThemeInit()
{
	CString strModulePath, strThemePath;

	// 메인 INI 파일 전체 경로를 구한다
	strModulePath = gf_GetModulePath();
	
	// 메인 INI파일에서 테마 폴더 경로를 구한다
	if( gf_GetTextFromIniFile(dfINI_MAIN_FILE_NAME, TEXT("Theme"), TEXT("Path"), strThemePath) )
	{
		// 테마 폴더 경로 설정
		gf_SetThemePath(strThemePath);
	}
}

// 테마 폴더 경로 설정
void gf_SetThemePath(CString strThemePath)
{
	// 폴더 경로 끝에 폴더 구분자가 없다면
	if( strThemePath.Right(1) != "\\" )
		strThemePath += "\\";

	if( strThemePath.Left(1) == "." )
	{
		strThemePath = strThemePath.Mid(1);
	}
		
	// 하드 드라이버 정보가 없다면
	if( strThemePath.Find(TEXT(":")) < 0 )
	{
		//실행파일이 존재하는 폴더 경로 반환
		g_strThemePath = gf_GetModulePath() + strThemePath;
	}
}

// 테마명을 반환
CString gf_GetThemeName()
{
	CString strThemeName;
	int nPos=0;

	// 테마 폴더 경로
	strThemeName = g_strThemePath;

	// 테마 경로가 없다면 함수 탈출
	if( strThemeName.GetLength() < 1 )
		return TEXT("");

	// 테마 경로 마지막에 폴더 구분자를 제거
	if( strThemeName.Right(1) == "\\" || strThemeName.Right(1) == "/" )
		strThemeName = strThemeName.Left(strThemeName.GetLength()-1);

	//문자열의 위치를 뒤쪽에서 부터 검사
	nPos = gf_FindStringBackward(strThemeName, TEXT("\\"));
	if( nPos < 0 )
		//문자열의 위치를 뒤쪽에서 부터 검사
		nPos = gf_FindStringBackward(strThemeName, TEXT("/"));

	if( nPos > 0)
		strThemeName = strThemeName.Mid(nPos+1);

	return strThemeName;
}

//문자열의 위치를 뒤쪽에서 부터 검사하는 함수(시작 위치 지정)
int gf_FindStringBackward(CString strData, CString strKey, int nStartL)
{
	int nPos1=-1, nPos2=-1;
	CString strBuf = TEXT("");

	strBuf = strData.Left(nStartL);

	while( (nPos1=strBuf.Find(strKey, nPos2+1)) >= 0 )
	{
		nPos2 = nPos1;
	}
	return nPos2;
}

//문자열의 위치를 뒤쪽에서 부터 검사하는 함수
int gf_FindStringBackward(CString strData, CString strKey)
{
	int nPos1=-1, nPos2=-1;

	while( (nPos1=strData.Find(strKey, nPos2+1)) >= 0 )
	{
		nPos2 = nPos1;
	}
	return nPos2;
}

// INI파일에서 텍스트 정보를 가져오는 함수
BOOL gf_GetTextFromIniFile(CString strIniFileName, LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString &strText)
{
	//CString sText;
	CString sPath = ::gf_GetModulePath() + strIniFileName;

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

// INI 파일에 텍스트 정보를 저장
BOOL gf_SetTextToIniFile(CString strIniFileName, LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString strText)
{
	CString sPath;
	
	// INI 파일 경로에 하드 드라이브명이 포함되지 않았다면
	if( strIniFileName.Find(TEXT(":")) < 0 )
		sPath = gf_GetModulePath() + strIniFileName;
	// INI 파일 경로에 하드 드라이브명이 포함되었다면
	else
		sPath = strIniFileName;

	BOOL bResult = ::WritePrivateProfileString(lpGrpName, lpCtrlName, strText, sPath);
	
	return bResult;
}

// 실행파일이 존재하는 폴더 경로 반환
CString gf_GetModulePath()
{
	wchar_t szPath[MAX_PATH];
	memset(szPath, 0x00, MAX_PATH);

	::GetModuleFileName(NULL, szPath, MAX_PATH);

	CString sTempPath = szPath;
	int iLength = sTempPath.GetLength();
	int iPos = sTempPath.ReverseFind(TCHAR('\\'));

	CString sModulePath = sTempPath.Left(iPos);
	sModulePath += "\\";

	return sModulePath;

}

// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
void gf_DrawImagePartResizeTransparent(CPaintDC *pdc, CBitmap *pBitmap, CRect rImgPart, CRect rImgView)
{
	if( rImgPart.left >= rImgPart.right || rImgPart.top >= rImgPart.bottom )
		return;
	if( rImgView.left >= rImgView.right || rImgView.top >= rImgView.bottom )
		return;

	////////////////////////////////////////////////////////////////////////////
	// Prepare everything for drawing
	////////////////////////////////////////////////////////////////////////////

	CRect l_rcClient ;

	l_rcClient = rImgView;
	//GetClientRect( &l_rcClient ) ;

	CDC		 l_BufferDC ;
			 l_BufferDC.CreateCompatibleDC( pdc ) ;
	CBitmap  l_BufferBitmap ;
			 //l_BufferBitmap.CreateCompatibleBitmap( pdc, l_rcClient.Width(), l_rcClient.Height() ) ;
			l_BufferBitmap.CreateCompatibleBitmap( pdc, rImgView.Width(), rImgView.Height() ) ;
	CBitmap* l_pOldBufferBitmap = l_BufferDC.SelectObject( &l_BufferBitmap ) ;
	CDC		 l_MaskDC ;
			 l_MaskDC.CreateCompatibleDC( pdc ) ;
	CBitmap  l_MaskBitmap ;
			 //l_MaskBitmap.CreateBitmap( l_rcClient.Width(), l_rcClient.Height(), 1, 1, NULL ) ;
			l_MaskBitmap.CreateBitmap( rImgView.Width(), rImgView.Height(), 1, 1, NULL ) ;
	CBitmap* l_pOldMaskBitmap = l_MaskDC.SelectObject( &l_MaskBitmap ) ;

	#define ROP4_TRANSPARENTBLIT		0xCCAA0020
	#define SRCMASK						0x00220326

	////////////////////////////////////////////////////////////////////////////
	// Fill with transparent color
	////////////////////////////////////////////////////////////////////////////

	l_BufferDC.FillSolidRect( &rImgView, RGB( 255, 0, 255 ) ) ;

	////////////////////////////////////////////////////////////////////////////
	// Blit the bitmap to the buffer
	////////////////////////////////////////////////////////////////////////////

	CDC		 l_MemoryDC ;
			 l_MemoryDC.CreateCompatibleDC( pdc ) ;
	CBitmap* l_pOldMemoryBitmap = l_MemoryDC.SelectObject( pBitmap ) ;

	//****** 이미지 생성 ******//
	//l_BufferDC.BitBlt( 0, 0, rImgView.Width(), rImgView.Height(), &l_MemoryDC, rImgPart.left, rImgPart.top, SRCCOPY ) ;
	l_BufferDC.StretchBlt(0, 0, rImgView.Width(), rImgView.Height(), &l_MemoryDC, rImgPart.left, rImgPart.top, rImgPart.Width(), rImgPart.Height(), SRCCOPY );

	l_MemoryDC.SelectObject( l_pOldMemoryBitmap ) ;

	////////////////////////////////////////////////////////////////////////////
	// Create the mask.
	////////////////////////////////////////////////////////////////////////////

	COLORREF l_crOldBack = l_BufferDC.SetBkColor( RGB( 255, 0, 255 ) ) ;

	//****** 이미지에 투명 마스크 적용 ******//
	l_MaskDC.BitBlt( 0, 0, l_rcClient.Width(), l_rcClient.Height(), &l_BufferDC, 0, 0, SRCCOPY ) ;

	l_BufferDC.SetBkColor( l_crOldBack ) ;

	////////////////////////////////////////////////////////////////////////////
	// Draw the bitmap transparently now, since not all devices support this
	// and Windows 98 does not (duh?), if this fails, we do it the long way.
	////////////////////////////////////////////////////////////////////////////

	//****** 투명 적용된 이미지를 화면에 출력 ******//
	if( ! pdc->MaskBlt( rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(),
					   		 &l_BufferDC,
					   		 0,0,
					   		 l_MaskBitmap,
					   		 0,0,
					   		 ROP4_TRANSPARENTBLIT ) )
	{
		CDC		 l_CopyDC ;
				 l_CopyDC.CreateCompatibleDC( pdc ) ;
		CBitmap  l_CopyBitmap ;
				 l_CopyBitmap.CreateCompatibleBitmap( pdc, l_rcClient.Width(), l_rcClient.Height() ) ;
		CBitmap* l_pOldCopyBitmap = l_CopyDC.SelectObject( &l_CopyBitmap ) ;

		l_CopyDC.BitBlt(   0, 0, l_rcClient.Width(), l_rcClient.Height(), pdc,  0, 0, SRCCOPY  ) ;
		l_CopyDC.BitBlt(   0, 0, l_rcClient.Width(), l_rcClient.Height(), &l_MaskDC,   0, 0, SRCAND   ) ;
		l_BufferDC.BitBlt( 0, 0, l_rcClient.Width(), l_rcClient.Height(), &l_MaskDC,   0, 0, SRCMASK  ) ;
		l_CopyDC.BitBlt(   0, 0, l_rcClient.Width(), l_rcClient.Height(), &l_BufferDC, 0, 0, SRCPAINT ) ;
		pdc->BitBlt(  rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(), &l_CopyDC,   0, 0, SRCCOPY  ) ;
		//pdc->BitBlt(  rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(), &l_CopyDC, rImgPart.left, rImgPart.top, SRCCOPY  ) ;

		l_CopyDC.SelectObject( l_pOldCopyBitmap ) ;
	}

	////////////////////////////////////////////////////////////////////////////
	// Clean up.
	////////////////////////////////////////////////////////////////////////////

	l_MaskDC.SelectObject( l_pOldMaskBitmap ) ;
	l_BufferDC.SelectObject( l_pOldBufferBitmap ) ;
}

// 이미지 크기 변경하여 그리는 함수(투명 지원)
void gf_DrawImageResizeTransparent(CPaintDC *pdc, CBitmap *pBitmap, CRect rArea)
{
	CRect rImage;
	BITMAP bm;
    pBitmap->GetBitmap(&bm);
	
	// 이미지 크기	
	rImage.left = 0;
	rImage.top = 0;
	rImage.right  = bm.bmWidth;
	rImage.bottom = bm.bmHeight;

	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImage, rArea);
}

// 이미지의 일부를 9조각으로 분할, 크기 변경하여 화면에 표시하는 함수(투명 지원)
void gf_DrawImagePartTransparentResizeBlock(CPaintDC *pdc, CBitmap *pBitmap, CRect rImgPart, CRect rArea)
{
	int nBlockWidth1=0, nBlockWidth2=0, nBlockWidth3=0, nBlockHeight1=0, nBlockHeight2=0, nBlockHeight3=0;
	int nHorizonBlock[4] = {0,50,150,200}, nVerticalBlock[4] = {0,50,150,200};
	int nGapX=50, nGapY=50;

	if( rImgPart.left >= rImgPart.right || rImgPart.top >= rImgPart.bottom )
		return;
	if( rArea.left >= rArea.right || rArea.top >= rArea.bottom )
		return;

	BITMAP bm;
    pBitmap->GetBitmap(&bm);

	// 부분 이미지를 자를 블럭 수치
	
	if( rImgPart.Width()-2 <= nGapX * 2 )
		nGapX = rImgPart.Width() / 3;

	if( rImgPart.Height()-2 <= nGapY * 2 )
		nGapY = rImgPart.Height() / 3;

	nHorizonBlock[1] = nGapX;
	nHorizonBlock[3] = rImgPart.Width();
	nHorizonBlock[2] = rImgPart.Width() - nGapX;
	nVerticalBlock[1] = nGapY;
	nVerticalBlock[3] = rImgPart.Height();
	nVerticalBlock[2] = rImgPart.Height() - nGapY;
	
	// 부분 이미지의 가운데 블럭 크기를 구한다
	nBlockWidth1 = nHorizonBlock[1] - nHorizonBlock[0];
	nBlockWidth3 = nHorizonBlock[3] - nHorizonBlock[2];
	nBlockWidth2 = rImgPart.Width() - nBlockWidth1 - nBlockWidth3;
	nBlockHeight1 = nVerticalBlock[1] - nVerticalBlock[0];
	nBlockHeight3 = nVerticalBlock[3] - nVerticalBlock[2];
	nBlockHeight2 = rImgPart.Height() - nBlockHeight1 - nBlockHeight3;
	// 화면 영역 가운데 블럭의 크기를 구한다
	int nScrBlockWidth2 = rArea.Width() - nBlockWidth1 - nBlockWidth3;
	int nScrBlockHeigh2 = rArea.Height() - nBlockHeight1 - nBlockHeight3;

	CRect rImgBlock, rImgView;

	// Mid-Mid 이미지 화면에 표시
	rImgView.left = rArea.left+nBlockWidth1;
	rImgView.top = rArea.top+nBlockHeight1;
	rImgView.right = rImgView.left + nScrBlockWidth2;
	rImgView.bottom = rImgView.top + nScrBlockHeigh2;
	rImgBlock.left = rImgPart.left+nBlockWidth1;
	rImgBlock.top = rImgPart.top+nBlockHeight1;
	rImgBlock.right = rImgBlock.left + nBlockWidth2;
	rImgBlock.bottom = rImgBlock.top + nBlockHeight2;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgBlock, rImgView);
	
	// Mid-Top 이미지 화면에 표시
	rImgView.left = rArea.left+nBlockWidth1;
	rImgView.top = rArea.top;
	rImgView.right = rImgView.left + nScrBlockWidth2;
	rImgView.bottom = rImgView.top + nBlockHeight1;
	rImgBlock.left = rImgPart.left+nBlockWidth1;
	rImgBlock.top = rImgPart.top;
	rImgBlock.right = rImgBlock.left + nBlockWidth2;
	rImgBlock.bottom = rImgBlock.top + nBlockHeight1;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgBlock, rImgView);
	
	// Mid-Bottom 이미지 화면에 표시
	rImgView.left = rArea.left+nBlockWidth1;
	rImgView.top = rArea.bottom-nBlockHeight3;
	rImgView.right = rImgView.left + nScrBlockWidth2;
	rImgView.bottom = rImgView.top + nBlockHeight3;
	rImgBlock.left = rImgPart.left+nBlockWidth1;
	rImgBlock.top = rImgPart.bottom-nBlockHeight3;
	rImgBlock.right = rImgBlock.left + nBlockWidth2;
	rImgBlock.bottom = rImgBlock.top + nBlockHeight3;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgBlock, rImgView);
	
	// Left-Mid 이미지 화면에 표시
	rImgView.left = rArea.left;
	rImgView.top = rArea.top+nBlockHeight1;
	rImgView.right = rImgView.left + nBlockWidth1;
	rImgView.bottom = rImgView.top + nScrBlockHeigh2;
	rImgBlock.left = rImgPart.left;
	rImgBlock.top = rImgPart.top+nBlockHeight1;
	rImgBlock.right = rImgBlock.left + nBlockWidth1;
	rImgBlock.bottom = rImgBlock.top + nBlockHeight2;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgBlock, rImgView);
	
	// Right-Mid 이미지 화면에 표시
	rImgView.left = rArea.right-nBlockWidth3;
	rImgView.top = rArea.top+nBlockHeight1;
	rImgView.right = rImgView.left + nBlockWidth3;
	rImgView.bottom = rImgView.top + nScrBlockHeigh2;
	rImgBlock.left = rImgPart.right-nBlockWidth3;
	rImgBlock.top = rImgPart.top+nBlockHeight1;
	rImgBlock.right = rImgBlock.left + nBlockWidth3;
	rImgBlock.bottom = rImgBlock.top + nBlockHeight2;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgBlock, rImgView);
	
	// Left-Top 이미지 화면에 표시
	rImgView.left = rArea.left;
	rImgView.top = rArea.top;
	rImgView.right = rImgView.left + nBlockWidth1;
	rImgView.bottom = rImgView.top + nBlockHeight1;
	rImgBlock.left = rImgPart.left;
	rImgBlock.top = rImgPart.top;
	rImgBlock.right = rImgBlock.left + nBlockWidth1;
	rImgBlock.bottom = rImgBlock.top + nBlockHeight1;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgBlock, rImgView);
	
	// Right-Top 이미지 화면에 표시
	rImgView.left = rArea.right-nBlockWidth3;
	rImgView.top = rArea.top;
	rImgView.right = rImgView.left + nBlockWidth3;
	rImgView.bottom = rImgView.top + nBlockHeight1;
	rImgBlock.left = rImgPart.right-nBlockWidth3;
	rImgBlock.top = rImgPart.top;
	rImgBlock.right = rImgBlock.left + nBlockWidth3;
	rImgBlock.bottom = rImgBlock.top + nBlockHeight1;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgBlock, rImgView);
	
	// Left-Bottom 이미지 화면에 표시
	rImgView.left = rArea.left;
	rImgView.top = rArea.bottom-nBlockHeight3;
	rImgView.right = rImgView.left + nBlockWidth1;
	rImgView.bottom = rImgView.top + nBlockHeight3;
	rImgBlock.left = rImgPart.left;
	rImgBlock.top = rImgPart.bottom-nBlockHeight3;
	rImgBlock.right = rImgBlock.left + nBlockWidth1;
	rImgBlock.bottom = rImgBlock.top + nBlockHeight3;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgBlock, rImgView);
	
	// Right-Bottom 이미지 화면에 표시
	rImgView.left = rArea.right-nBlockWidth3;
	rImgView.top = rArea.bottom-nBlockHeight3;
	rImgView.right = rImgView.left + nBlockWidth3;
	rImgView.bottom = rImgView.top + nBlockHeight3;
	rImgBlock.left = rImgPart.right-nBlockWidth3;
	rImgBlock.top = rImgPart.bottom-nBlockHeight3;
	rImgBlock.right = rImgBlock.left + nBlockWidth3;
	rImgBlock.bottom = rImgBlock.top + nBlockHeight3;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgBlock, rImgView);

}

// 사각형 선을 그리는 함수
void gf_DrawRectLine(CRect rLine, COLORREF crLeft, COLORREF crTop, COLORREF crRight, COLORREF crBottom, CPaintDC *pdc)
{
	CPen *pOldPen, *pPenLeft, *pPenTop, *pPenRight, *pPenBottom;

	pPenLeft = new CPen(PS_SOLID, 1, crLeft);
	pPenTop = new CPen(PS_SOLID, 1, crTop);
	pPenRight = new CPen(PS_SOLID, 1, crRight);
	pPenBottom = new CPen(PS_SOLID, 1, crBottom);
	
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

// 라인(선)을 그리는 함수
void gf_DrawLine(CPoint po1, CPoint po2, COLORREF crLine, int nLineTick, CPaintDC *pdc)
{
	CPen *pOldPen, *pNewPen;
	pNewPen = new CPen(PS_SOLID, nLineTick, crLine);
	pOldPen = pdc->SelectObject(pNewPen);

	pdc->MoveTo(po1);
	pdc->LineTo(po2);

	pdc->SelectObject(pOldPen);
	delete pNewPen;
}

// 이미지를 Pattern 브러쉬 형식으로 화면 영역에 그리는 함수(바둑판 형식)
void gf_DrawImageByPatternBrush(CPaintDC *pdc, CBitmap *pBitmap, CRect rImgPart, CRect rScreen)
{
	int i=0, j=0;
	CRect rImgBlock, rImgView;

	// 이미지가 표시될 화면 영역 초기화
	rImgView.top = rScreen.top;
	rImgView.bottom = rImgView.top + rImgPart.Height();

	while( rImgView.top < rScreen.bottom )
	{
		rImgBlock = rImgPart;
		if( rImgView.bottom > rScreen.bottom)
		{
			rImgView.bottom = rScreen.bottom;
			rImgBlock.bottom = rImgBlock.top + rImgView.Height();
		}
		// Left-RIght 좌표값 초기화
		rImgView.left = rScreen.left;
		rImgView.right = rImgView.left + rImgPart.Width();

		while( rImgView.left < rScreen.right )
		{
			if( rImgView.right > rScreen.right )
			{
				rImgView.right = rScreen.right;
				rImgBlock.right = rImgBlock.left + rImgView.Width();
			}
			
			// 이미지 일부분을 원래 크기 대로 그리는 함수(투명 지원)
			gf_DrawTransparentImage(pBitmap, rImgBlock, rImgView, pdc);

			// Left-RIght 좌표값 증가
			rImgView.left += rImgPart.Width();
			rImgView.right += rImgPart.Width();
		}

		// Top-Bottom 좌표값 증가
		rImgView.top += rImgPart.Height();
		rImgView.bottom += rImgPart.Height();
	}

}

// 투명 배경 이미지를 그리는 함수
void gf_DrawTransparentImage(CBitmap *pBitmap, CRect rImgPart, CRect rImgView, CPaintDC *pdc)
{
	//CBitmap bmpBitmap;
	//비트맵 이미지 로딩
	//bmpBitmap.LoadBitmap(nImageID);

	////////////////////////////////////////////////////////////////////////////
	// Prepare everything for drawing
	////////////////////////////////////////////////////////////////////////////

	CRect l_rcClient ;

	l_rcClient = rImgView;
	//pWnd->GetClientRect( &l_rcClient ) ;

	CDC		 l_BufferDC ;
			 l_BufferDC.CreateCompatibleDC( pdc ) ;
	CBitmap  l_BufferBitmap ;
			 //l_BufferBitmap.CreateCompatibleBitmap( pdc, l_rcClient.Width(), l_rcClient.Height() ) ;
			l_BufferBitmap.CreateCompatibleBitmap( pdc, rImgView.Width(), rImgView.Height() ) ;
	CBitmap* l_pOldBufferBitmap = l_BufferDC.SelectObject( &l_BufferBitmap ) ;
	CDC		 l_MaskDC ;
			 l_MaskDC.CreateCompatibleDC( pdc ) ;
	CBitmap  l_MaskBitmap ;
			 //l_MaskBitmap.CreateBitmap( l_rcClient.Width(), l_rcClient.Height(), 1, 1, NULL ) ;
			l_MaskBitmap.CreateBitmap( rImgView.Width(), rImgView.Height(), 1, 1, NULL ) ;
	CBitmap* l_pOldMaskBitmap = l_MaskDC.SelectObject( &l_MaskBitmap ) ;

	#define ROP4_TRANSPARENTBLIT		0xCCAA0020
	#define SRCMASK						0x00220326

	////////////////////////////////////////////////////////////////////////////
	// Fill with transparent color
	////////////////////////////////////////////////////////////////////////////

	l_BufferDC.FillSolidRect( &rImgView, dfCOLOR_TRANSPARENT ) ;

	////////////////////////////////////////////////////////////////////////////
	// Blit the bitmap to the buffer
	////////////////////////////////////////////////////////////////////////////

	CDC		 l_MemoryDC ;
			 l_MemoryDC.CreateCompatibleDC( pdc ) ;
	CBitmap* l_pOldMemoryBitmap = l_MemoryDC.SelectObject( pBitmap ) ;

	l_BufferDC.BitBlt( 0, 0, rImgView.Width(), rImgView.Height(), &l_MemoryDC, rImgPart.left, rImgPart.top, SRCCOPY ) ;
	//l_BufferDC.BitBlt( 0, 0, rImgView.Width(), rImgView.Height(), &l_MemoryDC, 0, 0, SRCCOPY ) ;

	l_MemoryDC.SelectObject( l_pOldMemoryBitmap ) ;

	////////////////////////////////////////////////////////////////////////////
	// Create the mask.
	////////////////////////////////////////////////////////////////////////////

	COLORREF l_crOldBack = l_BufferDC.SetBkColor( RGB( 255, 0, 255 ) ) ;

	//l_MaskDC.BitBlt( 0, 0, l_rcClient.Width(), l_rcClient.Height(), &l_BufferDC, rImgPart.left, rImgPart.top, SRCCOPY ) ;
	l_MaskDC.BitBlt( 0, 0, l_rcClient.Width(), l_rcClient.Height(), &l_BufferDC, 0, 0, SRCCOPY ) ;

	l_BufferDC.SetBkColor( l_crOldBack ) ;

	////////////////////////////////////////////////////////////////////////////
	// Draw the bitmap transparently now, since not all devices support this
	// and Windows 98 does not (duh?), if this fails, we do it the long way.
	////////////////////////////////////////////////////////////////////////////

	if( ! pdc->MaskBlt( rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(),
							//0, 0, l_rcClient.Width(), l_rcClient.Height(),
					   		 &l_BufferDC,
							 //rImgPart.left, rImgPart.top,
					   		 0,0,
					   		 l_MaskBitmap,
					   		 0,
					   		 0,
					   		 ROP4_TRANSPARENTBLIT ) )
	{
		CDC		 l_CopyDC ;
				 l_CopyDC.CreateCompatibleDC( pdc ) ;
		CBitmap  l_CopyBitmap ;
				 l_CopyBitmap.CreateCompatibleBitmap( pdc, l_rcClient.Width(), l_rcClient.Height() ) ;
		CBitmap* l_pOldCopyBitmap = l_CopyDC.SelectObject( &l_CopyBitmap ) ;

		l_CopyDC.BitBlt(   0, 0, l_rcClient.Width(), l_rcClient.Height(), pdc,  0, 0, SRCCOPY  ) ;
		l_CopyDC.BitBlt(   0, 0, l_rcClient.Width(), l_rcClient.Height(), &l_MaskDC,   0, 0, SRCAND   ) ;
		l_BufferDC.BitBlt( 0, 0, l_rcClient.Width(), l_rcClient.Height(), &l_MaskDC,   0, 0, SRCMASK  ) ;
		l_CopyDC.BitBlt(   0, 0, l_rcClient.Width(), l_rcClient.Height(), &l_BufferDC, 0, 0, SRCPAINT ) ;
		pdc->BitBlt(  rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(), &l_CopyDC,   0, 0, SRCCOPY  ) ;
		//pdc->BitBlt(  rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(), &l_CopyDC, rImgPart.left, rImgPart.top, SRCCOPY  ) ;
		

		l_CopyDC.SelectObject( l_pOldCopyBitmap ) ;
	}

	////////////////////////////////////////////////////////////////////////////
	// Clean up.
	////////////////////////////////////////////////////////////////////////////

	l_MaskDC.SelectObject( l_pOldMaskBitmap ) ;
	l_BufferDC.SelectObject( l_pOldBufferBitmap ) ;
}

// 투명 배경 이미지를 크기 변경하여 그리는 함수
void gf_DrawTransparentResize(CBitmap *pBitmap, CRect rImgPart, CRect rImgView, CPaintDC *pdc)
{
	//CBitmap bmpBitmap;
	//비트맵 이미지 로딩
	//bmpBitmap.LoadBitmap(nImageID);

	////////////////////////////////////////////////////////////////////////////
	// Prepare everything for drawing
	////////////////////////////////////////////////////////////////////////////

	//CRect l_rcClient ;

	//pWnd->GetClientRect( &l_rcClient ) ;

	CDC		 l_BufferDC ;
			 l_BufferDC.CreateCompatibleDC( pdc ) ;
	CBitmap  l_BufferBitmap ;
			 //l_BufferBitmap.CreateCompatibleBitmap( pdc, l_rcClient.Width(), l_rcClient.Height() ) ;
			l_BufferBitmap.CreateCompatibleBitmap( pdc, rImgView.Width(), rImgView.Height() ) ;
	CBitmap* l_pOldBufferBitmap = l_BufferDC.SelectObject( &l_BufferBitmap ) ;
	CDC		 l_MaskDC ;
			 l_MaskDC.CreateCompatibleDC( pdc ) ;
	CBitmap  l_MaskBitmap ;
			 //l_MaskBitmap.CreateBitmap( l_rcClient.Width(), l_rcClient.Height(), 1, 1, NULL ) ;
			l_MaskBitmap.CreateBitmap( rImgView.Width(), rImgView.Height(), 1, 1, NULL ) ;
	CBitmap* l_pOldMaskBitmap = l_MaskDC.SelectObject( &l_MaskBitmap ) ;

	#define ROP4_TRANSPARENTBLIT		0xCCAA0020
	#define SRCMASK						0x00220326

	////////////////////////////////////////////////////////////////////////////
	// Fill with transparent color
	////////////////////////////////////////////////////////////////////////////

	l_BufferDC.FillSolidRect( &rImgView, dfCOLOR_TRANSPARENT ) ;

	////////////////////////////////////////////////////////////////////////////
	// Blit the bitmap to the buffer
	////////////////////////////////////////////////////////////////////////////

	CDC		 l_MemoryDC ;
			 l_MemoryDC.CreateCompatibleDC( pdc ) ;
	CBitmap* l_pOldMemoryBitmap = l_MemoryDC.SelectObject( pBitmap ) ;

	//****** 이미지 생성 ******//
	//l_BufferDC.BitBlt( 0, 0, rImgView.Width(), rImgView.Height(), &l_MemoryDC, rImgPart.left, rImgPart.top, SRCCOPY ) ;
	l_BufferDC.StretchBlt(0, 0, rImgView.Width(), rImgView.Height(), &l_MemoryDC, rImgPart.left, rImgPart.top, rImgPart.Width(), rImgPart.Height(), SRCCOPY );

	l_MemoryDC.SelectObject( l_pOldMemoryBitmap ) ;

	////////////////////////////////////////////////////////////////////////////
	// Create the mask.
	////////////////////////////////////////////////////////////////////////////

	COLORREF l_crOldBack = l_BufferDC.SetBkColor( RGB( 255, 0, 255 ) ) ;

	//****** 이미지에 투명 마스크 적용 ******//
	l_MaskDC.BitBlt( 0, 0, rImgView.Width(), rImgView.Height(), &l_BufferDC, 0, 0, SRCCOPY ) ;

	l_BufferDC.SetBkColor( l_crOldBack ) ;

	////////////////////////////////////////////////////////////////////////////
	// Draw the bitmap transparently now, since not all devices support this
	// and Windows 98 does not (duh?), if this fails, we do it the long way.
	////////////////////////////////////////////////////////////////////////////

	//****** 투명 적용된 이미지를 화면에 출력 ******//
	if( ! pdc->MaskBlt( rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(),
					   		 &l_BufferDC,
					   		 0,0,
					   		 l_MaskBitmap,
					   		 0,0,
					   		 ROP4_TRANSPARENTBLIT ) )
	{
		CDC		 l_CopyDC ;
				 l_CopyDC.CreateCompatibleDC( pdc ) ;
		CBitmap  l_CopyBitmap ;
				 l_CopyBitmap.CreateCompatibleBitmap( pdc, rImgView.Width(), rImgView.Height() ) ;
		CBitmap* l_pOldCopyBitmap = l_CopyDC.SelectObject( &l_CopyBitmap ) ;

		l_CopyDC.BitBlt(   0, 0, rImgView.Width(), rImgView.Height(), pdc,  0, 0, SRCCOPY  ) ;
		l_CopyDC.BitBlt(   0, 0, rImgView.Width(), rImgView.Height(), &l_MaskDC,   0, 0, SRCAND   ) ;
		l_BufferDC.BitBlt( 0, 0, rImgView.Width(), rImgView.Height(), &l_MaskDC,   0, 0, SRCMASK  ) ;
		l_CopyDC.BitBlt(   0, 0, rImgView.Width(), rImgView.Height(), &l_BufferDC, 0, 0, SRCPAINT ) ;
		pdc->BitBlt(  rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(), &l_CopyDC,   0, 0, SRCCOPY  ) ;
		//pdc->BitBlt(  rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(), &l_CopyDC, rImgPart.left, rImgPart.top, SRCCOPY  ) ;

		l_CopyDC.SelectObject( l_pOldCopyBitmap ) ;
	}

	////////////////////////////////////////////////////////////////////////////
	// Clean up.
	////////////////////////////////////////////////////////////////////////////

	l_MaskDC.SelectObject( l_pOldMaskBitmap ) ;
	l_BufferDC.SelectObject( l_pOldBufferBitmap ) ;
}

// 이미지를 9조각으로 분할하여 화면에 표시하는 함수(투명 적용)
void gf_DrawImagePartSkin(CPaintDC *pdc, CBitmap *pBitmap, CRect rImage, CRect rScreen)
{
	CRect rAreaPart, rImagePart;
	CDC dcMem;
    dcMem.CreateCompatibleDC(pdc);
	int nImageBlockWidth1=0, nImageBlockWidth2=0, nImageBlockWidth3=0;
    int nImageBlockHeight1=0, nImageBlockHeight2=0, nImageBlockHeight3=0;
    
	// 이미지를 표시할 화면의 크기를 구한다
	//this->GetClientRect(&rArea);
	// 이미지 가운데 블럭의 크기를 구한다
	nImageBlockWidth1 = rImage.Width() / 3;
	nImageBlockWidth3 = nImageBlockWidth1;
	nImageBlockWidth2 = rImage.Width() - nImageBlockWidth1 - nImageBlockWidth3;
	nImageBlockHeight1 = rImage.Height() / 3;
	nImageBlockHeight3 = nImageBlockHeight1;
	nImageBlockHeight2 = rImage.Height() - nImageBlockHeight1 - nImageBlockHeight3;

	// 화면 영역 가운데 블럭의 크기를 구한다
	int nScrBlockWidth2 = rScreen.Width() - nImageBlockWidth1 - nImageBlockWidth3;
	int nScrBlockHeigh2 = rScreen.Height() - nImageBlockHeight1 - nImageBlockHeight3;

	//****** Mid-Mid 이미지 ******//
	// 화면에 표시될 위치 좌표
	rAreaPart.left = nImageBlockWidth1;
	rAreaPart.top = nImageBlockHeight1;
	rAreaPart.right = rAreaPart.left + nScrBlockWidth2;
	rAreaPart.bottom = rAreaPart.top + nScrBlockHeigh2;
	// 이미지 조각 영역
	rImagePart.left = rImage.left + nImageBlockWidth1;
	rImagePart.top = rImage.top + nImageBlockHeight1;
	rImagePart.right = rImagePart.left + nImageBlockWidth2;
	rImagePart.bottom = rImagePart.top + nImageBlockHeight2;
	//투명 배경 이미지를 크기 변경하여 그리는 함수
	gf_DrawTransparentResize(pBitmap, rImagePart, rAreaPart, pdc);

	//****** Mid-Top 이미지 ******//
	// 화면에 표시될 위치 좌표
	rAreaPart.left = nImageBlockWidth1;
	rAreaPart.top = 0;
	rAreaPart.right = rAreaPart.left + nScrBlockWidth2;
	rAreaPart.bottom = rAreaPart.top + nImageBlockHeight1;
	// 이미지 조각 영역
	rImagePart.left = rImage.left+nImageBlockWidth1;
	rImagePart.top = rImage.top;
	rImagePart.right = rImagePart.left + nImageBlockWidth2;
	rImagePart.bottom = rImagePart.top + nImageBlockHeight1;
	//투명 배경 이미지를 크기 변경하여 그리는 함수
	gf_DrawTransparentResize(pBitmap, rImagePart, rAreaPart, pdc);

	//****** Mid-Bottom 이미지 ******//
	// 화면에 표시될 위치 좌표
	rAreaPart.left = nImageBlockWidth1;
	rAreaPart.top = rScreen.Height()-nImageBlockHeight3;
	rAreaPart.right = rAreaPart.left + nScrBlockWidth2;
	rAreaPart.bottom = rAreaPart.top + nImageBlockHeight3;
	// 이미지 조각 영역
	rImagePart.left = rImage.left+nImageBlockWidth1;
	rImagePart.top = rImage.bottom-nImageBlockHeight3;
	rImagePart.right = rImagePart.left + nImageBlockWidth2;
	rImagePart.bottom = rImagePart.top + nImageBlockHeight3;
	//투명 배경 이미지를 크기 변경하여 그리는 함수
	gf_DrawTransparentResize(pBitmap, rImagePart, rAreaPart, pdc);

	//****** Left-Mid 이미지 ******//
	// 화면에 표시될 위치 좌표
	rAreaPart.left = 0;
	rAreaPart.top = nImageBlockHeight1;
	rAreaPart.right = rAreaPart.left + nImageBlockWidth1;
	rAreaPart.bottom = rAreaPart.top + nScrBlockHeigh2;
	// 이미지 조각 영역
	rImagePart.left = rImage.left;
	rImagePart.top = rImage.top + nImageBlockHeight1;
	rImagePart.right = rImagePart.left + nImageBlockWidth1;
	rImagePart.bottom = rImagePart.top + nImageBlockHeight2;
	//투명 배경 이미지를 크기 변경하여 그리는 함수
	gf_DrawTransparentResize(pBitmap, rImagePart, rAreaPart, pdc);

	//****** Right-Mid 이미지 ******//
	// 화면에 표시될 위치 좌표
	rAreaPart.left = rScreen.Width()-nImageBlockWidth3;
	rAreaPart.top = nImageBlockHeight1;
	rAreaPart.right = rAreaPart.left + nImageBlockWidth3;
	rAreaPart.bottom = rAreaPart.top + nScrBlockHeigh2;
	// 이미지 조각 영역
	rImagePart.left = rImage.right-nImageBlockWidth3;
	rImagePart.top = rImage.top+nImageBlockHeight1;
	rImagePart.right = rImagePart.left + nImageBlockWidth3;
	rImagePart.bottom = rImagePart.top + nImageBlockHeight2;
	//투명 배경 이미지를 크기 변경하여 그리는 함수
	gf_DrawTransparentResize(pBitmap, rImagePart, rAreaPart, pdc);

	//****** Left-Top 이미지 ******//
	// 화면에 표시될 위치 좌표
	rAreaPart.left = 0;
	rAreaPart.top = 0;
	rAreaPart.right = rAreaPart.left + nImageBlockWidth1;
	rAreaPart.bottom = rAreaPart.top + nImageBlockHeight1;
	// 이미지 조각 영역
	rImagePart.left = rImage.left;
	rImagePart.top = rImage.top;
	rImagePart.right = rImagePart.left + nImageBlockWidth1;
	rImagePart.bottom = rImagePart.right + nImageBlockHeight1;
	//투명 배경 이미지를 그리는 함수
	gf_DrawTransparentImage(pBitmap, rImagePart, rAreaPart, pdc);

	//****** Right-Top 이미지 ******//
	// 화면에 표시될 위치 좌표
	rAreaPart.left = rScreen.Width()-nImageBlockWidth3;
	rAreaPart.top = 0;
	rAreaPart.right = rAreaPart.left + nImageBlockWidth3;
	rAreaPart.bottom = rAreaPart.top + nImageBlockHeight1;
	// 이미지 조각 영역
	rImagePart.left = rImage.right-nImageBlockWidth3;
	rImagePart.top = rImage.top;
	rImagePart.right = rImagePart.left + nImageBlockWidth3;
	rImagePart.bottom = rImagePart.right + nImageBlockHeight1;
	//투명 배경 이미지를 그리는 함수
	gf_DrawTransparentImage(pBitmap, rImagePart, rAreaPart, pdc);

	//****** Left-Bottom 이미지 ******//
	// 화면에 표시될 위치 좌표
	rAreaPart.left = 0;
	rAreaPart.top = rScreen.Height()-nImageBlockHeight3;
	rAreaPart.right = rAreaPart.left + nImageBlockWidth1;
	rAreaPart.bottom = rAreaPart.top + nImageBlockHeight3;
	// 이미지 조각 영역
	rImagePart.left = rImage.left;
	rImagePart.top = rImage.bottom-nImageBlockHeight3;
	rImagePart.right = rImagePart.left + nImageBlockWidth1;
	rImagePart.bottom = rImagePart.right + nImageBlockHeight3;
	//투명 배경 이미지를 그리는 함수
	gf_DrawTransparentImage(pBitmap, rImagePart, rAreaPart, pdc);

	//****** Right-Bottom 이미지 ******//
	// 화면에 표시될 위치 좌표
	rAreaPart.left = rScreen.Width()-nImageBlockWidth3;
	rAreaPart.top = rScreen.Height()-nImageBlockHeight3;
	rAreaPart.right = rAreaPart.left + nImageBlockWidth3;
	rAreaPart.bottom = rAreaPart.top + nImageBlockHeight3;
	// 이미지 조각 영역
	rImagePart.left = rImage.right-nImageBlockWidth3;
	rImagePart.top = rImage.bottom-nImageBlockHeight3;
	rImagePart.right = rImagePart.left + nImageBlockWidth3;
	rImagePart.bottom = rImagePart.right + nImageBlockHeight3;
	//투명 배경 이미지를 그리는 함수
	gf_DrawTransparentImage(pBitmap, rImagePart, rAreaPart, pdc);

}

// 테두리를 가지는 사각형을 그리는 함수
void gf_DrawRectWithOutline(CPaintDC *pdc, COLORREF crBack, COLORREF crOutlineLT, COLORREF crOutlineRB, int nOutlineTick, CRect rRect)
{
	// 화면을 컬러로 채운다
	pdc->FillRect(rRect, &CBrush(crBack));

	rRect.right --;
	rRect.bottom --;

	for(int i=0 ; i < nOutlineTick ; i++)
	{
		//사각형 선을 그리는 함수
		gf_DrawRectLine(rRect, crOutlineLT, crOutlineLT, crOutlineRB, crOutlineRB, pdc);
		//사각형 좌표 변경
		rRect.left ++;
		rRect.top ++;
		rRect.right --;
		rRect.bottom --;
	}
}

// 모서리가 둥근 사각형을 그리는 함수
void gf_DrawRoundEdgeRect(CPaintDC *pdc, COLORREF crBack, COLORREF crOutline, int nOutlineTick, int nEdgeRound, CRect rRect)
{
	int nEdgeGap1=0, nEdgeGap2=0;
	
	// 외곽선 두께에 따라서 여백(Gap)을 달리한다
	nEdgeGap1 = (nOutlineTick / 2);
	nEdgeGap2 = nEdgeGap1;
	if( (nOutlineTick/2)*2 == nOutlineTick)
		nEdgeGap2 --;
	if( nEdgeGap1 < 0 ) nEdgeGap1 = 0;
	if( nEdgeGap2 < 0 ) nEdgeGap2 = 0;

	rRect.left += nEdgeGap1;
	rRect.top += nEdgeGap1;
	rRect.right -= nEdgeGap2;
	rRect.bottom -= nEdgeGap2;
	
	CBrush *pBrBack, *pOldBrush;
	CPen *pLinePen, *pOldPen;

	pBrBack = new CBrush(crBack);
	pLinePen = new CPen(PS_SOLID, nOutlineTick, crOutline);

	pOldBrush = pdc->SelectObject(pBrBack);
	pOldPen = pdc->SelectObject(pLinePen);
	
	pdc->RoundRect(&rRect, CPoint(nEdgeRound, nEdgeRound));
	//pdc->Rectangle(rWnd);
	
	pdc->SelectObject(pOldBrush);
	pdc->SelectObject(pOldPen);

	delete pBrBack;
	delete pLinePen;
}

// 이미지를 9조각으로 분할, 크기 변경하여 화면에 표시하는 함수(투명 지원)
void gf_DrawImageTransparentResizeBlock(CPaintDC *pdc, CBitmap *pBitmap, CRect rArea)
{
	int nBlockWidth1=0, nBlockWidth3=0, nBlockWidth2=0, nBlockHeight1=0, nBlockHeight2=0, nBlockHeight3=0;
	int nHorizonBlock[4] = {0,50,150,200}, nVerticalBlock[4] = {0,50,150,200};
	int nGapX=100, nGapY=100;
	CRect rImage;

	BITMAP bm;
    pBitmap->GetBitmap(&bm);
	
	// 이미지 크기	
	rImage.left = 0;
	rImage.top = 0;
	rImage.right  = bm.bmWidth;
	rImage.bottom = bm.bmHeight;

	// 이미지를 자를 블럭 수치
	
	//if( rImage.Width()-2 <= nGapX * 2 )
		nGapX = rImage.Width() / 3;

	//if( rImage.Height()-2 <= nGapY * 2 )
		nGapY = rImage.Height() / 3;

	nHorizonBlock[1] = nGapX;
	nHorizonBlock[3] = rImage.Width();
	nHorizonBlock[2] = rImage.Width() - nGapX;
	nVerticalBlock[1] = nGapY;
	nVerticalBlock[3] = rImage.Height();
	nVerticalBlock[2] = rImage.Height() - nGapY;
	
	// 이미지의 가운데 블럭 크기를 구한다
	nBlockWidth1 = nHorizonBlock[1] - nHorizonBlock[0];
	nBlockWidth3 = nHorizonBlock[3] - nHorizonBlock[2];
	nBlockWidth2 = rImage.Width() - nBlockWidth1 - nBlockWidth3;
	nBlockHeight1 = nVerticalBlock[1] - nVerticalBlock[0];
	nBlockHeight3 = nVerticalBlock[3] - nVerticalBlock[2];
	nBlockHeight2 = rImage.Height() - nBlockHeight1 - nBlockHeight3;
	// 화면 영역 가운데 블럭의 크기를 구한다
	int nScrBlockWidth2 = rArea.Width() - nBlockWidth1 - nBlockWidth3;
	int nScrBlockHeigh2 = rArea.Height() - nBlockHeight1 - nBlockHeight3;

	CRect rImgPart, rImgView;

	// Mid-Mid 이미지 화면에 표시
	rImgView.left = rArea.left+nBlockWidth1;
	rImgView.top = rArea.top+nBlockHeight1;
	rImgView.right = rImgView.left + nScrBlockWidth2;
	rImgView.bottom = rImgView.top + nScrBlockHeigh2;
	rImgPart.left = rImage.left+nBlockWidth1;
	rImgPart.top = rImage.top+nBlockHeight1;
	rImgPart.right = rImgPart.left + nBlockWidth2;
	rImgPart.bottom = rImgPart.top + nBlockHeight2;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgPart, rImgView);

	// Mid-Top 이미지 화면에 표시
	rImgView.left = rArea.left+nBlockWidth1;
	rImgView.top = rArea.top;
	rImgView.right = rImgView.left + nScrBlockWidth2;
	rImgView.bottom = rImgView.top + nBlockHeight1;
	rImgPart.left = rImage.left+nBlockWidth1;
	rImgPart.top = rImage.top;
	rImgPart.right = rImgPart.left + nBlockWidth2;
	rImgPart.bottom = rImgPart.top + nBlockHeight1;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgPart, rImgView);

	// Mid-Bottom 이미지 화면에 표시
	rImgView.left = rArea.left+nBlockWidth1;
	rImgView.top = rArea.bottom-nBlockHeight3;
	rImgView.right = rImgView.left + nScrBlockWidth2;
	rImgView.bottom = rImgView.top + nBlockHeight3;
	rImgPart.left = rImage.left+nBlockWidth1;
	rImgPart.top = rImage.bottom-nBlockHeight3;
	rImgPart.right = rImgPart.left + nBlockWidth2;
	rImgPart.bottom = rImgPart.top + nBlockHeight3;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgPart, rImgView);

	// Left-Mid 이미지 화면에 표시
	rImgView.left = rArea.left;
	rImgView.top = rArea.top+nBlockHeight1;
	rImgView.right = rImgView.left + nBlockWidth1;
	rImgView.bottom = rImgView.top + nScrBlockHeigh2;
	rImgPart.left = rImage.left;
	rImgPart.top = rImage.top+nBlockHeight1;
	rImgPart.right = rImgPart.left + nBlockWidth1;
	rImgPart.bottom = rImgPart.top + nBlockHeight2;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgPart, rImgView);

	// Right-Mid 이미지 화면에 표시
	rImgView.left = rArea.right-nBlockWidth3;
	rImgView.top = rArea.top+nBlockHeight1;
	rImgView.right = rImgView.left + nBlockWidth3;
	rImgView.bottom = rImgView.top + nScrBlockHeigh2;
	rImgPart.left = rImage.right-nBlockWidth3;
	rImgPart.top = rImage.top+nBlockHeight1;
	rImgPart.right = rImgPart.left + nBlockWidth3;
	rImgPart.bottom = rImgPart.top + nBlockHeight2;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgPart, rImgView);

	// Left-Top 이미지 화면에 표시
	rImgView.left = rArea.left;
	rImgView.top = rArea.top;
	rImgView.right = rImgView.left + nBlockWidth1;
	rImgView.bottom = rImgView.top + nBlockHeight1;
	rImgPart.left = rImage.left;
	rImgPart.top = rImage.top;
	rImgPart.right = rImgPart.left + nBlockWidth1;
	rImgPart.bottom = rImgPart.top + nBlockHeight1;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgPart, rImgView);

	// Right-Top 이미지 화면에 표시
	rImgView.left = rArea.right-nBlockWidth3;
	rImgView.top = rArea.top;
	rImgView.right = rImgView.left + nBlockWidth3;
	rImgView.bottom = rImgView.top + nBlockHeight1;
	rImgPart.left = rImage.right-nBlockWidth3;
	rImgPart.top = rImage.top;
	rImgPart.right = rImgPart.left + nBlockWidth3;
	rImgPart.bottom = rImgPart.top + nBlockHeight1;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgPart, rImgView);

	// Left-Bottom 이미지 화면에 표시
	rImgView.left = rArea.left;
	rImgView.top = rArea.bottom-nBlockHeight3;
	rImgView.right = rImgView.left + nBlockWidth1;
	rImgView.bottom = rImgView.top + nBlockHeight3;
	rImgPart.left = rImage.left;
	rImgPart.top = rImage.bottom-nBlockHeight3;
	rImgPart.right = rImgPart.left + nBlockWidth1;
	rImgPart.bottom = rImgPart.top + nBlockHeight3;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgPart, rImgView);

	// Right-Bottom 이미지 화면에 표시
	rImgView.left = rArea.right-nBlockWidth3;
	rImgView.top = rArea.bottom-nBlockHeight3;
	rImgView.right = rImgView.left + nBlockWidth3;
	rImgView.bottom = rImgView.top + nBlockHeight3;
	rImgPart.left = rImage.right-nBlockWidth3;
	rImgPart.top = rImage.bottom-nBlockHeight3;
	rImgPart.right = rImgPart.left + nBlockWidth3;
	rImgPart.bottom = rImgPart.top + nBlockHeight3;
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgPart, rImgView);

}

// 화면에 타원(면)을 그린다
void gf_DrawCircle(CRect rArea, COLORREF crLine, int nLineTick, COLORREF crCircle, CPaintDC *pdc)
{
	// 브러쉬 생성
	CBrush *pOldBrush, *pNewBrush;
	pNewBrush = new CBrush(crCircle);
	// 브러쉬 등록
	pOldBrush = pdc->SelectObject(pNewBrush);

	// 펜 생성
	CPen *pOldPen, *pNewPen;
	pNewPen = new CPen(PS_SOLID, nLineTick, crLine);
	// 펜 등록
	pOldPen = pdc->SelectObject(pNewPen);

	// 화면에 타원을 그린다
	pdc->Ellipse(rArea);

	// DC 초기화
	pdc->SelectObject(pOldBrush);
	delete pNewBrush;
	pdc->SelectObject(pOldPen);
	delete pNewPen;

}

// 이미지를 원본 그대로 1회 화면에 정렬해서 그리는 함수(투명 지원)
void gf_DrawImageTransparent(CPaintDC *pdc, CBitmap *pBitmap, CRect rScreen, UINT unAlignFormat)
{
	CRect rImgPart, rImgView;
	BITMAP bm;

    pBitmap->GetBitmap(&bm);

	// 이미지 영역좌표
	rImgPart.left = 0;
	rImgPart.top = 0;
	rImgPart.right = bm.bmWidth;
	rImgPart.bottom = bm.bmHeight;

	// 이미지를 그릴 화면 영역좌표
	rImgView = rImgPart;
	// 이미지 정렬 방식 - Right 일때
	if( (unAlignFormat & DT_RIGHT) == DT_RIGHT )
	{
		rImgView.right = rScreen.right;
		rImgView.left = rImgView.right - rImgPart.Width();
	}
	// 이미지 정렬 방식 - Center 일때
	else if( (unAlignFormat & DT_CENTER) == DT_CENTER )
	{
		rImgView.left = rScreen.CenterPoint().x - (rImgPart.Width()/2);
		rImgView.right = rScreen.CenterPoint().x + (rImgPart.Width()/2);
	}
	// 이미지 정렬 방식 - Left 일때
	else if( (unAlignFormat & DT_LEFT) == DT_LEFT )
	{
		rImgView.left = 0;
		rImgView.right = rImgPart.Width();
	}
	// 이미지 정렬 방식 - Bottom 일때
	if( (unAlignFormat & DT_BOTTOM) == DT_BOTTOM )
	{
		rImgView.bottom = rScreen.bottom;
		rImgView.top = rImgView.bottom - rImgPart.Height();
	}
	// 이미지 정렬 방식 - VCenter 일때
	else if( (unAlignFormat & DT_VCENTER) == DT_VCENTER )
	{
		rImgView.top = rScreen.CenterPoint().y - (rImgPart.Height()/2);
		rImgView.bottom = rScreen.CenterPoint().y + (rImgPart.Height()/2);
	}	
	// 이미지 정렬 방식 - Top 일때
	else if( (unAlignFormat & DT_TOP) == DT_TOP )
	{
		rImgView.top = 0;
		rImgView.bottom = rImgPart.Height();
	}
	
	// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
	gf_DrawImagePartResizeTransparent(pdc, pBitmap, rImgPart, rImgView);

}

// 이미지 파일을 읽어서 CBitmap으로 반환
BOOL gf_LoadPictureFile(LPCTSTR szFile, CBitmap *pBitmap)
{
	if (pBitmap == NULL)
		return FALSE;

	// open file
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	// get file size
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == -1)
		return FALSE;

	// alloc memory based on file size
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
	if (hGlobal == NULL)
		return FALSE;

	LPVOID pvData = NULL;
	pvData = GlobalLock(hGlobal);
	if (pvData == NULL)
	{
		GlobalFree(hGlobal);
		return FALSE;
	}

	// read file and store in global memory
	DWORD dwBytesRead = 0;
	BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);

	GlobalUnlock(hGlobal);
	CloseHandle(hFile);
	
	if (bRead == FALSE)
	{
		GlobalFree(hGlobal);
		return FALSE;
	}

	// create IStream* from global memory
	LPSTREAM pstm = NULL;
	HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
	if (pstm == NULL)
	{
		GlobalFree(hGlobal);
		return FALSE;
	}

	// Create IPicture from image file
	LPPICTURE gpPicture = NULL;
	hr = ::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (LPVOID*)&gpPicture);
	pstm->Release();

	if (!gpPicture)
	{
		GlobalFree(hGlobal);
		return FALSE;
	}

	OLE_HANDLE hBitmap = NULL;
	gpPicture->get_Handle(&hBitmap);
	if (hBitmap == NULL)
	{
		GlobalFree(hGlobal);
		return FALSE;
	}

	pBitmap->DeleteObject();
	pBitmap->Attach((HGDIOBJ)hBitmap);

	GlobalFree(hGlobal);
	return TRUE;
}

// 상대 영역좌표를 절대 영역좌표로 변환
CRect gf_RelativeToAbsolute(CRect rRelative, CWnd *pParent)
{
	CRect rScreen, rAbsolute;

	// 화면 영역좌표를 구한다
	pParent->GetClientRect(&rScreen);

	// Left 값
	rAbsolute.left = rScreen.left + rRelative.left;
	// Top 값
	rAbsolute.top = rScreen.top + rRelative.top;

	// Right값이 절대좌표라면
	if( rRelative.right > 0 )
		rAbsolute.right = rRelative.right;
	// Right값이 상대좌표라면
	else
		rAbsolute.right = rScreen.right + rRelative.right;

	// Bottom값이 절대좌표라면
	if( rRelative.bottom > 0 )
		rAbsolute.bottom = rRelative.bottom;
	// Bottom값이 상대좌표라면
	else
		rAbsolute.bottom = rScreen.bottom + rRelative.bottom;

	return rAbsolute;
}

// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
CRect gf_TransCtrlPosToAbsolute(CRect rInitPos, CWnd *pParent)
{
	CRect rScreen;

	pParent->GetClientRect(&rScreen);

	// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
	return gf_TransCtrlPosToAbsolute(rInitPos, rScreen);
}

// 특정 폴더에 해당 파일이 존재하는지 검사해서 반환
BOOL gf_FindFileInFolder(CString strSearchFile, CString strFolderPath)
{
	CString strPattern, strFileName;
    HANDLE hFile;
    WIN32_FIND_DATA FileInformation;

	if( strFolderPath.GetLength() < 1 )
		return FALSE;
	if( strFolderPath.Right(1) != "\\" && strFolderPath.Right(1) != "/")
		strFolderPath += "\\";
	strPattern = strFolderPath + TEXT("*.*");

    hFile = ::FindFirstFile(strPattern, &FileInformation);

    if(hFile != INVALID_HANDLE_VALUE) {
        do {
            strFileName = FileInformation.cFileName;

			// 일치하는 파일을 찾았다면 TRUE를 반환
			if( strFileName == strSearchFile )
			{
				::FindClose(hFile);
				return TRUE;
			}
            
        } while(::FindNextFile(hFile, &FileInformation) == TRUE);
        ::FindClose(hFile);
    }

	return FALSE;
}

// Text를 화면에 표시하는 함수 (다중 라인 지원)
void gf_DrawText(CPaintDC *pdc, CString strText, int nFontSize, COLORREF crFontColor, CRect rArea, UINT nFormat, BOOL bFontBold)
{
	CFont *pftOld, FontInfo;
	int nFontHeavy=0;
	
	// 폰트 두께
	nFontHeavy = ( bFontBold ) ? FW_BOLD : FW_NORMAL;
	//사용할 폰트생성
	FontInfo.CreateFont(nFontSize, 0,0,0, nFontHeavy, 0,0,0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("돋움"));

	//폰트와 색상 지정
	pftOld = pdc->SelectObject(&FontInfo);
	pdc->SetBkMode(TRANSPARENT);
	pdc->SetTextColor(crFontColor);

	// 텍스트를 화면에 표시
	//pdc->DrawText(strText, &rArea, nFormat);	

	CString strTemp;
	int nPos=0;
	
	// "\n"을 줄바꿈 기호로 변경
	while( (nPos=strText.Find(TEXT("\\n"))) >= 0 )
	{
		strTemp = strText.Left(nPos) + '\n';
		strTemp += strText.Mid(nPos+2);
		strText = strTemp;
	}

	int i=0, nLineEnd=0, nLineCount=0;
	CRect rText[50];
	CString strLineText[50];
	
	// 캡션 텍스트가 여러줄 일경우에 
	do {
		if( (nLineEnd = strText.Find(TEXT("\n"))) >= 0 )
		{
			strLineText[nLineCount] = strText.Left(nLineEnd);
			strText = strText.Mid(nLineEnd+1);
		}
		else
		{
			strLineText[nLineCount] = strText;
			strText = "";
		}

		nLineCount ++;
		
	} while( strText.GetLength() > 0 );

	int nTextAreaHeight=0;

	// 단위 문자열 높이
	if( nLineCount < 1 )
		nLineCount = 1;
	nTextAreaHeight = rArea.Height() / nLineCount;
	
	// 첫번째 단위 문자열 위치 계산
	rText[0] = rArea;
	rText[0].top = rArea.top + ((rArea.Height() - (nTextAreaHeight * nLineCount)) / 2);
	rText[0].bottom = rText[0].top + nTextAreaHeight;

	// 나머지 단위 문자열 위치 계산
	for(i=1 ; i < nLineCount ; i++)
	{
		rText[i] = rArea;
		rText[i].top = rText[0].top + nTextAreaHeight * i;
		rText[i].bottom = rText[i].top + nTextAreaHeight;
	}

	//문자를 화면에 표시
	for(i=0 ; i < nLineCount ; i++)
	{
		pdc->DrawText(strLineText[i], &rText[i], nFormat|DT_SINGLELINE);
	}
	
	// 폰트 초기화
	pdc->SelectObject(pftOld);
	// 폰트 삭제
	FontInfo.DeleteObject();
}

// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
CRect gf_TransCtrlPosToAbsolute(CRect rInitPos, CRect rScreen)
{
	CRect rCtrl;
	long nPercentSign=10000, nStartVal=0, nPlusMinus=0, nPosPercentBasis=0, nPosPercentDistance=0;
	float fPercentUnit=0., fWidthUnit=0., fHeightUnit=0.;
	fPercentUnit = (float)dfCOORDINATE_PERCENT_UNIT;
	
	//좌우 폭 퍼센트 단위를 구한다
	//fWidthUnit = (float)rScreen.Width() / fPercentUnit;
	//높이 퍼센트 단위를 구한다
	//fHeightUnit = (float)rScreen.Height() / fPercentUnit;

	/****** left값 계산 ******/
	// 컨트롤의 개별 위치값을 '상대좌표' 에서 '절대좌표'로 변환하여 반환
	rCtrl.left = gf_TransCtrlPosToAbsoluteOne(rInitPos.left, rScreen.Width());

	/*//Pixel 단위일때
	if( abs(rInitPos.left) < nPercentSign )
	{
		//수평 위치가 좌측 기준일때
		if( rInitPos.left >= 0 )
		{
			rCtrl.left = rInitPos.left;
		}
		//수평 위치가 우측 기준일때
		else
		{
			rCtrl.left = rScreen.Width() + rInitPos.left;
		}
	}
	//퍼센트 단위 일때
	else
	{
		if( abs(rInitPos.left) == nPercentSign*100 )
		{
			nPosPercentBasis = 100;
			nPosPercentDistance = 0;
		}
		else
		{
			// 기준 퍼센트값
			nPosPercentBasis = (int)((float)abs(rInitPos.left)/(float)nPercentSign);
			// 기준에서 거리값
			nPosPercentDistance = abs(rInitPos.left) - abs(nPosPercentBasis*nPercentSign);
		}
		// 기준에서 거리값이 음수라면
		if( rInitPos.left < 0 )
			nPosPercentDistance *= -1;
		// 최종 left 절대좌표값
		rCtrl.left = (int)((float)nPosPercentBasis*fWidthUnit) + nPosPercentDistance;
	}*/

	/****** top값 계산 ******/
	// 컨트롤의 개별 위치값을 '상대좌표' 에서 '절대좌표'로 변환하여 반환
	rCtrl.top = gf_TransCtrlPosToAbsoluteOne(rInitPos.top, rScreen.Height());

	/*//Pixel 단위일때
	if( abs(rInitPos.top) < nPercentSign )
	{
		//수평 위치가 위쪽 기준일때
		if( rInitPos.top >= 0 )
		{
			rCtrl.top = rInitPos.top;
		}
		//수평 위치가 아래쪽 기준일때
		else
		{
			rCtrl.top = rScreen.Height() + rInitPos.top;
		}
	}
	//퍼센트 단위 일때
	else
	{
		if( abs(rInitPos.top) == nPercentSign*100 )
		{
			nPosPercentBasis = 100;
			nPosPercentDistance = 0;
		}
		else
		{
			// 기준 퍼센트값
			nPosPercentBasis = (int)((float)abs(rInitPos.top)/(float)nPercentSign);
			// 기준에서 거리값
			nPosPercentDistance = abs(rInitPos.top) - abs(nPosPercentBasis*nPercentSign);
		}
		// 기준에서 거리값이 음수라면
		if( rInitPos.top < 0 )
			nPosPercentDistance *= -1;
		// 최종 top 절대좌표값
		rCtrl.top = (int)((float)nPosPercentBasis*fHeightUnit) + nPosPercentDistance;
	}*/

	/****** right값 계산 ******/
	// 컨트롤의 개별 위치값을 '상대좌표' 에서 '절대좌표'로 변환하여 반환
	rCtrl.right = gf_TransCtrlPosToAbsoluteOne(rInitPos.right, rScreen.Width());

	/*//Pixel 단위일때
	if( abs(rInitPos.right) < nPercentSign )
	{
		//수평 위치가 좌측 기준일때
		if( rInitPos.right >= 0 )
		{
			rCtrl.right = rInitPos.right;
		}
		//수평 위치가 우측 기준일때
		else
		{
			rCtrl.right = rScreen.Width() + rInitPos.right;
		}
	}
	//퍼센트 단위 일때
	else
	{
		if( abs(rInitPos.right) == nPercentSign*100 )
		{
			nPosPercentBasis = 100;
			nPosPercentDistance = 0;
		}
		else
		{
			// 기준 퍼센트값
			nPosPercentBasis = (int)((float)abs(rInitPos.right)/(float)nPercentSign);
			// 기준에서 거리값
			nPosPercentDistance = abs(rInitPos.right) - abs(nPosPercentBasis*nPercentSign);
		}
		// 기준에서 거리값이 음수라면
		if( rInitPos.right < 0 )
			nPosPercentDistance *= -1;
		// 최종 right 절대좌표값
		rCtrl.right = (int)((float)nPosPercentBasis*fWidthUnit) + nPosPercentDistance;
	}*/

	/****** bottom값 계산 ******/
	// 컨트롤의 개별 위치값을 '상대좌표' 에서 '절대좌표'로 변환하여 반환
	rCtrl.bottom = gf_TransCtrlPosToAbsoluteOne(rInitPos.bottom, rScreen.Height());

	/*//Pixel 단위일때
	if( abs(rInitPos.bottom) < nPercentSign )
	{
		//수평 위치가 위쪽 기준일때
		if( rInitPos.bottom >= 0 )
		{
			rCtrl.bottom = rInitPos.bottom;
		}
		//수평 위치가 아래쪽 기준일때
		else
		{
			rCtrl.bottom = rScreen.Height() + rInitPos.bottom;
		}
	}
	//퍼센트 단위 일때
	else
	{
		if( abs(rInitPos.bottom) == nPercentSign*100 )
		{
			nPosPercentBasis = 100;
			nPosPercentDistance = 0;
		}
		else
		{
			// 기준 퍼센트값
			nPosPercentBasis = (int)((float)abs(rInitPos.bottom)/(float)nPercentSign);
			// 기준에서 거리값
			nPosPercentDistance = abs(rInitPos.bottom) - abs(nPosPercentBasis*nPercentSign);
		}
		// 기준에서 거리값이 음수라면
		if( rInitPos.bottom < 0 )
			nPosPercentDistance *= -1;
		// 최종 bottom 절대좌표값
		rCtrl.bottom = (int)((float)nPosPercentBasis*fHeightUnit) + nPosPercentDistance;
	}*/

	return rCtrl;
}

// 컨트롤의 개별 위치값을 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
long gf_TransCtrlPosToAbsoluteOne(long nInitPos, long nScreen)
{
	long nAbsolutePos=0, nPercentSign=10000, nPosPercentBasis=0, nPosPercentDistance=0;
	double fPercentUnit=0.;
	
	fPercentUnit = (double)nScreen / (double)dfCOORDINATE_PERCENT_UNIT;

	//Pixel 단위일때
	if( abs(nInitPos) < nPercentSign )
	{
		//수평 위치가 위쪽 기준일때
		if( nInitPos >= 0 )
		{
			nAbsolutePos = nInitPos;
		}
		//수평 위치가 아래쪽 기준일때
		else
		{
			nAbsolutePos = nScreen + nInitPos;
		}
	}
	//퍼센트 단위 일때
	else
	{
		if( abs(nInitPos) == nPercentSign*dfCOORDINATE_PERCENT_UNIT )
		{
			nPosPercentBasis = dfCOORDINATE_PERCENT_UNIT;
			nPosPercentDistance = 0;
		}
		else
		{
			// 기준 퍼센트값
			nPosPercentBasis = (long)((double)abs(nInitPos)/(double)nPercentSign);
			// 기준에서 거리값
			nPosPercentDistance = abs(nInitPos) - abs(nPosPercentBasis*nPercentSign);
		}
		// 기준에서 거리값이 음수라면
		if( nInitPos < 0 )
			nPosPercentDistance *= -1;
		// 최종 bottom 절대좌표값
		nAbsolutePos = (long)((double)nPosPercentBasis*fPercentUnit) + nPosPercentDistance;
	}

	return nAbsolutePos;
}