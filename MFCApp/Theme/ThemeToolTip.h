/****************************************************************/
/*																*/
/*		FILE NAME		:	ThemeToolTip.h						*/
/*		DISCRIPTION		:	Theme ToolTip Class					*/
/*																*/
/*		PROGRAMMER		:	Jung, Sang-Soo						*/
/*		Mail			:	topofsan@hotmail.com				*/
/*		Home			:	http://rlxk.co.kr					*/
/*		LAST UPDATE		:	2008.10.18							*/
/****************************************************************/

#if !defined(AFX_THEMETOOLTIP_H__B4D87791_C08A_42F8_8067_5678EB2E76F7__INCLUDED_)
#define AFX_THEMETOOLTIP_H__B4D87791_C08A_42F8_8067_5678EB2E76F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThemeToolTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CThemeToolTip window

class CThemeToolTip : public CToolTipCtrl
{
// Construction
public:
	CThemeToolTip();
	CString m_strToolTipText;

	CWnd*	m_pParent;
	UINT	m_unToolTipID;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemeToolTip)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetToolTipText();
	void ChangeToolTip(CString strToolTip);
	void CreateToolTip(CWnd *pParent, UINT unToolTipID, CString strToolTip);
	void AddToolEx(CWnd *pWnd, CRect rTool, UINT unID, CString strToolTip);
	virtual ~CThemeToolTip();

	// Generated message map functions
protected:
	//{{AFX_MSG(CThemeToolTip)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMETOOLTIP_H__B4D87791_C08A_42F8_8067_5678EB2E76F7__INCLUDED_)

/*************************/
/* Global 변수/함수 정의 */
/*************************/

// INI 파일 폴더 경로
#define dfINI_MAIN_FILE_NAME														TEXT("info.ini")
#define dfCOLOR_TRANSPARENT						RGB(255,0,255)			// 투명을 의미하는 색깔
#define dfTIMER_INVALIDATE						WM_USER+116				// 화면을 새로 그리는 타이머

#define dfCOORDINATE_PERCENT_UNIT				1000					// 좌표 퍼센트 단위

// 테마 폴더 경로 저장 변수
extern CString g_strThemePath;

// 테마 관련 전역변수를 초기화하는 함수
extern void gf_ThemeInit();
// 테마명을 반환
extern CString gf_GetThemeName();
// INI파일에서 텍스트 정보를 가져오는 함수
extern BOOL gf_GetTextFromIniFile(CString strIniFileName, LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString &strText);
// 실행파일이 존재하는 폴더 경로 반환
extern CString gf_GetModulePath();
// 이미지 일부분을 크기 변경하여 그리는 함수(투명 지원)
extern void gf_DrawImagePartResizeTransparent(CPaintDC *pdc, CBitmap *pBitmap, CRect rImgPart, CRect rImgView);
// 이미지 크기 변경하여 그리는 함수(투명 지원)
extern void gf_DrawImageResizeTransparent(CPaintDC *pdc, CBitmap *pBitmap, CRect rArea);
// 이미지의 일부를 9조각으로 분할, 크기 변경하여 화면에 표시하는 함수(투명 지원)
extern void gf_DrawImagePartTransparentResizeBlock(CPaintDC *pdc, CBitmap *pBitmap, CRect rImgPart, CRect rArea);
// 사각형 선을 그리는 함수
extern void gf_DrawRectLine(CRect rLine, COLORREF crLeft, COLORREF crTop, COLORREF crRight, COLORREF crBottom, CPaintDC *pdc);
// 라인(선)을 그리는 함수
extern void gf_DrawLine(CPoint po1, CPoint po2, COLORREF crLine, int nLineTick, CPaintDC *pdc);
// 이미지를 Pattern 브러쉬 형식으로 화면 영역에 그리는 함수(바둑판 형식)
extern void gf_DrawImageByPatternBrush(CPaintDC *pdc, CBitmap *pBitmap, CRect rImgPart, CRect rScreen);
// 투명 배경 이미지를 그리는 함수
extern void gf_DrawTransparentImage(CBitmap *pBitmap, CRect rImgPart, CRect rImgView, CPaintDC *pdc);
// 투명 배경 이미지를 크기 변경하여 그리는 함수
extern void gf_DrawTransparentResize(CBitmap *pBitmap, CRect rImgPart, CRect rImgView, CPaintDC *pdc);
// 이미지를 9조각으로 분할하여 화면에 표시하는 함수(투명 적용)
extern void gf_DrawImagePartSkin(CPaintDC *pdc, CBitmap *pBitmap, CRect rImage, CRect rScreen);
// 테두리를 가지는 사각형을 그리는 함수
extern void gf_DrawRectWithOutline(CPaintDC *pdc, COLORREF crBack, COLORREF crOutlineLT, COLORREF crOutlineRB, int nOutlineTick, CRect rRect);
// 모서리가 둥근 사각형을 그리는 함수
extern void gf_DrawRoundEdgeRect(CPaintDC *pdc, COLORREF crBack, COLORREF crOutline, int nOutlineTick, int nEdgeRound, CRect rRect);
// Text를 화면에 표시하는 함수 
extern void gf_DrawText(CPaintDC *pdc, CString strText, int nFontSize, COLORREF crFontColor, CRect rArea, UINT nFormat, BOOL bFontBold);
// 이미지를 9조각으로 분할, 크기 변경하여 화면에 표시하는 함수(투명 지원)
extern void gf_DrawImageTransparentResizeBlock(CPaintDC *pdc, CBitmap *pBitmap, CRect rArea);
// 화면에 타원(면)을 그린다
extern void gf_DrawCircle(CRect rArea, COLORREF crLine, int nLineTick, COLORREF crCircle, CPaintDC *pdc);
// 이미지를 원본 그대로 1회 화면에 정렬해서 그리는 함수(투명 지원)
extern void gf_DrawImagePartTransparent(CPaintDC *pdc, CBitmap *pBitmap, CRect rImgPart, CRect rScreen, UINT unAlignFormat);
// 이미지를 원본 그대로 1회 화면에 정렬해서 그리는 함수(투명 지원)
extern void gf_DrawImageTransparent(CPaintDC *pdc, CBitmap *pBitmap, CRect rScreen, UINT unAlignFormat=DT_LEFT|DT_TOP);
// 이미지 파일을 읽어서 CBitmap으로 반환
extern BOOL gf_LoadPictureFile(LPCTSTR szFile, CBitmap *pBitmap);
// 상대 영역좌표를 절대 영역좌표로 변환
extern CRect gf_RelativeToAbsolute(CRect rRelative, CWnd *pParent);
// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
extern CRect gf_TransCtrlPosToAbsolute(CRect rInitPos, CRect rScreen);
// 컨트롤의 위치좌표를 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
extern CRect gf_TransCtrlPosToAbsolute(CRect rInitPos, CWnd *pParent);
// 특정 폴더에 해당 파일이 존재하는지 검사해서 반환
extern BOOL gf_FindFileInFolder(CString strSearchFile, CString strFolderPath);
// 테마 폴더 경로 설정
extern void gf_SetThemePath(CString strThemePath);
// INI 파일에 텍스트 정보를 저장
extern BOOL gf_SetTextToIniFile(CString strIniFileName, LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString strText);
//문자열의 위치를 뒤쪽에서 부터 검사하는 함수(시작 위치 지정)
extern int gf_FindStringBackward(CString strData, CString strKey, int nStartL);
//문자열의 위치를 뒤쪽에서 부터 검사하는 함수
extern int gf_FindStringBackward(CString strData, CString strKey);
// 컨트롤의 개별 위치값을 '상대좌표' 에서 '절대좌표'로 변환하여 반환하는 함수
extern long gf_TransCtrlPosToAbsoluteOne(long nInitPos, long nScreen);